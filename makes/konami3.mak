PSP_EBOOT_TITLE = pspMame Deathrash Vol 3

#OPT_DEFS += -DLINK_ROT=1

# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="driver_indytemp,driver_xybots,driver_gauntlet,driver_gaunt2\
,driver_vindctr2,driver_atetris,driver_atetrisa,driver_atetrisb,driver_atetrsb2,driver_toobin\
,driver_vindictr,driver_klax,driver_blstroid,driver_eprom,driver_skullxbo,driver_badlands\
,driver_stlforce,driver_twinbrat,driver_mugsmash,driver_diverboy,driver_ppmast93,driver_mwarr\
,driver_captaven,driver_metmqstr,driver_nmaster,driver_mazinger,driver_rohga,driver_wizdfire\
,driver_nitrobal,driver_schmeisr,driver_term2,driver_narc,driver_strkforc,driver_smashtv\
,driver_totcarn,driver_mk"


COREDEFS += -DTINY_POINTER="&driver_indytemp,&driver_xybots,&driver_gauntlet,&driver_gaunt2\
,&driver_vindctr2,&driver_atetris,&driver_atetrisa,&driver_atetrisb,&driver_atetrsb2,&driver_toobin\
,&driver_vindictr,&driver_klax,&driver_blstroid,&driver_eprom,&driver_skullxbo,&driver_badlands\
,&driver_stlforce,&driver_twinbrat,&driver_mugsmash,&driver_diverboy,&driver_ppmast93,&driver_mwarr\
,&driver_captaven,&driver_metmqstr,&driver_nmaster,&driver_mazinger,&driver_rohga,&driver_wizdfire\
,&driver_nitrobal,&driver_schmeisr,&driver_term2,&driver_narc,&driver_strkforc,&driver_smashtv\
,&driver_totcarn,&driver_mk"


# uses these CPUs
CPUS+=KONAMI_ORIG@NOT
CPUS+=G65816@
CPUS+=HD6309_ORIG
CPUS+=I8039@NOT
CPUS+=M6800@
CPUS+=M6809_ORIG
CPUS+=N2A03@
CPUS+=SPC700@
CPUS+=Z80_ORIG
CPUS+=M6803
CPUS+=M68000_ORIG
CPUS+=M68010_ORIG
CPUS+=M68020_ORIG@not
CPUS+=H6280
CPUS+=HD63701_ORIG
CPUS+=N7751@NOT
CPUS+=I8751_ORIG@NOT
CPUS+=I8752_ORIG@NOT
CPUS+=M6502_ORIG
CPUS+=M6808_ORIG
CPUS+=M6809E_ORIG__
CPUS+=ADSP2105_ORIG
CPUS+=ADSP2101_ORIG
CPUS+=ADSP2100_ORIG
CPUS+=ADSP2104_ORIG
CPUS+=ADSP2105_ORIG
CPUS+=ADSP2115_ORIG
CPUS+=HD63705_ORIGINAL@NOT
CPUS+=M68EC020_ORIG@NOT
CPUS+=V33_ORIG@NOT
CPUS+=V30_ORIG@NOT
CPUS+=ARM
CPUS+=I86@NOT
CPUS+=TMS34010_ORIG

#M68000_ORIG
#CPUS+=TMS34010_ORIG__@NOT

# uses these SOUNDs
SOUNDS+=SN76496
SOUNDS+=MSM5205
SOUNDS+=YM2203_ORIG
SOUNDS+=YM2151_ORIG
SOUNDS+=OKIM6295
SOUNDS+=K007232_ORIG@NOT
SOUNDS+=CUSTOM@NOT
SOUNDS+=AY8910@NOT
SOUNDS+=DAC
SOUNDS+=DMADAC@
SOUNDS+=NAMCO@NOT
SOUNDS+=TMS5220
SOUNDS+=K007232@
SOUNDS+=K051649@
SOUNDS+=K054539@NOT
SOUNDS+=YM2413
SOUNDS+=YM3438
SOUNDS+=RF5C68@NOT
SOUNDS+=YM2612_ORIG
SOUNDS+=NES@
SOUNDS+=OKIM6295@
SOUNDS+=SAMPLES@
SOUNDS+=SN76496@
SOUNDS+=UPD7759@NOT
SOUNDS+=VLM5030@
SOUNDS+=YM2151@
SOUNDS+=YM3812_ORIG
SOUNDS+=K053260_ORIG@NOT
SOUNDS+=SAMPLES
SOUNDS+=YM3526_ORIG
SOUNDS+=POKEY
SOUNDS+=HC55516@NOT
SOUNDS+=YM2610_ORIG
SOUNDS+=YM2610B_ORIG
SOUNDS+=C140@NOT
SOUNDS+=NAMCO_CUS30@NOT
SOUNDS+=ES5505@NOT
SOUNDS+=IREMGA20

SOUNDS+=TMS5110@
SOUNDS+=TMS36XX@
SOUNDS+=DISCRETE@
SOUNDS+=NAMCO@
SOUNDS+=BSMT2000
SOUNDS+=YMZ280B
SOUNDS+=HC55516
SOUNDS+=CEM3394
SOUNDS+=DMADAC

#SOUNDS+=K053260@
#SOUNDS+=MSM5205@




