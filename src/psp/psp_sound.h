//============================================================
//
//	psp_sound.c - PSP sound functions
//
//============================================================

#ifndef PSP_SOUND_H
#define PSP_SOUND_H


#define PSP_SOUND_SAMPLES		(736*2)
#define PSP_SOUND_BUFFER_SIZE	(PSP_SOUND_SAMPLES*2)

struct sound_t
{
	int  stack;
	int  stereo;
	void (*callback)(int p);
};

#define PSP_MAXOUT		(+32767)
#define PSP_MINOUT		(-32768)

#define PSP_SOUND_Limit(val, max, min)			\
{										\
	if (val > max) val = max;			\
	else if (val < min) val = min;		\
}

extern struct sound_t *psp_sound;

void psp_sound_init(void);
void psp_sound_exit(void);
void psp_sound_enable(unsigned char set_to_enable/*int enable*/);
void osd_sound_enable(int enable_it);
void psp_sound_set_volume(void);
int psp_sound_start(void);
void psp_sound_stop(void);
void psp_sound_set_priority(int priority);


//void psp_sound_stop(void);

//static int pga_channel_thread(int args, void *argp);
//void pga_channel_thread_callback(int channel, void *buf, unsigned long reqn);

//extern int pgaInit(void);
//extern void sound_thread_stop(void);
//int pgaOutBlocking(unsigned long channel,unsigned long vol1,unsigned long vol2,void *buf);



#endif /* PSP_SOUND_H */
