
#define FRAC_BIT	12
#define FRAC_SIZE	(1 << FRAC_BIT)


/***************************************************************************

An Hitachi HD637A01X0 MCU programmed to act as a sample player.
Used by some Namco System 86 games.

The MCU has internal ROM which hasn't been dumped, so here we simulate its
simple functions.

The chip can address ROM space up to 8 block of 0x10000 bytes. At the beginning
of each block there's a table listing the start offset of each sample.
Samples are 8 bit unsigned, 0xff marks the end of the sample. 0x00 is used for
silence compression: '00 nn' must be replaced by nn+1 times '80'.

訳（ダイタイ）：このチップは殆ど「８ビットＰＣＭ」と一緒だけど、
0x00のデーターだけ「ランレングス伸張機能」がついてるでづね。

***************************************************************************/

typedef struct
{
	int select;
	int playing;
	int base_addr;
	int position;
	int volume;
	int silence_counter;
	int stream_pos;
	int prev_sample;
} voice;

typedef struct
{
	voice voices[2];
	int source_step;
	UINT8 *rom;
	INT32 buffer[PSP_SOUND_SAMPLES];
} n63701x_t;

static n63701x_t PSP_ALIGN_DATA N63701X;
static n63701x_t *n63701x = &N63701X;


void n63701x_reset(void)
{
	memset(n63701x->voices, 0, sizeof(n63701x->voices));
	memset(n63701x->buffer, 0, sizeof(n63701x->buffer));
}


static void n63701x_init(int clock, int rate)
{
	memset(n63701x, 0, sizeof(n63701x_t));

	clock /= 1000;
#if 1
	clock *= (sr/*+1*/);
#endif
	n63701x->source_step = (clock << FRAC_BIT) / 44100/*rate*/;

	if (memory_region(REGION_SOUND1))
		n63701x->rom = memory_region(REGION_SOUND1);

	n63701x_reset();
}


static void n63701x_update(int length)
{
	int ch;

	memset(n63701x->buffer, 0, sizeof(n63701x->buffer));

	for (ch = 0; ch < 2; ch++)
	{
		INT32 *buf = n63701x->buffer;
		voice *v = &n63701x->voices[ch];

		if (v->playing)
		{
//			static const int vol_table[4] = { 26, 84, 200, 258 };
/* 他の音源と音を混ぜる倍率 */
#define N63701X_MIX_MULTI 2.5
			static const int vol_table[4] =
			{
			N63701X_MIX_MULTI * 26,
			N63701X_MIX_MULTI * 84,
			N63701X_MIX_MULTI * 200,
			N63701X_MIX_MULTI * 258
			};
#undef N63701X_MIX_MULTI
			UINT8 *base = n63701x->rom + v->base_addr;
			int pos = v->position;
			int vol = vol_table[v->volume];
			int p;
			int sample = v->prev_sample;

			for (p = 0; p < length; p++)
			{
				if (v->stream_pos <= 0)
				{
					v->stream_pos += FRAC_SIZE;

					if (v->silence_counter)
					{
						v->silence_counter--;
						sample = 0;
					}
					else
					{
						int data = base[(pos++) & 0xffff];

						if (0xff==data) /* end of sample(0xffはおしまい) */
						{
							v->playing = 0;
							v->stream_pos = 0;
							sample = 0;
							break;
						}
						else if (0x00==data)	/* silence compression（０はランレングスで圧縮済み） */
						{
							data = base[(pos++) & 0xffff];
							v->silence_counter = data;
							sample = 0;
						}
						else	/* それ以外は「８ビットＰＣＭ」 */
						{
							sample = vol * (data - 0x80);	/* 0x80引いて「符号」を変換． */
						}
					}
				}
				v->stream_pos -= n63701x->source_step;
				(*buf++) += (INT32)(sample);		/* sound mixed */
			}
			v->position = pos;
			v->prev_sample = sample;
		}
	}
}


static void n63701x_write(int offset, int data)
{
	int ch = offset >> 1;

	if (offset & 1)
	{
		n63701x->voices[ch].select = data;
	}
	else
	{
		if (n63701x->voices[ch].select & 0x1f)
		{
			int rom_offs;

			n63701x->voices[ch].playing = 1;
			n63701x->voices[ch].base_addr = 0x10000 * ((n63701x->voices[ch].select & 0xe0) >> 5);
			rom_offs = n63701x->voices[ch].base_addr + 2 * ((n63701x->voices[ch].select & 0x1f) - 1);
			n63701x->voices[ch].position = (n63701x->rom[rom_offs] << 8) + n63701x->rom[rom_offs + 1];
			n63701x->voices[ch].volume = data >> 6;
			n63701x->voices[ch].silence_counter = 0;
		}
	}
}



/* -------chip inter face------- */

#if 1
WRITE8_HANDLER( namco_63701x_write )
{
	n63701x_write(offset, data);
}
#endif




