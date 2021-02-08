PSP_EBOOT_TITLE = pspMame Deathrash Vol I

#OPT_DEFS += -DLINK_ROT=1

# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="driver_gberet,driver_rushatck,driver_gberetb,driver_mrgoemon\
,driver_empcity,driver_empcityu,driver_empcityj,driver_stfight\
,driver_stfighta,driver_chiller,driver_crossbow,driver_cheyenne,driver_kungfum,driver_kungfud,driver_spartanx\
,driver_kungfub,driver_kungfub2,driver_spelunkr,driver_spelnkrj,driver_spelunk2,driver_bloodbro\
,driver_bloodbra,driver_weststry,driver_skysmash,driver_cabal,driver_cabal2,driver_cabalbl\
,driver_toki,driver_tokia,driver_tokij,driver_tokiu,driver_tokib,driver_aliens\
,driver_cbuster,driver_cbusterw,driver_cbusterj,driver_twocrude,driver_simpsons\
,driver_truco,driver_trucocl,driver_vendetta\
,driver_xmen,driver_splatter,driver_pitfall2,driver_pitfallu,driver_chplft,driver_chplftb\
,driver_chplftbl,driver_wboy,driver_wbdeluxe,driver_wbml,driver_wbmljo,driver_wbmljb,driver_wbmlb\
,driver_shinobi,driver_shinobl,driver_ddux,driver_dduxbl,driver_goldnaxe,driver_goldnabl\
,driver_insectx,driver_extrmatn,driver_kageki,driver_exprraid,driver_exprrada\
,driver_wexpress,driver_wexpresb,driver_wexpresc,driver_shootout,driver_shootouj,driver_shootoub\
,driver_sidepckt,driver_sidepctj,driver_sidepctb,driver_pcktgal,driver_pcktgalb,driver_pcktgal2\
,driver_spool3,driver_spool3i,driver_rygar,driver_rygar2,driver_rygarj,driver_crimfght\
,driver_crimfgt2,driver_crimfgtj,driver_pbaction,driver_pbactio2,driver_tehkanwc\
,driver_gridiron,driver_teedoff,driver_solomon,driver_jackal,driver_topgunr,driver_jackalj\
,driver_topgunbl,driver_atetris,driver_atetrisa,driver_atetrisb,driver_shollow,driver_tron\
,driver_kroozr,driver_domino,driver_wacko,driver_twotiger,driver_journey,driver_streetsm,driver_pow,driver_searchar\
,driver_ikari3,driver_sqix,driver_sqixa,driver_sqixbl,driver_ddcrew,driver_ddcrewu,driver_ddcrew2,driver_ddcrew1\
,driver_astorm,driver_astormbl,driver_bayroute,driver_riotcity,driver_puzznic,driver_silentd\
,driver_spacedx,driver_crimec,driver_rambo3,driver_tmnt,driver_tmnt2,driver_ssriders,driver_mia,driver_asterix\
,driver_devilw,driver_scontra,driver_gbusters"


