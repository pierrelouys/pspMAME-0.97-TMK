
/* ======================================================================== */
/* =========================== UTILITY FUNCTIONS ========================== */
/* ======================================================================== */


/* ---------------------------- Read Immediate ---------------------------- */

/* Handles all immediate reads, does address error check, function code setting,
 * and prefetching if they are enabled in m68kconf.h
 */
INLINE uint m68ki_read_imm_16(void)
{
	m68ki_set_fc(FLAG_S | FUNCTION_CODE_USER_PROGRAM); /* auto-disable (see m68kcpu.h) */
	m68ki_check_address_error(REG_PC, MODE_READ, FLAG_S | FUNCTION_CODE_USER_PROGRAM); /* auto-disable (see m68kcpu.h) */
#if M68K_EMULATE_PREFETCH
	if(MASK_OUT_BELOW_2(REG_PC) != CPU_PREF_ADDR)
	{
		CPU_PREF_ADDR = MASK_OUT_BELOW_2(REG_PC);
		CPU_PREF_DATA = m68k_read_immediate_32(ADDRESS_68K(CPU_PREF_ADDR));
	}
	REG_PC += 2;
	return MASK_OUT_ABOVE_16(CPU_PREF_DATA >> ((2-((REG_PC-2)&2))<<3));
#else
	REG_PC += 2;
	return m68k_read_immediate_16(ADDRESS_68K(REG_PC-2));
#endif /* M68K_EMULATE_PREFETCH */
}
INLINE uint m68ki_read_imm_32(void)
{
#if M68K_EMULATE_PREFETCH
	uint temp_val;

	m68ki_set_fc(FLAG_S | FUNCTION_CODE_USER_PROGRAM); /* auto-disable (see m68kcpu.h) */
	m68ki_check_address_error(REG_PC, MODE_READ, FLAG_S | FUNCTION_CODE_USER_PROGRAM); /* auto-disable (see m68kcpu.h) */
	if(MASK_OUT_BELOW_2(REG_PC) != CPU_PREF_ADDR)
	{
		CPU_PREF_ADDR = MASK_OUT_BELOW_2(REG_PC);
		CPU_PREF_DATA = m68k_read_immediate_32(ADDRESS_68K(CPU_PREF_ADDR));
	}
	temp_val = CPU_PREF_DATA;
	REG_PC += 2;
	if(MASK_OUT_BELOW_2(REG_PC) != CPU_PREF_ADDR)
	{
		CPU_PREF_ADDR = MASK_OUT_BELOW_2(REG_PC);
		CPU_PREF_DATA = m68k_read_immediate_32(ADDRESS_68K(CPU_PREF_ADDR));
		temp_val = MASK_OUT_ABOVE_32((temp_val << 16) | (CPU_PREF_DATA >> 16));
	}
	REG_PC += 2;

	return temp_val;
#else
	m68ki_set_fc(FLAG_S | FUNCTION_CODE_USER_PROGRAM); /* auto-disable (see m68kcpu.h) */
	m68ki_check_address_error(REG_PC, MODE_READ, FLAG_S | FUNCTION_CODE_USER_PROGRAM); /* auto-disable (see m68kcpu.h) */
	REG_PC += 4;
	return m68k_read_immediate_32(ADDRESS_68K(REG_PC-4));
#endif /* M68K_EMULATE_PREFETCH */
}



/* ------------------------- Top level read/write ------------------------- */

/* Handles all memory accesses (except for immediate reads if they are
 * configured to use separate functions in m68kconf.h).
 * All memory accesses must go through these top level functions.
 * These functions will also check for address error and set the function
 * code if they are enabled in m68kconf.h.
 */
INLINE uint m68ki_read_8_fc(uint address, uint fc)
{
	m68ki_set_fc(fc); /* auto-disable (see m68kcpu.h) */
	return m68k_read_memory_8(ADDRESS_68K(address));
}
INLINE uint m68ki_read_16_fc(uint address, uint fc)
{
	m68ki_set_fc(fc); /* auto-disable (see m68kcpu.h) */
	m68ki_check_address_error(address, MODE_READ, fc); /* auto-disable (see m68kcpu.h) */
	return m68k_read_memory_16(ADDRESS_68K(address));
}
INLINE uint m68ki_read_32_fc(uint address, uint fc)
{
	m68ki_set_fc(fc); /* auto-disable (see m68kcpu.h) */
	m68ki_check_address_error(address, MODE_READ, fc); /* auto-disable (see m68kcpu.h) */
	return m68k_read_memory_32(ADDRESS_68K(address));
}

INLINE void m68ki_write_8_fc(uint address, uint fc, uint value)
{
	m68ki_set_fc(fc); /* auto-disable (see m68kcpu.h) */
	m68k_write_memory_8(ADDRESS_68K(address), value);
}
INLINE void m68ki_write_16_fc(uint address, uint fc, uint value)
{
	m68ki_set_fc(fc); /* auto-disable (see m68kcpu.h) */
	m68ki_check_address_error(address, MODE_WRITE, fc); /* auto-disable (see m68kcpu.h) */
	m68k_write_memory_16(ADDRESS_68K(address), value);
}
INLINE void m68ki_write_32_fc(uint address, uint fc, uint value)
{
	m68ki_set_fc(fc); /* auto-disable (see m68kcpu.h) */
	m68ki_check_address_error(address, MODE_WRITE, fc); /* auto-disable (see m68kcpu.h) */
	m68k_write_memory_32(ADDRESS_68K(address), value);
}

