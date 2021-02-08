//#include <s tdio.h>
//#include <s tdlib.h>
//#include "m 68k.h"
//#include "m 68000.h"
//#include "s tate.h"

/* global access */

struct m68k_memory_interface m68k_memory_intf;
offs_t m68k_encrypted_opcode_start[MAX_CPU];
offs_t m68k_encrypted_opcode_end[MAX_CPU];


void m68k_set_encrypted_opcode_range(int cpunum, offs_t start, offs_t end)
{
	m68k_encrypted_opcode_start[cpunum] = start;
	m68k_encrypted_opcode_end[cpunum] = end;
}




#ifndef A68K0

/****************************************************************************
 * 16-bit data memory interface
 ****************************************************************************/

static UINT32 readlong_d16(offs_t address)
{
	UINT32 result = program_read_word_16be(address) << 16;
	return result | program_read_word_16be(address + 2);
}

static void writelong_d16(offs_t address, UINT32 data)
{
	program_write_word_16be(address, data >> 16);
	program_write_word_16be(address + 2, data);
}

/* interface for 24-bit address bus, 16-bit data bus (68000, 68010) */
static const struct m68k_memory_interface interface_d16 =
{
	0,							/*opcode_xor*/
	program_read_byte_16be, 	/*read8*/
	program_read_word_16be, 	/*read16*/
	readlong_d16,				/*read32*/
	program_write_word_16be,	/*write16*/
	writelong_d16,				/*write32*/
	program_write_byte_16be,	/*write8*/
};
//1EEAA6
#endif // A68K0

/****************************************************************************
 * 32-bit data memory interface
 ****************************************************************************/

#ifndef A68K2

/* global access */
struct m68k_memory_interface m68k_memory_intf;

#endif // A68K2


static void set_irq_line(int irqline, int state)
{
	if (irqline == INPUT_LINE_NMI)
		irqline = 7;
	switch(state)
	{
		case CLEAR_LINE:
			m68k_set_irq(0);
			break;
		case ASSERT_LINE:
			m68k_set_irq(irqline);
			break;
		default:
			m68k_set_irq(irqline);
			break;
	}
}


/****************************************************************************
 * 68000 section
 ****************************************************************************/

#ifndef A68K0

static void m68000_init(void)
{
	m68k_init();
//	m68k_set_cpu_type(M68K_CPU_TYPE_68000);
	m68k_memory_intf = interface_d16;
	m68k_state_register("m68000");
}

static void m68000_reset(void* param)
{
	m68k_pulse_reset();
}

static void m68000_exit(void)
{
	/* nothing to do */
}

static int m68000_execute(int cycles)
{
	return m68k_execute(cycles);
}

static void m68000_get_context(void *arg_dst)
{
	m68k_get_context(arg_dst);
}

static void m68000_set_context(void *arg_src)
{
	if (m68k_memory_intf.read8 != program_read_byte_16be)
		m68k_memory_intf = interface_d16;
	m68k_set_context(arg_src);
}

//static offs_t m68000_dasm(char *buffer, offs_t pc)
//{
//	M68K_SET_PC_CALLBACK(pc);
//#ifdef MAME_DEBUG
//	return m68k_disassemble( buffer, pc, M68K_CPU_TYPE_68000 );
//#else
//	sprintf( buffer, "$%04X", m68k_read_immediate_16(pc) );
//	return 2;
//#endif
//}


#endif // A68K0



//#ifndef A68K0

/**************************************************************************
 * Generic set_info
 **************************************************************************/

