AR = mipsel-elf-ar
CC = mipsel-elf-gcc
LD = mipsel-elf-ld
ASM = mipsel-elf-as
STRIP =mipsel-elf-strip

CFLAGS += -D_HUGE=HUGE_VAL -DCLIB_DECL=
ASMFLAGS = -march=r4000 -g -mabi=eabi -mgp32 -c -xassembler -O

# add libs
#TMK LIBS += -lalleg -laudio -lemu -lzvg
LIBS = -lc -lm /usr/local/lib/gcc/mipsel-elf/3.4.2/libgcc.a

# only PSP specific output files and rules
OSOBJS = $(OBJ)/psp/psp.o $(OBJ)/psp/video.o $(OBJ)/psp/blit.o \
	$(OBJ)/psp/sound.o $(OBJ)/psp/input.o $(OBJ)/psp/rc.o $(OBJ)/psp/misc.o \
	$(OBJ)/psp/ticker.o $(OBJ)/psp/config.o $(OBJ)/psp/fronthlp.o \
	$(OBJ)/psp/fileio.o $(OBJ)/psp/debugwin.o \
	$(OBJ)/psp/syscalls.o \
        $(OBJ)/psp/pg.o $(OBJ)/psp/pspmain.o $(OBJ)/psp/menu.o \
	$(OBJ)/psp/y_malloc.o \
	$(OBJ)/psp/v_malloc.o \

#	$(OBJ)/psp/malloc.o \
#	$(OBJ)/psp/y_malloc.o \
#	$(OBJ)/psp/malloc.o \
#	$(OBJ)/psp/v_malloc.o \
#	$(OBJ)/psp/fpgnulib.o \

# startup functions
$(OBJ)/psp/startup.o: src/psp/startup.s
	@echo Assembling $<...
	$(CC) -o $@ $(ASMFLAGS) $(subst -D,-d,$(ASMDEFS)) $<

# $(OBJ)/psp/fpgnulib.o: src/psp/fpgnulib.c
