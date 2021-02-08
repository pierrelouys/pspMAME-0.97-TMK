###########################################################################
#
#   PSPMAME Makefile
#
#   Core makefile for building MAME and derivatives
#
#   Copyright (c) 1996-2007, Nicola Salmoria and the MAME Team.
#   Visit http://mamedev.org for licensing and usage restrictions.
#
###########################################################################


#PSP SLIM STUFF -repuken2-

PSP_LARGE_MEMORY = 1
BUILD_PRX = 1

# set this to mame, mess or the destination you want to build.



#################
# たぶん動く(?)もの
#################

#---- TAITO 実験

#TARGET = ninjya_warriors
#TARGET = darius
#TARGET = taitoB
#TARGET = taitoAX
#TARGET = halleys

#TARGET = night_striker
	# GFX(pspのメモリ)が足りないので無理やり表示。画面がバグる。

#---- namco 実験

#TARGET = namcoNA
#TARGET = mappy
#TARGET = pacman
	# 註：pengo は セガ だけど ナムコ音源。

#TARGET = namcos86
	# namcos86 : mame版は 遅いので、あくまで実験用。NJ氏の専用版使おう。
	# 註：mame版の実験：音源やＣＰＵ等の互換性検証用途。

#---- UPL/capcom 実験 (UVC : UPL vs CAPCOM)

#TARGET = ninjya_kun2

#---- capcom 実験

#TARGET = street_fighter
#TARGET = makai_mura
#TARGET = gun_smoke

#---- SEGA 実験

#TARGET = turbo_outrun
#TARGET = fantasy_zone
	#fantasy_zone ok[20070124]
	#fantasy_zone ng[20070206]終了できない

#---- IREM 実験

#TARGET = m72
	#専用tilemap作り直さないとバグる。CPUも不安定。

#---- TOUA 実験
#TARGET = zerowing
#TARGET = raizing
	#raizing ok[20070124]

#---- NMK実験
#TARGET = nmk

#---- SETA 実験

#TARGET = seta

#---- Nichibutu 実験

#TARGET = terracre
	#terracre ng[20070323](YM3526_ng psg_ok)
	#terracre ok[20070617]ドライバ(src/drivers/terracre.c)(ym3526_info11)のバグでFA.
#TARGET = crazy_climber

#---- konami 実験

#TARGET = ajax
#############NEW MAKES!!!

#TARGET = konami2
TARGET = konami3

#TARGET = thunder_cross
	#thunder_cross ok[20070319](でも音2151(test c)オカシイ)
#TARGET = bubblesys
	#bubblesys ng[20070206]
	#bubblesys ok[20070621](でも音声ミキシング変)

#TARGET = circus_charlie

#---- kaneko 実験

#TARGET = air_buster
	#air_buster ok[20070203](z80s2)

#---- DECO 実験

#TARGET = darwin4078
	#darwin4078 ok[20070619]

#---- tecmo/tehkan 実験

#TARGET = starforce

#---- nintendo 実験

#TARGET = mario_bros

#---- UNIVERSAL 実験

#TARGET = mr_do

#---- SNK 実験

#TARGET = aso



#################↑ここを変えるのでっす。


#################
# 一応動くが、（音源やＣＰＵ等の）実験が目的なもの
#################

#---- TAITO 実験

#TARGET = qix
	# qix : (註：discrete音源、実験用) とてつもなく遅すぎて、
	# （パラメーターを予めセットして別にレンダリングとか）実装方法変えない限り、
	# 現状実用性はない。


#---- DECO 実験

#TARGET = battlera
	# battlera : (註：HuC6280実験用) (HuC6280音源が)遅すぎて現状実用性はない。


#################
# 動くかワカラナイ(たぶん動かない)もの
#################

#################
#TARGET = darius2_dual
#TARGET = block_block

#################↑めも。

#---- TAITO 実験

#TARGET = full_throttle
#TARGET = chuuka_taisen

#TARGET = syvalion

#TARGET = taitoF1
#TARGET = metal_black
#TARGET = taitoF2
#TARGET = darius2d
#TARGET = nightstr
#TARGET = taitoZ
#TARGET = taitoA2
#TARGET = taito2

#---- namco 実験

