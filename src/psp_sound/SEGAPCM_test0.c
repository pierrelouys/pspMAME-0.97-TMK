/*************

SEGA PCM for OUT RUN.

**************/

struct segapcm
{
	UINT8  *ram;
	UINT32 *step;
	const UINT8 *rom;
	const UINT8 *rom_end;
	int bankshift;
	int bankmask;
	UINT16 low[16];
//	int rate;
//	sound_stream * stream;

	INT32 buffer[PSP_SOUND_SAMPLES];
};
static struct segapcm *spcm;

static void SEGAPCM_Update_stream(/*s16 *mixing_buffer,*/ int length)
//void *param, stream_sample_t **inputs, stream_sample_t **buffer, int length)
{
//	struct segapcm *spcm = param;

//	memset(buffer[0], 0, length*sizeof(*buffer[0]));
//	memset(buffer[1], 0, length*sizeof(*buffer[1]));
//	memset(mixing_buffer, 0, (length/*+length*/) );

	memset(spcm->buffer, 0, sizeof(spcm->buffer));

	int ch;
	for(ch=0; ch<16; ch++)
	{
		INT32 *buf = spcm->buffer;
		if(!(spcm->ram[0x86+8*ch] & 1))
		{
			UINT8 *base = spcm->ram+8*ch;
			UINT32 addr = (base[5] << 24) | (base[4] << 16) | spcm->low[ch];
			UINT16 loop = (base[0x85] << 8)|base[0x84];
			UINT8 end = base[6]+1;
//			UINT8 delta = base[7];
			UINT32 step = spcm->step[/*delta*/base[7]];
//			UINT8 voll = base[2];
//			UINT8 volr = base[3];
//			UINT8 flags = base[0x86];
			const UINT8 *rom = spcm->rom + ((/*flags*/base[0x86] & spcm->bankmask) << spcm->bankshift);
			int jjj;
			for(jjj=0; jjj<(length/*+length*/); jjj++)
			{
				if((addr>>24) == end)
				{
					if(!(/*flags*/base[0x86] & 2))
					{
						addr = (loop<<16);
					}
					else
					{
						/*flags*/base[0x86] |= 1;
						break;
					}
				}
			//	const UINT8 *ptr;
			//	ptr = rom + (addr>>16);
				if( (/*ptr*/(const UINT8 *)(rom + (addr>>16) )) < (spcm->rom_end) )
				{
//					volatile /*INT8*/INT32 v;
//					v = (rom[addr>>16] - 0x80);
#if 0
/* stereo */
					mixing_buffer[jjj] += (v*base[2]/*voll*/);	jjj++;
					mixing_buffer[jjj] += (v*base[3]/*volr*/);	jjj++;
#else
/* monoral */
{
//typedef INT32 _sFMSAMPLE_MIX;
//	_sFMSAMPLE_MIX out_MONO;
//#define _sMAXOUT		/*(+32767)*/(+16383)
//#define _sMINOUT		/*(-32768)*/(-16384)
//		out_MONO  = (v*base[2]/*voll*/) + (v*base[3]/*volr*/);
//		PSP_SOUND_Limit(out_MONO, _sMAXOUT, _sMINOUT);
//					mixing_buffer[jjj] += out_MONO; jjj++;
//					(*buf++) += (INT32)((v*base[2]/*voll*/) + (v*base[3]/*volr*/));
					(*buf++) += (INT32)(
						(INT32)(rom[addr>>16] - 0x80) *
						(
							(INT32)base[2]/*voll*/ + ((INT32)base[3]/*volr*/)
						)
					);
}
#endif
				}
				addr += step;
			}
			base[4] = addr>>16;
			base[5] = addr>>24;
//			base[0x86] = /*flags*/base[0x86];
			spcm->low[ch] = (/*flags*/base[0x86] & 1) ? (0) : (addr);
		}
	}

#if 0
			int jjj;
			for(jjj=0; jjj<(length/*+length*/); )
			{
{
typedef INT32 _sFMSAMPLE_MIX;
	_sFMSAMPLE_MIX out_MONO;
#define _sMAXOUT		/*(+32767)*/(+16383)
#define _sMINOUT		/*(-32768)*/(-16384)
		out_MONO  = mixing_buffer[jjj];

		PSP_SOUND_Limit(out_MONO, _sMAXOUT, _sMINOUT);
					mixing_buffer[jjj] = out_MONO;	jjj++;
}
			}
#endif

}




