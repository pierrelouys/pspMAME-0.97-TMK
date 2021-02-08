# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="driver_alphamis,driver_aso,driver_athena,driver_pc_bstar,driver_bbusters,driver_bermudao,driver_bermudat,driver_bermudaa,driver_buriki,driver_chopper,driver_choppera,driver_chopperb,driver_countryc,driver_powj,driver_dogosoke,driver_fantasyj,driver_fantasyu,driver_fantasy,driver_fatfurwa,driver_fitegol2,driver_fitegolf,driver_ftsoccer,driver_gensitou,driver_sgladiat,driver_goldmedl,driver_goldmeda,driver_gwar,driver_gwara,driver_gwarj,driver_hal21,driver_hal21j,driver_ikarijp,driver_ikari3,driver_ikari,driver_joyfulr,driver_jcross,driver_legofair,driver_lasso,driver_madcrash,driver_jongbou,driver_mainsnk,driver_marvins,driver_mechatt,driver_meijinsn,driver_mnchmobl,driver_ozmawars,driver_ozmawar2,driver_pow,driver_paddlema,driver_pballoon,driver_prehislu,driver_prehisle,driver_psychosj,driver_psychos,driver_roadedge,driver_safarir,driver_sams64_2,driver_sercharu,driver_searchar,driver_sasuke,driver_satansat,driver_streetsj,driver_streets1,driver_streetsm,driver_streetsw,driver_tnk3j,driver_tnk3,driver_tnexspce,driver_tdfever,driver_tdfeverj,driver_tdfever2,driver_vangrdce,driver_vanguarj,driver_vanguard,driver_vangrd2,driver_victroad,driver_worldwar,driver_zarzon"

COREDEFS += -DTINY_POINTER="&driver_alphamis,&driver_aso,&driver_athena,&driver_pc_bstar,&driver_bbusters,&driver_bermudao,&driver_bermudat,&driver_bermudaa,&driver_buriki,&driver_chopper,&driver_choppera,&driver_chopperb,&driver_countryc,&driver_powj,&driver_dogosoke,&driver_fantasyj,&driver_fantasyu,&driver_fantasy,&driver_fatfurwa,&driver_fitegol2,&driver_fitegolf,&driver_ftsoccer,&driver_gensitou,&driver_sgladiat,&driver_goldmedl,&driver_goldmeda,&driver_gwar,&driver_gwara,&driver_gwarj,&driver_hal21,&driver_hal21j,&driver_ikarijp,&driver_ikari3,&driver_ikari,&driver_joyfulr,&driver_jcross,&driver_legofair,&driver_lasso,&driver_madcrash,&driver_jongbou,&driver_mainsnk,&driver_marvins,&driver_mechatt,&driver_meijinsn,&driver_mnchmobl,&driver_ozmawars,&driver_ozmawar2,&driver_pow,&driver_paddlema,&driver_pballoon,&driver_prehislu,&driver_prehisle,&driver_psychosj,&driver_psychos,&driver_roadedge,&driver_safarir,&driver_sams64_2,&driver_sercharu,&driver_searchar,&driver_sasuke,&driver_satansat,&driver_streetsj,&driver_streets1,&driver_streetsm,&driver_streetsw,&driver_tnk3j,&driver_tnk3,&driver_tnexspce,&driver_tdfever,&driver_tdfeverj,&driver_tdfever2,&driver_vangrdce,&driver_vanguarj,&driver_vanguard,&driver_vangrd2,&driver_victroad,&driver_worldwar,&driver_zarzon"

# uses these CPUs
CPUS+=M68000@
CPUS+=8080@
CPUS+=M6502@
CPUS+=N2A03@
CPUS+=R4600@
CPUS+=V30@
CPUS+=Z80@


# uses these SOUNDs
SOUNDS+=AY8910@
SOUNDS+=CUSTOM@
SOUNDS+=DAC@
SOUNDS+=NAMCO@
SOUNDS+=NES@
SOUNDS+=SAMPLES@
SOUNDS+=SN76477@
SOUNDS+=SN76496@
SOUNDS+=UPD7759@
SOUNDS+=Y8950@
SOUNDS+=YM2203@
SOUNDS+=YM2413@
SOUNDS+=YM2608@
SOUNDS+=YM2610@
SOUNDS+=YM3526@
SOUNDS+=YM3812@