#if (1==M68K_SIMULATE_PD_WRITES)
INLINE void m68ki_write_32_pd_fc(uint address, uint fc, uint value)
{
	m68ki_set_fc(fc); /* auto-disable (see m68kcpu.h) */
	m68ki_check_address_error(address, MODE_WRITE, fc); /* auto-disable (see m68kcpu.h) */
	m68k_write_memory_32_pd(ADDRESS_68K(address), value);
}
#endif // (1==M68K_SIMULATE_PD_WRITES)


/* --------------------- Effective Address Calculation -------------------- */

/* The program counter relative addressing modes cause operands to be
 * retrieved from program space, not data space.
 */


/* Indexed addressing modes are encoded as follows:
 *
 * Base instruction format:
 * F E D C B A 9 8 7 6 | 5 4 3 | 2 1 0
 * x x x x x x x x x x | 1 1 0 | BASE REGISTER		(An)
 *
 * Base instruction format for destination EA in move instructions:
 * F E D C | B A 9	  | 8 7 6 | 5 4 3 2 1 0
 * x x x x | BASE REG | 1 1 0 | X X X X X X 	  (An)
 *
 * Brief extension format:
 *	F  |  E D C   |  B	|  A 9	| 8 | 7 6 5 4 3 2 1 0
 * D/A | REGISTER | W/L | SCALE | 0 |  DISPLACEMENT
 *
 * Full extension format:
 *	F	  E D C 	 B	   A 9	  8   7    6	5 4 	  3   2 1 0
 * D/A | REGISTER | W/L | SCALE | 1 | BS | IS | BD SIZE | 0 | I/IS
 * BASE DISPLACEMENT (0, 16, 32 bit)				(bd)
 * OUTER DISPLACEMENT (0, 16, 32 bit)				(od)
 *
 * D/A: 	0 = Dn, 1 = An							(Xn)
 * W/L: 	0 = W (sign extend), 1 = L				(.SIZE)
 * SCALE:	00=1, 01=2, 10=4, 11=8					(*SCALE)
 * BS:		0=add base reg, 1=suppress base reg 	(An suppressed)
 * IS:		0=add index, 1=suppress index			(Xn suppressed)
 * BD SIZE: 00=reserved, 01=NULL, 10=Word, 11=Long	(size of bd)
 *
 * IS I/IS Operation
 * 0  000  No Memory Indirect
 * 0  001  indir prex with null outer
 * 0  010  indir prex with word outer
 * 0  011  indir prex with long outer
 * 0  100  reserved
 * 0  101  indir postx with null outer
 * 0  110  indir postx with word outer
 * 0  111  indir postx with long outer
 * 1  000  no memory indirect
 * 1  001  mem indir with null outer
 * 1  010  mem indir with word outer
 * 1  011  mem indir with long outer
 * 1  100-111  reserved
 */
INLINE uint m68ki_get_ea_ix(uint An)
{
	/* An = base register */
	uint extension = m68ki_read_imm_16();
	uint Xn = 0;						/* Index register */

	//if(CPU_TYPE_IS_010_LESS(1/*CPU_TYPE*/))
	{
		/* Calculate index */
		Xn = REG_DA[extension>>12]; 	/* Xn */
		if(!BIT_B(extension))			/* W/L */
			Xn = MAKE_INT_16(Xn);

		/* Add base register and displacement and return */
		return An + Xn + MAKE_INT_8(extension);
	}

#if 0
	/* Brief extension format */
	if(!BIT_8(extension))
	{
		/* Calculate index */
		Xn = REG_DA[extension>>12]; 	/* Xn */
		if(!BIT_B(extension))			/* W/L */
			Xn = MAKE_INT_16(Xn);
		/* Add scale if proper CPU type */
		if(CPU_TYPE_IS_EC020_PLUS(CPU_TYPE))
			Xn <<= (extension>>9) & 3;	/* SCALE */

		/* Add base register and displacement and return */
		return An + Xn + MAKE_INT_8(extension);
	}

	/* Full extension format */

	USE_CYCLES(m68ki_ea_idx_cycle_table[extension&0x3f]);

	/* Check if base register is present */
	if(BIT_7(extension))				/* BS */
		An = 0; 						/* An */

	/* Check if index is present */
	if(!BIT_6(extension))				/* IS */
	{
		Xn = REG_DA[extension>>12]; 	/* Xn */
		if(!BIT_B(extension))			/* W/L */
			Xn = MAKE_INT_16(Xn);
		Xn <<= (extension>>9) & 3;		/* SCALE */
	}

	{
	uint bd = 0;						/* Base Displacement */
	/* Check if base displacement is present */
	if(BIT_5(extension))				/* BD SIZE */
		bd = BIT_4(extension) ? m68ki_read_imm_32() : MAKE_INT_16(m68ki_read_imm_16());

	/* If no indirect action, we are done */
	if(!(extension&7))					/* No Memory Indirect */
		return An + bd + Xn;

	{
	uint od = 0;						/* Outer Displacement */
	/* Check if outer displacement is present */
	if(BIT_1(extension))				/* I/IS:  od */
		od = BIT_0(extension) ? m68ki_read_imm_32() : MAKE_INT_16(m68ki_read_imm_16());

	/* Postindex */
	if(BIT_2(extension))				/* I/IS:  0 = preindex, 1 = postindex */
		return m68ki_read_32(An + bd) + Xn + od;

	/* Preindex */
	return m68ki_read_32(An + bd + Xn) + od;
	}
	}
#endif //0
}
INLINE uint m68ki_get_ea_pcix(void)
{
	m68ki_use_program_space(); /* auto-disable */
	return m68ki_get_ea_ix(REG_PC);
}
INLINE uint m68ki_get_ea_pcdi(void)
{
	uint old_pc = REG_PC;
	m68ki_use_program_space(); /* auto-disable */
	return old_pc + MAKE_INT_16(m68ki_read_imm_16());
}


