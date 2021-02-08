# Hello World for PSP
# 2005.04.30  created by nem 

		.set noreorder

		.text

		.extern xmain


##############################################################################


		.ent _start
		.weak _start
_start:
		addiu 	$sp, 0x10
		sw		$ra, 0($sp)	
		sw		$16, 4($sp)
		sw		$17, 8($sp)

		la     $2,_gp
		move   $gp,$2

		move	$16, $4				# Save args
		move	$17, $5

		la	$4, _main_thread_name	# Main thread setup
		la		$5, xmain
		li		$6, 0x20				# Priority
		li		$7, 0x40000			# Stack size
		lui		$8, 0x8000				# Attributes
		jal		sceKernelCreateThread
		move	$9, $0

		move	$4, $2				# Start thread
		move	$5, $16
		jal		sceKernelStartThread
		move	$6, $17

		lw		$ra, 0($sp)
		lw		$16, 4($sp)
		lw		$17, 8($sp)
		move	$2, $0
		jr 		$ra
		addiu	$sp, 0x10

_main_thread_name:
		.asciiz	"user_main"

##############################################################################


		.section	.lib.ent,"wa",@progbits
__lib_ent_top:
		.word 0
		.word 0x80000000
		.word 0x00010104
		.word __entrytable


		.section	.lib.ent.btm,"wa",@progbits
__lib_ent_bottom:
		.word	0


		.section	.lib.stub,"wa",@progbits
__lib_stub_top:


		.section	.lib.stub.btm,"wa",@progbits
__lib_stub_bottom:
		.word	0


##############################################################################

		.section	".xodata.sceModuleInfo","wa",@progbits

__moduleinfo:
		.byte	0,0,1,1			#user mode
#		.byte	0,16,1,1		#kernel mode?

		.ascii	"HelloWorld"		#up to 28 char
		.align	5

		.word	_gp
		.word	__lib_ent_top
		.word	__lib_ent_bottom
		.word	__lib_stub_top
		.word	__lib_stub_bottom

##############################################################################

		.section	.rodata.entrytable,"wa",@progbits
__entrytable:
		.word 0xD632ACDB
		.word 0xF01D73A7
		.word _start
		.word __moduleinfo
		.word 0



###############################################################################

		.data


###############################################################################

		.bss


###############################################################################


	.macro	STUB_START	module,d1,d2

		.section	.rodata.stubmodulename
		.word	0
__stub_modulestr_\@:
		.asciz	"\module"
		.align	2

		.section	.lib.stub
		.word __stub_modulestr_\@
		.word \d1
		.word \d2
		.word __stub_idtable_\@
		.word __stub_text_\@

		.section	.rodata.stubidtable
__stub_idtable_\@:

		.section	.text.stub
__stub_text_\@:

	.endm


	.macro	STUB_END
	.endm


	.macro	STUB_FUNC	funcid,funcname

		.set push
		.set noreorder

		.section	.text.stub
		.weak	\funcname
