# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="driver_armedf,driver_bijokkog,driver_bijokkoy,driver_citylove,driver_mcitylov,driver_cop01,driver_cop01a,driver_cclimbrj,driver_cclimber,driver_cclmbr2a,driver_cclimbr2,driver_crystalg,driver_crystal2,driver_dacholer,driver_dangar,driver_dangar2,driver_friskyt,driver_friskyta,driver_galivan2,driver_galivan,driver_gionbana,driver_mgion,driver_gomoku,driver_hanaoji,driver_housemnq,driver_housemn2,driver_iemoto,driver_iemotom,driver_kickboy,driver_horekid,driver_kodure,driver_legiono,driver_legion,driver_magmax,driver_bakuhatu,driver_chinmoku,driver_cmehyou,driver_club90s,driver_club90sa,driver_finalbny,driver_mjfocus,driver_mjfocusm,driver_galkaika,driver_galkoku,driver_mgmen89,driver_mjgottsu,driver_hanamomo,driver_mhhonban,driver_hyouban,driver_janbari,driver_mjlstory,driver_mkeibaou,driver_koinomp,driver_mjkoiura,driver_mkoiuraa,driver_mjlaman,driver_lovehous,driver_ntopstar,driver_patimono,driver_pstadium,driver_sailorws,driver_sailorwa,driver_sailorwr,driver_msjiken,driver_mjsikakb,driver_mjsikakd,driver_mjsikakc,driver_mjsikaku,driver_mmsikaku,driver_mladyhtr,driver_triplew1,driver_triplew2,driver_uchuuai,driver_mjuraden,driver_vanilla,driver_wcatcher,driver_yosimoto,driver_korinai,driver_korinaim,driver_maiko,driver_mmaiko,driver_mmehyou,driver_mjgottub,driver_mjanbari,driver_pachiten,driver_mightguy,driver_mcontest,driver_moonaln,driver_moonal2,driver_moonal2b,driver_moonbase,driver_mooncrs2,driver_mooncrst,driver_mooncrsa,driver_mooncrsu,driver_moonqsr,driver_mshutlj2,driver_mshuttlj,driver_mshuttle,driver_musobana,driver_ngpgal,driver_ninjemak,driver_niyanpai,driver_ojousan,driver_ojousanm,driver_pairsnb,driver_pastelg,driver_qmhayaku,driver_radrad,driver_rjammer,driver_rollingc,driver_rugrats,driver_scandal,driver_scandalm,driver_secolove,driver_sectrzon,driver_amatelas,driver_seicross,driver_seiha,driver_seiham,driver_sfx,driver_skelagon,driver_amazon,driver_supergx,driver_hyhoo,driver_hyhoo2,driver_bigfghtr,driver_telmahjn,driver_terracra,driver_terracre,driver_terracrb,driver_terraf,driver_terrafu,driver_tokimbsj,driver_tokyogal,driver_tubep,driver_wiping,driver_youma"

COREDEFS += -DTINY_POINTER="&driver_armedf,&driver_bijokkog,&driver_bijokkoy,&driver_citylove,&driver_mcitylov,&driver_cop01,&driver_cop01a,&driver_cclimbrj,&driver_cclimber,&driver_cclmbr2a,&driver_cclimbr2,&driver_crystalg,&driver_crystal2,&driver_dacholer,&driver_dangar,&driver_dangar2,&driver_friskyt,&driver_friskyta,&driver_galivan2,&driver_galivan,&driver_gionbana,&driver_mgion,&driver_gomoku,&driver_hanaoji,&driver_housemnq,&driver_housemn2,&driver_iemoto,&driver_iemotom,&driver_kickboy,&driver_horekid,&driver_kodure,&driver_legiono,&driver_legion,&driver_magmax,&driver_bakuhatu,&driver_chinmoku,&driver_cmehyou,&driver_club90s,&driver_club90sa,&driver_finalbny,&driver_mjfocus,&driver_mjfocusm,&driver_galkaika,&driver_galkoku,&driver_mgmen89,&driver_mjgottsu,&driver_hanamomo,&driver_mhhonban,&driver_hyouban,&driver_janbari,&driver_mjlstory,&driver_mkeibaou,&driver_koinomp,&driver_mjkoiura,&driver_mkoiuraa,&driver_mjlaman,&driver_lovehous,&driver_ntopstar,&driver_patimono,&driver_pstadium,&driver_sailorws,&driver_sailorwa,&driver_sailorwr,&driver_msjiken,&driver_mjsikakb,&driver_mjsikakd,&driver_mjsikakc,&driver_mjsikaku,&driver_mmsikaku,&driver_mladyhtr,&driver_triplew1,&driver_triplew2,&driver_uchuuai,&driver_mjuraden,&driver_vanilla,&driver_wcatcher,&driver_yosimoto,&driver_korinai,&driver_korinaim,&driver_maiko,&driver_mmaiko,&driver_mmehyou,&driver_mjgottub,&driver_mjanbari,&driver_pachiten,&driver_mightguy,&driver_mcontest,&driver_moonaln,&driver_moonal2,&driver_moonal2b,&driver_moonbase,&driver_mooncrs2,&driver_mooncrst,&driver_mooncrsa,&driver_mooncrsu,&driver_moonqsr,&driver_mshutlj2,&driver_mshuttlj,&driver_mshuttle,&driver_musobana,&driver_ngpgal,&driver_ninjemak,&driver_niyanpai,&driver_ojousan,&driver_ojousanm,&driver_pairsnb,&driver_pastelg,&driver_qmhayaku,&driver_radrad,&driver_rjammer,&driver_rollingc,&driver_rugrats,&driver_scandal,&driver_scandalm,&driver_secolove,&driver_sectrzon,&driver_amatelas,&driver_seicross,&driver_seiha,&driver_seiham,&driver_sfx,&driver_skelagon,&driver_amazon,&driver_supergx,&driver_hyhoo,&driver_hyhoo2,&driver_bigfghtr,&driver_telmahjn,&driver_terracra,&driver_terracre,&driver_terracrb,&driver_terraf,&driver_terrafu,&driver_tokimbsj,&driver_tokyogal,&driver_tubep,&driver_wiping,&driver_youma"


# uses these CPUs
CPUS+=M68000@
CPUS+=8080@
CPUS+=NSC8105@
CPUS+=Z80@

CPUS+=N2A03@


# uses these SOUNDs
SOUNDS+=AY8910@
SOUNDS+=CUSTOM@
SOUNDS+=DAC@
SOUNDS+=MSM5205@
SOUNDS+=SAMPLES@
SOUNDS+=SN76477@
SOUNDS+=YM2203@
SOUNDS+=YM3526@
SOUNDS+=YM3812@

SOUNDS+=DISCRETE@
SOUNDS+=TMS5110@
SOUNDS+=SN76496@
SOUNDS+=NES@
SOUNDS+=NAMCO@
SOUNDS+=TMS36XX@

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
