/* ======================================================================== */
/* ========================= LICENSING & COPYRIGHT ======================== */
/* ======================================================================== */
/*
 *									MUSASHI
 *								  Version 3.3
 *
 * A portable Motorola M680x0 processor emulation engine.
 * Copyright 1998-2001 Karl Stenerud.  All rights reserved.
 *
 * This code may be freely used for non-commercial purposes as long as this
 * copyright notice remains unaltered in the source code and any binary files
 * containing this code in compiled form.
 *
 * All other lisencing terms must be negotiated with the author
 * (Karl Stenerud).
 *
 * The latest version of this code can be obtained at:
 * http://kstenerud.cjb.net
 */



#ifndef M68KCONF__HEADER
#define M68KCONF__HEADER


/* Configuration switches.
 * Use OPT_SPECIFY_HANDLER for configuration options that allow callbacks.
 * OPT_SPECIFY_HANDLER causes the core to link directly to the function
 * or macro you specify, rather than using callback functions whose pointer
 * must be passed in using m68k_set_xxx_callback().
 */
#define OPT_OFF 			0
#define OPT_ON				1
#define OPT_SPECIFY_HANDLER 2


/* ======================================================================== */
/* ============================== MAME STUFF ============================== */
/* ======================================================================== */

/* If you're compiling this for MAME, only change M68K_COMPILE_FOR_MAME
 * to OPT_ON and use m68kmame.h to configure the 68k core.
 */
#ifndef M68K_COMPILE_FOR_MAME
#define M68K_COMPILE_FOR_MAME	 OPT_ON
#endif /* M68K_COMPILE_FOR_MAME */


//#if M68K_COMPILE_FOR_MAME == OPT_OFF

//////////////////////////////
//	MAME configuration	   ///
//////////////////////////////



/* ======================================================================== */
/* ============================= CONFIGURATION ============================ */
/* ======================================================================== */

/* Turn ON if you want to use the following M68K variants */
//#define M68K_EMULATE_008			OPT_ON
//#define M68K_EMULATE_010			OPT_ON
//#define M68K_EMULATE_EC020		OPT_ON
//#define M68K_EMULATE_020			OPT_ON


//#ifdef A68K0
//#define M68K_EMULATE_010			OPT_OFF
//#else
/* M68K Variants */
//#if HAS_M68008
//#define M68K_EMULATE_008			OPT_ON
//#else
#define M68K_EMULATE_008			OPT_OFF
//#endif

//#if HAS_M68010
//#define M68K_EMULATE_010			OPT_ON
//#else
#define M68K_EMULATE_010			OPT_OFF
//#endif

//#endif	// A68K0

//#ifdef A68K2
//#define M68K_EMULATE_EC020		OPT_OFF
//#define M68K_EMULATE_020			OPT_OFF
//#else
//#undef	M68K_EMULATE_010
//#define M68K_EMULATE_010			OPT_ON

//#if HAS_M68EC020
//#define M68K_EMULATE_EC020		OPT_ON
//#else
#define M68K_EMULATE_EC020			OPT_OFF
//#endif

//#if HAS_M68020
//#define M68K_EMULATE_020			OPT_ON
//#else
#define M68K_EMULATE_020			OPT_OFF
//#endif

//#endif // A68K2


/* If ON, the CPU will call m68k_read_immediate_xx() for immediate addressing
 * and m68k_read_pcrelative_xx() for PC-relative addressing.
 * If off, all read requests from the CPU will be redirected to m68k_read_xx()
 */
/* OFF: だとサイズ大きくなる。（遅い？）outrun 起動しない。 */
/* ON : だとサイズ小さくなる。 [mame標準] */

//#define M68K_SEPARATE_READS			OPT_OFF/*default*/
#ifndef M68K_SEPARATE_READS
	#define M68K_SEPARATE_READS 		OPT_ON/*mame*/
#endif // M68K_SEPARATE_READS


/* If ON, the CPU will call m68k_write_32_pd() when it executes move.l with a
 * predecrement destination EA mode instead of m68k_write_32().
 * To simulate real 68k behavior, m68k_write_32_pd() must first write the high
 * word to [address+2], and then write the low word to [address].
 */
/*	*/
//# -DM68K_SIMULATE_PD_WRITES=0 現状全く同じ
//#define M68K_SIMULATE_PD_WRITES		OPT_OFF
#ifndef M68K_SIMULATE_PD_WRITES
	#define M68K_SIMULATE_PD_WRITES 	OPT_ON/*mame*/
#endif // M68K_SIMULATE_PD_WRITES