#if 0
/* Fetch operands */
INLINE uint OPER_AY_AI_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AY_AI_8(); 	return m68ki_read_8(ea); }
//#define OPER_AY_AI_8() (m68ki_read_8( (uint)(EA_AY_AI_8()) ) )
INLINE uint OPER_AY_AI_16(void) {TEMP_UINT32 /*uint*/ea = EA_AY_AI_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AY_AI_32(void) {TEMP_UINT32 /*uint*/ea = EA_AY_AI_32();	return m68ki_read_32(ea);}
INLINE uint OPER_AY_PI_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AY_PI_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_AY_PI_16(void) {TEMP_UINT32 /*uint*/ea = EA_AY_PI_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AY_PI_32(void) {TEMP_UINT32 /*uint*/ea = EA_AY_PI_32();	return m68ki_read_32(ea);}
INLINE uint OPER_AY_PD_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AY_PD_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_AY_PD_16(void) {TEMP_UINT32 /*uint*/ea = EA_AY_PD_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AY_PD_32(void) {TEMP_UINT32 /*uint*/ea = EA_AY_PD_32();	return m68ki_read_32(ea);}
INLINE uint OPER_AY_DI_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AY_DI_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_AY_DI_16(void) {TEMP_UINT32 /*uint*/ea = EA_AY_DI_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AY_DI_32(void) {TEMP_UINT32 /*uint*/ea = EA_AY_DI_32();	return m68ki_read_32(ea);}
INLINE uint OPER_AY_IX_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AY_IX_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_AY_IX_16(void) {TEMP_UINT32 /*uint*/ea = EA_AY_IX_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AY_IX_32(void) {TEMP_UINT32 /*uint*/ea = EA_AY_IX_32();	return m68ki_read_32(ea);}

INLINE uint OPER_AX_AI_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AX_AI_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_AX_AI_16(void) {TEMP_UINT32 /*uint*/ea = EA_AX_AI_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AX_AI_32(void) {TEMP_UINT32 /*uint*/ea = EA_AX_AI_32();	return m68ki_read_32(ea);}
INLINE uint OPER_AX_PI_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AX_PI_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_AX_PI_16(void) {TEMP_UINT32 /*uint*/ea = EA_AX_PI_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AX_PI_32(void) {TEMP_UINT32 /*uint*/ea = EA_AX_PI_32();	return m68ki_read_32(ea);}
INLINE uint OPER_AX_PD_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AX_PD_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_AX_PD_16(void) {TEMP_UINT32 /*uint*/ea = EA_AX_PD_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AX_PD_32(void) {TEMP_UINT32 /*uint*/ea = EA_AX_PD_32();	return m68ki_read_32(ea);}
INLINE uint OPER_AX_DI_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AX_DI_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_AX_DI_16(void) {TEMP_UINT32 /*uint*/ea = EA_AX_DI_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AX_DI_32(void) {TEMP_UINT32 /*uint*/ea = EA_AX_DI_32();	return m68ki_read_32(ea);}
INLINE uint OPER_AX_IX_8(void)	{TEMP_UINT32 /*uint*/ea = EA_AX_IX_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_AX_IX_16(void) {TEMP_UINT32 /*uint*/ea = EA_AX_IX_16();	return m68ki_read_16(ea);}
INLINE uint OPER_AX_IX_32(void) {TEMP_UINT32 /*uint*/ea = EA_AX_IX_32();	return m68ki_read_32(ea);}

INLINE uint OPER_A7_PI_8(void)	{TEMP_UINT32 /*uint*/ea = EA_A7_PI_8(); 	return m68ki_read_8(ea); }
INLINE uint OPER_A7_PD_8(void)	{TEMP_UINT32 /*uint*/ea = EA_A7_PD_8(); 	return m68ki_read_8(ea); }

INLINE uint OPER_AW_8(void) 	{TEMP_UINT32 /*uint*/ea = EA_AW_8();		return m68ki_read_8(ea); }
INLINE uint OPER_AW_16(void)	{TEMP_UINT32 /*uint*/ea = EA_AW_16();		return m68ki_read_16(ea);}
INLINE uint OPER_AW_32(void)	{TEMP_UINT32 /*uint*/ea = EA_AW_32();		return m68ki_read_32(ea);}
INLINE uint OPER_AL_8(void) 	{TEMP_UINT32 /*uint*/ea = EA_AL_8();		return m68ki_read_8(ea); }
INLINE uint OPER_AL_16(void)	{TEMP_UINT32 /*uint*/ea = EA_AL_16();		return m68ki_read_16(ea);}
INLINE uint OPER_AL_32(void)	{TEMP_UINT32 /*uint*/ea = EA_AL_32();		return m68ki_read_32(ea);}
INLINE uint OPER_PCDI_8(void)	{TEMP_UINT32 /*uint*/ea = EA_PCDI_8();		return m68ki_read_pcrel_8(ea); }
INLINE uint OPER_PCDI_16(void)	{TEMP_UINT32 /*uint*/ea = EA_PCDI_16(); 	return m68ki_read_pcrel_16(ea);}
INLINE uint OPER_PCDI_32(void)	{TEMP_UINT32 /*uint*/ea = EA_PCDI_32(); 	return m68ki_read_pcrel_32(ea);}
INLINE uint OPER_PCIX_8(void)	{TEMP_UINT32 /*uint*/ea = EA_PCIX_8();		return m68ki_read_pcrel_8(ea); }
INLINE uint OPER_PCIX_16(void)	{TEMP_UINT32 /*uint*/ea = EA_PCIX_16(); 	return m68ki_read_pcrel_16(ea);}
INLINE uint OPER_PCIX_32(void)	{TEMP_UINT32 /*uint*/ea = EA_PCIX_32(); 	return m68ki_read_pcrel_32(ea);}
#else
/* Fetch operands */
#define OPER_AY_AI_8()      (m68ki_read_8 (       (uint)(EA_AY_AI_8 ()  ) ) )
#define OPER_AY_AI_16()     (m68ki_read_16(       (uint)(EA_AY_AI_16()  ) ) )
#define OPER_AY_AI_32()     (m68ki_read_32(       (uint)(EA_AY_AI_32()  ) ) )
#define OPER_AY_PI_8()      (m68ki_read_8 (       (uint)(EA_AY_PI_8 ()  ) ) )
#define OPER_AY_PI_16()     (m68ki_read_16(       (uint)(EA_AY_PI_16()  ) ) )
#define OPER_AY_PI_32()     (m68ki_read_32(       (uint)(EA_AY_PI_32()  ) ) )
#define OPER_AY_PD_8()      (m68ki_read_8 (       (uint)(EA_AY_PD_8 ()  ) ) )
#define OPER_AY_PD_16()     (m68ki_read_16(       (uint)(EA_AY_PD_16()  ) ) )
#define OPER_AY_PD_32()     (m68ki_read_32(       (uint)(EA_AY_PD_32()  ) ) )
#define OPER_AY_DI_8()      (m68ki_read_8 (       (uint)(EA_AY_DI_8 ()  ) ) )
#define OPER_AY_DI_16()     (m68ki_read_16(       (uint)(EA_AY_DI_16()  ) ) )
#define OPER_AY_DI_32()     (m68ki_read_32(       (uint)(EA_AY_DI_32()  ) ) )
#define OPER_AY_IX_8()      (m68ki_read_8 (       (uint)(EA_AY_IX_8 ()  ) ) )
#define OPER_AY_IX_16()     (m68ki_read_16(       (uint)(EA_AY_IX_16()  ) ) )
#define OPER_AY_IX_32()     (m68ki_read_32(       (uint)(EA_AY_IX_32()  ) ) )

#define OPER_AX_AI_8()      (m68ki_read_8 (       (uint)(EA_AX_AI_8 ()  ) ) )
#define OPER_AX_AI_16()     (m68ki_read_16(       (uint)(EA_AX_AI_16()  ) ) )
#define OPER_AX_AI_32()     (m68ki_read_32(       (uint)(EA_AX_AI_32()  ) ) )
#define OPER_AX_PI_8()      (m68ki_read_8 (       (uint)(EA_AX_PI_8 ()  ) ) )
#define OPER_AX_PI_16()     (m68ki_read_16(       (uint)(EA_AX_PI_16()  ) ) )
#define OPER_AX_PI_32()     (m68ki_read_32(       (uint)(EA_AX_PI_32()  ) ) )
#define OPER_AX_PD_8()      (m68ki_read_8 (       (uint)(EA_AX_PD_8 ()  ) ) )
#define OPER_AX_PD_16()     (m68ki_read_16(       (uint)(EA_AX_PD_16()  ) ) )
#define OPER_AX_PD_32()     (m68ki_read_32(       (uint)(EA_AX_PD_32()  ) ) )
#define OPER_AX_DI_8()      (m68ki_read_8 (       (uint)(EA_AX_DI_8 ()  ) ) )
#define OPER_AX_DI_16()     (m68ki_read_16(       (uint)(EA_AX_DI_16()  ) ) )
#define OPER_AX_DI_32()     (m68ki_read_32(       (uint)(EA_AX_DI_32()  ) ) )
#define OPER_AX_IX_8()      (m68ki_read_8 (       (uint)(EA_AX_IX_8 ()  ) ) )
#define OPER_AX_IX_16()     (m68ki_read_16(       (uint)(EA_AX_IX_16()  ) ) )
#define OPER_AX_IX_32()     (m68ki_read_32(       (uint)(EA_AX_IX_32()  ) ) )

#define OPER_A7_PI_8()      (m68ki_read_8 (       (uint)(EA_A7_PI_8 ()  ) ) )
#define OPER_A7_PD_8()      (m68ki_read_8 (       (uint)(EA_A7_PD_8 ()  ) ) )

#define OPER_AW_8()         (m68ki_read_8 (       (uint)(EA_AW_8 ()     ) ) )
#define OPER_AW_16()        (m68ki_read_16(       (uint)(EA_AW_16()     ) ) )
#define OPER_AW_32()        (m68ki_read_32(       (uint)(EA_AW_32()     ) ) )
#define OPER_AL_8()         (m68ki_read_8 (       (uint)(EA_AL_8 ()     ) ) )
#define OPER_AL_16()        (m68ki_read_16(       (uint)(EA_AL_16()     ) ) )
#define OPER_AL_32()        (m68ki_read_32(       (uint)(EA_AL_32()     ) ) )
#define OPER_PCDI_8()       (m68ki_read_pcrel_8 ( (uint)(EA_PCDI_8 ()   ) ) )
#define OPER_PCDI_16()      (m68ki_read_pcrel_16( (uint)(EA_PCDI_16()   ) ) )
#define OPER_PCDI_32()      (m68ki_read_pcrel_32( (uint)(EA_PCDI_32()   ) ) )
#define OPER_PCIX_8()       (m68ki_read_pcrel_8 ( (uint)(EA_PCIX_8 ()   ) ) )
#define OPER_PCIX_16()      (m68ki_read_pcrel_16( (uint)(EA_PCIX_16()   ) ) )
#define OPER_PCIX_32()      (m68ki_read_pcrel_32( (uint)(EA_PCIX_32()   ) ) )
#endif


/* ---------------------------- Stack Functions --------------------------- */

/* Push/pull data from the stack */
INLINE void m68ki_push_16(uint value)
{
	REG_SP = MASK_OUT_ABOVE_32(REG_SP - 2);
	m68ki_write_16(REG_SP, value);
}

INLINE void m68ki_push_32(uint value)
{
	REG_SP = MASK_OUT_ABOVE_32(REG_SP - 4);
	m68ki_write_32(REG_SP, value);
}

INLINE uint m68ki_pull_16(void)
{
	REG_SP = MASK_OUT_ABOVE_32(REG_SP + 2);
	return m68ki_read_16(REG_SP-2);
}

INLINE uint m68ki_pull_32(void)
{
	REG_SP = MASK_OUT_ABOVE_32(REG_SP + 4);
	return m68ki_read_32(REG_SP-4);
}


/* Increment/decrement the stack as if doing a push/pull but
 * don't do any memory access.
 */
INLINE void m68ki_fake_push_16(void){	REG_SP = MASK_OUT_ABOVE_32(REG_SP - 2);}
INLINE void m68ki_fake_push_32(void){	REG_SP = MASK_OUT_ABOVE_32(REG_SP - 4);}
INLINE void m68ki_fake_pull_16(void){	REG_SP = MASK_OUT_ABOVE_32(REG_SP + 2);}
INLINE void m68ki_fake_pull_32(void){	REG_SP = MASK_OUT_ABOVE_32(REG_SP + 4);}


/* ----------------------------- Program Flow ----------------------------- */

/* Jump to a new program location or vector.
 * These functions will also call the pc_changed callback if it was enabled
 * in m68kconf.h.
 */
INLINE void m68ki_jump(uint new_pc)
{
	REG_PC = new_pc;
	m68ki_pc_changed(REG_PC);
}

INLINE void m68ki_jump_vector(uint vector)
{
	REG_PC = (vector<<2) /*+REG_VBR*/;
	REG_PC = m68ki_read_data_32(REG_PC);
	m68ki_pc_changed(REG_PC);
}


/* Branch to a new memory location.
 * The 32-bit branch will call pc_changed if it was enabled in m68kconf.h.
 * So far I've found no problems with not calling pc_changed for 8 or 16
 * bit branches.
 */
#if 0
	INLINE void m68ki_branch_8(uint offset){	REG_PC += MAKE_INT_8(offset);}
	INLINE void m68ki_branch_16(uint offset){	REG_PC += MAKE_INT_16(offset);}
//	INLINE void m68ki_branch_32(uint offset){	REG_PC += offset;	m68ki_pc_changed(REG_PC);}
#else
	#define m68ki_branch_8(offset) REG_PC += MAKE_INT_8((uint)offset);
	#define m68ki_branch_16(offset) REG_PC += MAKE_INT_16((uint)offset);
//	#define m68ki_branch_32(offset) REG_PC += ((uint)offset);	m68ki_pc_changed(REG_PC);
#endif





/* ---------------------------- Status Register --------------------------- */








/* Set the condition code register */
INLINE void m68ki_set_ccr(uint value)
{
	FLAG_X = BIT_4(value)  << 4;
	FLAG_N = BIT_3(value)  << 4;
	FLAG_Z = !BIT_2(value);
	FLAG_V = BIT_1(value)  << 6;
	FLAG_C = BIT_0(value)  << 8;
}

/* Set the status register but don't check for interrupts */
INLINE void m68ki_set_sr_noint(uint value)
{
	/* Mask out the "unimplemented" bits */
	value &= 0xa71f;/*m68000 CPU_SR_MASK*/

	/* Now set the status register */
	#if (1==M68K_EMULATE_TRACE)
	FLAG_T1 = BIT_F(value);
	FLAG_T0 = BIT_E(value);
	#endif //(1==M68K_EMULATE_TRACE)
	FLAG_INT_MASK = value & 0x0700;
	m68ki_set_ccr(value);
	//m68ki_set_sm_flag((value >> 11) & 6);
	/* Set the S and M flags and change the active stack pointer.
	 * Note that value MUST be 0, 2, 4, or 6 (bit2 = S, bit1 = M).
	 */
	//INLINE void m68ki_set_sm_flag(uint value)
	{
		/* Backup the old stack pointer */
		REG_SP_BASE[FLAG_S | ((FLAG_S>>1) & FLAG_M)] = REG_SP;
		/* Set the S and M flags */
		FLAG_S = /*value*/(((value >> 11) /*& 6*/)) & SFLAG_SET/*4*/;
		FLAG_M = /*value*/(((value >> 11) /*& 6*/)) & MFLAG_SET/*2*/;
		/* Set the new stack pointer */
		REG_SP = REG_SP_BASE[FLAG_S | ((FLAG_S>>1) & FLAG_M)];
	}
}

#if 0
/* Set the status register but don't check for interrupts nor
 * change the stack pointer
 */
/* Set the S and M flags.  Don't touch the stack pointer. */
INLINE void m68ki_set_sm_flag_nosp(uint value)
{
	/* Set the S and M flags */
	FLAG_S = value & SFLAG_SET;
	FLAG_M = value & MFLAG_SET;
}
INLINE void m68ki_set_sr_noint_nosp(uint value)
{
	/* Mask out the "unimplemented" bits */
	value &= 0xa71f;/*m68000 CPU_SR_MASK*/

	/* Now set the status register */
	#if (1==M68K_EMULATE_TRACE)
	FLAG_T1 = BIT_F(value);
	FLAG_T0 = BIT_E(value);
	#endif //(1==M68K_EMULATE_TRACE)
	FLAG_INT_MASK = value & 0x0700;
	m68ki_set_ccr(value);
	m68ki_set_sm_flag_nosp((value >> 11) & 6);
}
#endif



/* ------------------------- Exception Processing ------------------------- */

/* Initiate exception processing */
INLINE uint m68ki_init_exception(void)
{
	/* Save the old status register */
	uint sr = m68ki_get_sr();

	/* Turn off trace flag, clear pending traces */
	#if (1==M68K_EMULATE_TRACE)
	FLAG_T1 = FLAG_T0 = 0;
	#endif //(1==M68K_EMULATE_TRACE)
	m68ki_clear_trace();
	/* Enter supervisor mode */
	//m68ki_set_s_flag(SFLAG_SET);
	/* Set the S flag and change the active stack pointer.
	 * Note that value MUST be 4 or 0.
	 */
	//INLINE void m68ki_set_s_flag(uint value)
	{
		/* Backup the old stack pointer */
		REG_SP_BASE[FLAG_S | ((FLAG_S>>1) & FLAG_M)] = REG_SP;
		/* Set the S flag */
		FLAG_S = SFLAG_SET/*value*/;
		/* Set the new stack pointer */
		REG_SP = REG_SP_BASE[FLAG_S | ((FLAG_S>>1) & FLAG_M)];
	}
	return sr;
}

/* 3 word stack frame (68000 only) */
#define m68ki_stack_frame_3word(MY_pc,MY_sr) \
{	m68ki_push_32(MY_pc); \
	m68ki_push_16(MY_sr); \
}

/* Format 0 stack frame.
 * This is the standard stack frame for 68010+.
 */
//INLINE void m68ki_stack_frame_0000(uint pc, uint sr, uint vector)
//{
//	/* Stack a 3-word frame if we are 68000 */
//	//if(CPU_TYPE == CPU_TYPE_000 || CPU_TYPE == CPU_TYPE_008)
//	{
//		m68ki_stack_frame_3word(pc, sr);
//		return;
//	}
//	m68ki_push_16(vector<<2);
//	m68ki_push_32(pc);
//	m68ki_push_16(sr);
//}




/* Bus error stack frame (68000 only).
 */
INLINE void m68ki_stack_frame_buserr(uint sr)
{
	m68ki_push_32(REG_PC);
	m68ki_push_16(sr);
	m68ki_push_16(REG_IR);
	m68ki_push_32(m68ki_aerr_address);	/* access address */
	/* 0 0 0 0 0 0 0 0 0 0 0 R/W I/N FC
	 * R/W	0 = write, 1 = read
	 * I/N	0 = instruction, 1 = not
	 * FC	3-bit function code
	 */
	m68ki_push_16(m68ki_aerr_write_mode | MPU_INSTR_MODE | m68ki_aerr_fc);
}





/* Used for Group 2 exceptions.
 * These stack a type 2 frame on the 020.
 */
INLINE void m68ki_exception_trap(uint vector)
{
	uint sr = m68ki_init_exception();

//	m68ki_stack_frame_0000(REG_PC, sr, vector);
	m68ki_stack_frame_3word(REG_PC, sr);
	m68ki_jump_vector(vector);

	/* Use up some clock cycles */
	USE_CYCLES(34/*CYC_EXCEPTION[vector]*/);/*34==c68kタブン、テキトー*/
}

#if 0
/* Trap#n stacks a 0 frame but behaves like group2 otherwise */
INLINE void m68ki_exception_trapN(uint vector)
{
	uint sr = m68ki_init_exception();
//	m68ki_stack_frame_0000(REG_PC, sr, vector);
	m68ki_stack_frame_3word(REG_PC, sr);
	m68ki_jump_vector(vector);

	/* Use up some clock cycles */
	USE_CYCLES(CYC_EXCEPTION[vector]);
}
#else
	/*タブン、テキトー*/
	#define m68ki_exception_trapN m68ki_exception_trap
#endif

#if (1==M68K_EMULATE_TRACE)
/* Exception for trace mode */
INLINE void m68ki_exception_trace(void)
{
	uint sr = m68ki_init_exception();

	//if(CPU_TYPE_IS_010_LESS(CPU_TYPE))
	{
		#if (1==M68K_EMULATE_ADDRESS_ERROR)
		//if(CPU_TYPE_IS_000(CPU_TYPE))
		{
			MPU_INSTR_MODE = INSTRUCTION_NO;
		}
		#endif // (1==M68K_EMULATE_ADDRESS_ERROR)
	//	m68ki_stack_frame_0000(REG_PC, sr, EXCEPTION_TRACE);
		m68ki_stack_frame_3word(REG_PC, sr);
	}
	//else
	//	m68ki_stack_frame_0010(sr, EXCEPTION_TRACE);

	m68ki_jump_vector(EXCEPTION_TRACE);

	/* Trace nullifies a STOP instruction */
	MPU_STOPPED &= ~STOP_LEVEL_STOP;

	/* Use up some clock cycles */
	USE_CYCLES(34/*39*//*CYC_EXCEPTION[EXCEPTION_TRACE]*/);
}
#endif // (1==M68K_EMULATE_TRACE)

/* Exception for privilege violation */
INLINE void m68ki_exception_privilege_violation(void)
{
	uint sr = m68ki_init_exception();

	#if (1==M68K_EMULATE_ADDRESS_ERROR)
	//if(CPU_TYPE_IS_000(CPU_TYPE))
	{
		MPU_INSTR_MODE = INSTRUCTION_NO;
	}
	#endif // (1==M68K_EMULATE_ADDRESS_ERROR)

	#if (1==M68K_USE_PPC)
//	m68ki_stack_frame_0000(REG_PPC, sr, EXCEPTION_PRIVILEGE_VIOLATION);
	m68ki_stack_frame_3word(REG_PPC, sr);
	#else
	m68ki_stack_frame_3word(REG_PC, sr);
	#endif //(1==M68K_USE_PPC)
	m68ki_jump_vector(EXCEPTION_PRIVILEGE_VIOLATION);

	/* Use up some clock cycles and undo the instruction's cycles */
	USE_CYCLES(34/*CYC_EXCEPTION[EXCEPTION_PRIVILEGE_VIOLATION]*/ - CYC_INSTRUCTION[REG_IR]);
}

/* Exception for A-Line instructions */
INLINE void m68ki_exception_1010(void)
{
	uint sr;
#if (M68K_LOG_1010_1111 == OPT_ON)
	M68K_DO_LOG_EMU((M68K_LOG_FILEHANDLE "%s at %08x: called 1010 instruction %04x (%s)\n",
					 m68ki_cpu_names[CPU_TYPE], ADDRESS_68K(REG_PPC), REG_IR,
					 m68ki_disassemble_quick(ADDRESS_68K(REG_PPC))));
#endif
	sr = m68ki_init_exception();
	#if (1==M68K_USE_PPC)
//	m68ki_stack_frame_0000(REG_PPC, sr, EXCEPTION_1010);
	m68ki_stack_frame_3word(REG_PPC, sr);
	#else
	m68ki_stack_frame_3word(REG_PC, sr);
	#endif //(1==M68K_USE_PPC)
	m68ki_jump_vector(EXCEPTION_1010);

	#if 0
	/* Use up some clock cycles and undo the instruction's cycles */
	USE_CYCLES(4/*CYC_EXCEPTION[EXCEPTION_1010]*/ - CYC_INSTRUCTION[REG_IR]);
	#else
	USE_CYCLES(34);/*34==c68kタブン、テキトー*/
	#endif
}

/* Exception for F-Line instructions */
INLINE void m68ki_exception_1111(void)
{
	uint sr;
#if M68K_LOG_1010_1111 == OPT_ON
	M68K_DO_LOG_EMU((M68K_LOG_FILEHANDLE "%s at %08x: called 1111 instruction %04x (%s)\n",
					 m68ki_cpu_names[CPU_TYPE], ADDRESS_68K(REG_PPC), REG_IR,
					 m68ki_disassemble_quick(ADDRESS_68K(REG_PPC))));
#endif
	sr = m68ki_init_exception();
	#if (1==M68K_USE_PPC)
//	m68ki_stack_frame_0000(REG_PPC, sr, EXCEPTION_1111);
	m68ki_stack_frame_3word(REG_PPC, sr);
	#else
	m68ki_stack_frame_3word(REG_PC, sr);
	#endif //(1==M68K_USE_PPC)
	m68ki_jump_vector(EXCEPTION_1111);

	#if 0
	/* Use up some clock cycles and undo the instruction's cycles */
	USE_CYCLES(4/*CYC_EXCEPTION[EXCEPTION_1111]*/ - CYC_INSTRUCTION[REG_IR]);
	#else
	USE_CYCLES(34);/*34==c68kタブン、テキトー*/
	#endif
}

/* Exception for illegal instructions */
INLINE void m68ki_exception_illegal(void)
{
	uint sr;
	#if 0
	M68K_DO_LOG((M68K_LOG_FILEHANDLE "%s at %08x: illegal instruction %04x (%s)\n",
				 m68ki_cpu_names[CPU_TYPE], ADDRESS_68K(REG_PPC), REG_IR,
				 m68ki_disassemble_quick(ADDRESS_68K(REG_PPC))));
	#endif
	sr = m68ki_init_exception();

	#if (1==M68K_EMULATE_ADDRESS_ERROR)
	//if(CPU_TYPE_IS_000(CPU_TYPE))
	{
		MPU_INSTR_MODE = INSTRUCTION_NO;
	}
	#endif // (1==M68K_EMULATE_ADDRESS_ERROR)

	#if (1==M68K_USE_PPC)
//	m68ki_stack_frame_0000(REG_PPC, sr, EXCEPTION_ILLEGAL_INSTRUCTION);
	m68ki_stack_frame_3word(REG_PPC, sr);
	#else
	m68ki_stack_frame_3word(REG_PC, sr);
	#endif //(1==M68K_USE_PPC)
	m68ki_jump_vector(EXCEPTION_ILLEGAL_INSTRUCTION);

	/* Use up some clock cycles and undo the instruction's cycles */
	USE_CYCLES(34/*CYC_EXCEPTION[EXCEPTION_ILLEGAL_INSTRUCTION]*/ - CYC_INSTRUCTION[REG_IR]);
}

#if 0/*???*/
/* 010 020 以降 */
/* Exception for format errror in RTE */
INLINE void m68ki_exception_format_error(void)
{
	uint sr = m68ki_init_exception();
//	m68ki_stack_frame_0000(REG_PC, sr, EXCEPTION_FORMAT_ERROR);
	m68ki_stack_frame_3word(REG_PC, sr);
	m68ki_jump_vector(EXCEPTION_FORMAT_ERROR);

	/* Use up some clock cycles and undo the instruction's cycles */
	USE_CYCLES(4/*CYC_EXCEPTION[EXCEPTION_FORMAT_ERROR]*/ - CYC_INSTRUCTION[REG_IR]);
}
#endif //0/*???*/

#if (1==M68K_EMULATE_ADDRESS_ERROR)
/* Exception for address error */
INLINE void m68ki_exception_address_error(void)
{
	uint sr = m68ki_init_exception();

	/* If we were processing a bus error, address error, or reset,
	 * this is a catastrophic failure.
	 * Halt the CPU
	 */
	if(MPU_RUN_MODE == RUN_MODE_BERR_AERR_RESET)
	{
		m68k_read_memory_8(0x00ffff01);
		MPU_STOPPED = STOP_LEVEL_HALT;
		return;
	}
	MPU_RUN_MODE = RUN_MODE_BERR_AERR_RESET;

	/* Note: This is implemented for 68000 only! */
	m68ki_stack_frame_buserr(sr);

	m68ki_jump_vector(EXCEPTION_ADDRESS_ERROR);

	/* Use up some clock cycles and undo the instruction's cycles */
	USE_CYCLES(50/*CYC_EXCEPTION[EXCEPTION_ADDRESS_ERROR]*/ - CYC_INSTRUCTION[REG_IR]);
}
#endif // (1==M68K_EMULATE_ADDRESS_ERROR)

#if 1/*???*/
/* Service an interrupt request and start exception processing */
void m68ki_exception_interrupt(uint int_level)
{
	uint vector;
	uint sr;
	uint new_pc;

	#if (1==M68K_EMULATE_ADDRESS_ERROR)
	//if(CPU_TYPE_IS_000(CPU_TYPE))
	{
		MPU_INSTR_MODE = INSTRUCTION_NO;
	}
	#endif // (1==M68K_EMULATE_ADDRESS_ERROR)

	/* Turn off the stopped state */
	MPU_STOPPED &= ~STOP_LEVEL_STOP;

	/* If we are halted, don't do anything */
	if(MPU_STOPPED)
		return;

	/* Acknowledge the interrupt */
	vector = m68ki_int_ack(int_level);

	/* Get the interrupt vector */
	switch(vector){
	case M68K_INT_ACK_AUTOVECTOR:
		/* Use the autovectors.  This is the most commonly used implementation */
		vector = EXCEPTION_INTERRUPT_AUTOVECTOR+int_level;
		break;
#if 0/*???*/
	case M68K_INT_ACK_SPURIOUS:
		/* Called if no devices respond to the interrupt acknowledge */
		vector = EXCEPTION_SPURIOUS_INTERRUPT;
		break;
#endif //0/*???*/
	}
//	if(vector > 255)
//	{
//		M68K_DO_LOG_EMU((M68K_LOG_FILEHANDLE "%s at %08x: Interrupt acknowledge returned invalid vector $%x\n",
//				 m68ki_cpu_names[CPU_TYPE], ADDRESS_68K(REG_PC), vector));
//		return;
//	}

	/* Start exception processing */
	sr = m68ki_init_exception();

	/* Set the interrupt mask to the level of the one being serviced */
	FLAG_INT_MASK = int_level<<8;

	/* Get the new PC */
	new_pc = m68ki_read_data_32((vector<<2) /*+REG_VBR*/);
#if 0/*???*/
	/* If vector is uninitialized, call the uninitialized interrupt vector */
	if(new_pc == 0)
	{	new_pc = m68ki_read_data_32((EXCEPTION_UNINITIALIZED_INTERRUPT<<2) /*+REG_VBR*/);}
#endif //0/*???*/
	/* Generate a stack frame */
//	m68ki_stack_frame_0000(REG_PC, sr, vector);
	m68ki_stack_frame_3word(REG_PC, sr);
//	if(FLAG_M && CPU_TYPE_IS_EC020_PLUS(CPU_TYPE))
//	{
//		/* Create throwaway frame */
//		m68ki_set_sm_flag(FLAG_S);	/* clear M */
//		sr |= 0x2000; /* Same as SR in master stack frame except S is forced high */
//		m68ki_stack_frame_0001(REG_PC, sr, vector);
//	}

	m68ki_jump(new_pc);

	/* Defer cycle counting until later */
	#if 1
	/* 少し小さい */
	if(vector<48)	/*CPU_INT_CYCLES +=*/ USE_CYCLES(CYC_EXCEPTION[vector]);
	else			/*CPU_INT_CYCLES +=*/ USE_CYCLES(4);
	#else
	/*CPU_INT_CYCLES +=*/ USE_CYCLES((vector<48)?(CYC_EXCEPTION[vector]):(4));
	#endif

#if (0==M68K_EMULATE_INT_ACK)
	/* Automatically clear IRQ if we are not using an acknowledge scheme */
	CPU_INT_LEVEL = 0;
#endif /* M68K_EMULATE_INT_ACK */
}
#endif //0/*???*/

/* ASG: Check for interrupts */
INLINE void m68ki_check_interrupts(void)
{
	if(CPU_INT_LEVEL > FLAG_INT_MASK)
	{	m68ki_exception_interrupt(CPU_INT_LEVEL>>8);}
}

/* Set the status register and check for interrupts */
INLINE void m68ki_set_sr(uint value)
{
	m68ki_set_sr_noint(value);
	m68ki_check_interrupts();
}