COREDEFS += -DTINY_POINTER="&driver_gberet,&driver_rushatck,&driver_gberetb,&driver_mrgoemon\
,&driver_empcity,&driver_empcityu,&driver_empcityj,&driver_stfight\
,&driver_stfighta,&driver_chiller,&driver_crossbow,&driver_cheyenne,&driver_kungfum,&driver_kungfud\
,&driver_spartanx,&driver_kungfub,&driver_kungfub2,&driver_spelunkr,&driver_spelnkrj,&driver_spelunk2\
,&driver_bloodbro,&driver_bloodbra,&driver_weststry,&driver_skysmash,&driver_cabal,&driver_cabal2,&driver_cabalbl\
,&driver_toki,&driver_tokia,&driver_tokij,&driver_tokiu,&driver_tokib,&driver_aliens\
,&driver_cbuster,&driver_cbusterw,&driver_cbusterj,&driver_twocrude,&driver_simpsons\
,&driver_truco,&driver_trucocl,&driver_vendetta\
,&driver_xmen,&driver_splatter,&driver_pitfall2,&driver_pitfallu,&driver_chplft,&driver_chplftb\
,&driver_chplftbl,&driver_wboy,&driver_wbdeluxe,&driver_wbml,&driver_wbmljo,&driver_wbmljb,&driver_wbmlb\
,&driver_shinobi,&driver_shinobl,&driver_ddux,&driver_dduxbl,&driver_goldnaxe,&driver_goldnabl\
,&driver_insectx,&driver_extrmatn,&driver_kageki,&driver_exprraid,&driver_exprrada\
,&driver_wexpress,&driver_wexpresb,&driver_wexpresc,&driver_shootout,&driver_shootouj,&driver_shootoub\
,&driver_sidepckt,&driver_sidepctj,&driver_sidepctb,&driver_pcktgal,&driver_pcktgalb,&driver_pcktgal2\
,&driver_spool3,&driver_spool3i,&driver_rygar,&driver_rygar2,&driver_rygarj,&driver_crimfght\
,&driver_crimfgt2,&driver_crimfgtj,&driver_pbaction,&driver_pbactio2,&driver_tehkanwc\
,&driver_gridiron,&driver_teedoff,&driver_solomon,&driver_jackal,&driver_topgunr,&driver_jackalj\
,&driver_topgunbl,&driver_atetris,&driver_atetrisa,&driver_atetrisb,&driver_shollow,&driver_tron\
,&driver_kroozr,&driver_domino,&driver_wacko,&driver_twotiger,&driver_journey,&driver_streetsm,&driver_pow,&driver_searchar\
,&driver_ikari3,&driver_sqix,&driver_sqixa,&driver_sqixbl,&driver_ddcrew,&driver_ddcrewu,&driver_ddcrew2,&driver_ddcrew1\
,&driver_astorm,&driver_astormbl,&driver_bayroute,&driver_riotcity,&driver_puzznic,&driver_silentd\
,&driver_spacedx,&driver_crimec,&driver_rambo3,&driver_tmnt,&driver_tmnt2,&driver_ssriders\
,&driver_mia,&driver_asterix,&driver_devilw,&driver_scontra,&driver_gbusters"


# uses these CPUs
CPUS+=KONAMI_ORIG
CPUS+=G65816@
CPUS+=HD6309@
CPUS+=I8039
CPUS+=M6800@
CPUS+=M6809_ORIG
CPUS+=N2A03@
CPUS+=SPC700@
CPUS+=Z80_ORIG
CPUS+=M6803
CPUS+=M68000_ORIG
CPUS+=H6280
CPUS+=HD63701_ORIG
CPUS+=N7751
CPUS+=I8751_ORIG
CPUS+=I8752_ORIG
CPUS+=M6502_ORIG
CPUS+=M6808_ORIG
CPUS+=TMS34010_ORIG__
CPUS+=M6809E_ORIG__
CPUS+=ADSP2105_ORIG__
CPUS+=ADSP2101_ORIG__
CPUS+=ADSP2100_ORIG__
CPUS+=ADSP2104_ORIG__
CPUS+=ADSP2115_ORIG__
CPUS+=HD63705_ORIGINAL

#M68000_ORIG