/* If ON, CPU will call the interrupt acknowledge callback when it services an
 * interrupt.
 * If off, all interrupts will be autovectored and all interrupt requests will
 * auto-clear when the interrupt is serviced.
 */
/* 要するに IRQ mame なら ON で。  */
//#define M68K_EMULATE_INT_ACK		OPT_OFF
//#define M68K_INT_ACK_CALLBACK(A)	your_int_ack_handler_function(A)

#define M68K_EMULATE_INT_ACK		OPT_ON/*mame*/
#define M68K_INT_ACK_CALLBACK(A)

/* If ON, CPU will call the breakpoint acknowledge callback when it encounters
 * a breakpoint instruction and it is running a 68010+.
 */
/* m68010 以降のハードウェアーブレークポイント機能 */
//#define M68K_EMULATE_BKPT_ACK 	OPT_OFF
//#define M68K_BKPT_ACK_CALLBACK()	your_bkpt_ack_handler_function()

#define M68K_EMULATE_BKPT_ACK		OPT_OFF/*mame*/
#define M68K_BKPT_ACK_CALLBACK()

/* If ON, the CPU will monitor the trace flags and take trace exceptions
 */
/* ハードウェアーデバッグ機能 */
//#define M68K_EMULATE_TRACE		OPT_OFF
#define M68K_EMULATE_TRACE			OPT_OFF/*mame*/


/* If ON, CPU will call the output reset callback when it encounters a reset
 * instruction.
 */
/*	system16等のリセットコールバック */
//#define M68K_EMULATE_RESET		OPT_OFF
//#define M68K_RESET_CALLBACK() 	your_reset_handler_function()

#define M68K_EMULATE_RESET			OPT_ON/*mame*/
#define M68K_RESET_CALLBACK()


/* If ON, CPU will call the callback when it encounters a cmpi.l #v, dn
 * instruction.
 */
/*	system16等のFD1094のコールバックとか用の機能っぽい */
//#define M68K_CMPILD_HAS_CALLBACK	 OPT_OFF
//#define M68K_CMPILD_CALLBACK(v,r)  your_cmpild_handler_function(v,r)

#define M68K_CMPILD_HAS_CALLBACK	 OPT_ON/*mame*/
#define M68K_CMPILD_CALLBACK()

/* If ON, CPU will call the callback when it encounters a rte
 * instruction.
 */
/*	system16等のFD1094のコールバックとか用の機能っぽい */
//#define M68K_RTE_HAS_CALLBACK 	OPT_OFF
//#define M68K_RTE_CALLBACK()		your_rte_handler_function()

#define M68K_RTE_HAS_CALLBACK		OPT_ON/*mame*/
#define M68K_RTE_CALLBACK()

/* If ON, CPU will call the callback when it encounters a tas
 * instruction.
 */
/* メガドラ用機能っぽい */
//#define M68K_TAS_HAS_CALLBACK 	OPT_OFF
//#define M68K_TAS_CALLBACK()		your_tas_handler_function()

#define M68K_TAS_HAS_CALLBACK		OPT_OFF/*OPT_ON*//*mame*/
#define M68K_TAS_CALLBACK()

/* If ON, CPU will call the set fc callback on every memory access to
 * differentiate between user/supervisor, program/data access like a real
 * 68000 would.  This should be enabled and the callback should be set if you
 * want to properly emulate the m68010 or higher. (moves uses function codes
 * to read/write data from different address spaces)
 */
/* つか用途不明(???) */
//#define M68K_EMULATE_FC			OPT_OFF
//#define M68K_SET_FC_CALLBACK(A)	your_set_fc_handler_function(A)

#define M68K_EMULATE_FC 			OPT_OFF/*mame*/
#define M68K_SET_FC_CALLBACK(A)


/* If ON, CPU will call the pc changed callback when it changes the PC by a
 * large value.  This allows host programs to be nicer when it comes to
 * fetching immediate data and instructions on a banked memory system.
 */
/* 未実験 */
//#define M68K_MONITOR_PC			OPT_OFF
//#define M68K_SET_PC_CALLBACK(A)	your_pc_changed_handler_function(A)

#define M68K_MONITOR_PC 			OPT_SPECIFY_HANDLER/*mame*/
#define M68K_SET_PC_CALLBACK(A) 	change_pc(A)/*mame*/


/* If ON, CPU will call the instruction hook callback before every
 * instruction.
 */
/* 未実験 */
//#define M68K_INSTRUCTION_HOOK 	OPT_OFF
//#define M68K_INSTRUCTION_CALLBACK() your_instruction_hook_function()

