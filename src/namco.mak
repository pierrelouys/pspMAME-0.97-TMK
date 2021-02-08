# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="driver_baraduke,driver_baraduka,driver_bombbee,driver_boscomd,driver_boscomdo,driver_bosco,driver_boscoo,driver_boscoo2,driver_cutieq,driver_digduga1,driver_digdugat,driver_digdugb,driver_digdug,driver_digdug2,driver_digdug2o,driver_drgnbstr,driver_galagamw,driver_galagamk,driver_galaga,driver_galagao,driver_galaga3,driver_galaga3a,driver_galaga3m,driver_galmidw,driver_galmidwo,driver_galaxian,driver_galaxiaj,driver_gaplusa,driver_gapluso,driver_gaplus,driver_geebee,driver_geebeeg,driver_genpeitd,driver_grobda,driver_grobda2,driver_grobda3,driver_hopmappy,driver_kaitei,driver_kingbalj,driver_kingball,driver_mappyj,driver_mappy,driver_metrocrs,driver_metrocra,driver_motos,driver_navarone,driver_nrallyx,driver_pacnpal,driver_pacnpal2,driver_paclandm,driver_pacland,driver_pacland2,driver_pacland3,driver_pacnchmp,driver_pacman,driver_pacmod,driver_pacmanf,driver_pacplus,driver_phozon,driver_polepos,driver_polepos1,driver_poleposa,driver_polepos2,driver_poleps2a,driver_puckmanf,driver_puckman,driver_puckmana,driver_puckmod,driver_rallyx,driver_rallyxm,driver_rthunder,driver_rthundro,driver_skykidd,driver_skykid,driver_skykido,driver_skykiddx,driver_skykiddo,driver_sos,driver_superpac,driver_superpcm,driver_sxevious,driver_tankbatt,driver_roishtar,driver_todruaga,driver_todruago,driver_btlecity,driver_starlstr,driver_vsskykid,driver_supxevs,driver_tkoboxng,driver_warpwarp,driver_warpwarr,driver_warpwar2,driver_wndrmomo,driver_xeviousa,driver_xeviousb,driver_xeviousc,driver_xevious"

COREDEFS += -DTINY_POINTER="&driver_baraduke,&driver_baraduka,&driver_bombbee,&driver_boscomd,&driver_boscomdo,&driver_bosco,&driver_boscoo,&driver_boscoo2,&driver_cutieq,&driver_digduga1,&driver_digdugat,&driver_digdugb,&driver_digdug,&driver_digdug2,&driver_digdug2o,&driver_drgnbstr,&driver_galagamw,&driver_galagamk,&driver_galaga,&driver_galagao,&driver_galaga3,&driver_galaga3a,&driver_galaga3m,&driver_galmidw,&driver_galmidwo,&driver_galaxian,&driver_galaxiaj,&driver_gaplusa,&driver_gapluso,&driver_gaplus,&driver_geebee,&driver_geebeeg,&driver_genpeitd,&driver_grobda,&driver_grobda2,&driver_grobda3,&driver_hopmappy,&driver_kaitei,&driver_kingbalj,&driver_kingball,&driver_mappyj,&driver_mappy,&driver_metrocrs,&driver_metrocra,&driver_motos,&driver_navarone,&driver_nrallyx,&driver_pacnpal,&driver_pacnpal2,&driver_paclandm,&driver_pacland,&driver_pacland2,&driver_pacland3,&driver_pacnchmp,&driver_pacman,&driver_pacmod,&driver_pacmanf,&driver_pacplus,&driver_phozon,&driver_polepos,&driver_polepos1,&driver_poleposa,&driver_polepos2,&driver_poleps2a,&driver_puckmanf,&driver_puckman,&driver_puckmana,&driver_puckmod,&driver_rallyx,&driver_rallyxm,&driver_rthunder,&driver_rthundro,&driver_skykidd,&driver_skykid,&driver_skykido,&driver_skykiddx,&driver_skykiddo,&driver_sos,&driver_superpac,&driver_superpcm,&driver_sxevious,&driver_tankbatt,&driver_roishtar,&driver_todruaga,&driver_todruago,&driver_btlecity,&driver_starlstr,&driver_vsskykid,&driver_supxevs,&driver_tkoboxng,&driver_warpwarp,&driver_warpwarr,&driver_warpwar2,&driver_wndrmomo,&driver_xeviousa,&driver_xeviousb,&driver_xeviousc,&driver_xevious"

# uses these CPUs
CPUS+=8080@
CPUS+=HD63701@
CPUS+=M6502@
CPUS+=M6809@
CPUS+=N2A03@
CPUS+=Z80@
CPUS+=Z8000@

CPUS+=M6803@

#CPUS+=S2650@
#CPUS+=M65C02@
#CPUS+=M68000@
#CPUS+=H83002@
#CPUS+=M68EC020@
#CPUS+=8085A@
#CPUS+=I8035@


# uses these SOUNDs
SOUNDS+=CUSTOM@
SOUNDS+=DAC@
SOUNDS+=NAMCO@
SOUNDS+=NAMCO_15XX@
SOUNDS+=NAMCO_52XX@
SOUNDS+=NAMCO_54XX@
SOUNDS+=NAMCO_63701X@
SOUNDS+=NAMCO_CUS30@
SOUNDS+=NES@
SOUNDS+=SAMPLES@
SOUNDS+=YM2151@

SOUNDS+=AY8910@
SOUNDS+=DISCRETE@
SOUNDS+=SN76496@
SOUNDS+=TMS36XX@
SOUNDS+=TMS5110@


#SOUNDS+=NAMCONA@
#SOUNDS+=C352@

OBJS=	$(OBJ)/machine/scramble.o $(OBJ)/sndhrdw/scramble.o $(OBJ)/drivers/scramble.o \
		$(OBJ)/drivers/amidar.o \
		$(OBJ)/drivers/frogger.o \
		$(OBJ)/drivers/scobra.o \
		$(OBJ)/vidhrdw/galaxian.o $(OBJ)/sndhrdw/galaxian.o $(OBJ)/drivers/galaxian.o \
			$(OBJ)/vidhrdw/bagman.o $(OBJ)/machine/bagman.o $(OBJ)/drivers/bagman.o \
			$(OBJ)/vidhrdw/pacman.o $(OBJ)/drivers/pacman.o \
				$(OBJ)/vidhrdw/epos.o $(OBJ)/drivers/epos.o \
				$(OBJ)/machine/mspacman.o $(OBJ)/machine/pacplus.o \
				$(OBJ)/machine/jumpshot.o $(OBJ)/machine/theglobp.o \
				$(OBJ)/machine/acitya.o \
			$(OBJ)/vidhrdw/phoenix.o $(OBJ)/sndhrdw/phoenix.o $(OBJ)/drivers/phoenix.o \
				$(OBJ)/sndhrdw/pleiads.o \
			$(OBJ)/vidhrdw/cclimber.o $(OBJ)/sndhrdw/cclimber.o $(OBJ)/drivers/cclimber.o \
			$(OBJ)/vidhrdw/ladybug.o $(OBJ)/drivers/ladybug.o \

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
