#ifndef M68KMAME__HEADER
#define M68KMAME__HEADER

/* ======================================================================== */
/* ============================== MAME STUFF ============================== */
/* ======================================================================== */

//#include "driver.h"
//#include "cpuintrf.h"
//#include "mame_mem.h"
//#include "mame.h"//#include "mame dbg.h"

//#define _MAME_68K_INTERNAL_
//#include "m 68000.h"

//#define CALL_MAME_DEBUG ;

/* Configuration switches (see m68kconf.h for explanation) */
//#include "m 68kconf.h"

//#ifndef m68ki_remaining_cycles
//#define m68ki_remaining_cycles m68k_ICount
//#endif

extern struct m68k_memory_interface m68k_memory_intf;
extern offs_t m68k_encrypted_opcode_start[MAX_CPU];
extern offs_t m68k_encrypted_opcode_end[MAX_CPU];

#define m68k_read_memory_8(address) 		 (*m68k_memory_intf.read8)(address)
#define m68k_read_memory_16(address)		 (*m68k_memory_intf.read16)(address)
#define m68k_read_memory_32(address)		 (*m68k_memory_intf.read32)(address)

#if (1==M68K_SEPARATE_READS)
#define m68k_read_immediate_16(address) 	 m68kx_read_immediate_16(address)
#define m68k_read_immediate_32(address) 	 m68kx_read_immediate_32(address)
#define m68k_read_pcrelative_8(address) 	 m68kx_read_pcrelative_8(address)
#define m68k_read_pcrelative_16(address)	 m68kx_read_pcrelative_16(address)
#define m68k_read_pcrelative_32(address)	 m68kx_read_pcrelative_32(address)
#endif // (1==M68K_SEPARATE_READS)

#define m68k_read_disassembler_16(address)	 m68kx_read_immediate_16(address)
#define m68k_read_disassembler_32(address)	 m68kx_read_immediate_32(address)

#define m68k_write_memory_8(address, value)  (*m68k_memory_intf.write8)(address, value)
#define m68k_write_memory_16(address, value) (*m68k_memory_intf.write16)(address, value)
#define m68k_write_memory_32(address, value) (*m68k_memory_intf.write32)(address, value)
#define m68k_write_memory_32_pd(address, value) m68kx_write_memory_32_pd(address, value)

#if (1==M68K_SEPARATE_READS)
INLINE unsigned int m68k_read_immediate_16(unsigned int address);
INLINE unsigned int m68k_read_immediate_32(unsigned int address);

INLINE unsigned int m68k_read_pcrelative_8(unsigned int address);
INLINE unsigned int m68k_read_pcrelative_16(unsigned int address);
INLINE unsigned int m68k_read_pcrelative_32(unsigned int address);
#endif // (1==M68K_SEPARATE_READS)
INLINE void m68k_write_memory_32_pd(unsigned int address, unsigned int value);


INLINE unsigned int m68kx_read_immediate_16(unsigned int address)
{
	return cpu_readop16((address) ^ m68k_memory_intf.opcode_xor);
}

INLINE unsigned int m68kx_read_immediate_32(unsigned int address)
{
	return ((m68k_read_immediate_16(address) << 16) | m68k_read_immediate_16((address)+2));
}

INLINE unsigned int m68kx_read_pcrelative_8(unsigned int address)
{
	if (address >= m68k_encrypted_opcode_start[cpu_getactivecpu()] &&
			address < m68k_encrypted_opcode_end[cpu_getactivecpu()])
		return ((m68k_read_immediate_16(address&~1)>>(8*(1-(address & 1))))&0xff);
	else
		return m68k_read_memory_8(address);
}

INLINE unsigned int m68kx_read_pcrelative_16(unsigned int address)
{
	if (address >= m68k_encrypted_opcode_start[cpu_getactivecpu()] &&
			address < m68k_encrypted_opcode_end[cpu_getactivecpu()])
		return m68k_read_immediate_16(address);
	else
		return m68k_read_memory_16(address);
}

INLINE unsigned int m68kx_read_pcrelative_32(unsigned int address)
{
	if (address >= m68k_encrypted_opcode_start[cpu_getactivecpu()] &&
			address < m68k_encrypted_opcode_end[cpu_getactivecpu()])
		return m68k_read_immediate_32(address);
	else
		return m68k_read_memory_32(address);
}


/* Special call to simulate undocumented 68k behavior when move.l with a
 * predecrement destination mode is executed.
 * A real 68k first writes the high word to [address+2], and then writes the
 * low word to [address].
 */
INLINE void m68kx_write_memory_32_pd(unsigned int address, unsigned int value)
{
	(*m68k_memory_intf.write16)(address+2, value>>16);
	(*m68k_memory_intf.write16)(address, value&0xffff);
}


void m68k_set_encrypted_opcode_range(int cpunum, offs_t start, offs_t end);


/* ======================================================================== */
/* ============================== END OF FILE ============================= */
/* ======================================================================== */

#endif /* M68KMAME__HEADER */