#define M68K_INSTRUCTION_HOOK		OPT_SPECIFY_HANDLER/*mame*/
#define M68K_INSTRUCTION_CALLBACK() CALL_MAME_DEBUG/*mame*/


/* If ON, the CPU will emulate the 4-byte prefetch queue of a real 68000 */
/* 未実験 */
//#define M68K_EMULATE_PREFETCH 	OPT_OFF
#define M68K_EMULATE_PREFETCH		OPT_ON/*mame*/


/* If ON, the CPU will generate address error exceptions if it tries to
 * access a word or longword at an odd address.
 * NOTE: This is only emulated properly for 68000 mode.
 */
/* 未実験 */
//#define M68K_EMULATE_ADDRESS_ERROROPT_OFF
#define M68K_EMULATE_ADDRESS_ERROR	OPT_OFF/*mame*/


/* Turn ON to enable logging of illegal instruction calls.
 * M68K_LOG_FILEHANDLE must be #defined to a stdio file stream.
 * Turn on M68K_LOG_1010_1111 to log all 1010 and 1111 calls.
 */
/* 未実験 */
//#define M68K_LOG_ENABLE			OPT_OFF
#define M68K_LOG_ENABLE 			OPT_OFF/*mame*/
//#define M68K_LOG_1010_1111		OPT_OFF
#define M68K_LOG_1010_1111			OPT_OFF/*mame*/
//#define M68K_LOG_FILEHANDLE		some_file_handle
#define M68K_LOG_FILEHANDLE 		errorlog/*mame*/

/* ----------------------------- COMPATIBILITY ---------------------------- */

/* The following options set optimizations that violate the current ANSI
 * standard, but will be compliant under the forthcoming C9X standard.
 */


/* If ON, the enulation core will use 64-bit integers to speed up some
 * operations.
*/
/* 未実験 */
//#define M68K_USE_64_BIT			OPT_OFF
#define M68K_USE_64_BIT 			OPT_OFF/*mame*/


/* Set to your compiler's static inline keyword to enable it, or
 * set it to blank to disable it.
 * If you define INLINE in the makefile, it will override this value.
 * NOTE: not enabling inline functions will SEVERELY slow down emulation.
 */
//#ifndef INLINE
//#define INLINE static __inline__
//#endif /* INLINE */

//#endif /* M68K_COMPILE_FOR_MAME */



/* 何故か"m68kconf.h"に書いてもダメ。（謎） */

//#ifdef turbo_outrun
//	  #define M68K_FD1094_TYPE			OPT_ON
//#endif // turbo_outrun

//#ifdef cotton
//	  #define M68K_FD1094_TYPE			OPT_ON
//#endif // cotton

#ifndef M68K_FD1094_TYPE
	#define M68K_FD1094_TYPE			OPT_OFF
#endif // M68K_FD1094_TYPE


/* TAS命令、標準は「（速いから）セットしない」になってるので注意。 */
//#ifdef darius
//	/* DARIUS は SET しないと起動出来ない */
//	#define M68K_TAS_DO_SET 			OPT_ON
//#endif // darius

//#ifdef syvalion
///*効果不明...*/
//	/* DARIUS は SET しないと起動出来ない */
//	#define M68K_TAS_DO_SET 			OPT_ON
//#endif // syvalion

#ifndef M68K_TAS_DO_SET
	#define M68K_TAS_DO_SET 			OPT_OFF
#endif // M68K_TAS_DO_SET

/* (IRQ以外の)call back機能は (sys16系以外) mame で使ってない(っぽい) */
#ifndef M68K_USE_CALLBACK
	#define M68K_USE_CALLBACK			OPT_OFF/*OPT_OFF*/
#endif // M68K_USE_CALLBACK

/* system16 系でしか使ってない callback */
/* (CPUINFO_PTR_M68K_RESET_CALLBACK/CPUINFO_PTR_M68K_CMPILD_CALLBACK/CPUINFO_PTR_M68K_RTE_CALLBACK) */
#ifndef M68K_USE_SYS16_CALLBACK
	#define M68K_USE_SYS16_CALLBACK 	OPT_OFF/*OPT_OFF*/
#endif // M68K_USE_SYS16_CALLBACK

/* PPC == previous program counter (「現在のPC」ではなくて「直前のPC」 / デバッグ、プロテクト解除用) */
#ifndef M68K_USE_PPC
	#define M68K_USE_PPC 	/*OPT_ON*/OPT_OFF/*OPT_OFF*/
#endif // M68K_USE_PPC



/* ======================================================================== */
/* ============================== END OF FILE ============================= */
/* ======================================================================== */

#endif /* M68KCONF__HEADER */