# uses these SOUNDs
SOUNDS+=SN76496
SOUNDS+=MSM5205
SOUNDS+=YM2203_ORIG
SOUNDS+=YM2151_ORIG
SOUNDS+=OKIM6295
SOUNDS+=K007232_ORIG
SOUNDS+=CUSTOM
SOUNDS+=AY8910
SOUNDS+=DAC
SOUNDS+=DMADAC@
SOUNDS+=NAMCO
SOUNDS+=TMS5220
SOUNDS+=K007232@
SOUNDS+=K051649@
SOUNDS+=K054539
SOUNDS+=YM2413
SOUNDS+=YM3438
SOUNDS+=RF5C68
SOUNDS+=YM2612_ORIG
SOUNDS+=NES@
SOUNDS+=OKIM6295@
SOUNDS+=SAMPLES@
SOUNDS+=SN76496@
SOUNDS+=UPD7759
SOUNDS+=VLM5030@
SOUNDS+=YM2151@
SOUNDS+=YM3812_ORIG
SOUNDS+=K053260_ORIG
SOUNDS+=SAMPLES
SOUNDS+=YM3526_ORIG
SOUNDS+=POKEY
SOUNDS+=HC55516
SOUNDS+=YM2610_ORIG
SOUNDS+=YM2610B_ORIG
SOUNDS+=C140
SOUNDS+=NAMCO_CUS30

SOUNDS+=TMS5110@
SOUNDS+=TMS36XX@
SOUNDS+=DISCRETE@
SOUNDS+=NAMCO@

#SOUNDS+=K053260@
#SOUNDS+=MSM5205@




