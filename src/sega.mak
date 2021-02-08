# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="driver_005,driver_alibaba,driver_angelkds,driver_astrob1,driver_astrob2,driver_astrob2a,driver_astrob,driver_astrofl,driver_blockgal,driver_brdrline,driver_buckrog,driver_buckrogn,driver_calorie,driver_carnvckt,driver_carnival,driver_champbas,driver_champbb2,driver_chboxing,driver_chwrestl,driver_chplft,driver_chplftb,driver_commsega,driver_congo,driver_dakkochn,driver_digger,driver_dotrikun,driver_dotriku2,driver_fantzn2,driver_flashgal,driver_flashgla,driver_flicky,driver_flickyo,driver_futspy,driver_gardia,driver_hangonjr,driver_headon2,driver_hvymetal,driver_hopprobo,driver_invinco,driver_invds,driver_invho2,driver_ixion,driver_kopunch,driver_legend,driver_mrviking,driver_mrvikngj,driver_monsterb,driver_monster2,driver_myhero,driver_ninja,driver_nprincso,driver_nprincsu,driver_nsub,driver_opaopa,driver_pengo,driver_pengo2u,driver_pengo2,driver_pengo3u,driver_pengo4,driver_pignewta,driver_pignewt,driver_pitfall2,driver_pitfallu,driver_pulsar,driver_razmataz,driver_regulus,driver_regulusu,driver_reguluso,driver_repulse,driver_ridleofp,driver_robowres,driver_samurai,driver_ssanchan,driver_seganinj,driver_seganinu,driver_shtngmst,driver_sindbadm,driver_sspacatc,driver_sspaceat,driver_sspacat2,driver_sspacat3,driver_sspacaho,driver_spacfura,driver_spacfury,driver_spaceod,driver_spcpostn,driver_stactics,driver_sptrekct,driver_spacetrk,driver_spatter,driver_starjack,driver_startrek,driver_subroc3d,driver_suprloco,driver_szaxxon,driver_tacscan,driver_teddybb,driver_teddybbo,driver_tiptop,driver_tokisens,driver_tranqgun,driver_transfrm,driver_turbo,driver_turboa,driver_turbob,driver_ufosensi,driver_upndown,driver_upndownu,driver_wmatch,driver_wboyu,driver_wboy,driver_wboyo,driver_wboy2u,driver_wboy2,driver_wboy3,driver_wbdeluxe,driver_wbml,driver_wbmljo,driver_yamato,driver_yamato2,driver_zaxxon,driver_zaxxon2,driver_zektor,driver_zoom909"

COREDEFS += -DTINY_POINTER="&driver_005,&driver_alibaba,&driver_angelkds,&driver_astrob1,&driver_astrob2,&driver_astrob2a,&driver_astrob,&driver_astrofl,&driver_blockgal,&driver_brdrline,&driver_buckrog,&driver_buckrogn,&driver_calorie,&driver_carnvckt,&driver_carnival,&driver_champbas,&driver_champbb2,&driver_chboxing,&driver_chwrestl,&driver_chplft,&driver_chplftb,&driver_commsega,&driver_congo,&driver_dakkochn,&driver_digger,&driver_dotrikun,&driver_dotriku2,&driver_fantzn2,&driver_flashgal,&driver_flashgla,&driver_flicky,&driver_flickyo,&driver_futspy,&driver_gardia,&driver_hangonjr,&driver_headon2,&driver_hvymetal,&driver_hopprobo,&driver_invinco,&driver_invds,&driver_invho2,&driver_ixion,&driver_kopunch,&driver_legend,&driver_mrviking,&driver_mrvikngj,&driver_monsterb,&driver_monster2,&driver_myhero,&driver_ninja,&driver_nprincso,&driver_nprincsu,&driver_nsub,&driver_opaopa,&driver_pengo,&driver_pengo2u,&driver_pengo2,&driver_pengo3u,&driver_pengo4,&driver_pignewta,&driver_pignewt,&driver_pitfall2,&driver_pitfallu,&driver_pulsar,&driver_razmataz,&driver_regulus,&driver_regulusu,&driver_reguluso,&driver_repulse,&driver_ridleofp,&driver_robowres,&driver_samurai,&driver_ssanchan,&driver_seganinj,&driver_seganinu,&driver_shtngmst,&driver_sindbadm,&driver_sspacatc,&driver_sspaceat,&driver_sspacat2,&driver_sspacat3,&driver_sspacaho,&driver_spacfura,&driver_spacfury,&driver_spaceod,&driver_spcpostn,&driver_stactics,&driver_sptrekct,&driver_spacetrk,&driver_spatter,&driver_starjack,&driver_startrek,&driver_subroc3d,&driver_suprloco,&driver_szaxxon,&driver_tacscan,&driver_teddybb,&driver_teddybbo,&driver_tiptop,&driver_tokisens,&driver_tranqgun,&driver_transfrm,&driver_turbo,&driver_turboa,&driver_turbob,&driver_ufosensi,&driver_upndown,&driver_upndownu,&driver_wmatch,&driver_wboyu,&driver_wboy,&driver_wboyo,&driver_wboy2u,&driver_wboy2,&driver_wboy3,&driver_wbdeluxe,&driver_wbml,&driver_wbmljo,&driver_yamato,&driver_yamato2,&driver_zaxxon,&driver_zaxxon2,&driver_zektor,&driver_zoom909"

# uses these CPUs
CPUS+=8080@
CPUS+=I8035@
CPUS+=I8039@
CPUS+=N7751@
CPUS+=Z80@

CPUS+=N2A03@

#CPUS+=I8751@
#CPUS+=I8051@
#CPUS+=I8052@
#CPUS+=I8752@


# uses these SOUNDs
SOUNDS+=AY8910@
SOUNDS+=CUSTOM@
SOUNDS+=DAC@
SOUNDS+=MSM5205@
SOUNDS+=NAMCO@
SOUNDS+=SAMPLES@
SOUNDS+=SN76496@
SOUNDS+=SP0250@
SOUNDS+=TMS36XX@
SOUNDS+=YM2203@

SOUNDS+=NES@
SOUNDS+=TMS5110@
SOUNDS+=DISCRETE@

#SOUNDS+=OKIM6295@
#SOUNDS+=YM2151@
#SOUNDS+=YM3812@

#SOUNDS+=YM3438@
#SOUNDS+=RF5C68@
#SOUNDS+=SEGAPCM@
#SOUNDS+=YM2612@
#SOUNDS+=UPD7759@
#SOUNDS+=YM2413@


#SOUNDS+=MULTIPCM@

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