static void segapcm_start(int external_clock, int rate)///*int*/UINT8 sndindex, int external_clock, const void *config)
{
/* machine->drv->sound[1].config‚ª 1 ‚È‚Ì‚ÍA
	0(‚Â‚Ü‚èŽn‚ß‚É“o˜^) ‚ª YM2151 ‚ÅA
	1(‚Â‚Ü‚è‚Q”Ô–Ú‚É“o˜^) ‚ªsegaPCM‚¾‚©‚çB
	Ú‚µ‚­‚Í src/drivers/outrun.c ‚ðŽQÆ‚¹‚æB */
#define REGIST_ON_DRIVER_THE_SEGAPCM_SOUND_NUMBER (1)
	const struct SEGAPCMinterface *intf = machine->drv->sound[REGIST_ON_DRIVER_THE_SEGAPCM_SOUND_NUMBER].config;
#undef	REGIST_ON_DRIVER_THE_SEGAPCM_SOUND_NUMBER
//	struct segapcm *spcm;

	spcm = auto_malloc(sizeof(*spcm));
	memset(spcm, 0, sizeof(*spcm));

//	spcm->rate = /*SEGAPCM_SAMPLE15K*/external_clock/*external_clock*/;

	/* rom addr */
	spcm->rom = (const UINT8 *)memory_region(intf->region); 			/* ‚¿‚È‚Ý‚É src/drivers/outrun.c ‚Ìê‡ REGION_SOUND1 */
	spcm->rom_end = spcm->rom + memory_region_length(intf->region); 	/* ‚¿‚È‚Ý‚É src/drivers/outrun.c ‚Ìê‡ REGION_SOUND1 */

	/* calc step tables */
	spcm->step = auto_malloc(sizeof(UINT32)*256);
	{int i;
	for(i=0; i<256; i++)
	{
		spcm->step[i] = i*external_clock/*spcm->rate*/*(double)(65536/128) / rate/*machine->sample_rate*/;
	}}

	/* ram fill 0xff */
	spcm->ram = auto_malloc(0x800);
	memset(spcm->ram, 0xff, 0x800);

	/*	*/
	spcm->bankshift = (UINT8)(intf->bank);	/* ‚¿‚È‚Ý‚É src/drivers/outrun.c ‚Ìê‡ BANK_512 */

	/*	*/
	int mask;
	mask = intf->bank >> 16;				/* ‚¿‚È‚Ý‚É src/drivers/outrun.c ‚Ìê‡ BANK_512 */
	if(!mask){	mask = BANK_MASK7>>16;}

	{int rom_mask;
	for(rom_mask = 1; rom_mask < memory_region_length(intf->region); rom_mask *= 2);
	rom_mask--;
	spcm->bankmask = mask & (rom_mask >> spcm->bankshift);
	}
//	spcm->stream = stream_create(0, 2, machine->sample_rate, spcm, SEGAPCM_Update_stream );

//	return spcm;
}


/* -------chip inter face------- */


//WRITE8_HANDLER( SegaPCM_w ){}
//READ8_HANDLER( SegaPCM_r ){}



WRITE8_HANDLER( SegaPCM_w )
{
//	struct segapcm *spcm = sndti_token(SOUND_SEGAPCM, 0);
//	stream_update(spcm->stream, 0);
	spcm->ram[offset & 0x07ff] = data;
}

READ8_HANDLER( SegaPCM_r )
{
//	struct segapcm *spcm = sndti_token(SOUND_SEGAPCM, 0);
//	stream_update(spcm->stream, 0);
	return spcm->ram[offset & 0x07ff];
}
