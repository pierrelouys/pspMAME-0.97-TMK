
#-------------------------------------------------
# customizing sound set
#-------------------------------------------------



#ifeq ($(TARGET),ARIENAI)

	# 専用サウンドの場合、ディレクトリーを追加。
	OBJDIRS += $(OBJ)/psp_sound 

## direct stream 実験


	ifeq ($(TARGET),namcos86)
	SOUNDOBJS = $(OBJ)/psp_sound/sound_namcos86.o
	OPT_DEFS += -DPSP_DIRECT_STREAM=1
	OPT_DEFS += -DLINK_MAME_SOUND=0
	OPT_DEFS += -DLINK_MAME_SPEAKER=0
	endif

	# 速度が遅いカラ
	ifeq ($(TARGET),turbo_outrun)
	SOUNDOBJS = $(OBJ)/psp_sound/sound_outrun.o
	OPT_DEFS += -DPSP_DIRECT_STREAM=1
	OPT_DEFS += -DLINK_MAME_SOUND=1
	OPT_DEFS += -DLINK_MAME_SPEAKER=0
	endif

	# メモリが足りないカラ
#	ifeq ($(TARGET),cotton)
#	SOUNDOBJS = $(OBJ)/psp_sound/sound_cotton.o
#	OPT_DEFS += -DPSP_DIRECT_STREAM=1
#	OPT_DEFS += -DLINK_MAME_SOUND=0
#	endif

	# ナントナク
	ifeq ($(TARGET),fantasy_zone)
	SOUNDOBJS = $(OBJ)/psp_sound/sound_outrun.o
	OPT_DEFS += -DPSP_DIRECT_STREAM=1
	OPT_DEFS += -DLINK_MAME_SOUND=1
	OPT_DEFS += -DLINK_MAME_SPEAKER=0
	endif

	# 速度が遅いカラ
	ifeq ($(TARGET),taitoB)
	SOUNDOBJS = $(OBJ)/psp_sound/sound_2610_8MHz.o
	OPT_DEFS += -DPSP_DIRECT_STREAM=1
	OPT_DEFS += -DLINK_MAME_SOUND=1
	OPT_DEFS += -DLINK_MAME_SPEAKER=0
	endif

	# 速度が遅いカラ
	ifeq ($(TARGET),night_striker)
	SOUNDOBJS = $(OBJ)/psp_sound/sound_2610_8MHz.o
	OPT_DEFS += -DPSP_DIRECT_STREAM=1
	OPT_DEFS += -DLINK_MAME_SOUND=1
	OPT_DEFS += -DLINK_MAME_SPEAKER=0
	endif

# 音(ADPCM)がおかしい
	# 速度が遅いカラ
	ifeq ($(TARGET),ninjya_warriors)
	SOUNDOBJS = $(OBJ)/psp_sound/sound_2610_8MHz.o
	OPT_DEFS += -DPSP_DIRECT_STREAM=1
	OPT_DEFS += -DLINK_MAME_SOUND=1
	OPT_DEFS += -DLINK_MAME_SPEAKER=0
	endif

#	# 速度が遅いカラ
#	ifeq ($(TARGET),zerowing)
#	SOUNDOBJS = $(OBJ)/psp_sound/sound_3526_3812_4MHz.o
#	OPT_DEFS += -DPSP_DIRECT_STREAM=1
#	OPT_DEFS += -DLINK_MAME_SOUND=0
#	endif

	# ナントナク
	ifeq ($(TARGET),namcoNA)
	SOUNDOBJS = $(OBJ)/psp_sound/sound_NA.o
	OPT_DEFS += -DPSP_DIRECT_STREAM=1
	OPT_DEFS += -DLINK_MAME_SOUND=1
	OPT_DEFS += -DLINK_MAME_SPEAKER=0
	endif

	# ナントナク
	ifeq ($(TARGET),ajax)
	SOUNDOBJS = $(OBJ)/psp_sound/sound_a_jax.o
	OPT_DEFS += -DPSP_DIRECT_STREAM=1
	OPT_DEFS += -DLINK_MAME_SOUND=1
	OPT_DEFS += -DLINK_MAME_SPEAKER=0
	endif

#だめ(作ってない)
#	# サイドアーム 不安定の為
#	ifeq ($(TARGET),ninjya_kun2)
#	SOUNDOBJS = $(OBJ)/psp_sound/sound_ninjya_kun2.o
#	OPT_DEFS += -DPSP_DIRECT_STREAM=1
#	OPT_DEFS += -DLINK_MAME_SOUND=1
#	OPT_DEFS += -DLINK_MAME_SPEAKER=0
#	endif


#endif