#TARGET = pacland
	#pacland ng[20070205]
	#pacland ng[20070319]
	# pacland : 試行錯誤したが現状[20070915]動かない。原因不明。
	#ソースは色々変えた(動かそうと戻したり)ので昔の物より悪い。

#TARGET = namcos1	# 起動しない。

#---- SEGA 実験

#TARGET = space_harrier	# 激遅。
#TARGET = cotton

#---- konami 実験
#TARGET = gaia_police

#---- TOUA 実験
#TARGET = slapfight	# 起動しない。

#---- DECO 実験
#TARGET = srd08
#TARGET = deco16
#TARGET = kuga
	# kuga : 起動しない。
#TARGET = the_great_ragtime_show
	# decoic16 複雑すぎ...orz

#---- sega 実験
#TARGET = sys16	# 起動しない。

#---- JALECO 実験
#TARGET = megasys1	# 激遅。バグる。不安定。

#---- tecnos 実験
#TARGET = ddragon	# 激遅。バグる。不安定。

#---- capcom 実験
# black draon は tilemap 使う限り「メモリが足りなくてPSPで起動できない」です。
#TARGET = black_dragon	# 起動しない。

#---- SEIBU 実験
#TARGET = raiden	# 起動しない。


#------------------------------------------------------------------------------
# Auto Adjusts
#------------------------------------------------------------------------------

#TARGET = mame
#TARGET = mess
# example for a tiny compile
#TARGET = tiny
ifeq ($(TARGET),)
TARGET = mame
endif

# customize option
OPT_DEFS = -D$(TARGET)=1


#------------------------------------------------------------------------------
# Configurations
#------------------------------------------------------------------------------

# 註：OSDは将来対応したい気もするケド まだ作ってないです。psp以外はコンパイル出来ません。
# set this the operating system you're building for. ... but now only "psp".
  OSD = psp
# OSD = msdos
# OSD = windows

# psp     for PSPSDK 4.0.2 'http://www.xorloser.com/PSPDevWin32.zip (2nd December 2006)'
# windows for MINGW  3.2.0 'http://www.mame.net/zips/mingw-mame-20070617.exe' 'http://www.mame.net/zips/dx80_mgw.zip'
# msdos   for DJGPP  3.2.1

ifndef OSD
OSD = windows
endif

ifndef TARGETOS
ifeq ($(OSD),windows)
TARGETOS = win32
else
TARGETOS = psp
endif
endif

OPT_DEFS += -D$(OSD)=1



NAME = $(TARGET)$(TARGETOS)

# build the targets in different object dirs, since mess changes
# some structures and thus they can't be linked against each other.

SRC = src
OBJ = obj/$(NAME)

OBJ_CPU     = $(OBJ)/cpu
OBJ_AUDIO   = $(OBJ)/audio
OBJ_SOUND   = $(OBJ)/sound
OBJ_DRIVERS = $(OBJ)/drivers
OBJ_MACHINE = $(OBJ)/machine
#OBJ_VIDEO   = $(OBJ)/video
OBJ_VIDEO   = $(OBJ)/vidhrdw

# CPU core include paths
#VPATH=src $(wildcard $(SRC)/cpu/*)

# uncomment next line to use the new multiwindow debugger
#NEW_DEBUGGER = 1 # 廃止


#------------------------------------------------------------------------------
# Compiler Defines
#------------------------------------------------------------------------------

CDEFS = \
	-DLSB_FIRST \
	-DINLINE="static __inline__" \
	-DPI=M_PI 

#	-DCRLF=3 # 廃止
#	-DPSP=1  # 別で設定
#	-DCLOCKS_PER_SEC=1000000 # 廃止
#	-include psp/psp_main.h  # 別で設定

#ifdef NEW_DEBUGGER
#CDEFS += -DNEW_DEBUGGER # 廃止
#endif


#------------------------------------------------------------------------------
# Compiler Flags
#------------------------------------------------------------------------------

#PSPSDK_HEADER_FIX_WARNING =1
ifdef PSPSDK_HEADER_FIX_WARNING
CFLAGS = -Wstrict-prototypes
else
CFLAGS = 
endif

CFLAGS += \
	-fomit-frame-pointer \
	-fno-strict-aliasing \
	-Wno-sign-compare \
	-Wunused \
	-Wpointer-arith \
	-Wundef \
	-Wformat \
	-Wwrite-strings \
	-Wdisabled-optimization \
	-Wbad-function-cast

