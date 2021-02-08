PSP_EBOOT_TITLE = pspMame Deathrash Vol 2.5

#OPT_DEFS += -DLINK_ROT=1

# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="driver_empcity,driver_empcityu,driver_empcityj,driver_stfight\
,driver_stfighta,driver_shadfrce,driver_gangwars,driver_gangwarb,driver_ddragon,driver_ddragonw\
,driver_ddragonb,driver_ddragon2,driver_xsleena,driver_xsleenab,driver_solarwar,driver_renegade\
,driver_baddudes,driver_drgninja,driver_robocop,driver_slyspy,driver_midres,driver_bouldash\
,driver_hbarrel,driver_birdtry,driver_hippodrm,driver_growl,driver_thundfox,driver_footchmp,driver_ninjak\
,driver_pulirula,driver_deadconx,driver_vigilant,driver_buccanrs,driver_gunforce,driver_bmaster,driver_uccops\
,driver_mysticri,driver_hook,driver_rtypeleo,driver_gunforc2,driver_tokio,driver_bublbobl,driver_cninja\
,driver_robocop2,driver_edrandy,driver_nbbatman,driver_hharry,driver_xmultipl,driver_loht,driver_nspirit\
,driver_kengo,driver_gallop,driver_cosmccop,driver_airduel,driver_dbreed,driver_imgfight\
,driver_guardian,driver_roadriot,driver_hydra,driver_pitfight,driver_thunderj\
,driver_indytemp,driver_peterpak,driver_roadrunn"


COREDEFS += -DTINY_POINTER="&driver_empcity,&driver_empcityu,&driver_empcityj,&driver_stfight\
,&driver_stfighta,&driver_shadfrce,&driver_gangwars,&driver_gangwarb,&driver_ddragon,&driver_ddragonw\
,&driver_ddragonb,&driver_ddragon2,&driver_xsleena,&driver_xsleenab,&driver_solarwar,&driver_renegade\
,&driver_baddudes,&driver_drgninja,&driver_robocop,&driver_slyspy,&driver_midres,&driver_bouldash\
,&driver_hbarrel,&driver_birdtry,&driver_hippodrm,&driver_growl,&driver_thundfox,&driver_footchmp,&driver_ninjak\
,&driver_pulirula,&driver_deadconx,&driver_vigilant,&driver_buccanrs,&driver_gunforce,&driver_bmaster,&driver_uccops\
,&driver_mysticri,&driver_hook,&driver_rtypeleo,&driver_gunforc2,&driver_tokio,&driver_bublbobl,&driver_cninja\
,&driver_robocop2,&driver_edrandy,&driver_nbbatman,&driver_hharry,&driver_xmultipl,&driver_loht,&driver_nspirit\
,&driver_kengo,&driver_gallop,&driver_cosmccop,&driver_airduel,&driver_dbreed,&driver_imgfight\
,&driver_guardian,&driver_roadriot,&driver_hydra,&driver_pitfight,&driver_thunderj\
,&driver_indytemp,&driver_peterpak,&driver_roadrunn"


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
CPUS+=ADSP2105_ORIG__
CPUS+=ADSP2101_ORIG__
CPUS+=ADSP2100_ORIG__
CPUS+=ADSP2104_ORIG__
CPUS+=ADSP2115_ORIG__
CPUS+=HD63705_ORIGINAL@NOT
CPUS+=M68EC020_ORIG
CPUS+=V33_ORIG
CPUS+=V30_ORIG

#M68000_ORIG
#CPUS+=TMS34010_ORIG__@NOT

# uses these SOUNDs
SOUNDS+=SN76496@NOT
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

#SOUNDS+=K053260@
#SOUNDS+=MSM5205@




DRVLIBS = \
		$(OBJ)/machine/stfight.o $(OBJ)/vidhrdw/stfight.o $(OBJ)/drivers/stfight.o \
		$(OBJ)/vidhrdw/alpha68k.o $(OBJ)/drivers/alpha68k.o \
		$(OBJ)/machine/dec0.o $(OBJ)/vidhrdw/dec0.o $(OBJ)/drivers/dec0.o \
		$(OBJ)/vidhrdw/renegade.o $(OBJ)/drivers/renegade.o \
	$(OBJ)/vidhrdw/xain.o $(OBJ)/drivers/xain.o \
		$(OBJ)/vidhrdw/ddragon.o $(OBJ)/drivers/ddragon.o \
        $(OBJ)/vidhrdw/shadfrce.o $(OBJ)/drivers/shadfrce.o \
        $(OBJ)/machine/cchip.o \
        $(OBJ)/vidhrdw/taitoic.o $(OBJ)/sndhrdw/taitosnd.o \
        $(OBJ)/machine/mb87078.o \
        $(OBJ)/vidhrdw/taito_f2.o $(OBJ)/drivers/taito_f2.o \
        $(OBJ)/vidhrdw/vigilant.o $(OBJ)/drivers/vigilant.o \
	$(OBJ)/vidhrdw/m72.o $(OBJ)/sndhrdw/m72.o $(OBJ)/drivers/m72.o \
	$(OBJ)/machine/irem_cpu.o \
	$(OBJ)/vidhrdw/m90.o $(OBJ)/drivers/m90.o \
	$(OBJ)/vidhrdw/m92.o $(OBJ)/drivers/m92.o \
	$(OBJ)/vidhrdw/m107.o $(OBJ)/drivers/m107.o \
	$(OBJ)/machine/bublbobl.o $(OBJ)/vidhrdw/bublbobl.o $(OBJ)/drivers/bublbobl.o \
	$(OBJ)/machine/decocrpt.o $(OBJ)/machine/decoprot.o \
    $(OBJ)/vidhrdw/deco16ic.o \
    $(OBJ)/vidhrdw/cninja.o $(OBJ)/drivers/cninja.o \
    $(OBJ)/vidhrdw/atarisy1.o $(OBJ)/drivers/atarisy1.o \
    $(OBJ)/machine/atarigen.o $(OBJ)/sndhrdw/atarijsa.o \
    $(OBJ)/vidhrdw/atarimo.o $(OBJ)/vidhrdw/atarirle.o \
    $(OBJ)/machine/slapstic.o \
    $(OBJ)/vidhrdw/atarig1.o $(OBJ)/drivers/atarig1.o \
    $(OBJ)/vidhrdw/thunderj.o $(OBJ)/drivers/thunderj.o \
    $(OBJ)/vidhrdw/atarig42.o $(OBJ)/drivers/atarig42.o \
	$(OBJ)/machine/asic65.o \
        $(OBJ)/machine/z80fmly.o \
        $(OBJ)/machine/6821pia.o \
        $(OBJ)/machine/8255ppi.o \
        $(OBJ)/machine/6522via.o \
        $(OBJ)/machine/random.o \
        $(OBJ)/vidhrdw/res_net.o \
        
		
#$(OBJ)/vidhrdw/taito_f3.o $(OBJ)/sndhrdw/taito_f3.o $(OBJ)/drivers/taito_f3.o \
#DRVLIBS = \
#	$(OBJ)/konami.a \
	
#$(OBJ)/konami.a: \
#$(OBJ)/vidhrdw/gberet.o $(OBJ)/drivers/gberet.o \

# MAME specific core objs
COREOBJS += $(OBJ)/tiny.o $(OBJ)/cheat.o