DRVLIBS = \
	$(OBJ)/vidhrdw/gberet.o $(OBJ)/drivers/gberet.o \
		$(OBJ)/machine/stfight.o $(OBJ)/vidhrdw/stfight.o $(OBJ)/drivers/stfight.o \
		$(OBJ)/sndhrdw/exidy440.o $(OBJ)/vidhrdw/exidy440.o $(OBJ)/drivers/exidy440.o \
		$(OBJ)/sndhrdw/irem.o $(OBJ)/vidhrdw/m62.o $(OBJ)/drivers/m62.o \
		$(OBJ)/sndhrdw/seibu.o \
		$(OBJ)/vidhrdw/bloodbro.o $(OBJ)/drivers/bloodbro.o \
		$(OBJ)/vidhrdw/cabal.o $(OBJ)/drivers/cabal.o \
		$(OBJ)/vidhrdw/toki.o $(OBJ)/drivers/toki.o \
		$(OBJ)/machine/konami.o \
		$(OBJ)/vidhrdw/konamiic.o \
		$(OBJ)/vidhrdw/thunderx.o $(OBJ)/drivers/thunderx.o \
		$(OBJ)/vidhrdw/gbusters.o $(OBJ)/drivers/gbusters.o \
        $(OBJ)/vidhrdw/crimfght.o $(OBJ)/drivers/crimfght.o \
		$(OBJ)/vidhrdw/aliens.o $(OBJ)/drivers/aliens.o \
		$(OBJ)/vidhrdw/cbuster.o $(OBJ)/drivers/cbuster.o \
		$(OBJ)/vidhrdw/asterix.o $(OBJ)/drivers/asterix.o \
		$(OBJ)/machine/simpsons.o $(OBJ)/vidhrdw/simpsons.o $(OBJ)/drivers/simpsons.o \
		$(OBJ)/vidhrdw/truco.o $(OBJ)/drivers/truco.o \
		$(OBJ)/drivers/trucocl.o $(OBJ)/vidhrdw/trucocl.o \
		$(OBJ)/vidhrdw/vendetta.o $(OBJ)/drivers/vendetta.o \
		$(OBJ)/vidhrdw/twin16.o $(OBJ)/drivers/twin16.o \
		$(OBJ)/vidhrdw/tmnt.o $(OBJ)/drivers/tmnt.o \
		$(OBJ)/vidhrdw/xmen.o $(OBJ)/drivers/xmen.o \
		$(OBJ)/vidhrdw/moo.o $(OBJ)/drivers/moo.o \
		$(OBJ)/machine/namcos1.o $(OBJ)/vidhrdw/namcos1.o $(OBJ)/drivers/namcos1.o \
		$(OBJ)/vidhrdw/system1.o $(OBJ)/drivers/system1.o \
		$(OBJ)/vidhrdw/segasyse.o $(OBJ)/drivers/segasyse.o \
        $(OBJ)/machine/system16.o $(OBJ)/vidhrdw/system16.o $(OBJ)/vidhrdw/sys16spr.o \
	$(OBJ)/sndhrdw/system16.o $(OBJ)/vidhrdw/segaic16.o $(OBJ)/machine/segaic16.o \
	$(OBJ)/vidhrdw/segas16a.o $(OBJ)/drivers/segas16a.o \
	$(OBJ)/vidhrdw/segas16b.o $(OBJ)/drivers/segas16b.o \
	$(OBJ)/vidhrdw/segas18.o $(OBJ)/drivers/segas18.o \
	$(OBJ)/drivers/system18.o \
	$(OBJ)/machine/fd1094.o $(OBJ)/machine/s16fd.o $(OBJ)/machine/fd1089.o \
	$(OBJ)/machine/mc8123.o \
	$(OBJ)/drivers/system16.o $(OBJ)/drivers/segas16b.o \
	$(OBJ)/vidhrdw/segac2.o $(OBJ)/drivers/segac2.o \
	$(OBJ)/machine/tnzs.o $(OBJ)/vidhrdw/tnzs.o $(OBJ)/drivers/tnzs.o \
	$(OBJ)/vidhrdw/shootout.o $(OBJ)/drivers/shootout.o \
	$(OBJ)/vidhrdw/sidepckt.o $(OBJ)/drivers/sidepckt.o \
	$(OBJ)/vidhrdw/exprraid.o $(OBJ)/drivers/exprraid.o \
	$(OBJ)/vidhrdw/pcktgal.o $(OBJ)/drivers/pcktgal.o \
	$(OBJ)/vidhrdw/pbaction.o $(OBJ)/drivers/pbaction.o \
	$(OBJ)/vidhrdw/tehkanwc.o $(OBJ)/drivers/tehkanwc.o \
	$(OBJ)/vidhrdw/solomon.o $(OBJ)/drivers/solomon.o \
	$(OBJ)/vidhrdw/tecmo.o $(OBJ)/drivers/tecmo.o \
	$(OBJ)/machine/jackal.o $(OBJ)/vidhrdw/jackal.o $(OBJ)/drivers/jackal.o \
	$(OBJ)/machine/slapstic.o \
	$(OBJ)/vidhrdw/atetris.o $(OBJ)/drivers/atetris.o \
	$(OBJ)/machine/mcr.o $(OBJ)/sndhrdw/mcr.o \
	$(OBJ)/vidhrdw/mcr12.o $(OBJ)/vidhrdw/mcr3.o \
	$(OBJ)/drivers/mcr1.o $(OBJ)/drivers/mcr2.o $(OBJ)/drivers/mcr3.o \
	$(OBJ)/vidhrdw/mcr68.o $(OBJ)/drivers/mcr68.o \
	$(OBJ)/sndhrdw/williams.o \
	$(OBJ)/vidhrdw/snk68.o $(OBJ)/drivers/snk68.o \
	$(OBJ)/vidhrdw/superqix.o $(OBJ)/drivers/superqix.o \
	$(OBJ)/vidhrdw/taitoic.o $(OBJ)/sndhrdw/taitosnd.o \
	$(OBJ)/vidhrdw/taito_l.o $(OBJ)/drivers/taito_l.o \
	$(OBJ)/vidhrdw/taito_b.o $(OBJ)/drivers/taito_b.o \
	$(OBJ)/machine/mb87078.o \
	$(OBJ)/machine/segacrpt.o \
        $(OBJ)/machine/z80fmly.o \
        $(OBJ)/machine/6821pia.o \
        $(OBJ)/machine/8255ppi.o \
        $(OBJ)/machine/random.o \
        $(OBJ)/vidhrdw/res_net.o \
        
		

#DRVLIBS = \
#	$(OBJ)/konami.a \
	
#$(OBJ)/konami.a: \
#$(OBJ)/vidhrdw/gberet.o $(OBJ)/drivers/gberet.o \

# MAME specific core objs
COREOBJS += $(OBJ)/tiny.o $(OBJ)/cheat.o