#	-std=gnu99 
#	-G0 

# extra options needed *only* for the osd files
#CFLAGSOSDEPEND = $(CFLAGS)

# the windows osd code at least cannot be compiled with -pedantic
#CFLAGSPEDANTIC = $(CFLAGS) -pedantic


#------------------------------------------------------------------------------
# File include path
#------------------------------------------------------------------------------

INCDIR = \
	$(SRC) \
	$(SRC)/includes \
	$(SRC)/debug \
	$(SRC)/$(OSD) \
	$(SRC)/zlib


#------------------------------------------------------------------------------
# Linker Flags
#------------------------------------------------------------------------------

LIBDIR =
#LDFLAGS = -s
LDFLAGS      = -losl -lpspgum_vfpu -lpspvfpu
#LDFLAGS      = -L$(PSPDEV)/lib -lstdc++ -lSDL_mixer -lSDL -lSDL_gfx -lm -lGL -lvorbisidec
#LDFLAGS     += -L$(PSPSDK)/lib -lpspvfpu -lpspdebug -lpspgu -lpspctrl -lpspge -lpspdisplay -lpsphprm -lpspsdk -lpsprtc -lpspaudio -lc -lpspuser -lpsputility -lpspkernel -lpspnet_inet -lpsppower 



#------------------------------------------------------------------------------
# Library
#------------------------------------------------------------------------------

ZLIB = $(OBJ)/libz.a


#------------------------------------------------------------------------------
# Include Make Files
#------------------------------------------------------------------------------

CPUOBJS   = 
SOUNDOBJS = $(OBJ)/sndintrf.o $(OBJ_SOUND)/streams.o $(OBJ)/sound/flt_vol.o $(OBJ)/sound/flt_rc.o \

# windows/dos extra include. 'make all' only, need it at first ???. 
# windows や dos の gcc(ming)の make.exe(3.X.X. ??) は 'make all' の機能が完全で無いミタイ。(_$(OSD).makに書いても無効)
# だからここの場所に必要。
ifeq ($(OSD),msdos)
all:	maked_directry $(EXTRA_TARGETS)
endif
ifeq ($(OSD),windows)
all:	maked_directry $(EXTRA_TARGETS)
endif

# include the various .mak files
include ./makes/$(TARGET).mak
include $(SRC)/_cpu.mak
include $(SRC)/_sound.mak
include $(SRC)/_emu.mak
include $(SRC)/$(OSD)/_$(OSD).mak

#------------------------------------------------------------------------------
# Object Directory
#------------------------------------------------------------------------------

OBJDIRS += \
	obj \
	$(OBJ) \
	$(OBJ)/$(OSD) \
	$(OBJ_CPU) \
	$(OBJ_AUDIO) \
	$(OBJ_SOUND) \
	$(OBJ_DRIVERS) \
	$(OBJ_MACHINE) \
	$(OBJ_VIDEO) \
	$(OBJ)/etc \
	$(OBJ)/zlib \
	pbp \
	pbp/$(TARGET)


CDEFS += $(CPUDEFS) $(SOUNDDEFS) $(COREDEFS) $(DRVDEFS) $(OPT_DEFS)
OBJS  += $(CPUOBJS) $(SOUNDOBJS) $(COREOBJS) $(DRVLIBS) $(OSOBJS) $(ZLIB)

# $(OBJ)/tiny.o
#	$(sort $(CPUOBJS)) 
#	$(sort $(SOUNDOBJS)) 


#------------------------------------------------------------------------------
# Include build.mak for PSPSDK
#------------------------------------------------------------------------------

# 註： ここは  pspsdk の都合上、「後」にインクルード。

ifeq ($(OSD),psp)
USE_MY_PSPSDK = 1
ifdef USE_MY_PSPSDK
	PSPSDK=$(shell psp-config --pspsdk-path)
	include $(PSPSDK)/lib/build.mak
#else
	# 「njemu_source_20070223 互換」 + 「EBOOT.PBP 実行圧縮 対応」
#	include $(SRC)/$(OSD)/_build.mak
#endif
else
	include $(SRC)/$(OSD)/_win_build.mak
endif
endif

#------------------------------------------------------------------------------
# Make Rules
#------------------------------------------------------------------------------

