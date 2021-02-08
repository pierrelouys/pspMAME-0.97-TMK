# the core object files (without target specific objects;
# those are added in the target.mak files)
COREOBJS += \
	$(OBJ)/mame.o \

## [tilemap]標準TILEMAP、使わないなら１(自動設定)。(基本は１)
ifneq ($(filter NO_TILEMAP,$(PSP_EXTENSION_OPTS)),)
	OPT_DEFS += -DLINK_TILEMAP=0
else
	OPT_DEFS += -DLINK_TILEMAP=1
	COREOBJS += $(OBJ)/tilemap.o 
endif

############repuken2  #dbus16 for bloodbros #psp_use_sound_lacth for tmnt+ drivers
ifeq ($(thunder_cross,$(TARGET)),)
    #OPT_DEFS += -DLINK_ANALOG=1
    #OPT_DEFS += -DLINK_GUNCON=1
    OPT_DEFS += -DLINK_IRQ0=1
	OPT_DEFS += -DLINK_IRQ2=1
	OPT_DEFS += -DLINK_IRQ4=1
	OPT_DEFS += -DLINK_IRQ5=1
	OPT_DEFS += -DLINK_IRQ6=1
    OPT_DEFS += -DLINK_FLIP_SCREEN=1
	OPT_DEFS += -DBUS_16=1
	#OPT_DEFS += -DPSP_USE_SOUND_LATCH=1
	#OPT_DEFS += -DLINK_SHADOW_HIGHLIGHT=1
endif

COREOBJS += \
	$(OBJ)/version.o \
    $(OBJ)/drawgfx.o \
	$(OBJ)/common.o \
	$(OBJ)/usrintrf.o \
	$(OBJ)/ui_text.o \
	$(OBJ)/cpuintrf.o \
	$(OBJ)/cpuexec.o \
	$(OBJ)/cpuint.o \
	$(OBJ)/memory.o \
	$(OBJ)/timer.o \
	$(OBJ)/palette.o \
	$(OBJ)/input.o \
	$(OBJ)/inptport.o \
	$(OBJ)/config.o \
	$(OBJ_VIDEO)/generic.o \
	$(OBJ)/vidhrdw/vector.o \
	$(OBJ)/machine/eeprom.o \
	$(OBJ)/profiler.o \
	$(OBJ)/unzip.o \
	$(OBJ)/audit.o \
	$(OBJ)/fileio.o \
	$(OBJ)/state.o \
	$(OBJ)/datafile.o \
    $(OBJ)/hiscore.o \
	$(OBJ)/png.o \
	$(OBJ)/hash.o \
	$(OBJ)/sha1.o \
	$(OBJ)/chd.o \
	$(OBJ)/md5.o \
	$(OBJ)/sound/wavwrite.o \
	$(OBJ)/harddisk.o

#	$(OBJ)/config.o 
#	$(OBJ)/artwork.o \
	

## [state]ダミー（互換性向上の為）
ifneq ($(filter STATE,$(PSP_EXTENSION_OPTS)),)
	OPT_DEFS += -DLINK_STATE=1
	COREOBJS += $(OBJ)/state.o 
else
	OPT_DEFS += -DLINK_STATE=0
endif

## [common eeprom]標準EEP-ROM
ifneq ($(filter EEPROM,$(PSP_EXTENSION_OPTS)),)
	OPT_DEFS += -DLINK_EEPROM=1
	OPT_DEFS += -DLINK_NVRAM=1
	COREOBJS += $(OBJ)/machine/eeprom.o 
else
	OPT_DEFS += -DLINK_EEPROM=0
endif


## [cheat]ダミー（互換性向上の為）
#ifneq ($(filter CHEAT,$(PSP_EXTENSION_OPTS)),)
#	OPT_DEFS += -DLINK_CHEAT=1
#	COREOBJS += $(OBJ)/cheat.o 
#else
	OPT_DEFS += -DLINK_CHEAT=0
#endif



#	$(OBJ)/state.o 

#$(OBJ)/sndintrf.o 
#	$(OBJ)/ui_text.o 

#$(OBJ)/ui_text.o 
#	$(OBJ)/sound/wavwrite.o
#	$(OBJ)/profiler.o 

#
#	$(OBJ)/png.o 
#
#	$(OBJ)/chd.o $(OBJ)/audit.o 
#
#
#$(OBJ)/datafile.o
# $(OBJ)/hiscore.o
#$(OBJ)/info.o 
# $(OBJ)/sha1.o
# $(OBJ)/md5.o
#
#$(OBJ)/version.o
# $(OBJ)/artwork.o 
#
#	$(OBJ_VIDEO)/vector.o
#	$(OBJ)/harddisk.o $(OBJ)/cdrom.o 

#$(OBJ)/sound/filter.o $(OBJ)/sound/flt_vol.o $(OBJ)/sound/flt_rc.o 


#COREOBJS += $(sort $(DBGOBJS))

TOOLS = 
#romcmp$(EXE) chdman$(EXE)