\funcname:
		jr	$ra
		nop

		.section	.rodata.stubidtable
		.word	\funcid

		.set pop

	.endm


	STUB_START	"sceDisplay",0x40010000,0x00050005
	STUB_FUNC	0x0E20F177,sceDisplaySetMode
	STUB_FUNC	0x289D82FE,sceDisplaySetFrameBuf
	STUB_FUNC	0x984C27E7,sceDisplayWaitVblankStart
	STUB_FUNC	0x36cdfade,sceDisplayWaitVblank
	STUB_FUNC	0xdba6c4c4,sceDisplayGetFramePerSec
	STUB_END

	STUB_START	"sceCtrl",0x40010000,0x00040005 
	STUB_FUNC	0x6a2774f3,sceCtrlInit 
	STUB_FUNC	0x1f4011e6,sceCtrlSetAnalogMode 
	STUB_FUNC	0x1f803938,sceCtrlReadBufferPositive
	STUB_FUNC	0x3a622550,sceCtrlPeekBufferPositive
	STUB_END 

	STUB_START   "IoFileMgrForUser",0x40010000,0x000D0005 
	STUB_FUNC   0xb29ddf9c,sceIoDopen
	STUB_FUNC   0xe3eb004c,sceIoDread
	STUB_FUNC   0xeb092469,sceIoDclose
	STUB_FUNC   0x6a638d83,sceIoRead
	STUB_FUNC   0x42ec03ac,sceIoWrite
	STUB_FUNC   0x27eb27b8,sceIoLseek
	STUB_FUNC   0x810c4bc3,sceIoClose
	STUB_FUNC   0x109f50bc,sceIoOpen
	STUB_FUNC   0xF27A9C51,sceIoRemove
	STUB_FUNC   0x6A70004,sceIoMkdir
	STUB_FUNC   0x1117C65F,sceIoRmdir
	STUB_FUNC   0x54F5FB11,sceIoDevctl
	STUB_FUNC   0x779103A0,sceIoRename
	STUB_END  

	STUB_START	"LoadExecForUser",0x40010000,0x20005
	STUB_FUNC	0x5572A5F,sceKernelExitGame
	STUB_FUNC	0x4AC57943,sceKernelRegisterExitCallback
	STUB_END

	STUB_START	"scePower",0x40010000,0x20005 
	STUB_FUNC	0x4B7766E,scePowerRegisterCallback
	STUB_FUNC	0x737486F2,scePowerSetClockFrequency
	STUB_END

	STUB_START	"sceAudio",0x40010000,0x00090005 
	STUB_FUNC	0x136CAF51,sceAudioOutputBlocking
	STUB_FUNC	0xE2D56B2D,sceAudioOutputPanned
	STUB_FUNC	0x13F592BC,sceAudioOutputPannedBlocking
	STUB_FUNC	0x5EC81C55,sceAudioChReserve
	STUB_FUNC	0x6FC46853,sceAudioChRelease
	STUB_FUNC	0xE9D97901,sceAudioGetChannelRestLen
	STUB_FUNC	0xCB2E439E,sceAudioSetChannelDataLen
	STUB_FUNC	0x95FD0C2D,sceAudioChangeChannelConfig
	STUB_FUNC	0xB7E1D8E7,sceAudioChangeChannelVolume
	STUB_END

	STUB_START	"UtilsForUser",0x40010000,0x00040005
	STUB_FUNC	0x91E4F6A7,sceKernelLibcClock
	STUB_FUNC	0x27CC57F0,sceKernelLibcTime
	STUB_FUNC	0x71EC4271,sceKernelLibcGettimeofday
	STUB_FUNC	0x79D1C3FA,sceKernelDcacheWritebackAll
	STUB_END

	STUB_START	"ThreadManForUser",0x40010000,0x000F0005
	STUB_FUNC	0x446D8DE6,sceKernelCreateThread
	STUB_FUNC	0xF475845D,sceKernelStartThread
        STUB_FUNC       0xAA73C935,sceKernelExitThread 
	STUB_FUNC	0x9ACE131E,sceKernelSleepThread
        STUB_FUNC       0x55C20A00,sceKernelCreateEventFlag 
        STUB_FUNC       0xEF9E4C70,sceKernelDeleteEventFlag 
        STUB_FUNC       0x1FB15A32,sceKernelSetEventFlag 
        STUB_FUNC       0x812346E4,sceKernelClearEventFlag 
        STUB_FUNC       0x402FCF22,sceKernelWaitEventFlag 
	STUB_FUNC	0x82826F70,sceKernelPollCallbacks
	STUB_FUNC	0xE81CAF8F,sceKernelCreateCallback
        STUB_FUNC       0x278C0DF5,sceKernelWaitThreadEnd
        STUB_FUNC       0x9FA03CD3,sceKernelDeleteThread
	STUB_FUNC	0xceadeb47,sceKernelDelayThread
	STUB_FUNC	0xbd123d9e,sceKernelDelaySysClockThread
	STUB_END

	STUB_START	"SysMemUserForUser",0x40000000,0x00050005
	STUB_FUNC	0x237DBD4F,sceKernelAllocPartitionMemory
	STUB_FUNC	0x9D9A5BA1,sceKernelGetBlockHeadAddr
	STUB_FUNC	0xB6D61D02,sceKernelFreePartitionMemory
	STUB_FUNC	0xa291f107,sceKernelMaxFreeMemSize
	STUB_FUNC	0xf919f628,sceKernelTotalFreeMemSize
	STUB_END

	STUB_START	"sceDmac",0x40010000,0x00020005
	STUB_FUNC	0x617f3fe6,sceDmacMemcpy
	STUB_FUNC	0xd97f94d8,sceDmacTryMemcpy
	STUB_END

#	STUB_START	"sceFpu",0x40010000,0x00010005
#	STUB_FUNC	0x3af6984a,sceFpuTrunc
#	STUB_END

###############################################################################

	.text

	.end _start