$(OBJ)/libz.a: \
	$(OBJ)/zlib/adler32.o \
	$(OBJ)/zlib/crc32.o \
	$(OBJ)/zlib/inflate.o \
	$(OBJ)/zlib/inftrees.o \
	$(OBJ)/zlib/inffast.o \
	$(OBJ)/zlib/zutil.o

#	$(OBJ)/zlib/trees.o 
#	$(OBJ)/zlib/compress.o 
#	$(OBJ)/zlib/deflate.o 

#	$(OBJ)/zlib/gzio.o 
#	$(OBJ)/zlib/infback.o 
#	$(OBJ)/zlib/uncompr.o 

# zlib only sized optimized (-O1)
$(OBJ)/zlib/%.o: $(SRC)/zlib/%.c
	@echo Compiling Zlib $<...
	@$(CC) $(CDEFS) -O1 $(CFLAGS) -c $< -o $@

# speed optimized (-O3)
$(OBJ)/%.o: $(SRC)/%.c
	@echo Compiling $<...
	@$(CC) $(CDEFS) -O3 $(CFLAGS) -c $< -o $@

$(OBJ)/%.a:
	@echo Archiving $@...
	@$(AR) -r $@ $^

# 以下２行は 不要 、取っても構わない。 # 廃止
#$(sort $(OBJDIRS)):                   # 廃止
#	@$(MD) -p $(subst //,\,$@)         # 廃止
# 不要 は、ここまで。                  # 廃止

pspclean:
	@echo Remove all object files and directories.
	@$(RM) -f -rd $(OBJ)

#maketree:
#	@echo Making object tree...

# maketree $(sort $(OBJDIRS))


maked_directry:
	@echo Making object tree for $(TARGET) ...
	@$(MD) -p $(subst //,\,$(sort $(OBJDIRS)))
	@$(RM) -f PARAM.SFO

copy_bak_pbp: 
	@cp EBOOT.PBP pbp/$(TARGET)
	@ls -l EBOOT.PBP

## re-make the M68000
68:
	@echo Delete M68000 object ...
	@$(RM) -f $(OBJ)/cpu/m68000/*.o
	@$(RM) -f $(OBJ)/cpu/n68000/*.o
	@$(RM) -f $(OBJ)/cpu/na68000/*.o
	@$(RM) -f $(OBJ)/cpu/nb68000/*.o
	@$(RM) -f $(OBJ)/cpu/da68000/*.o

## re-make the src/cpuint.o for adjust IRQ_LINE
int:
	@echo Delete intrrupt object ...
	@$(RM) -f $(OBJ)/cpuint.o

## re-make the src/tiny.o for makes/game_name.mak
tiny:
	@echo Delete the tiny base file object ...
	@$(RM) -f $(OBJ)/tiny.o

## re-make the font. (src/psp/psp_data.o)
font:
	@echo Delete font object ...
	@$(RM) -f $(OBJ)/psp/psp_data.o

## re-make the tilemap. (src/tilemap00.o)
tile:
	@echo Delete mame tilemap system object ...
	@$(RM) -f $(OBJ)/tilemap00.o

## re-make the psp_sound. (src/psp/psp_sound.o)
sound:
	@echo Delete sound interface object ...
	@$(RM) -f $(OBJ)/psp/psp_menu.o
	@$(RM) -f $(OBJ)/psp/psp_sound.o
	@$(RM) -f $(OBJ)/psp/osd_config.o
	@$(RM) -f $(OBJ)/mame.o
	@$(RM) -f $(OBJ)/drivers/*.o
	@$(RM) -f $(OBJ)/psp_sound/*.o

delpbp:
	@echo Delete all pbp tree ...
	@$(RM) -f -rd pbp


ifeq ($(OSD),windows)
#DELTREE = deltree /Y obj     # 廃止
#DELTREE = $(RM) -f -r $(OBJ) # 廃止
DELTREE = $(RM) -f -r obj
endif

ifeq ($(OSD),msdos)
DELTREE = deltree /Y obj
endif

ifeq ($(OSD),psp)
DELTREE = @$(RM) -f -rd obj
endif

rr:
	@echo Remove all temporaly files.
	@$(RM) -f PARAM.SFO
	@$(RM) -f *.elf
	@$(DELTREE)

lse:
	ls -al *.PBP