DRVLIBS = \
    $(OBJ)/vidhrdw/atarisy1.o $(OBJ)/drivers/atarisy1.o \
    $(OBJ)/vidhrdw/atarisy2.o $(OBJ)/drivers/atarisy2.o \
    $(OBJ)/machine/atarigen.o $(OBJ)/sndhrdw/atarijsa.o \
    $(OBJ)/vidhrdw/atarimo.o $(OBJ)/vidhrdw/atarirle.o \
    $(OBJ)/machine/slapstic.o \
    $(OBJ)/vidhrdw/gauntlet.o $(OBJ)/drivers/gauntlet.o \
	$(OBJ)/vidhrdw/atetris.o $(OBJ)/drivers/atetris.o \
	$(OBJ)/vidhrdw/toobin.o $(OBJ)/drivers/toobin.o \
	$(OBJ)/vidhrdw/vindictr.o $(OBJ)/drivers/vindictr.o \
	$(OBJ)/vidhrdw/klax.o $(OBJ)/drivers/klax.o \
	$(OBJ)/vidhrdw/blstroid.o $(OBJ)/drivers/blstroid.o \
	$(OBJ)/vidhrdw/xybots.o $(OBJ)/drivers/xybots.o \
	$(OBJ)/vidhrdw/eprom.o $(OBJ)/drivers/eprom.o \
	$(OBJ)/vidhrdw/skullxbo.o $(OBJ)/drivers/skullxbo.o \
	$(OBJ)/vidhrdw/badlands.o $(OBJ)/drivers/badlands.o \
    $(OBJ)/vidhrdw/atarig1.o $(OBJ)/drivers/atarig1.o \
    $(OBJ)/vidhrdw/thunderj.o $(OBJ)/drivers/thunderj.o \
    $(OBJ)/machine/asic65.o \
    $(OBJ)/vidhrdw/mugsmash.o $(OBJ)/drivers/mugsmash.o \
	$(OBJ)/vidhrdw/stlforce.o $(OBJ)/drivers/stlforce.o \
	$(OBJ)/vidhrdw/diverboy.o $(OBJ)/drivers/diverboy.o \
	$(OBJ)/drivers/ppmast93.o \
	$(OBJ)/drivers/mwarr.o \
	$(OBJ)/machine/decocrpt.o $(OBJ)/machine/decoprot.o \
	$(OBJ)/vidhrdw/deco16ic.o \
	$(OBJ)/vidhrdw/rohga.o $(OBJ)/drivers/rohga.o \
    $(OBJ)/vidhrdw/deco32.o $(OBJ)/drivers/deco32.o \
    $(OBJ)/drivers/deco156.o \
	$(OBJ)/vidhrdw/deco_mlc.o $(OBJ)/drivers/deco_mlc.o \
	$(OBJ)/vidhrdw/cave.o $(OBJ)/drivers/cave.o \
	$(OBJ)/machine/williams.o $(OBJ)/vidhrdw/williams.o $(OBJ)/sndhrdw/williams.o $(OBJ)/drivers/williams.o \
    $(OBJ)/sndhrdw/wow.o $(OBJ)/sndhrdw/gorf.o \
	$(OBJ)/machine/mcr.o $(OBJ)/sndhrdw/mcr.o \
	$(OBJ)/vidhrdw/mcr12.o $(OBJ)/vidhrdw/mcr3.o \
	$(OBJ)/drivers/mcr1.o $(OBJ)/drivers/mcr2.o $(OBJ)/drivers/mcr3.o \
	$(OBJ)/vidhrdw/mcr68.o $(OBJ)/drivers/mcr68.o \
	$(OBJ)/vidhrdw/balsente.o $(OBJ)/machine/balsente.o $(OBJ)/drivers/balsente.o \
	$(OBJ)/vidhrdw/gridlee.o $(OBJ)/sndhrdw/gridlee.o $(OBJ)/drivers/gridlee.o \
	$(OBJ)/vidhrdw/exterm.o $(OBJ)/drivers/exterm.o \
	$(OBJ)/machine/midwayic.o $(OBJ)/sndhrdw/dcs.o \
	$(OBJ)/machine/midyunit.o $(OBJ)/vidhrdw/midyunit.o $(OBJ)/drivers/midyunit.o \
	$(OBJ)/drivers/midxunit.o \
	$(OBJ)/machine/midwunit.o $(OBJ)/drivers/midwunit.o \
	$(OBJ)/machine/midtunit.o $(OBJ)/vidhrdw/midtunit.o $(OBJ)/drivers/midtunit.o \
	$(OBJ)/sndhrdw/cage.o \
	    $(OBJ)/machine/idectrl.o \
        $(OBJ)/machine/ticket.o \
        $(OBJ)/machine/z80fmly.o \
        $(OBJ)/machine/6821pia.o \
        $(OBJ)/machine/8255ppi.o \
        $(OBJ)/machine/6522via.o \
        $(OBJ)/machine/random.o \
        $(OBJ)/vidhrdw/res_net.o \
        

#$(OBJ)/machine/ticket.o \ SHOULD LEAVE        
		
#$(OBJ)/vidhrdw/taito_f3.o $(OBJ)/sndhrdw/taito_f3.o $(OBJ)/drivers/taito_f3.o \
#DRVLIBS = \
#	$(OBJ)/konami.a \
	
#$(OBJ)/konami.a: \
#$(OBJ)/vidhrdw/gberet.o $(OBJ)/drivers/gberet.o \

# MAME specific core objs
COREOBJS += $(OBJ)/tiny.o $(OBJ)/cheat.o