static void m68000_set_info(UINT32 state, union cpuinfo *info)
{
	switch (state)
	{
		/* --- the following bits of info are set as 64-bit signed integers --- */
		case CPUINFO_INT_INPUT_STATE + 0:			set_irq_line(0, info->i);					break;
		case CPUINFO_INT_INPUT_STATE + 1:			set_irq_line(1, info->i);					break;
		case CPUINFO_INT_INPUT_STATE + 2:			set_irq_line(2, info->i);					break;
		case CPUINFO_INT_INPUT_STATE + 3:			set_irq_line(3, info->i);					break;
		case CPUINFO_INT_INPUT_STATE + 4:			set_irq_line(4, info->i);					break;
		case CPUINFO_INT_INPUT_STATE + 5:			set_irq_line(5, info->i);					break;
		case CPUINFO_INT_INPUT_STATE + 6:			set_irq_line(6, info->i);					break;
		case CPUINFO_INT_INPUT_STATE + 7:			set_irq_line(7, info->i);					break;

		case CPUINFO_INT_PC:						m68k_set_reg(M68K_REG_PC, info->i&0x00ffffff); break;
		case CPUINFO_INT_REGISTER + M68K_PC:		m68k_set_reg(M68K_REG_PC, info->i); 		break;
		case CPUINFO_INT_SP:
		case CPUINFO_INT_REGISTER + M68K_SP:		m68k_set_reg(M68K_REG_SP, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_ISP:		m68k_set_reg(M68K_REG_ISP, info->i);		break;
		case CPUINFO_INT_REGISTER + M68K_USP:		m68k_set_reg(M68K_REG_USP, info->i);		break;
		case CPUINFO_INT_REGISTER + M68K_SR:		m68k_set_reg(M68K_REG_SR, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_D0:		m68k_set_reg(M68K_REG_D0, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_D1:		m68k_set_reg(M68K_REG_D1, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_D2:		m68k_set_reg(M68K_REG_D2, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_D3:		m68k_set_reg(M68K_REG_D3, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_D4:		m68k_set_reg(M68K_REG_D4, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_D5:		m68k_set_reg(M68K_REG_D5, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_D6:		m68k_set_reg(M68K_REG_D6, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_D7:		m68k_set_reg(M68K_REG_D7, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_A0:		m68k_set_reg(M68K_REG_A0, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_A1:		m68k_set_reg(M68K_REG_A1, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_A2:		m68k_set_reg(M68K_REG_A2, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_A3:		m68k_set_reg(M68K_REG_A3, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_A4:		m68k_set_reg(M68K_REG_A4, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_A5:		m68k_set_reg(M68K_REG_A5, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_A6:		m68k_set_reg(M68K_REG_A6, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_A7:		m68k_set_reg(M68K_REG_A7, info->i); 		break;
		case CPUINFO_INT_REGISTER + M68K_PREF_ADDR: m68k_set_reg(M68K_REG_PREF_ADDR, info->i);	break;
		/* --- the following bits of info are set as pointers to data or functions --- */
		case CPUINFO_PTR_IRQ_CALLBACK:				m68k_set_int_ack_callback(info->irqcallback); break;
#if (1==M68K_USE_SYS16_CALLBACK)
		// CPUINFO_PTR_M68K_RESET_CALLBACK outrun/cotton/x-board ‚ÅŽg‚¤
		case CPUINFO_PTR_M68K_RESET_CALLBACK:		m68k_set_reset_instr_callback(info->f); 	break;

		// CPUINFO_PTR_M68K_CMPILD_CALLBACK sys16“™‚ÅŽg‚¤
		case CPUINFO_PTR_M68K_CMPILD_CALLBACK:		m68k_set_cmpild_instr_callback((void (*)(unsigned int,int))(info->f));		break;

		// CPUINFO_PTR_M68K_RTE_CALLBACK sys16“™‚ÅŽg‚¤
		case CPUINFO_PTR_M68K_RTE_CALLBACK: 		m68k_set_rte_instr_callback(info->f);		break;
#endif // (1==M68K_USE_SYS16_CALLBACK)
	}
}


/**************************************************************************
 * Generic get_info
 **************************************************************************/

void m68000_get_info(UINT32 state, union cpuinfo *info)
{
	switch (state)
	{
		/* --- the following bits of info are returned as 64-bit signed integers --- */
		case CPUINFO_INT_CONTEXT_SIZE:					info->i = m68k_get_context(NULL);		break;
		case CPUINFO_INT_INPUT_LINES:					info->i = 8;							break;
		case CPUINFO_INT_DEFAULT_IRQ_VECTOR:			info->i = -1;							break;
		case CPUINFO_INT_ENDIANNESS:					info->i = CPU_IS_BE;					break;
#if (1==PSP_2006)
/* ]—ˆŒÝŠ·—p */
		case CPUINFO_INT_CLOCK_DIVIDER: 				info->i = 1;							break;
		case CPUINFO_INT_MIN_INSTRUCTION_BYTES: 		info->i = 2;							break;
		case CPUINFO_INT_MAX_INSTRUCTION_BYTES: 		info->i = 10;							break;
		case CPUINFO_INT_MIN_CYCLES:					info->i = 4;							break;
		case CPUINFO_INT_MAX_CYCLES:					info->i = 158;							break;
#endif //(1==PSP_2006)
/* m68000 have program address spaces */
		case CPUINFO_INT_DATABUS_WIDTH + ADDRESS_SPACE_PROGRAM: info->i = 16;					break;
		case CPUINFO_INT_ADDRBUS_WIDTH + ADDRESS_SPACE_PROGRAM: info->i = 24;					break;
		#if (1==LINK_ADDRBUS_SHIFT)
		case CPUINFO_INT_ADDRBUS_SHIFT + ADDRESS_SPACE_PROGRAM: info->i = 0;					break;
		#endif //(1==LINK_ADDRBUS_SHIFT)
/* m68000 not have I/O address spaces */
		case CPUINFO_INT_DATABUS_WIDTH + ADDRESS_SPACE_IO:		info->i = 0;					break;
		case CPUINFO_INT_ADDRBUS_WIDTH + ADDRESS_SPACE_IO:		info->i = 0;					break;
		#if (1==LINK_ADDRBUS_SHIFT)
		case CPUINFO_INT_ADDRBUS_SHIFT + ADDRESS_SPACE_IO:		info->i = 0;					break;
		#endif //(1==LINK_ADDRBUS_SHIFT)
#if (1==LINK_DATA_REGION)
/* m68000 not have data address spaces */
		case CPUINFO_INT_DATABUS_WIDTH + ADDRESS_SPACE_DATA:	info->i = 0;					break;
		case CPUINFO_INT_ADDRBUS_WIDTH + ADDRESS_SPACE_DATA:	info->i = 0;					break;
		#if (1==LINK_ADDRBUS_SHIFT)
		case CPUINFO_INT_ADDRBUS_SHIFT + ADDRESS_SPACE_DATA:	info->i = 0;					break;
		#endif //(1==LINK_ADDRBUS_SHIFT)
#endif // (1==LINK_DATA_REGION)

		case CPUINFO_INT_INPUT_STATE + 0:				info->i = 0;	/* fix me */			break;
		case CPUINFO_INT_INPUT_STATE + 1:				info->i = 0;	/* fix me */			break;
		case CPUINFO_INT_INPUT_STATE + 2:				info->i = 0;	/* fix me */			break;
		case CPUINFO_INT_INPUT_STATE + 3:				info->i = 0;	/* fix me */			break;
		case CPUINFO_INT_INPUT_STATE + 4:				info->i = 0;	/* fix me */			break;
		case CPUINFO_INT_INPUT_STATE + 5:				info->i = 0;	/* fix me */			break;
		case CPUINFO_INT_INPUT_STATE + 6:				info->i = 0;	/* fix me */			break;
		case CPUINFO_INT_INPUT_STATE + 7:				info->i = 0;	/* fix me */			break;
#if (1==PSP_2006)
/* ]—ˆŒÝŠ·—p */
		case CPUINFO_INT_PREVIOUSPC:					info->i = m68k_get_reg(NULL, M68K_REG_PPC); break;
#endif //(1==PSP_2006)

		case CPUINFO_INT_PC:							info->i = m68k_get_reg(NULL, M68K_REG_PC)&0x00ffffff; break;
		case CPUINFO_INT_REGISTER + M68K_PC:			info->i = m68k_get_reg(NULL, M68K_REG_PC); break;
		case CPUINFO_INT_SP:
		case CPUINFO_INT_REGISTER + M68K_SP:			info->i = m68k_get_reg(NULL, M68K_REG_SP); break;
		case CPUINFO_INT_REGISTER + M68K_ISP:			info->i = m68k_get_reg(NULL, M68K_REG_ISP); break;
		case CPUINFO_INT_REGISTER + M68K_USP:			info->i = m68k_get_reg(NULL, M68K_REG_USP); break;
		case CPUINFO_INT_REGISTER + M68K_SR:			info->i = m68k_get_reg(NULL, M68K_REG_SR); break;
		case CPUINFO_INT_REGISTER + M68K_D0:			info->i = m68k_get_reg(NULL, M68K_REG_D0); break;
		case CPUINFO_INT_REGISTER + M68K_D1:			info->i = m68k_get_reg(NULL, M68K_REG_D1); break;
		case CPUINFO_INT_REGISTER + M68K_D2:			info->i = m68k_get_reg(NULL, M68K_REG_D2); break;
		case CPUINFO_INT_REGISTER + M68K_D3:			info->i = m68k_get_reg(NULL, M68K_REG_D3); break;
		case CPUINFO_INT_REGISTER + M68K_D4:			info->i = m68k_get_reg(NULL, M68K_REG_D4); break;
		case CPUINFO_INT_REGISTER + M68K_D5:			info->i = m68k_get_reg(NULL, M68K_REG_D5); break;
		case CPUINFO_INT_REGISTER + M68K_D6:			info->i = m68k_get_reg(NULL, M68K_REG_D6); break;
		case CPUINFO_INT_REGISTER + M68K_D7:			info->i = m68k_get_reg(NULL, M68K_REG_D7); break;
		case CPUINFO_INT_REGISTER + M68K_A0:			info->i = m68k_get_reg(NULL, M68K_REG_A0); break;
		case CPUINFO_INT_REGISTER + M68K_A1:			info->i = m68k_get_reg(NULL, M68K_REG_A1); break;
		case CPUINFO_INT_REGISTER + M68K_A2:			info->i = m68k_get_reg(NULL, M68K_REG_A2); break;
		case CPUINFO_INT_REGISTER + M68K_A3:			info->i = m68k_get_reg(NULL, M68K_REG_A3); break;
		case CPUINFO_INT_REGISTER + M68K_A4:			info->i = m68k_get_reg(NULL, M68K_REG_A4); break;
		case CPUINFO_INT_REGISTER + M68K_A5:			info->i = m68k_get_reg(NULL, M68K_REG_A5); break;
		case CPUINFO_INT_REGISTER + M68K_A6:			info->i = m68k_get_reg(NULL, M68K_REG_A6); break;
		case CPUINFO_INT_REGISTER + M68K_A7:			info->i = m68k_get_reg(NULL, M68K_REG_A7); break;
		case CPUINFO_INT_REGISTER + M68K_PREF_ADDR: 	info->i = m68k_get_reg(NULL, M68K_REG_PREF_ADDR); break;
		case CPUINFO_INT_REGISTER + M68K_PREF_DATA: 	info->i = m68k_get_reg(NULL, M68K_REG_PREF_DATA); break;

		/* --- the following bits of info are returned as pointers to data or functions --- */
		case CPUINFO_PTR_SET_INFO:						info->setinfo = m68000_set_info;		break;
		case CPUINFO_PTR_GET_CONTEXT:					info->getcontext = m68000_get_context;	break;
		case CPUINFO_PTR_SET_CONTEXT:					info->setcontext = m68000_set_context;	break;
		case CPUINFO_PTR_INIT:							info->init = m68000_init;				break;
		case CPUINFO_PTR_RESET: 						info->reset = m68000_reset; 			break;
		case CPUINFO_PTR_EXIT:							info->exit = m68000_exit;				break;
		case CPUINFO_PTR_EXECUTE:						info->execute = m68000_execute; 		break;
		case CPUINFO_PTR_BURN:							info->burn = NULL;						break;
#ifdef MAME_DEBUG
//		case CPUINFO_PTR_DISASSEMBLE:					info->disassemble = m68000_dasm;		break;
#endif /* MAME_DEBUG */
		case CPUINFO_PTR_IRQ_CALLBACK:					/* fix me */							break;
		#if (16==CPU_ICOUNT_LENGTH)
		case CPUINFO_PTR_INSTRUCTION_COUNTER16: 		info->icount16 = &m68k_ICount;			break;
		#else
		case CPUINFO_PTR_INSTRUCTION_COUNTER:			info->icount = &m68k_ICount;			break;
		#endif //(16==CPU_ICOUNT_LENGTH)

		/* --- the following bits of info are returned as NULL-terminated strings --- */
		case CPUINFO_STR_NAME:							info->s = (char *) "68000"; break;
#if (1==PSP_2006)
/* ]—ˆŒÝŠ·—p */
//		case CPUINFO_STR_CORE_FAMILY:					info->s = (char *) "Motorola 68K"; break;
//		case CPUINFO_STR_CORE_VERSION:					info->s = (char *) "3.2"; break;
//		case CPUINFO_STR_CORE_CREDITS:					info->s = (char *) "Copyright 1999-2000 Karl Stenerud. All rights reserved. (2.1 fixes HJB)"); break;
#endif //(1==PSP_2006)
	}
}

