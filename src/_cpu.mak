###########################################################################
#
#   cpu.mak
#
#   Rules for building CPU cores
#
#   Copyright (c) 1996-2007, Nicola Salmoria and the MAME Team.
#   Visit http://mamedev.org for licensing and usage restrictions.
#
###########################################################################


################
################  Motorola 8bit class
################


#-------------------------------------------------
# (m6502) Mostek 6502 and its many derivatives
#-------------------------------------------------

ifneq ($(filter M6502,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M6502=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6502.o: m6502.c m6502.h ops02.h t6502.c t65c02.c t65sc02.c t6510.c
else
CPUDEFS += -DHAS_M6502=0
endif

ifneq ($(filter M65C02,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M65C02=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6502.o: m6502.c m6502.h ops02.h t6502.c t65c02.c t65sc02.c t6510.c
else
CPUDEFS += -DHAS_M65C02=0
endif

ifneq ($(filter M65SC02,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M65SC02=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6502.o: m6502.c m6502.h ops02.h t6502.c t65c02.c t65sc02.c t6510.c
else
CPUDEFS += -DHAS_M65SC02=0
endif

ifneq ($(filter M65CE02,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M65CE02=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m65ce02.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m65ce02.o: m65ce02.c m65ce02.h opsce02.h t65ce02.c
else
CPUDEFS += -DHAS_M65CE02=0
endif

ifneq ($(filter M6509,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M6509=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6509.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6509.o: m6509.c m6509.h ops09.h t6509.c
else
CPUDEFS += -DHAS_M6509=0
endif

ifneq ($(filter M6510,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M6510=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6502.o: m6502.c m6502.h ops02.h t6502.c t65c02.c t65sc02.c t6510.c
else
CPUDEFS += -DHAS_M6510=0
endif

ifneq ($(filter M6510T,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M6510=1 -DHAS_M6510T=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6502.o: m6502.c m6502.h ops02.h t6502.c t65c02.c t65sc02.c t6510.c
else
CPUDEFS += -DHAS_M6510T=0
endif

ifneq ($(filter M7501,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M6510=1 -DHAS_M7501=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6502.o: m6502.c m6502.h ops02.h t6502.c t65c02.c t65sc02.c t6510.c
else
CPUDEFS += -DHAS_M7501=0
endif

ifneq ($(filter M8502,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M6510=1 -DHAS_M8502=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6502.o: m6502.c m6502.h ops02.h t6502.c t65c02.c t65sc02.c t6510.c
else
CPUDEFS += -DHAS_M8502=0
endif

ifneq ($(filter N2A03,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_N2A03=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6502.o: m6502.c m6502.h ops02.h t6502.c t65c02.c t65sc02.c t6510.c
else
CPUDEFS += -DHAS_N2A03=0
endif

ifneq ($(filter DECO16,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_DECO16=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
#$(OBJ_CPU)/m6502/m6502.o: m6502.c m6502.h ops02.h t6502.c t65c02.c t65sc02.c t6510.c tdeco16.c
else
CPUDEFS += -DHAS_DECO16=0
endif

ifneq ($(filter M4510,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M4510=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6502/m4510.o
DBGOBJS += $(OBJ_CPU)/m6502/6502dasm.o
else
CPUDEFS += -DHAS_M4510=0
endif

#-------------------------------------------------
# (m6502) Hudsonsoft 6280
#-------------------------------------------------

ifneq ($(filter H6280,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/h6280
CPUDEFS += -DHAS_H6280_ORIG=1
CPUOBJS += $(OBJ_CPU)/h6280/h6280.o
#DBGOBJS += $(OBJ_CPU)/h6280/6280dasm.o
#$(OBJ_CPU)/h6280/h6280.o: h6280.c h6280.h h6280ops.h tblh6280.c
else
CPUDEFS += -DHAS_H6280_ORIG=0
endif
#-------------------------------------------------
# (m6502) G65816
#-------------------------------------------------

ifneq ($(filter G65816,$(CPUS)),)
G6D = cpu/g65816
OBJDIRS += $(OBJ)/$(G6D)
CPUDEFS += -DHAS_G65816=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ)/$(G6D)/g65816.o
CPUOBJS += $(OBJ)/$(G6D)/g65816o0.o
CPUOBJS += $(OBJ)/$(G6D)/g65816o1.o
CPUOBJS += $(OBJ)/$(G6D)/g65816o2.o
CPUOBJS += $(OBJ)/$(G6D)/g65816o3.o
CPUOBJS += $(OBJ)/$(G6D)/g65816o4.o
DBGOBJS += $(OBJ)/$(G6D)/g65816ds.o
#$(OBJ)/$(G6D)/g65816.o: $(G6D)/g65816.c $(G6D)/g65816.h $(G6D)/g65816cm.h $(G6D)/g65816op.h
#$(OBJ)/$(G6D)/g65816o0.o: $(G6D)/g65816o0.c $(G6D)/g65816.h $(G6D)/g65816cm.h $(G6D)/g65816op.h
#$(OBJ)/$(G6D)/g65816o1.o: $(G6D)/g65816o0.c $(G6D)/g65816.h $(G6D)/g65816cm.h $(G6D)/g65816op.h
#$(OBJ)/$(G6D)/g65816o2.o: $(G6D)/g65816o0.c $(G6D)/g65816.h $(G6D)/g65816cm.h $(G6D)/g65816op.h
#$(OBJ)/$(G6D)/g65816o3.o: $(G6D)/g65816o0.c $(G6D)/g65816.h $(G6D)/g65816cm.h $(G6D)/g65816op.h
#$(OBJ)/$(G6D)/g65816o4.o: $(G6D)/g65816o0.c $(G6D)/g65816.h $(G6D)/g65816cm.h $(G6D)/g65816op.h
else
CPUDEFS += -DHAS_G65816=0
endif

#-------------------------------------------------
# (m6502) Mitsubishi M37702 and M37710 (based on 65C816)
#-------------------------------------------------

ifneq ($(filter M37710,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m37710
CPUDEFS += -DHAS_M37710=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/m37710/m37710.o
CPUOBJS += $(OBJ_CPU)/m37710/m37710o0.o
CPUOBJS += $(OBJ_CPU)/m37710/m37710o1.o
CPUOBJS += $(OBJ_CPU)/m37710/m37710o2.o
CPUOBJS += $(OBJ_CPU)/m37710/m37710o3.o
CPUOBJS += $(OBJ_CPU)/m37710/m7700ds.o
#$(OBJ_CPU)/m37710/m37710.o: m37710.c m37710.h m37710o0.c m37710o1.c m37710o2.c m37710o3.c m37710op.h m7700ds.h
#$(OBJ_CPU)/m37710/m37710o0.o: m37710.h m37710o0.c m37710op.h m7700ds.h
#$(OBJ_CPU)/m37710/m37710o1.o: m37710.h m37710o1.c m37710op.h m7700ds.h
#$(OBJ_CPU)/m37710/m37710o2.o: m37710.h m37710o2.c m37710op.h m7700ds.h
#$(OBJ_CPU)/m37710/m37710o3.o: m37710.h m37710o3.c m37710op.h m7700ds.h
#$(OBJ_CPU)/m37710/m7700ds.o: m7700ds.c m7700ds.h
else
CPUDEFS += -DHAS_M37710=0
endif

#-------------------------------------------------
# (m6502) Sony/Nintendo SPC700
#-------------------------------------------------

ifneq ($(filter SPC700,$(CPUS)),)
SPCD = cpu/spc700
OBJDIRS += $(OBJ_CPU)/spc700
CPUDEFS += -DHAS_SPC700=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/spc700/spc700.o
DBGOBJS += $(OBJ_CPU)/spc700/spc700ds.o
#$(OBJ_CPU)/spc700/spc700.o: spc700/spc700.c spc700/spc700.h
else
CPUDEFS += -DHAS_SPC700=0
endif

#-------------------------------------------------
# Motorola 680x (m6800 series)
#-------------------------------------------------

ifneq ($(filter M6800,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6800
CPUDEFS += -DHAS_M6800=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6800/m6800.o
DBGOBJS += $(OBJ_CPU)/m6800/6800dasm.o
#$(OBJ_CPU)/m6800/m6800.o: m6800.c m6800.h 6800ops.c 6800tbl.c
else
CPUDEFS += -DHAS_M6800=0
endif

ifneq ($(filter M6801,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6800
CPUDEFS += -DHAS_M6801=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6800/m6800.o
DBGOBJS += $(OBJ_CPU)/m6800/6800dasm.o
#$(OBJ_CPU)/m6800/m6800.o: m6800.c m6800.h 6800ops.c 6800tbl.c
else
CPUDEFS += -DHAS_M6801=0
endif

ifneq ($(filter M6802,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6800
CPUDEFS += -DHAS_M6802=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6800/m6800.o
DBGOBJS += $(OBJ_CPU)/m6800/6800dasm.o
#$(OBJ_CPU)/m6800/m6800.o: m6800.c m6800.h 6800ops.c 6800tbl.c
else
CPUDEFS += -DHAS_M6802=0
endif

ifneq ($(filter M6803,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6800
CPUDEFS += -DHAS_M6803=1
CPUDEFS += -DREPUKEN2_DEATHRASH=1#repuken2 added
CPUOBJS += $(OBJ_CPU)/m6800/m6800.o
#DBGOBJS += $(OBJ_CPU)/m6800/6800dasm.o
#$(OBJ_CPU)/m6800/m6800.o: m6800.c m6800.h 6800ops.c 6800tbl.c
else
CPUDEFS += -DHAS_M6803=0
endif

ifneq ($(filter M6808_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6800
CPUDEFS += -DHAS_M6808_ORIG=1
CPUOBJS += $(OBJ_CPU)/m6800/m6800.o
#DBGOBJS += $(OBJ_CPU)/m6800/6800dasm.o
#$(OBJ_CPU)/m6800/m6800.o: m6800.c m6800.h 6800ops.c 6800tbl.c
else
CPUDEFS += -DHAS_M6808_ORIG=0
endif

ifneq ($(filter HD63701,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6800
CPUDEFS += -DHAS_HD63701=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6800/m6800.o
DBGOBJS += $(OBJ_CPU)/m6800/6800dasm.o
#$(OBJ_CPU)/m6800/m6800.o: m6800.c m6800.h 6800ops.c 6800tbl.c
else
CPUDEFS += -DHAS_HD63701=0
endif

ifneq ($(filter NSC8105,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6800
CPUDEFS += -DHAS_NSC8105=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6800/m6800.o
DBGOBJS += $(OBJ_CPU)/m6800/6800dasm.o
#$(OBJ_CPU)/m6800/m6800.o: m6800.c m6800.h 6800ops.c 6800tbl.c
else
CPUDEFS += -DHAS_NSC8105=0
endif

#-------------------------------------------------
# Motorola 6805 (m6805 series)
#-------------------------------------------------

ifneq ($(filter M6805,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6805
CPUDEFS += -DHAS_M6805=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6805/m6805.o
DBGOBJS += $(OBJ_CPU)/m6805/6805dasm.o
#$(OBJ_CPU)/m6805/m6805.o: m6805.c m6805.h 6805ops.c
else
CPUDEFS += -DHAS_M6805=0
endif

ifneq ($(filter M68705,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6805
CPUDEFS += -DHAS_M68705=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6805/m6805.o
DBGOBJS += $(OBJ_CPU)/m6805/6805dasm.o
#$(OBJ_CPU)/m6805/m6805.o: m6805.c m6805.h 6805ops.c
else
CPUDEFS += -DHAS_M68705=0
endif

ifneq ($(filter HD63705,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6805
CPUDEFS += -DHAS_HD63705=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6805/m6805.o
DBGOBJS += $(OBJ_CPU)/m6805/6805dasm.o
#$(OBJ_CPU)/m6805/m6805.o: m6805.c m6805.h 6805ops.c
else
CPUDEFS += -DHAS_HD63705=0
endif

#-------------------------------------------------
# (m6809) Motorola 6809 (m6809 series)
#-------------------------------------------------

#########
#CPU_SELECT = TYPE_MAME6809
 CPU_SELECT = TYPE_C6809
#########
ifeq ($(TARGET),circus_charlie)
	#CPU_SELECT = TYPE_MAME6809
	#CPU_SELECT = TYPE_B6809
	 CPU_SELECT = TYPE_C6809
endif
#########
ifeq ($(CPU_SELECT),TYPE_MAME6809)
ifneq ($(filter M6809,$(CPUS)),)
 OBJDIRS += $(OBJ_CPU)/m6809
 CPUDEFS += -DHAS_M6809=1
 OPT_DEFS += -DBUS_8=1
 CPUOBJS += $(OBJ_CPU)/m6809/m6809.o
#DBGOBJS += $(OBJ_CPU)/m6809/6809dasm.o
#$(OBJ_CPU)/m6809/m6809.o: m6809.c m6809.h 6809ops.c 6809tbl.c
else
CPUDEFS += -DHAS_M6809=0
endif
endif
#########
ifeq ($(CPU_SELECT),TYPE_B6809)
ifneq ($(filter M6809,$(CPUS)),)
 OBJDIRS += $(OBJ_CPU)/m6809b
 CPUDEFS += -DHAS_M6809=1
 OPT_DEFS += -DBUS_8=1
 CPUOBJS += $(OBJ_CPU)/m6809b/m6809.o
#DBGOBJS += $(OBJ_CPU)/m6809b/6809dasm.o
#$(OBJ_CPU)/m6809b/m6809.o: m6809.c m6809.h 6809ops.c 6809tbl.c
else
CPUDEFS += -DHAS_M6809=0
endif
endif
#########
ifeq ($(CPU_SELECT),TYPE_C6809)
ifneq ($(filter M6809,$(CPUS)),)
# c6809
 OBJDIRS += $(OBJ)/cpu/m6809
CPUDEFS += -DHAS_M6809=1
 OPT_DEFS += -DBUS_8=1
 CPUOBJS += $(OBJ)/cpu/m6809/m6809.o
#$(OBJ_CPU)/c6809/m6809.o: m6809.c m6809.h c6809op.c c6809op10.c c6809op11.c c6809jmp.c
else
CPUDEFS += -DHAS_M6809=0
endif
endif
#########
ifneq ($(filter M6809E,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6809
CPUDEFS += -DHAS_M6809E=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/m6809/m6809.o
DBGOBJS += $(OBJ_CPU)/m6809/6809dasm.o
#$(OBJ_CPU)/m6809/m6809.o: m6809.c m6809.h 6809ops.c 6809tbl.c
else
CPUDEFS += -DHAS_M6809E=0
endif
#########

#-------------------------------------------------
# (m6809) Hitachi 6309
#-------------------------------------------------

ifneq ($(filter HD6309_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/hd6309
CPUDEFS += -DHAS_HD6309_ORIG=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/hd6309/hd6309.o
#DBGOBJS += $(OBJ_CPU)/hd6309/6309dasm.o
#$(OBJ_CPU)/hd6309/hd6309.o: hd6309.c hd6309.h 6309ops.c 6309tbl.c
else
CPUDEFS += -DHAS_HD6309_ORIG=0
endif

#-------------------------------------------------
# Z80 ORIGINAL
#-------------------------------------------------
ifneq ($(filter KONAMI_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ)/cpu/konami
CPUDEFS += -DHAS_KONAMI_ORIG=1
CPUOBJS += $(OBJ)/cpu/konami/konami.o
DBGOBJS += $(OBJ_CPU)/konami/knmidasm.o
else
CPUDEFS += -DHAS_KONAMI_ORIG=0
endif

ifneq ($(filter Z80_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ)/cpu/z80
CPUDEFS += -DHAS_Z80=1
CPUOBJS += $(OBJ)/cpu/z80/z80.o
else
CPUDEFS += -DHAS_Z80=0
endif

ifneq ($(filter M6809_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ)/cpu/m6809
CPUDEFS += -DHAS_M6809=1
CPUOBJS += $(OBJ)/cpu/m6809/m6809.o
else
CPUDEFS += -DHAS_M6809=0
endif

ifneq ($(filter M68000_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ)/cpu/m68000
CPUDEFS += -DHAS_M68000_ORIG=1
M68000_GENERATED_OBJS = \
$(OBJ)/cpu/m68000/m68kops.o $(OBJ)/cpu/m68000/m68kopac.o \
$(OBJ)/cpu/m68000/m68kopdm.o $(OBJ)/cpu/m68000/m68kopnz.o
CPUOBJS += $(M68000_GENERATED_OBJS) $(OBJ)/cpu/m68000/m68kcpu.o $(OBJ)/cpu/m68000/m68kmame.o
#DBGOBJS += $(OBJ)/cpu/m68000/m68kdasm.o
else
CPUDEFS += -DHAS_M68000_ORIG=0
endif

ifneq ($(filter M68EC020_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ)/cpu/m68000
CPUDEFS += -DHAS_M68EC020_ORIG=1
M68000_GENERATED_OBJS = \
$(OBJ)/cpu/m68000/m68kops.o $(OBJ)/cpu/m68000/m68kopac.o \
$(OBJ)/cpu/m68000/m68kopdm.o $(OBJ)/cpu/m68000/m68kopnz.o
CPUOBJS += $(M68000_GENERATED_OBJS) $(OBJ)/cpu/m68000/m68kcpu.o $(OBJ)/cpu/m68000/m68kmame.o
#DBGOBJS += $(OBJ)/cpu/m68000/m68kdasm.o
else
CPUDEFS += -DHAS_M68EC020_ORIG=0
endif

ifneq ($(filter M68000_NJ,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/nb68000
CPUDEFS += -DHAS_M68000_NJ=1
OPT_DEFS += -DBUS_16=1
CPUOBJS +=	$(OBJ_CPU)/nb68000/m68000.o
#$(OBJ_CPU)/nb68000/m68kmame.o
else
CPUDEFS += -DHAS_M68000_NJ=0
endif





CPU=$(strip $(findstring M6800_ORIG,$(CPUS)))
ifneq ($(CPU),)
OBJDIRS += $(OBJ)/cpu/m6800
CPUDEFS += -DHAS_M6800_ORIG=1
CPUOBJS += $(OBJ)/cpu/m6800/m6800.o
#$(OBJ)/cpu/m6800/m6800.o: m6800.c m6800.h 6800ops.c 6800tbl.c
else
CPUDEFS += -DHAS_M6800_ORIG=0
endif

ifneq ($(filter HD63701_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ)/cpu/m6800
CPUDEFS += -DHAS_HD63701=1
CPUOBJS += $(OBJ)/cpu/m6800/m6800.o
#DBGOBJS += $(OBJ)/cpu/m6800/6800dasm.o
#$(OBJ)/cpu/m6800/m6800.o: m6800.c m6800.h 6800ops.c 6800tbl.c
else
CPUDEFS += -DHAS_HD63701=0
endif

ifneq ($(filter M6502_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m6502
CPUDEFS += -DHAS_M6502_ORIG=1
CPUOBJS += $(OBJ_CPU)/m6502/m6502.o
else
CPUDEFS += -DHAS_M6502_ORIG=0
endif


ifneq ($(filter M6809E_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ)/cpu/m6809
CPUDEFS += -DHAS_M6809E_ORIG=1
CPUOBJS += $(OBJ)/cpu/m6809/m6809.o
#$(OBJ)/cpu/m6809/m6809.o: m6809.c m6809.h 6809ops.c 6809tbl.c
else
CPUDEFS += -DHAS_M6809E_ORIG=0
endif

CPU=$(strip $(findstring TMS34010_ORIG,$(CPUS)))
ifneq ($(CPU),)
OBJDIRS += $(OBJ)/cpu/tms34010
CPUDEFS += -DHAS_TMS34010_ORIG=1
CPUOBJS += $(OBJ)/cpu/tms34010/tms34010.o $(OBJ)/cpu/tms34010/34010fld.o
#$(OBJ)/cpu/tms34010/tms34010.o: tms34010.c tms34010.h 34010ops.c 34010gfx.c 34010tbl.c
else
CPUDEFS += -DHAS_TMS34010_ORIG=0
endif

ifneq ($(filter ADSP2105_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2105_ORIG=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
else
CPUDEFS += -DHAS_ADSP2105_ORIG=0
endif

ifneq ($(filter ADSP2101_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2101_ORIG=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
else
CPUDEFS += -DHAS_ADSP2101_ORIG=0
endif

ifneq ($(filter ADSP2104_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2104_ORIG=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
else
CPUDEFS += -DHAS_ADSP2104_ORIG=0
endif


ifneq ($(filter ADSP2100_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2100_ORIG=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
else
CPUDEFS += -DHAS_ADSP2100_ORIG=0
endif

ifneq ($(filter ADSP2115_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2115_ORIG=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
else
CPUDEFS += -DHAS_ADSP2115_ORIG=0
endif


ifneq ($(filter V30_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ)/cpu/nec
CPUDEFS += -DHAS_V30_ORIG=1
CPUDEFS += -DINCLUDE_V20=1
CPUOBJS += $(OBJ)/cpu/nec/nec.o
else
CPUDEFS += -DHAS_V30_ORIG=0
endif

ifneq ($(filter V33_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ)/cpu/nec
CPUDEFS += -DHAS_V33_ORIG=1
CPUOBJS += $(OBJ)/cpu/nec/nec.o
else
CPUDEFS += -DHAS_V33_ORIG=0
endif


#-------------------------------------------------

#-------------------------------------------------
# (m6809) Konami custom CPU (6809-based)
#-------------------------------------------------
#########
#CPU_SELECT = TYPE_KONAMI_A1
#CPU_SELECT = TYPE_KONAMI_B1
#########
#ifeq ($(TARGET),thunder_cross)
# CPU_SELECT = TYPE_KONAMI_A1
#endif
#########
#ifeq ($(CPU_SELECT),TYPE_KONAMI_A1)
ifneq ($(filter KONAMI,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/konami
CPUDEFS += -DHAS_KONAMI=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/konami/konami.o
DBGOBJS += $(OBJ_CPU)/konami/knmidasm.o
#$(OBJ_CPU)/konami/konami.o: konami.c konami.h konamops.c konamtbl.c
else
CPUDEFS += -DHAS_KONAMI=0
endif
#endif
#########


################
################  Intel 8bit class
################


#-------------------------------------------------
# (i8080) Intel 8080/8085A
#-------------------------------------------------

ifneq ($(filter 8080,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8085
CPUDEFS += -DHAS_8080=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i8085/i8085.o
DBGOBJS += $(OBJ_CPU)/i8085/8085dasm.o
#$(OBJ_CPU)/i8085/i8085.o: i8085.c i8085.h i8085cpu.h i8085daa.h
else
CPUDEFS += -DHAS_8080=0
endif

ifneq ($(filter 8085A,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8085
CPUDEFS += -DHAS_8085A=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i8085/i8085.o
DBGOBJS += $(OBJ_CPU)/i8085/8085dasm.o
#$(OBJ_CPU)/i8085/i8085.o: i8085.c i8085.h i8085cpu.h i8085daa.h
else
CPUDEFS += -DHAS_8085A=0
endif

#-------------------------------------------------
# Intel 8039 and derivatives
#-------------------------------------------------

ifneq ($(filter I8035,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8039
CPUDEFS += -DHAS_I8035=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i8039/i8039.o
DBGOBJS += $(OBJ_CPU)/i8039/8039dasm.o
#$(OBJ_CPU)/i8039/i8039.o: i8039.c i8039.h
else
CPUDEFS += -DHAS_I8035=0
endif

ifneq ($(filter I8039,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8039
CPUDEFS += -DHAS_I8039=1
CPUOBJS += $(OBJ_CPU)/i8039/i8039.o
#DBGOBJS += $(OBJ_CPU)/i8039/8039dasm.o
#$(OBJ_CPU)/i8039/i8039.o: i8039.c i8039.h
else
CPUDEFS += -DHAS_I8039=0
endif

ifneq ($(filter I8048,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8039
CPUDEFS += -DHAS_I8048=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i8039/i8039.o
DBGOBJS += $(OBJ_CPU)/i8039/8039dasm.o
#$(OBJ_CPU)/i8039/i8039.o: i8039.c i8039.h
else
CPUDEFS += -DHAS_I8048=0
endif

ifneq ($(filter N7751,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8039
CPUDEFS += -DHAS_N7751=1
CPUOBJS += $(OBJ_CPU)/i8039/i8039.o
#DBGOBJS += $(OBJ_CPU)/i8039/8039dasm.o
#$(OBJ_CPU)/i8039/i8039.o: i8039.c i8039.h
else
CPUDEFS += -DHAS_N7751=0
endif

#-------------------------------------------------
# Intel 8x41
#-------------------------------------------------

ifneq ($(filter I8X41,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8x41
CPUDEFS += -DHAS_I8X41=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i8x41/i8x41.o
DBGOBJS += $(OBJ_CPU)/i8x41/8x41dasm.o
#$(OBJ_CPU)/i8x41/i8x41.o: i8x41.c i8x41.h
else
CPUDEFS += -DHAS_I8X41=0
endif

#-------------------------------------------------
# Intel 8051 and derivatives
#-------------------------------------------------

ifneq ($(filter I8051,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8051
CPUDEFS += -DHAS_I8051=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i8051/i8051.o
DBGOBJS += $(OBJ_CPU)/i8051/8051dasm.o
#$(OBJ_CPU)/i8051/i8051.o: i8051.c i8051.h i8051ops.c
else
CPUDEFS += -DHAS_I8051=0
endif

ifneq ($(filter I8052,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8051
CPUDEFS += -DHAS_I8052=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i8051/i8051.o
DBGOBJS += $(OBJ_CPU)/i8051/8051dasm.o
#$(OBJ_CPU)/i8051/i8051.o: i8051.c i8051.h i8051ops.c
else
CPUDEFS += -DHAS_I8052=0
endif

ifneq ($(filter I8751_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8051
CPUDEFS += -DHAS_I8751_ORIG=1
CPUOBJS += $(OBJ_CPU)/i8051/i8051.o
#$(OBJ_CPU)/i8051/i8051.o: i8051.c i8051.h i8051ops.c
else
CPUDEFS += -DHAS_I8751_ORIG=0
endif

ifneq ($(filter I8752_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i8051
CPUDEFS += -DHAS_I8752_ORIG=1
CPUOBJS += $(OBJ_CPU)/i8051/i8051.o
else
CPUDEFS += -DHAS_I8752_ORIG=0
endif




#-------------------------------------------------
# (i8080) Intel 80x86 series
#-------------------------------------------------

ifneq ($(filter I86,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i86
CPUDEFS += -DHAS_I86=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i86/i86.o $(OBJ_CPU)/i86/instr86.o
DBGOBJS += $(OBJ_CPU)/i386/i386dasm.o
#$(OBJ)/cpu/i86/i86.o: i86.c instr86.c i86.h i86intf.h ea.h host.h modrm.h
else
CPUDEFS += -DHAS_I86=0
endif

ifneq ($(filter I88,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i86
CPUDEFS += -DHAS_I88=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i86/i86.o
DBGOBJS += $(OBJ_CPU)/i386/i386dasm.o
#$(OBJ_CPU)/i86/i86.o: i86.c instr86.c i86.h i86intf.h ea.h host.h modrm.h
else
CPUDEFS += -DHAS_I88=0
endif

ifneq ($(filter I186,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i86
CPUDEFS += -DHAS_I186=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i86/i86.o
DBGOBJS += $(OBJ_CPU)/i386/i386dasm.o
#$(OBJ_CPU)/i86/i86.o: i86.c instr186.c i86.h i186intf.h ea.h host.h modrm.h
else
CPUDEFS += -DHAS_I186=0
endif

ifneq ($(filter I188,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i86
CPUDEFS += -DHAS_I188=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i86/i86.o
DBGOBJS += $(OBJ_CPU)/i386/i386dasm.o
#$(OBJ_CPU)/i86/i86.o: i86.c instr186.c i86.h i186intf.h ea.h host.h modrm.h
else
CPUDEFS += -DHAS_I188=0
endif

ifneq ($(filter I286,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i86
CPUDEFS += -DHAS_I286=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i86/i286.o
DBGOBJS += $(OBJ_CPU)/i386/i386dasm.o
#$(OBJ_CPU)/i86/i86.o: i86.c instr286.c i86.h i286intf.h ea.h host.h modrm.h
else
CPUDEFS += -DHAS_I286=0
endif

ifneq ($(filter I386,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i386
CPUDEFS += -DHAS_I386=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/i386/i386.o
DBGOBJS += $(OBJ_CPU)/i386/i386dasm.o
#$(OBJ_CPU)/i386/i386.o: i386.c i386.h i386intf.h i386op16.c i386op16.h i386op32.c i386op32.h i386ops.c
else
CPUDEFS += -DHAS_I386=0
endif

#-------------------------------------------------
# (i8080) NEC V-series, Intel i80x86 compatible
#-------------------------------------------------

# V20 V25 は 8bit bus.
# V30 V33 は 16bit bus. だが mame097 では 8bit bus で emulate.
# 一方 V60 は 16bit bus. V70 は 32bit bus. (V20/25/30/33 (i80x86系) と V60/V70 (RISC系) は 設計が全然違う)

ifneq ($(filter V20,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/nec
CPUDEFS += -DHAS_V20=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/nec/nec.o
DBGOBJS += $(OBJ_CPU)/i386/i386dasm.o
#$(OBJ_CPU)/nec/nec.o: nec.c nec.h necintrf.h necea.h nechost.h necinstr.h necmodrm.h
else
CPUDEFS += -DHAS_V20=0
endif

#########
 CPU_SELECT = TYPE_V30_A1
#CPU_SELECT = TYPE_V30_B1
#########
ifeq ($(CPU_SELECT),TYPE_V30_A1)
ifneq ($(filter NEC30,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/nec30
CPUDEFS += -DHAS_NEC30=1
OPT_DEFS += -DBUS_8=1
#OPT_DEFS += -DBUS_16=1#mame119
CPUOBJS += $(OBJ_CPU)/nec30/nec.o
#$(OBJ_CPU)/nec30/nec.o: nec.c nec.h necintrf.h necea.h nechost.h necinstr.h necmodrm.h
else
CPUDEFS += -DHAS_NEC30=0
endif
endif
#########
ifeq ($(CPU_SELECT),TYPE_V30_B1)
ifneq ($(filter NEC30,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/nec30b
CPUDEFS += -DHAS_NEC30=1
OPT_DEFS += -DBUS_8=1
#OPT_DEFS += -DBUS_16=1#mame119
CPUOBJS += $(OBJ_CPU)/nec30b/nec.o
#$(OBJ_CPU)/nec30b/nec.o: nec.c nec.h necintrf.h necea.h nechost.h necinstr.h necmodrm.h
else
CPUDEFS += -DHAS_NEC30=0
endif
endif
#########


ifneq ($(filter V30,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/nec
CPUDEFS += -DHAS_V30=1
OPT_DEFS += -DBUS_8=1
#OPT_DEFS += -DBUS_16=1#mame119
CPUOBJS += $(OBJ_CPU)/nec/nec.o
DBGOBJS += $(OBJ_CPU)/i386/i386dasm.o
#$(OBJ_CPU)/nec/nec.o: nec.c nec.h necintrf.h necea.h nechost.h necinstr.h necmodrm.h
else
CPUDEFS += -DHAS_V30=0
endif

ifneq ($(filter V33,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/nec
CPUDEFS += -DHAS_V33=1
OPT_DEFS += -DBUS_8=1
#OPT_DEFS += -DBUS_16=1#mame119
CPUOBJS += $(OBJ_CPU)/nec/nec.o
DBGOBJS += $(OBJ_CPU)/i386/i386dasm.o
#$(OBJ_CPU)/nec/nec.o: nec.c nec.h necintrf.h necea.h nechost.h necinstr.h necmodrm.h
else
CPUDEFS += -DHAS_V33=0
endif


#-------------------------------------------------
# (i8080) Zilog Z80
#-------------------------------------------------

#CPU_SELECT = TYPE_Z80R097A
#CPU_SELECT = TYPE_Z80R097B
#CPU_SELECT = TYPE_Z80R118B
 CPU_SELECT = TYPE_Z80S2
#CPU_SELECT = TYPE_Z80FZ1
#CPU_SELECT = TYPE_BZ80

####################
ifeq ($(TARGET),fantasy_zone)
#CPU_SELECT = TYPE_Z80FZ1
 CPU_SELECT = TYPE_BZ80
endif
ifeq ($(TARGET),raizing)
 #CPU_SELECT = TYPE_Z80FZ1
endif
ifeq ($(TARGET),turbo_outrun)
 CPU_SELECT = TYPE_BZ80
endif
ifeq ($(TARGET),space_harrier)
 CPU_SELECT = TYPE_BZ80
#CPU_SELECT = TYPE_Z80FZ1
endif
ifeq ($(TARGET),cotton)
 CPU_SELECT = TYPE_BZ80
#CPU_SELECT = TYPE_Z80FZ1
endif
#ifeq ($(TARGET),pacman)
#CPU_SELECT = TYPE_BZ80
#CPU_SELECT = TYPE_Z80FZ1
#endif
######## taito 系 ########
#ifeq ($(TARGET),taitoAX)
# CPU_SELECT = TYPE_BZ80  # (レインボーアイランド等)テンポが遅すぎる。
# CPU_SELECT = TYPE_Z80R118B
#endif
#ifeq ($(TARGET),taitoB)
# CPU_SELECT = TYPE_BZ80  ## (ガンフロンティア等)テンポが遅すぎる。
# CPU_SELECT = TYPE_Z80R118B
#endif
ifeq ($(TARGET),darius)
 CPU_SELECT = TYPE_BZ80
# CPU_SELECT = TYPE_Z80R118B
endif
ifeq ($(TARGET),ninjya_warriors)
# TYPE_Z80S2  [SF#02(b)](933kb) fskip7_22050_sound_test_0xBB_PAUSE_ON 62-64%(61-65%)
# TYPE_BZ80   [BZ a #00](947kb) fskip7_22050_sound_test_0xBB_PAUSE_ON 64-65%(63-67%)
# TYPE_Z80FZ1 [FZ#02(e)](944kb) fskip7_22050_sound_test_0xBB_PAUSE_ON 62-64%(61-65%)
# TYPE_CPU_SELECT = BZ80 # 音出ない事がある。
 # TYPE_REFZ80 なら ok!
#CPU_SELECT = TYPE_Z80R097A
 CPU_SELECT = TYPE_Z80R118B
#CPU_SELECT = TYPE_Z80FZ1
endif
####################
ifeq ($(TARGET),thunder_cross)
# 音がオカシイので換装実験。
 CPU_SELECT = TYPE_Z80S2
#CPU_SELECT = TYPE_BZ80
endif
####################
#ifeq ($(TARGET),air_buster)
#CPU_SELECT = TYPE_Z80S2
#endif
####################
#ifeq ($(TARGET),starforce)
#ifeq ($(TARGET),terracre)
#ifeq ($(TARGET),air_buster)

####################
ifeq ($(TARGET),ninjya_kun2)
# (ninjya_kun2の場合のみ何故か)一番動作が安定し速い.(Z80FZ2より速い)
 CPU_SELECT = TYPE_Z80S2
endif
ifeq ($(TARGET),gun_smoke)
 CPU_SELECT = TYPE_Z80S2
endif
ifeq ($(TARGET),crazy_climber)
 CPU_SELECT = TYPE_Z80S2
endif
#ifeq ($(TARGET),ARIENAI)
# 特別おまけ版のみ
#	ifeq ($(TARGET),starforce)
#	CPU_SELECT = TYPE_Z80S2
#	endif
#endif
####################
ifeq ($(CPU_SELECT),TYPE_Z80FZ1)
#air_busterはこれでは死ぬ。
#raizingはこのz80でイケルっぽい。（確認は完全でない）
#fantasy_zoneはこのz80
#starforceはこのz80でイケル。
ifneq ($(filter Z80,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/z80fz2e
CPUDEFS += -DHAS_Z80=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/z80fz2e/z80.o
DBGOBJS += $(OBJ_CPU)/z80fz2e/z80dasm.o
#$(OBJ_CPU)/z80fz2e/z80.o: z80.c z80.h
else
CPUDEFS += -DHAS_Z80=0
endif
endif
####################
ifeq ($(CPU_SELECT),TYPE_Z80S2)
#air_busterこのz80
#starforceはこのz80
ifneq ($(filter Z80,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/z80s2c
CPUDEFS += -DHAS_Z80=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/z80s2c/z80.o
DBGOBJS += $(OBJ_CPU)/z80s2c/z80dasm.o
#$(OBJ_CPU)/z80s2c/z80.o: z80.c z80.h
else
CPUDEFS += -DHAS_Z80=0
endif
endif
####################
ifeq ($(CPU_SELECT),TYPE_BZ80)
ifneq ($(filter Z80,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/bz80
CPUDEFS += -DHAS_Z80=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/bz80/z80.o
DBGOBJS += $(OBJ_CPU)/bz80/z80dasm.o
#$(OBJ_CPU)/bz80/z80.o: z80.c z80.h
else
CPUDEFS += -DHAS_Z80=0
endif
endif
####################
ifeq ($(CPU_SELECT),TYPE_Z80R097A)
ifneq ($(filter Z80,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/z80R097A
CPUDEFS += -DHAS_Z80=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/z80R097A/z80.o
DBGOBJS += $(OBJ_CPU)/z80R097A/z80dasm.o
#$(OBJ_CPU)/z80R097A/z80.o: z80.c z80.h
else
CPUDEFS += -DHAS_Z80=0
endif
endif
####################
ifeq ($(CPU_SELECT),TYPE_Z80R097B)
ifneq ($(filter Z80,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/z80R097B
CPUDEFS += -DHAS_Z80=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/z80R097B/z80.o
DBGOBJS += $(OBJ_CPU)/z80R097B/z80dasm.o
#$(OBJ_CPU)/z80R097B/z80.o: z80.c z80.h
else
CPUDEFS += -DHAS_Z80=0
endif
endif
####################
ifeq ($(CPU_SELECT),TYPE_Z80R118B)
ifneq ($(filter Z80,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/z80R118B2
CPUDEFS += -DHAS_Z80=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/z80R118B2/z80.o
DBGOBJS += $(OBJ_CPU)/z80R118B2/z80dasm.o
#$(OBJ_CPU)/z80R118B2/z80.o: z80.c z80.h
else
CPUDEFS += -DHAS_Z80=0
endif
endif
####################

#-------------------------------------------------
# (i8080) [NO USED MAME] Game Boy Z-80
#-------------------------------------------------

#-------------------------------------------------
# Zilog Z180
#-------------------------------------------------

ifneq ($(filter Z180,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/z180
CPUDEFS += -DHAS_Z180=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/z180/z180.o
DBGOBJS += $(OBJ_CPU)/z180/z180dasm.o
#$(OBJ_CPU)/z180/z180.o: z180.c z180.h z180daa.h z180op.c z180ops.h z180tbl.h z180cb.c z180dd.c z180ed.c z180fd.c z180xy.c
else
CPUDEFS += -DHAS_Z180=0
endif

#-------------------------------------------------
# Signetics 2650
#-------------------------------------------------

ifneq ($(filter S2650,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/s2650
CPUDEFS += -DHAS_S2650=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/s2650/s2650.o
DBGOBJS += $(OBJ_CPU)/s2650/2650dasm.o
#$(OBJ_CPU)/s2650/s2650.o: s2650.c s2650.h s2650cpu.h
else
CPUDEFS += -DHAS_S2650=0
endif


################
################  PIC 16bit class
################

#-------------------------------------------------
# Microchip PIC16C5x
#-------------------------------------------------

ifneq ($(filter PIC16C54,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/pic16c5x
CPUDEFS += -DHAS_PIC16C54=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/pic16c5x/pic16c5x.o
DBGOBJS += $(OBJ_CPU)/pic16c5x/16c5xdsm.o
#$(OBJ_CPU)/pic16c5x/pic16c5x.o: pic16c5x.c pic16c5x.h
else
CPUDEFS += -DHAS_PIC16C54=0
endif

ifneq ($(filter PIC16C55,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/pic16c5x
CPUDEFS += -DHAS_PIC16C55=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/pic16c5x/pic16c5x.o
DBGOBJS += $(OBJ_CPU)/pic16c5x/16c5xdsm.o
#$(OBJ_CPU)/pic16c5x/pic16c5x.o: pic16c5x.c pic16c5x.h
else
CPUDEFS += -DHAS_PIC16C55=0
endif

ifneq ($(filter PIC16C56,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/pic16c5x
CPUDEFS += -DHAS_PIC16C56=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/pic16c5x/pic16c5x.o
DBGOBJS += $(OBJ_CPU)/pic16c5x/16c5xdsm.o
#$(OBJ_CPU)/pic16c5x/pic16c5x.o: pic16c5x.c pic16c5x.h
else
CPUDEFS += -DHAS_PIC16C56=0
endif

ifneq ($(filter PIC16C57,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/pic16c5x
CPUDEFS += -DHAS_PIC16C57=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/pic16c5x/pic16c5x.o
DBGOBJS += $(OBJ_CPU)/pic16c5x/16c5xdsm.o
#$(OBJ_CPU)/pic16c5x/pic16c5x.o: pic16c5x.c pic16c5x.h
else
CPUDEFS += -DHAS_PIC16C57=0
endif

ifneq ($(filter PIC16C58,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/pic16c5x
CPUDEFS += -DHAS_PIC16C58=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/pic16c5x/pic16c5x.o
DBGOBJS += $(OBJ_CPU)/pic16c5x/16c5xdsm.o
#$(OBJ_CPU)/pic16c5x/pic16c5x.o: pic16c5x.c pic16c5x.h
else
CPUDEFS += -DHAS_PIC16C58=0
endif

################
################  m68000 16bit CISC class
################

#-------------------------------------------------
# [polepos.c only] Zilog Z8000
#-------------------------------------------------

ifneq ($(filter Z8000,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/z8000
CPUDEFS += -DHAS_Z8000=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/z8000/z8000.o
DBGOBJS += $(OBJ_CPU)/z8000/8000dasm.o
#$(OBJ_CPU)/z8000/z8000.o: z8000.c z8000.h z8000cpu.h z8000dab.h z8000ops.c z8000tbl.c
else
CPUDEFS += -DHAS_Z8000=0
endif

#-------------------------------------------------
# Motorola 68000 series
#-------------------------------------------------

###################

#CPU_SELECT = TYPE_NJ68K
#CPU_SELECT = TYPE_NA68K
 CPU_SELECT = TYPE_NB68K
#CPU_SELECT = TYPE_ND68K

###################

# ダライアスは NA68000 NB68000 では、起動出来ない。
#ifeq ($(TARGET),darius)
# CPU_SELECT = TYPE_ND68K
#endif

# んと、現状(2007-08-24)これくらいの差．
# link NB68000(EBOOT.PBP 977,583bytes(955kb)) fskip07@22050 72% at outrun game start line.
# link ND68000(EBOOT.PBP 957,464bytes(936kb)) fskip07@22050 68% at outrun game start line.
#	コンパクト	大きい   NB68000  <  ND68000 小さい
#	実行速度	速い     NB68000  >  ND68000 遅い
#	ダライアス	起動不可 NB68000 <<< ND68000 可能
#ifeq ($(TARGET),turbo_outrun)
# CPU_SELECT = TYPE_ND68K
#endif

###################
ifeq ($(CPU_SELECT),TYPE_NJ68K)
ifneq ($(filter N68000,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/nj68000
CPUDEFS += -DHAS_N68000=1
OPT_DEFS += -DBUS_16=1
CPUOBJS +=	$(OBJ_CPU)/nj68000/m68000.o
# $(OBJ_CPU)/nj68000/c68k.o 
else
CPUDEFS += -DHAS_N68000=0
endif
endif
###################

###################
ifeq ($(CPU_SELECT),TYPE_NA68K)
ifneq ($(filter N68000,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/na68000
CPUDEFS += -DHAS_N68000=1
OPT_DEFS += -DBUS_16=1
CPUOBJS +=	$(OBJ_CPU)/na68000/m68kcpu.o 
#$(OBJ_CPU)/na68000/m68kmame.o
else
CPUDEFS += -DHAS_N68000=0
endif
endif
###################


###################
ifeq ($(CPU_SELECT),TYPE_NB68K)
ifneq ($(filter N68000,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/nb68000
CPUDEFS += -DHAS_N68000=1
OPT_DEFS += -DBUS_16=1
CPUOBJS +=	$(OBJ_CPU)/nb68000/m68000.o 
#$(OBJ_CPU)/nb68000/m68kmame.o
else
CPUDEFS += -DHAS_N68000=0
endif
endif
###################

###################
ifeq ($(CPU_SELECT),TYPE_ND68K)
ifneq ($(filter N68000,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/nd68000
CPUDEFS += -DHAS_N68000=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/nd68000/m68kcpu.o 
#DBGOBJS += $(OBJ_CPU)/nd68000/m68kdasm.o
else
CPUDEFS += -DHAS_N68000=0
endif
endif
###################



###################
ifneq ($(filter M68000,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m68000
CPUDEFS += -DHAS_M68000=1
OPT_DEFS += -DBUS_16=1
M68000_GENERATED_OBJS = \
	$(OBJ_CPU)/m68000/m68kops.o $(OBJ_CPU)/m68000/m68kopac.o \
	$(OBJ_CPU)/m68000/m68kopdm.o $(OBJ_CPU)/m68000/m68kopnz.o
CPUOBJS += $(OBJ_CPU)/m68000/m68kcpu.o $(OBJ_CPU)/m68000/m68kmame.o
DBGOBJS += $(OBJ_CPU)/m68000/m68kdasm.o
else
CPUDEFS += -DHAS_M68000=0
endif

ifneq ($(filter M68008,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m68000
CPUDEFS += -DHAS_M68008=1
OPT_DEFS += -DBUS_8=1
M68000_GENERATED_OBJS = \
	$(OBJ_CPU)/m68000/m68kops.o $(OBJ_CPU)/m68000/m68kopac.o \
	$(OBJ_CPU)/m68000/m68kopdm.o $(OBJ_CPU)/m68000/m68kopnz.o
CPUOBJS += $(OBJ_CPU)/m68000/m68kcpu.o $(OBJ_CPU)/m68000/m68kmame.o
DBGOBJS += $(OBJ_CPU)/m68000/m68kdasm.o
else
CPUDEFS += -DHAS_M68008=0
endif

ifneq ($(filter M68010_ORIG,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m68000
CPUDEFS += -DHAS_M68010_ORIG=1
OPT_DEFS += -DBUS_16=1
M68000_GENERATED_OBJS = \
	$(OBJ_CPU)/m68000/m68kops.o $(OBJ_CPU)/m68000/m68kopac.o \
	$(OBJ_CPU)/m68000/m68kopdm.o $(OBJ_CPU)/m68000/m68kopnz.o
CPUOBJS += $(OBJ_CPU)/m68000/m68kcpu.o $(OBJ_CPU)/m68000/m68kmame.o
DBGOBJS += $(OBJ_CPU)/m68000/m68kdasm.o
else
CPUDEFS += -DHAS_M68010_ORIG=0
endif

ifneq ($(filter M68EC020,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m68000
CPUDEFS += -DHAS_M68EC020=1
OPT_DEFS += -DBUS_32=1
M68000_GENERATED_OBJS = \
	$(OBJ_CPU)/m68000/m68kops.o $(OBJ_CPU)/m68000/m68kopac.o \
	$(OBJ_CPU)/m68000/m68kopdm.o $(OBJ_CPU)/m68000/m68kopnz.o
CPUOBJS += $(OBJ_CPU)/m68000/m68kcpu.o $(OBJ_CPU)/m68000/m68kmame.o
DBGOBJS += $(OBJ_CPU)/m68000/m68kdasm.o
else
CPUDEFS += -DHAS_M68EC020=0
endif

ifneq ($(filter M68020,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/m68000
CPUDEFS += -DHAS_M68020=1
OPT_DEFS += -DBUS_32=1
M68000_GENERATED_OBJS = \
	$(OBJ_CPU)/m68000/m68kops.o $(OBJ_CPU)/m68000/m68kopac.o \
	$(OBJ_CPU)/m68000/m68kopdm.o $(OBJ_CPU)/m68000/m68kopnz.o
CPUOBJS +=	$(OBJ_CPU)/m68000/m68kcpu.o $(OBJ_CPU)/m68000/m68kmame.o
DBGOBJS += $(OBJ_CPU)/m68000/m68kdasm.o
else
CPUDEFS += -DHAS_M68020=0
endif


#-------------------------------------------------
# NEC V60/V70
#-------------------------------------------------

ifneq ($(filter V60,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/v60
CPUDEFS += -DHAS_V60=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/v60/v60.o
DBGOBJS += $(OBJ_CPU)/v60/v60d.o
#$(OBJ_CPU)/v60/v60.o: am.c am1.c am2.c am3.c op12.c op2.c op3.c op4.c op5.c op6.c op7a.c optable.c v60.c v60.h v60d.c
else
CPUDEFS += -DHAS_V60=0
endif

ifneq ($(filter V70,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/v60
CPUDEFS += -DHAS_V70=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/v60/v60.o
DBGOBJS += $(OBJ_CPU)/v60/v60d.o
#$(OBJ_CPU)/v60/v60.o: am.c am1.c am2.c am3.c op12.c op2.c op3.c op4.c op5.c op6.c op7a.c optable.c v60.c v60.h v60d.c
else
CPUDEFS += -DHAS_V70=0
endif

################
################  32 bit RISC
################

#-------------------------------------------------
# Hitachi SH2
#-------------------------------------------------

ifneq ($(filter SH2,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/sh2
CPUDEFS += -DHAS_SH2=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/sh2/sh2.o
DBGOBJS += $(OBJ_CPU)/sh2/sh2dasm.o
#$(OBJ_CPU)/sh2/sh2.o: sh2.c sh2.h
else
CPUDEFS += -DHAS_SH2=0
endif

#-------------------------------------------------
# (SH2 base) Saturn
#-------------------------------------------------

#-------------------------------------------------
# Hitachi SH4
#-------------------------------------------------


#-------------------------------------------------
# MIPS R3000 (MIPS I/II) series
#-------------------------------------------------

ifneq ($(filter R3000,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/r3000
CPUDEFS += -DHAS_R3000=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/mips/r3000.o
DBGOBJS += $(OBJ_CPU)/mips/r3kdasm.o
#$(OBJ_CPU)/mips/r3000.o: r3000.c r3000.h
else
CPUDEFS += -DHAS_R3000=0
endif

#-------------------------------------------------
# MIPS R3000 PS / Sony PlayStation CPU (R3000-based + GTE)
#-------------------------------------------------

ifneq ($(filter PSXCPU,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/mips
CPUDEFS += -DHAS_PSXCPU=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/mips/psx.o
DBGOBJS += $(OBJ_CPU)/mips/mipsdasm.o
#$(OBJ_CPU)/mips/psx.o: psx.c psx.h
else
CPUDEFS += -DHAS_PSXCPU=0
endif

#-------------------------------------------------
# MIPS R3000 RSP / Nintendo/SGI RSP (R3000-based + vector processing)
#-------------------------------------------------

ifneq ($(filter RSP,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/mips
CPUDEFS += -DHAS_RSP=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/mips/rsp.o
DBGOBJS += $(OBJ_CPU)/mips/rsp_dasm.o
#$(OBJ_CPU)/mips/rsp.o: rsp.c rsp.h
else
CPUDEFS += -DHAS_RSP=0
endif

#-------------------------------------------------
# MIPS R4000 (MIPS III/IV) series
#-------------------------------------------------

ifneq ($(filter R4600,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/mips
CPUDEFS += -DHAS_R4600=1
OPT_DEFS += -DBUS_32=1
ifdef X86_MIPS3_DRC
CPUOBJS += $(OBJ_CPU)/mips/mips3drc.o
#$(OBJ_CPU)/mips/mips3drc.o: mips3drc.c mdrcold.c mips3.h
else
CPUOBJS += $(OBJ_CPU)/mips/mips3.o
#$(OBJ_CPU)/mips/mips3.o: mips3.c mips3.h
endif
DBGOBJS += $(OBJ_CPU)/mips/mips3dsm.o
else
CPUDEFS += -DHAS_R4600=0
endif

ifneq ($(filter R4700,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/mips
CPUDEFS += -DHAS_R4700=1
OPT_DEFS += -DBUS_32=1
ifdef X86_MIPS3_DRC
CPUOBJS += $(OBJ_CPU)/mips/mips3drc.o
#$(OBJ_CPU)/mips/mips3drc.o: mips3drc.c mips3.h
else
CPUOBJS += $(OBJ_CPU)/mips/mips3.o
#$(OBJ_CPU)/mips/mips3.o: mips3.c mips3.h
endif
DBGOBJS += $(OBJ_CPU)/mips/mips3dsm.o
else
CPUDEFS += -DHAS_R4700=0
endif

ifneq ($(filter R5000,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/mips
CPUDEFS += -DHAS_R5000=1
OPT_DEFS += -DBUS_32=1
ifdef X86_MIPS3_DRC
CPUOBJS += $(OBJ_CPU)/mips/mips3drc.o
#$(OBJ_CPU)/mips/mips3drc.o: mips3drc.c mips3.h
else
CPUOBJS += $(OBJ_CPU)/mips/mips3.o
#$(OBJ_CPU)/mips/mips3.o: mips3.c mips3.h
endif
DBGOBJS += $(OBJ_CPU)/mips/mips3dsm.o
else
CPUDEFS += -DHAS_R5000=0
endif

ifneq ($(filter QED5271,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/mips
CPUDEFS += -DHAS_QED5271=1
OPT_DEFS += -DBUS_32=1
ifdef X86_MIPS3_DRC
CPUOBJS += $(OBJ_CPU)/mips/mips3drc.o
#$(OBJ_CPU)/mips/mips3drc.o: mips3drc.c mips3.h
else
CPUOBJS += $(OBJ_CPU)/mips/mips3.o
#$(OBJ_CPU)/mips/mips3.o: mips3.c mips3.h
endif
DBGOBJS += $(OBJ_CPU)/mips/mips3dsm.o
else
CPUDEFS += -DHAS_QED5271=0
endif

ifneq ($(filter RM7000,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/mips
CPUDEFS += -DHAS_RM7000=1
OPT_DEFS += -DBUS_32=1
ifdef X86_MIPS3_DRC
CPUOBJS += $(OBJ_CPU)/mips/mips3drc.o
#$(OBJ_CPU)/mips/mips3drc.o: mips3drc.c mips3.h
else
CPUOBJS += $(OBJ_CPU)/mips/mips3.o
#$(OBJ_CPU)/mips/mips3.o: mips3.c mips3.h
endif
DBGOBJS += $(OBJ_CPU)/mips/mips3dsm.o
else
CPUDEFS += -DHAS_RM7000=0
endif

#-------------------------------------------------
# Motorola PowerPC series
#-------------------------------------------------

ifneq ($(filter PPC403,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/powerpc
CPUDEFS += -DHAS_PPC403=1
OPT_DEFS += -DBUS_32=1
ifdef X86_PPC_DRC
CPUOBJS += $(OBJ_CPU)/powerpc/ppcdrc.o
#$(OBJ_CPU)/powerpc/ppcdrc.o: ppcdrc.c ppc.h drc_ops.c drc_ops.h ppc_ops.c ppc403.c
else
CPUOBJS += $(OBJ_CPU)/powerpc/ppc.o
#$(OBJ_CPU)/powerpc/ppc.o: ppc.c ppc.h ppc_ops.c ppc_ops.h ppc403.c
endif
DBGOBJS += $(OBJ_CPU)/powerpc/ppc_dasm.o
else
CPUDEFS += -DHAS_PPC403=0
endif

ifneq ($(filter PPC602,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/powerpc
CPUDEFS += -DHAS_PPC602=1
OPT_DEFS += -DBUS_64=1
ifdef X86_PPC_DRC
CPUOBJS += $(OBJ_CPU)/powerpc/ppcdrc.o
#$(OBJ_CPU)/powerpc/ppcdrc.o: ppcdrc.c ppc.h drc_ops.c drc_ops.h ppc_ops.c ppc602.c
else
CPUOBJS += $(OBJ_CPU)/powerpc/ppc.o
#$(OBJ_CPU)/powerpc/ppc.o: ppc.c ppc.h ppc_ops.c ppc_ops.h ppc602.c
endif
DBGOBJS += $(OBJ_CPU)/powerpc/ppc_dasm.o
else
CPUDEFS += -DHAS_PPC602=0
endif

ifneq ($(filter PPC603,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/powerpc
CPUDEFS += -DHAS_PPC603=1
OPT_DEFS += -DBUS_64=1
ifdef X86_PPC_DRC
CPUOBJS += $(OBJ_CPU)/powerpc/ppcdrc.o
#$(OBJ_CPU)/powerpc/ppcdrc.o: ppcdrc.c ppc.h drc_ops.c drc_ops.h ppc_ops.c ppc603.c
else
CPUOBJS += $(OBJ_CPU)/powerpc/ppc.o
#$(OBJ_CPU)/powerpc/ppc.o: ppc.c ppc.h ppc_ops.c ppc_ops.h ppc603.c
endif
DBGOBJS += $(OBJ_CPU)/powerpc/ppc_dasm.o
else
CPUDEFS += -DHAS_PPC603=0
endif








#-------------------------------------------------
# Acorn ARM series
#-------------------------------------------------

ifneq ($(filter ARM,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/arm
CPUDEFS += -DHAS_ARM=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/arm/arm.o
DBGOBJS += $(OBJ_CPU)/arm/armdasm.o
#$(OBJ_CPU)/arm/arm.o: arm.c arm.h
else
CPUDEFS += -DHAS_ARM=0
endif

ifneq ($(filter ARM7,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/arm7
CPUDEFS += -DHAS_ARM7=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/arm7/arm7.o
DBGOBJS += $(OBJ_CPU)/arm7/arm7dasm.o
#$(OBJ_CPU)/arm7/arm7.o: arm7.c arm7.h
else
CPUDEFS += -DHAS_ARM7=0
endif

#-------------------------------------------------
# Advanced Digital Chips SE3208
#-------------------------------------------------

ifneq ($(filter SE3208,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/se3208
CPUDEFS += -DHAS_SE3208=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/se3208/se3208.o
DBGOBJS += $(OBJ_CPU)/se3208/se3208dis.o
#$(OBJ_CPU)/se3208/se3208.o: se3208.c se3208.h se3208dis.c
else
CPUDEFS += -DHAS_SE3208=0
endif

#-------------------------------------------------
# Analog Devices ADSP21xx series
#-------------------------------------------------

ifneq ($(filter ADSP2100,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2100=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
DBGOBJS += $(OBJ_CPU)/adsp2100/2100dasm.o
#$(OBJ_CPU)/adsp2100/adsp2100.o: adsp2100.c adsp2100.h 2100ops.c
else
CPUDEFS += -DHAS_ADSP2100=0
endif

ifneq ($(filter ADSP2101,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2101=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
DBGOBJS += $(OBJ_CPU)/adsp2100/2100dasm.o
#$(OBJ_CPU)/adsp2100/adsp2100.o: adsp2100.c adsp2100.h 2100ops.c
else
CPUDEFS += -DHAS_ADSP2101=0
endif

ifneq ($(filter ADSP2104,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2104=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
DBGOBJS += $(OBJ_CPU)/adsp2100/2100dasm.o
#$(OBJ_CPU)/adsp2100/adsp2100.o: adsp2100.c adsp2100.h 2100ops.c
else
CPUDEFS += -DHAS_ADSP2104=0
endif

ifneq ($(filter ADSP2105,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2105=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
DBGOBJS += $(OBJ_CPU)/adsp2100/2100dasm.o
#$(OBJ_CPU)/adsp2100/adsp2100.o: adsp2100.c adsp2100.h 2100ops.c
else
CPUDEFS += -DHAS_ADSP2105=0
endif

ifneq ($(filter ADSP2115,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2115=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
DBGOBJS += $(OBJ_CPU)/adsp2100/2100dasm.o
#$(OBJ_CPU)/adsp2100/adsp2100.o: adsp2100.c adsp2100.h 2100ops.c
else
CPUDEFS += -DHAS_ADSP2115=0
endif

ifneq ($(filter ADSP2181,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/adsp2100
CPUDEFS += -DHAS_ADSP2181=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/adsp2100/adsp2100.o
DBGOBJS += $(OBJ_CPU)/adsp2100/2100dasm.o
#$(OBJ_CPU)/adsp2100/adsp2100.o: adsp2100.c adsp2100.h 2100ops.c
else
CPUDEFS += -DHAS_ADSP2181=0
endif

#-------------------------------------------------
# Analog Devices "Sharc" ADSP21062
#-------------------------------------------------


#-------------------------------------------------
# AT&T DSP32C
#-------------------------------------------------

ifneq ($(filter DSP32C,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/dsp32
CPUDEFS += -DHAS_DSP32C=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/dsp32/dsp32.o
DBGOBJS += $(OBJ_CPU)/dsp32/dsp32dis.o
#$(OBJ_CPU)/dsp32/dsp32.o: dsp32.c dsp32.h
else
CPUDEFS += -DHAS_DSP32C=0
endif


#-------------------------------------------------
# Intel i960
#-------------------------------------------------

ifneq ($(filter I960,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/i960
CPUDEFS += -DHAS_I960=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/i960/i960.o
DBGOBJS += $(OBJ_CPU)/i960/i960dis.o
#$(OBJ_CPU)/i960/i960.o: i960.c i960.h
else
CPUDEFS += -DHAS_I960=0
endif


#-------------------------------------------------
# NEC V810 (uPD70732)
#-------------------------------------------------

ifneq ($(filter V810,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/v810
CPUDEFS += -DHAS_V810=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/v810/v810.o
DBGOBJS += $(OBJ_CPU)/v810/v810dasm.o
#$(OBJ_CPU)/v810/v810.o: v810.c v810.h
else
CPUDEFS += -DHAS_V810=0
endif

################
################  other
################







#-------------------------------------------------
# Alpha 8201
#-------------------------------------------------

#-------------------------------------------------
# APEXC
#-------------------------------------------------


#-------------------------------------------------
# Atari custom RISC processor
#-------------------------------------------------

#-------------------------------------------------
# Atari Jaguar custom DSPs
#-------------------------------------------------

#-------------------------------------------------
# RCA CDP1802
#-------------------------------------------------

#-------------------------------------------------
# National Semiconductor COP4xx
#-------------------------------------------------

#-------------------------------------------------
# CP1610
#-------------------------------------------------

#-------------------------------------------------
# Cinematronics vector "CPU"
#-------------------------------------------------

#-------------------------------------------------
# DEC T-11
#-------------------------------------------------

#-------------------------------------------------
# F8
#-------------------------------------------------

#-------------------------------------------------
# Hitachi H8/3002
#-------------------------------------------------

ifneq ($(filter H83002,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/h83002
CPUDEFS += -DHAS_H83002=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/h83002/h83002.o $(OBJ_CPU)/h83002/h8periph.o
DBGOBJS += $(OBJ_CPU)/h83002/h8disasm.o
#$(OBJ_CPU)/h83002/h83002.o: h83002.c h83002.h h8priv.h
#$(OBJ_CPU)/h83002/h8disasm.o: h8disasm.c
#$(OBJ_CPU)/h83002/h8periph.o: h8periph.c h8priv.h
else
CPUDEFS += -DHAS_H83002=0
endif

#-------------------------------------------------
# Hyperstone E1 series
#-------------------------------------------------


#-------------------------------------------------
# LH5801
#-------------------------------------------------

#-------------------------------------------------
# Fujitsu MB88xx
#-------------------------------------------------

#-------------------------------------------------
# Fujitsu MB86233
#-------------------------------------------------

#-------------------------------------------------
# Motorola 68HC11
#-------------------------------------------------

ifneq ($(filter MC68HC11,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/mc68hc11
CPUDEFS += -DHAS_MC68HC11=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/mc68hc11/mc68hc11.o
DBGOBJS += $(OBJ_CPU)/mc68hc11/hc11dasm.o
#$(OBJ_CPU)/mc68hc11/mc68hc11.o: mc68hc11.c hc11dasm.c
else
CPUDEFS += -DHAS_MC68HC11=0
endif

#-------------------------------------------------
# Motorola/Freescale dsp56k
#-------------------------------------------------

#-------------------------------------------------
# PDP-1
#-------------------------------------------------


#-------------------------------------------------
# NEC uPD7810 series
#-------------------------------------------------

ifneq ($(filter UPD7810,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/upd7810
CPUDEFS += -DHAS_UPD7810=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/upd7810/upd7810.o
DBGOBJS += $(OBJ_CPU)/upd7810/7810dasm.o
#$(OBJ_CPU)/upd7810/upd7810.o: upd7810.c 7810tbl.c 7810ops.c upd7810.h
else
CPUDEFS += -DHAS_UPD7810=0
endif

ifneq ($(filter UPD7807,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/upd7810
CPUDEFS += -DHAS_UPD7807=1
OPT_DEFS += -DBUS_8=1
CPUOBJS += $(OBJ_CPU)/upd7810/upd7810.o
DBGOBJS += $(OBJ_CPU)/upd7810/7810dasm.o
#$(OBJ_CPU)/upd7810/upd7810.o: upd7810.c 7810tbl.c 7810ops.c upd7810.h
else
CPUDEFS += -DHAS_UPD7807=0
endif

#-------------------------------------------------
# SC61860
#-------------------------------------------------

#-------------------------------------------------
# SM8500
#-------------------------------------------------


#-------------------------------------------------
# SSP1610
#-------------------------------------------------

#-------------------------------------------------
# Texas Instruments TMS7000 series
#-------------------------------------------------

#-------------------------------------------------
# Texas Instruments TMS99xx series
#-------------------------------------------------

#-------------------------------------------------
# Texas Instruments TMS340x0 graphics controllers
#-------------------------------------------------

ifneq ($(filter TMS34010,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/tms34010
CPUDEFS += -DHAS_TMS34010=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/tms34010/tms34010.o $(OBJ_CPU)/tms34010/34010fld.o
DBGOBJS += $(OBJ_CPU)/tms34010/34010dsm.o
#$(OBJ_CPU)/tms34010/tms34010.o: tms34010.c tms34010.h 34010ops.c 34010gfx.c 34010tbl.c
else
CPUDEFS += -DHAS_TMS34010=0
endif

ifneq ($(filter TMS34020,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/tms34020
CPUDEFS += -DHAS_TMS34020=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/tms34010/tms34010.o $(OBJ_CPU)/tms34010/34010fld.o
DBGOBJS += $(OBJ_CPU)/tms34010/34010dsm.o
#$(OBJ_CPU)/tms34010/tms34010.o: tms34010.c tms34010.h 34010ops.c 34010gfx.c 34010tbl.c
else
CPUDEFS += -DHAS_TMS34020=0
endif

#-------------------------------------------------
# Texas Instruments TMS3201x DSP
#-------------------------------------------------

ifneq ($(filter TMS32010,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/tms32010
CPUDEFS += -DHAS_TMS32010=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/tms32010/tms32010.o
DBGOBJS += $(OBJ_CPU)/tms32010/32010dsm.o
#$(OBJ_CPU)/tms32010/tms32010.o: tms32010.c tms32010.h
else
CPUDEFS += -DHAS_TMS32010=0
endif

#-------------------------------------------------
# Texas Instruments TMS3202x DSP
#-------------------------------------------------

ifneq ($(filter TMS32025,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/tms32025
CPUDEFS += -DHAS_TMS32025=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/tms32025/tms32025.o
DBGOBJS += $(OBJ_CPU)/tms32025/32025dsm.o
#$(OBJ_CPU)/tms32025/tms32025.o: tms32025.c tms32025.h
else
CPUDEFS += -DHAS_TMS32025=0
endif

ifneq ($(filter TMS32026,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/tms32025
CPUDEFS += -DHAS_TMS32026=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/tms32025/tms32025.o
DBGOBJS += $(OBJ_CPU)/tms32025/32025dsm.o
#$(OBJ_CPU)/tms32025/tms32025.o: tms32025.c tms32025.h
else
CPUDEFS += -DHAS_TMS32026=0
endif

#-------------------------------------------------
# Texas Instruments TMS3203x DSP
#-------------------------------------------------

ifneq ($(filter TMS32031,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/tms32031
CPUDEFS += -DHAS_TMS32031=1
OPT_DEFS += -DBUS_32=1
CPUOBJS += $(OBJ_CPU)/tms32031/tms32031.o
DBGOBJS += $(OBJ_CPU)/tms32031/dis32031.o
#$(OBJ_CPU)/tms32031/tms32031.o: tms32031.c tms32031.h
else
CPUDEFS += -DHAS_TMS32031=0
endif

#-------------------------------------------------
# Texas Instruments TMS3205x DSP
#-------------------------------------------------

ifneq ($(filter TMS32051,$(CPUS)),)
OBJDIRS += $(OBJ_CPU)/tms32051
CPUDEFS += -DHAS_TMS32051=1
OPT_DEFS += -DBUS_16=1
CPUOBJS += $(OBJ_CPU)/tms32051/tms32051.o
DBGOBJS += $(OBJ_CPU)/tms32051/dis32051.o
#$(OBJ_CPU)/tms32051/tms32051.o: tms32051.c tms32051.h
else
CPUDEFS += -DHAS_TMS32051=0
endif

#-------------------------------------------------
# Toshiba TLCS-90 Series
#-------------------------------------------------

#-------------------------------------------------
# TX0
#-------------------------------------------------


#-------------------------------------------------
# [NO USED MAME] Nintendo Minx
#-------------------------------------------------



# List of CPU core (and, for a debug build, disassembler) object files



