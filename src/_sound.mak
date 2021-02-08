###########################################################################
#
#   sound.mak
#
#   Rules for building sound cores
#
#   Copyright (c) 1996-2007, Nicola Salmoria and the MAME Team.
#   Visit http://mamedev.org for licensing and usage restrictions.
#
###########################################################################


#-------------------------------------------------
# Core sound types
#-------------------------------------------------

ifneq ($(filter CUSTOM,$(SOUNDS)),)
SOUNDDEFS += -DHAS_CUSTOM=1
SOUNDOBJS += $(OBJ_SOUND)/custom.o
else
SOUNDDEFS += -DHAS_CUSTOM=0
endif

ifneq ($(filter SAMPLES,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SAMPLES=1
SOUNDOBJS += $(OBJ_SOUND)/samples.o
else
SOUNDDEFS += -DHAS_SAMPLES=0
endif


#-------------------------------------------------
# DACs
#-------------------------------------------------

ifneq ($(filter DAC,$(SOUNDS)),)
SOUNDDEFS += -DHAS_DAC=1
SOUNDOBJS += $(OBJ_SOUND)/dac.o
else
SOUNDDEFS += -DHAS_DAC=0
endif

ifneq ($(filter DMADAC,$(SOUNDS)),)
SOUNDDEFS += -DHAS_DMADAC=1
SOUNDOBJS += $(OBJ_SOUND)/dmadac.o
else
SOUNDDEFS += -DHAS_DMADAC=0
endif

#-------------------------------------------------
# CD audio
#-------------------------------------------------

ifneq ($(filter CDDA,$(SOUNDS)),)
SOUNDDEFS += -DHAS_CDDA=1
SOUNDOBJS += $(OBJ_SOUND)/cdda.o
else
SOUNDDEFS += -DHAS_CDDA=0
endif

#-------------------------------------------------
# Discrete component audio
#-------------------------------------------------

ifneq ($(filter DISCRETE,$(SOUNDS)),)
SOUNDDEFS += -DHAS_DISCRETE=1
SOUNDOBJS += $(OBJ_SOUND)/discrete.o
#$(OBJ_SOUND)/discrete.o: src/sound/discrete.c src/sound/discrete.h
#		src/sound/disc_dev.c src/sound/disc_flt.c src/sound/disc_inp.c
#		src/sound/disc_mth.c src/sound/disc_wav.c
else
SOUNDDEFS += -DHAS_DISCRETE=0
endif

#-------------------------------------------------
# Atari custom sound chips
#-------------------------------------------------

ifneq ($(filter POKEY,$(SOUNDS)),)
SOUNDDEFS += -DHAS_POKEY=1
SOUNDOBJS += $(OBJ_SOUND)/pokey.o
else
SOUNDDEFS += -DHAS_POKEY=0
endif

ifneq ($(filter TIA,$(SOUNDS)),)
SOUNDDEFS += -DHAS_TIA=1
SOUNDOBJS += $(OBJ_SOUND)/tiasound.o $(OBJ_SOUND)/tiaintf.o
else
SOUNDDEFS += -DHAS_TIA=0
endif

#-------------------------------------------------
# Bally Astrocade sound system
#-------------------------------------------------

##[NO_SUPPORT]
##ifneq ($(filter ASTROCADE,$(SOUNDS)),)
####SOUNDDEFS += -DHAS_ASTROCADE=1
##SOUNDOBJS += $(OBJ_SOUND)/astrocde.o
##else
##SOUNDDEFS += -DHAS_ASTROCADE=0
##endif

#-------------------------------------------------
# CEM 3394 analog synthesizer chip
#-------------------------------------------------

ifneq ($(filter CEM3394,$(SOUNDS)),)
SOUNDDEFS += -DHAS_CEM3394=1
SOUNDOBJS += $(OBJ_SOUND)/cem3394.o
else
SOUNDDEFS += -DHAS_CEM3394=0
endif

#-------------------------------------------------
# Data East custom sound chips
#-------------------------------------------------

ifneq ($(filter BSMT2000,$(SOUNDS)),)
SOUNDDEFS += -DHAS_BSMT2000=1
SOUNDOBJS += $(OBJ_SOUND)/bsmt2000.o
else
SOUNDDEFS += -DHAS_BSMT2000=0
endif

#-------------------------------------------------
# Ensoniq 5503 (Apple IIgs)
#-------------------------------------------------

#-------------------------------------------------
# Ensoniq 5505/5506
#-------------------------------------------------

ifneq ($(filter ES5505,$(SOUNDS)),)
SOUNDDEFS += -DHAS_ES5505=1
SOUNDOBJS += $(OBJ_SOUND)/es5506.o
else
SOUNDDEFS += -DHAS_ES5505=0
endif

ifneq ($(filter ES5506,$(SOUNDS)),)
SOUNDDEFS += -DHAS_ES5506=1
SOUNDOBJS += $(OBJ_SOUND)/es5506.o
else
SOUNDDEFS += -DHAS_ES5506=0
endif

#-------------------------------------------------
# Excellent Systems ADPCM sound chip
#-------------------------------------------------

ifneq ($(filter ES8712,$(SOUNDS)),)
SOUNDDEFS += -DHAS_ES8712=1
SOUNDOBJS += $(OBJ_SOUND)/es8712.o
else
SOUNDDEFS += -DHAS_ES8712=0
endif

#-------------------------------------------------
# Gaelco custom sound chips
#-------------------------------------------------

ifneq ($(filter GAELCO_CG1V,$(SOUNDS)),)
SOUNDDEFS += -DHAS_GAELCO_CG1V=1
SOUNDOBJS += $(OBJ_SOUND)/gaelco.o
else
SOUNDDEFS += -DHAS_GAELCO_CG1V=0
endif

ifneq ($(filter GAELCO_GAE1,$(SOUNDS)),)
SOUNDDEFS += -DHAS_GAELCO_GAE1=1
SOUNDOBJS += $(OBJ_SOUND)/gaelco.o
else
SOUNDDEFS += -DHAS_GAELCO_GAE1=0
endif

#-------------------------------------------------
# RCA CDP1869
#-------------------------------------------------

#-------------------------------------------------
# GI AY-8910
#-------------------------------------------------

##############
# mame純正OPN(YM2203/YM2610/YM2610B)用SSGの場合、「後で」インクルード。
ifneq ($(filter AY8910,$(SOUNDS)),)
SOUNDDEFS += -DHAS_AY8910=1
SOUNDOBJS += $(OBJ_SOUND)/ay8910.o
else
SOUNDDEFS += -DHAS_AY8910=0
endif
##############

#-------------------------------------------------
# Harris HC55516 CVSD
#-------------------------------------------------

ifneq ($(filter HC55516,$(SOUNDS)),)
SOUNDDEFS += -DHAS_HC55516=1
SOUNDOBJS += $(OBJ_SOUND)/hc55516.o
else
SOUNDDEFS += -DHAS_HC55516=0
endif

#-------------------------------------------------
# Hudsonsoft C6280 sound chip
#-------------------------------------------------

ifneq ($(filter C6280,$(SOUNDS)),)
SOUNDDEFS += -DHAS_C6280=1
SOUNDOBJS += $(OBJ_SOUND)/c6280.o
else
SOUNDDEFS += -DHAS_C6280=0
endif

#-------------------------------------------------
# ICS2115 sound chip
#-------------------------------------------------

ifneq ($(filter ICS2115,$(SOUNDS)),)
SOUNDDEFS += -DHAS_ICS2115=1
SOUNDOBJS += $(OBJ_SOUND)/ics2115.o
else
SOUNDDEFS += -DHAS_ICS2115=0
endif

#-------------------------------------------------
# Irem custom sound chips
#-------------------------------------------------

ifneq ($(filter IREMGA20,$(SOUNDS)),)
SOUNDDEFS += -DHAS_IREMGA20=1
SOUNDOBJS += $(OBJ_SOUND)/iremga20.o
else
SOUNDDEFS += -DHAS_IREMGA20=0
endif

#-------------------------------------------------
# Konami custom sound chips
#-------------------------------------------------

ifneq ($(filter K005289,$(SOUNDS)),)
SOUNDDEFS += -DHAS_K005289=1
SOUNDOBJS += $(OBJ_SOUND)/k005289.o
else
SOUNDDEFS += -DHAS_K005289=0
endif

ifneq ($(filter K007232,$(SOUNDS)),)
SOUNDDEFS += -DHAS_K007232=1
SOUNDOBJS += $(OBJ_SOUND)/k007232.o
else
SOUNDDEFS += -DHAS_K007232=0
endif

ifneq ($(filter K051649,$(SOUNDS)),)
SOUNDDEFS += -DHAS_K051649=1
SOUNDOBJS += $(OBJ_SOUND)/k051649.o
else
SOUNDDEFS += -DHAS_K051649=0
endif

ifneq ($(filter K053260,$(SOUNDS)),)
SOUNDDEFS += -DHAS_K053260=1
SOUNDOBJS += $(OBJ_SOUND)/k053260.o
else
SOUNDDEFS += -DHAS_K053260=0
endif

ifneq ($(filter K054539,$(SOUNDS)),)
SOUNDDEFS += -DHAS_K054539=1
SOUNDOBJS += $(OBJ_SOUND)/k054539.o
else
SOUNDDEFS += -DHAS_K054539=0
endif

#-------------------------------------------------
# Namco custom sound chips
#-------------------------------------------------

ifneq ($(filter NAMCO,$(SOUNDS)),)
SOUNDDEFS += -DHAS_NAMCO=1
SOUNDOBJS += $(OBJ_SOUND)/namco.o
else
SOUNDDEFS += -DHAS_NAMCO=0
endif

ifneq ($(filter NAMCO_15XX,$(SOUNDS)),)
SOUNDDEFS += -DHAS_NAMCO_15XX=1
SOUNDOBJS += $(OBJ_SOUND)/namco.o
else
SOUNDDEFS += -DHAS_NAMCO_15XX=0
endif

ifneq ($(filter NAMCO_CUS30,$(SOUNDS)),)
SOUNDDEFS += -DHAS_NAMCO_CUS30=1
SOUNDOBJS += $(OBJ_SOUND)/namco.o
else
SOUNDDEFS += -DHAS_NAMCO_CUS30=0
endif

ifneq ($(filter NAMCO_52XX,$(SOUNDS)),)
SOUNDDEFS += -DHAS_NAMCO_52XX=1
SOUNDOBJS += $(OBJ_SOUND)/namco52.o
else
SOUNDDEFS += -DHAS_NAMCO_52XX=0
endif

ifneq ($(filter NAMCO_54XX,$(SOUNDS)),)
SOUNDDEFS += -DHAS_NAMCO_54XX=1
SOUNDOBJS += $(OBJ_SOUND)/namco54.o
else
SOUNDDEFS += -DHAS_NAMCO_54XX=0
endif

ifneq ($(filter NAMCO_63701X,$(SOUNDS)),)
SOUNDDEFS += -DHAS_NAMCO_63701X=1
SOUNDOBJS += $(OBJ_SOUND)/n63701x.o
else
SOUNDDEFS += -DHAS_NAMCO_63701X=0
endif

ifneq ($(filter NAMCONA,$(SOUNDS)),)
SOUNDDEFS += -DHAS_NAMCONA=1
SOUNDOBJS += $(OBJ_SOUND)/namcona.o
else
SOUNDDEFS += -DHAS_NAMCONA=0
endif

ifneq ($(filter C140,$(SOUNDS)),)
SOUNDDEFS += -DHAS_C140=1
SOUNDOBJS += $(OBJ_SOUND)/c140.o
else
SOUNDDEFS += -DHAS_C140=0
endif

ifneq ($(filter C352,$(SOUNDS)),)
SOUNDDEFS += -DHAS_C352=1
SOUNDOBJS += $(OBJ_SOUND)/c352.o
else
SOUNDDEFS += -DHAS_C352=0
endif

#-------------------------------------------------
# Nintendo custom sound chips
#-------------------------------------------------

ifneq ($(filter NES,$(SOUNDS)),)
SOUNDDEFS += -DHAS_NES=1
ifndef MESS
SOUNDOBJS += $(OBJ_SOUND)/nes_apu.o
else
SOUNDOBJS += $(OBJ)/mess/sound/nes_apu2.o $(OBJ)/mess/sound/nesintf.o
endif
else
SOUNDDEFS += -DHAS_NES=0
endif

#-------------------------------------------------
# NEC uPD7759 ADPCM sample player
#-------------------------------------------------

ifneq ($(filter UPD7759,$(SOUNDS)),)
SOUNDDEFS += -DHAS_UPD7759=1
SOUNDOBJS += $(OBJ_SOUND)/upd7759.o
else
SOUNDDEFS += -DHAS_UPD7759=0
endif

#-------------------------------------------------
# OKI ADPCM sample players
#-------------------------------------------------

ifneq ($(filter MSM5205,$(SOUNDS)),)
SOUNDDEFS += -DHAS_MSM5205=1
SOUNDOBJS += $(OBJ_SOUND)/msm5205.o
else
SOUNDDEFS += -DHAS_MSM5205=0
endif

ifneq ($(filter MSM5232,$(SOUNDS)),)
SOUNDDEFS += -DHAS_MSM5232=1
SOUNDOBJS += $(OBJ_SOUND)/msm5232.o
else
SOUNDDEFS += -DHAS_MSM5232=0
endif

ifneq ($(filter OKIM6295,$(SOUNDS)),)
SOUNDDEFS += -DHAS_OKIM6295=1
SOUNDOBJS += $(OBJ_SOUND)/okim6295.o
else
SOUNDDEFS += -DHAS_OKIM6295=0
endif

#-------------------------------------------------
# Philips SAA1099
#-------------------------------------------------

ifneq ($(filter SAA1099,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SAA1099=1
SOUNDOBJS += $(OBJ_SOUND)/saa1099.o
else
SOUNDDEFS += -DHAS_SAA1099=0
endif

#-------------------------------------------------
# QSound sample player
#-------------------------------------------------

ifneq ($(filter QSOUND,$(SOUNDS)),)
SOUNDDEFS += -DHAS_QSOUND=1
SOUNDOBJS += $(OBJ_SOUND)/qsound.o
else
SOUNDDEFS += -DHAS_QSOUND=0
endif

#-------------------------------------------------
# Ricoh sample players
#-------------------------------------------------

ifneq ($(filter RF5C68,$(SOUNDS)),)
SOUNDDEFS += -DHAS_RF5C68=1
SOUNDOBJS += $(OBJ_SOUND)/rf5c68.o
else
SOUNDDEFS += -DHAS_RF5C68=0
endif

#-------------------------------------------------
# Sega custom sound chips
#-------------------------------------------------

ifneq ($(filter SEGAPCM,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SEGAPCM=1
SOUNDOBJS += $(OBJ_SOUND)/segapcm.o
else
SOUNDDEFS += -DHAS_SEGAPCM=0
endif

ifneq ($(filter MULTIPCM,$(SOUNDS)),)
SOUNDDEFS += -DHAS_MULTIPCM=1
SOUNDOBJS += $(OBJ_SOUND)/multipcm.o
else
SOUNDDEFS += -DHAS_MULTIPCM=0
endif

ifneq ($(filter SCSP,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SCSP=1
SOUNDOBJS += $(OBJ_SOUND)/scsp.o
else
SOUNDDEFS += -DHAS_SCSP=0
endif

#-------------------------------------------------
# Seta custom sound chips
#-------------------------------------------------

ifneq ($(filter ST0016,$(SOUNDS)),)
SOUNDDEFS += -DHAS_ST0016=1
SOUNDOBJS += $(OBJ_SOUND)/st0016.o
else
SOUNDDEFS += -DHAS_ST0016=0
endif

ifneq ($(filter X1_010,$(SOUNDS)),)
SOUNDDEFS += -DHAS_X1_010=1
SOUNDOBJS += $(OBJ_SOUND)/x1_010.o
else
SOUNDDEFS += -DHAS_X1_010=0
endif

#-------------------------------------------------
# SID custom sound chips
#-------------------------------------------------

#-------------------------------------------------
# Sony custom sound chips
#-------------------------------------------------

ifneq ($(filter PSXSPU,$(SOUNDS)),)
SOUNDDEFS += -DHAS_PSXSPU=1
SOUNDOBJS += $(OBJ_SOUND)/psx.o
else
SOUNDDEFS += -DHAS_PSXSPU=0
endif

#-------------------------------------------------
# SP0256 speech synthesizer
#-------------------------------------------------

#-------------------------------------------------
# SP0250 speech synthesizer
#-------------------------------------------------

ifneq ($(filter SP0250,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SP0250=1
SOUNDOBJS += $(OBJ_SOUND)/sp0250.o
else
SOUNDDEFS += -DHAS_SP0250=0
endif

#-------------------------------------------------
# S14001A speech synthesizer
#-------------------------------------------------

#-------------------------------------------------
# Texas Instruments SN76477 analog chip
#-------------------------------------------------

ifneq ($(filter SN76477,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SN76477=1
SOUNDOBJS += $(OBJ_SOUND)/sn76477.o
else
SOUNDDEFS += -DHAS_SN76477=0
endif

#-------------------------------------------------
# Texas Instruments SN76496
#-------------------------------------------------

ifneq ($(filter SN76496,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SN76496=1
SOUNDOBJS += $(OBJ_SOUND)/sn76496.o
else
SOUNDDEFS += -DHAS_SN76496=0
endif

ifneq ($(filter SN76489,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SN76489=1
SOUNDOBJS += $(OBJ_SOUND)/sn76496.o
else
SOUNDDEFS += -DHAS_SN76489=0
endif

ifneq ($(filter SN76489A,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SN76489A=1
SOUNDOBJS += $(OBJ_SOUND)/sn76496.o
else
SOUNDDEFS += -DHAS_SN76489A=0
endif

#-------------------------------------------------
# Texas Instruments TMS36xx doorbell chime
#-------------------------------------------------

ifneq ($(filter TMS36XX,$(SOUNDS)),)
SOUNDDEFS += -DHAS_TMS36XX=1
SOUNDOBJS += $(OBJ_SOUND)/tms36xx.o
else
SOUNDDEFS += -DHAS_TMS36XX=0
endif

#-------------------------------------------------
# Texas Instruments TMS3615 Octave Multiple Tone Synthesizer
#-------------------------------------------------

#-------------------------------------------------
# Texas Instruments TMS5110 speech synthesizers
#-------------------------------------------------

ifneq ($(filter TMS5110,$(SOUNDS)),)
SOUNDDEFS += -DHAS_TMS5110=1
SOUNDOBJS += $(OBJ_SOUND)/tms5110.o $(OBJ_SOUND)/5110intf.o
else
SOUNDDEFS += -DHAS_TMS5110=0
endif

ifneq ($(filter TMS5220,$(SOUNDS)),)
SOUNDDEFS += -DHAS_TMS5220=1
SOUNDOBJS += $(OBJ_SOUND)/tms5220.o $(OBJ_SOUND)/5220intf.o
else
SOUNDDEFS += -DHAS_TMS5220=0
endif

#-------------------------------------------------
# VLM5030 speech synthesizer
#-------------------------------------------------

ifneq ($(filter VLM5030,$(SOUNDS)),)
SOUNDDEFS += -DHAS_VLM5030=1
SOUNDOBJS += $(OBJ_SOUND)/vlm5030.o
else
SOUNDDEFS += -DHAS_VLM5030=0
endif

#-------------------------------------------------
# Votrax speech synthesizer
#-------------------------------------------------

ifneq ($(filter VOTRAX,$(SOUNDS)),)
SOUNDDEFS += -DHAS_VOTRAX=1
SOUNDOBJS += $(OBJ_SOUND)/votrax.o
else
SOUNDDEFS += -DHAS_VOTRAX=0
endif

#-------------------------------------------------
# VRender0 custom sound chip
#-------------------------------------------------

ifneq ($(filter VRENDER0,$(SOUNDS)),)
SOUNDDEFS += -DHAS_VRENDER0=1
SOUNDOBJS += $(OBJ_SOUND)/vrender0.o
else
SOUNDDEFS += -DHAS_VRENDER0=0
endif

#-------------------------------------------------
# WAVE file (used for MESS cassette)
#-------------------------------------------------

#-------------------------------------------------
# Yamaha FM synthesizers
#-------------------------------------------------

#########
#YM2151TYPE = REF2151
#YM2151TYPE = TESTB
 YM2151TYPE = TESTC
########
ifeq ($(YM2151TYPE),REF2151)
ifneq ($(filter YM2151,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2151=1
 SOUNDOBJS += $(OBJ_SOUND)/2151intf.o
# $(OBJ_SOUND)/ym2151mame.o
#SOUNDOBJS += $(OBJ_SOUND)/2151intf.o $(OBJ_SOUND)/gens2612.o
else
SOUNDDEFS += -DHAS_YM2151=0
endif
endif
#########
ifeq ($(YM2151TYPE),TESTB)
ifneq ($(filter YM2151,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2151=1
 SOUNDOBJS += $(OBJ_SOUND)/2151Bintf.o
else
SOUNDDEFS += -DHAS_YM2151=0
endif
endif
#########
ifeq ($(YM2151TYPE),TESTC)
ifneq ($(filter YM2151,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2151=1
 SOUNDOBJS += $(OBJ_SOUND)/2151Cintf.o
else
SOUNDDEFS += -DHAS_YM2151=0
endif
endif
#########



#########
#YM2203TYPE = REF2203
#YM2203TYPE = TESTA_4STREAM
 YM2203TYPE = TESTC_1STREAM
########
#ifeq ($(TARGET),air_buster)
#	#YM2203TYPE = REF2203
#	 YM2203TYPE = TESTA_4STREAM
#endif
########
#ifeq ($(TARGET),ninjya_kun2)
#	#YM2203TYPE = REF2203
#	 YM2203TYPE = TESTA_4STREAM
#endif
####################
ifeq ($(YM2203TYPE),REF2203)
ifneq ($(filter YM2203,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2203=1
SOUNDOBJS += $(OBJ_SOUND)/2203intf.o $(OBJ_SOUND)/ay8910ym.o $(OBJ_SOUND)/fm.o
else
SOUNDDEFS += -DHAS_YM2203=0
endif
endif
########
ifeq ($(YM2203TYPE),TESTA_4STREAM)
ifneq ($(filter YM2203,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2203=1
SOUNDOBJS += $(OBJ_SOUND)/2203Aintf.o
else
SOUNDDEFS += -DHAS_YM2203=0
endif
endif
########
#/* サイドアームダメ？？？ */
ifeq ($(YM2203TYPE),TESTC_1STREAM)
ifneq ($(filter YM2203,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2203=1
SOUNDOBJS += $(OBJ_SOUND)/2203Cintf.o
else
SOUNDDEFS += -DHAS_YM2203=0
endif
endif
########


ifneq ($(filter YM2413,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2413=1
SOUNDOBJS += $(OBJ_SOUND)/2413intf.o $(OBJ_SOUND)/ym2413.o
else
SOUNDDEFS += -DHAS_YM2413=0
endif

ifneq ($(filter YM2203_ORIG,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2203=1
SOUNDOBJS += $(OBJ)/sound/2203intf.o $(OBJ)/sound/ay8910.o $(OBJ)/sound/fm.o
else
SOUNDDEFS += -DHAS_YM2203=0
endif


ifneq ($(filter YM2608,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2608=1
# SOUNDS+=AY8910@ # もし「ay8910ym」を使わない場合に追記。
SOUNDOBJS += $(OBJ_SOUND)/2608intf.o $(OBJ_SOUND)/ay8910ym.o $(OBJ_SOUND)/fm.o $(OBJ_SOUND)/ymdeltat.o
else
SOUNDDEFS += -DHAS_YM2608=0
endif



#########
#YM2610TYPE = REF2610
 YM2610TYPE = TESTA
#YM2610TYPE = TESTB

#########
ifeq ($(YM2610TYPE),REF2610)
ifneq ($(filter YM2610,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2610=1
# SOUNDS+=AY8910@ # もし「ay8910ym」を使わない場合に追記。
SOUNDOBJS += $(OBJ_SOUND)/2610intf.o $(OBJ_SOUND)/ay8910ym.o $(OBJ_SOUND)/fm.o $(OBJ_SOUND)/ymdeltat.o
else
SOUNDDEFS += -DHAS_YM2610=0
endif
endif
#########
ifeq ($(YM2610TYPE),TESTA)
ifneq ($(filter YM2610,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2610=1
SOUNDOBJS += $(OBJ_SOUND)/2610_A2intf.o
else
SOUNDDEFS += -DHAS_YM2610=0
endif
endif
#########
ifeq ($(YM2610TYPE),TESTB)
ifneq ($(filter YM2610,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2610=1
SOUNDOBJS += $(OBJ_SOUND)/2610_Bintf.o
else
SOUNDDEFS += -DHAS_YM2610=0
endif
endif
#########
ifneq ($(filter YM2610B,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2610B=1
# SOUNDS+=AY8910@ # もし「ay8910ym」を使わない場合に追記。
SOUNDOBJS += $(OBJ_SOUND)/2610intf.o $(OBJ_SOUND)/ay8910ym.o $(OBJ_SOUND)/fm.o $(OBJ_SOUND)/ymdeltat.o
else
SOUNDDEFS += -DHAS_YM2610B=0
endif
#########


ifneq ($(filter SN76496,$(SOUNDS)),)
SOUNDDEFS += -DHAS_SN76496=1
SOUNDOBJS += $(OBJ)/sound/sn76496.o
else
SOUNDDEFS += -DHAS_SN76496=0
endif
#########


ifneq ($(filter YM2612,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2612=1
# SOUNDS+=AY8910@ # もし「ay8910ym」を使わない場合に追記。 $(OBJ_SOUND)/ay8910ym.o
SOUNDOBJS += $(OBJ_SOUND)/2612intf.o $(OBJ_SOUND)/fm.o
else
SOUNDDEFS += -DHAS_YM2612=0
endif

ifneq ($(filter YM3438,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM3438=1
# SOUNDS+=AY8910@ # もし「ay8910ym」を使わない場合に追記。 $(OBJ_SOUND)/ay8910ym.o
SOUNDOBJS += $(OBJ_SOUND)/2612intf.o $(OBJ_SOUND)/fm.o
else
SOUNDDEFS += -DHAS_YM3438=0
endif


########### OPL ###########

#########
#YM3526TYPE = REF3526
 YM3526TYPE = TESTA
#YM3526TYPE = TESTT
###########

###########
ifeq ($(YM3526TYPE),REF3526)
ifneq ($(filter YM3812,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM3812=1
SOUNDOBJS += $(OBJ_SOUND)/3812intf.o $(OBJ_SOUND)/fmopl.o
else
SOUNDDEFS += -DHAS_YM3812=0
endif
endif
##
ifeq ($(YM3526TYPE),TESTA)
ifneq ($(filter YM3812,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM3812=1
SOUNDOBJS += $(OBJ_SOUND)/3526_3812_Aintf.o
else
SOUNDDEFS += -DHAS_YM3812=0
endif
endif
##
ifeq ($(YM3526TYPE),TESTT)
ifneq ($(filter YM3812,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM3812=1
SOUNDOBJS += $(OBJ_SOUND)/3526_3812_Tintf.o
else
SOUNDDEFS += -DHAS_YM3812=0
endif
endif
###########

###########
ifeq ($(YM3526TYPE),REF3526)
ifneq ($(filter YM3526,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM3526=1
SOUNDOBJS += $(OBJ_SOUND)/3812intf.o $(OBJ_SOUND)/fmopl.o
else
SOUNDDEFS += -DHAS_YM3526=0
endif
endif
##
ifeq ($(YM3526TYPE),TESTA)
ifneq ($(filter YM3526,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM3526=1
SOUNDOBJS += $(OBJ_SOUND)/3526_3812_Aintf.o
else
SOUNDDEFS += -DHAS_YM3526=0
endif
endif
##
ifeq ($(YM3526TYPE),TESTT)
ifneq ($(filter YM3526,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM3526=1
SOUNDOBJS += $(OBJ_SOUND)/3526_3812_Tintf.o
else
SOUNDDEFS += -DHAS_YM3526=0
endif
endif
#########



ifneq ($(filter Y8950,$(SOUNDS)),)
SOUNDDEFS += -DHAS_Y8950=1
SOUNDOBJS += $(OBJ_SOUND)/3812intf.o $(OBJ_SOUND)/fmopl.o $(OBJ_SOUND)/ymdeltat.o
else
SOUNDDEFS += -DHAS_Y8950=0
endif

ifneq ($(filter YMF262,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YMF262=1
SOUNDOBJS += $(OBJ_SOUND)/ymf262.o $(OBJ_SOUND)/262intf.o
else
SOUNDDEFS += -DHAS_YMF262=0
endif

ifneq ($(filter YMF271,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YMF271=1
SOUNDOBJS += $(OBJ_SOUND)/ymf271.o
else
SOUNDDEFS += -DHAS_YMF271=0
endif

ifneq ($(filter YMF278B,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YMF278B=1
SOUNDOBJS += $(OBJ_SOUND)/ymf278b.o
else
SOUNDDEFS += -DHAS_YMF278B=0
endif

#-------------------------------------------------
# Yamaha YMZ280B ADPCM
#-------------------------------------------------

##############

ifneq ($(filter YMZ280B,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YMZ280B=1
SOUNDOBJS += $(OBJ_SOUND)/ymz280b.o
else
SOUNDDEFS += -DHAS_YMZ280B=0
endif

################################################################################################
################################################################################################
ifneq ($(filter YM2151_ORIG,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2151=1
SOUNDOBJS += $(OBJ)/sound/2151intf.o $(OBJ)/sound/ym2151.o
else
SOUNDDEFS += -DHAS_YM2151=0
endif

ifneq ($(filter K007232_ORIG,$(SOUNDS)),)
SOUNDDEFS += -DHAS_K007232=1
SOUNDOBJS += $(OBJ)/sound/k007232.o
else
SOUNDDEFS += -DHAS_K007232=0
endif

ifneq ($(filter K053260_ORIG,$(SOUNDS)),)
SOUNDDEFS += -DHAS_K053260=1
SOUNDOBJS += $(OBJ)/sound/k053260.o
else
SOUNDDEFS += -DHAS_K053260=0
endif

ifneq ($(filter YM3812_ORIG,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM3812=1
SOUNDOBJS += $(OBJ)/sound/3812intf.o $(OBJ)/sound/fmopl.o
else
SOUNDDEFS += -DHAS_YM3812=0
endif


ifneq ($(filter YM2612_ORIG,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2612=1
SOUNDOBJS += $(OBJ)/sound/2612intf.o $(OBJ)/sound/ay8910.o $(OBJ)/sound/fm.o
else
SOUNDDEFS += -DHAS_YM2612=0
endif

ifneq ($(filter YM3526_ORIG,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM3526=1
SOUNDOBJS += $(OBJ)/sound/3812intf.o $(OBJ)/sound/fmopl.o
else
SOUNDDEFS += -DHAS_YM3526=0
endif


ifneq ($(filter YM2610_ORIG,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2610=1
SOUNDOBJS += $(OBJ)/sound/2610intf.o $(OBJ)/sound/ay8910.o $(OBJ)/sound/fm.o $(OBJ)/sound/ymdeltat.o
else
SOUNDDEFS += -DHAS_YM2610=1
endif

ifneq ($(filter YM2610B_ORIG,$(SOUNDS)),)
SOUNDDEFS += -DHAS_YM2610B=1
SOUNDOBJS += $(OBJ)/sound/2610intf.o $(OBJ)/sound/ay8910.o $(OBJ)/sound/fm.o $(OBJ)/sound/ymdeltat.o
else
SOUNDDEFS += -DHAS_YM2610B=0
endif