SOUNDS+=YM2610B@
SOUNDS+=DISCRETE@


DRVLIBS = \
	obj/drivers/pacman.a obj/drivers/epos.a obj/drivers/nichibut.a \
	obj/drivers/phoenix.a obj/drivers/namco.a obj/drivers/univers.a obj/drivers/nintendo.a \
	obj/drivers/midw8080.a obj/drivers/meadows.a obj/drivers/cvs.a obj/drivers/midway.a \
	obj/drivers/irem.a obj/drivers/gottlieb.a obj/drivers/taito.a obj/drivers/toaplan.a obj/drivers/cave.a \
	obj/drivers/kyugo.a obj/drivers/williams.a obj/drivers/gremlin.a obj/drivers/vicdual.a \
	obj/drivers/capcom.a obj/drivers/itech.a obj/drivers/leland.a obj/drivers/sega.a \
	obj/drivers/dataeast.a obj/drivers/tehkan.a obj/drivers/konami.a \
	obj/drivers/exidy.a obj/drivers/atari.a obj/drivers/snk.a obj/drivers/alpha.a obj/drivers/technos.a \
	obj/drivers/stern.a obj/drivers/gameplan.a obj/drivers/zaccaria.a \
	obj/drivers/upl.a obj/drivers/nmk.a obj/drivers/cinemar.a obj/drivers/cinemav.a \
	obj/drivers/thepit.a obj/drivers/valadon.a obj/drivers/seibu.a obj/drivers/tad.a obj/drivers/jaleco.a \
	obj/drivers/vsystem.a obj/drivers/psikyo.a obj/drivers/orca.a obj/drivers/gaelco.a \
	obj/drivers/kaneko.a obj/drivers/seta.a obj/drivers/atlus.a \
	obj/drivers/sun.a obj/drivers/suna.a obj/drivers/dooyong.a obj/drivers/tong.a \
	obj/drivers/comad.a obj/drivers/playmark.a obj/drivers/pacific.a obj/drivers/tecfri.a \
	obj/drivers/metro.a obj/drivers/venture.a obj/drivers/yunsung.a obj/drivers/zilec.a \
	obj/drivers/fuuki.a obj/drivers/unico.a obj/drivers/dynax.a \
	obj/drivers/sigma.a obj/drivers/igs.a obj/drivers/ramtek.a obj/drivers/omori.a obj/drivers/tch.a \
	obj/drivers/usgames.a obj/drivers/sanritsu.a obj/drivers/rare.a \
	obj/drivers/alba.a obj/drivers/homedata.a obj/drivers/artmagic.a \
	obj/drivers/taiyo.a obj/drivers/edevices.a obj/drivers/other.a obj/drivers/excelent.a obj/drivers/nix.a \
	obj/drivers/arcadia.a obj/drivers/othertrv.a obj/drivers/other68k.a obj/drivers/otherz80.a \
	obj/drivers/neogeo.a $(OBJ)/shared.a \

$(OBJ)/shared.a: \
	$(OBJ)/machine/segacrpt.o \
	$(OBJ)/machine/tmp68301.o \
	$(OBJ)/machine/6532riot.o \
	$(OBJ)/machine/z80fmly.o \
	$(OBJ)/machine/6821pia.o \
	$(OBJ)/machine/8255ppi.o \
	$(OBJ)/machine/6522via.o \
	$(OBJ)/machine/7474.o \
	$(OBJ)/machine/idectrl.o \
	$(OBJ)/machine/ticket.o \
	$(OBJ)/machine/random.o \
	$(OBJ)/machine/adc083x.o \
	$(OBJ)/vidhrdw/crtc6845.o \
	$(OBJ)/vidhrdw/avgdvg.o \
	$(OBJ)/vidhrdw/poly.o \
	$(OBJ)/vidhrdw/tlc34076.o \
	$(OBJ)/vidhrdw/res_net.o \

# MAME specific core objs
COREOBJS += $(OBJ)/tiny.o $(OBJ)/cheat.o
