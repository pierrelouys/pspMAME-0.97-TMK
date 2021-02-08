/***************************************************************************

	NAMCO sound driver.

	This driver handles the four known types of NAMCO wavetable sounds:

		- 3-voice mono (PROM-based design: Pac-Man, Pengo, Dig Dug, etc)
		- 8-voice quadrophonic (Pole Position 1, Pole Position 2)
		- 8-voice mono (custom 15XX: Mappy, Dig Dug 2, etc)
		- 8-voice stereo (System 1)

***************************************************************************/

/* this structure defines the parameters for a channel */
typedef struct
{
	UINT32 counter;
	/*int*/UINT32 freq;
	int volume;
	int waveform;
	int noise_freq;
	int noise_seed;
	UINT32 noise_counter;
	/*int*/UINT8 noise_sw;
	/*int*/UINT8 noise_state;
} sound_channel;



struct namco_sound
{
	/* data about the sound system */
	sound_channel channel_list[8];
	sound_channel *last_channel;

	/* global sound parameters */
	int sound_enable;
	int namco_clock;
	int sample_rate;
	float freqbase;

	INT32 buffer[PSP_SOUND_SAMPLES];
};

static struct namco_sound PSP_ALIGN_DATA NAMCO_CUS30;
static struct namco_sound *namcos1 = &NAMCO_CUS30;

/* xxx globals available to everyone */
//UINT8 *namco_wavedata;
static UINT8 namco_wavedata[0x400];

static void namco_cus30_update(int length)
{
	sound_channel *voice;
	float freqbase = namcos1->freqbase;
	int delta;

	/* zap the contents of the buffer */
	memset(namcos1->buffer, 0, sizeof(namcos1->buffer));

	/* if no sound, we're done */
	if (namcos1->sound_enable == 0)
		return;

	/* loop over each voice and add its contribution */
	for (voice = namcos1->channel_list; voice < namcos1->last_channel; voice++)
	{
		INT32 *buf = namcos1->buffer;
		int volume = voice->volume;

		if (volume)
		{
			int i;
		//	int step;
			if (voice->noise_sw)
			{
			s16 noise_data;
				delta = (int)((float)voice->noise_freq * freqbase);
			//	step = voice->noise_counter;

				noise_data=0x7 * volume;
				/* add our contribution */
				for (i = 0; i < length; i++)
				{
					int cnt;

					if (voice->noise_state)
						(*buf++) += noise_data;
					else
						(*buf++) -= noise_data;

					voice->noise_counter/*step*/ += delta;
					cnt = voice->noise_counter/*step*/ >> 12;
					voice->noise_counter/*step*/ &= (1 << 12) - 1;
					for ( ;cnt > 0; cnt--)
					{
						if ((voice->noise_seed + 1) & 2) voice->noise_state ^= 1;
						if (voice->noise_seed & 1) voice->noise_seed ^= 0x28000;
						voice->noise_seed >>= 1;
					}
				}
				/* update the counter for this voice */
			//	voice->noise_counter = step;
			}
			else
			{
				UINT8 *pw = &namco_wavedata[16 * voice->waveform];

				delta = (int)((float)voice->freq * freqbase);
			//	step = voice->counter;

				for (i = 0; i < length; i++)
				{
					(*buf++) += ((pw[(voice->counter/*step*/ >> 16) & 0x0f] & 0xff) - 128) * volume;
					voice->counter/*step*/ += delta;
				}
				voice->counter += voice->counter/*step*/;
			}
		}
	}
}


static void namco_cus30_init(int clock, int rate, int num_voices)
{
	sound_channel *voice;

	memset(namcos1, 0, sizeof(NAMCO_CUS30));

	namcos1->last_channel = namcos1->channel_list + num_voices;
	namcos1->namco_clock  = clock;
	namcos1->sample_rate  = rate;
	namcos1->freqbase	  = ((float)clock * (float)(sr/*+1*/)) / (float)44100/*rate*/;
	namcos1->sound_enable = 1;

	for (voice = namcos1->channel_list; voice < namcos1->last_channel; voice++)
	{
		voice->freq = 0;
		voice->volume = 0;
		voice->waveform = 0;
		voice->counter = 0;
		voice->noise_freq = 0;
		voice->noise_seed = 1;
		voice->noise_counter = 0;
		voice->noise_sw = 0;
		voice->noise_state = 0;
	}
}




READ8_HANDLER( namcos1_cus30_r )
{
	return namco_wavedata[offset];
}

/* offset: ---- ---1 --cc csss */
/* c: channel(0-7) */
/* s: select regs(0-7) */
WRITE8_HANDLER( namcos1_cus30_w )
{
	if (namco_wavedata[offset] == data) 	return;

	/* set the register */
	namco_wavedata[offset] = data;

	if (offset >= 0x100)
	{
		if(offset < 0x140)
		{
			sound_channel *voice;
			UINT8 channel_offs;

			channel_offs = offset & 0x38;
			/* recompute the voice parameters */
			voice = namcos1->channel_list + ((channel_offs) / 8);
			switch (offset & 7)
			{
			case 0x00:
				voice->volume = data & 0x0f;	/* モノラルの場合の音量 */
			/*	voice->volume[0] = data & 0x0f;  */
				break;
			case 0x01:
				voice->waveform = (data >> 4) & 15;
				/*not_break;*/
			case 0x02:
				/*not_break;*/
			case 0x03:
				/* the frequency has 20 bits */
				voice->freq  = (namco_wavedata[channel_offs + 0x01+ 0x100] & 15) << 16;
				voice->freq +=	namco_wavedata[channel_offs + 0x02+ 0x100] << 8;
				voice->freq +=	namco_wavedata[channel_offs + 0x03+ 0x100];
				voice->noise_freq = namco_wavedata[channel_offs + 0x03+ 0x100] << 5;
				break;
			case 0x04:
				if (++voice == namcos1->last_channel){	voice = namcos1->channel_list;}
				/* ステレオの場合、ここで「次の」音量 */
			/*	voice->volume[1] = data & 0x0f;  */ /* 註：ここは次のVOICEの音量 */
				voice->noise_sw = ((data & 0x80) >> 7);/* noise switch */
				break;
			}
		}
	}
}

