/***************************************************************************

  namcos86.c

***************************************************************************/

#ifndef NAMCOS86_SOUND_H
#define NAMCOS86_SOUND_H

typedef void (*FM_IRQHANDLER_2151)(int irq);

//void YM2151Init(int clock, int rate, FM_IRQHANDLER_2151 IRQHandler);
void YM2151Reset(void);
void YM2151Update(int p);
void YM2151WriteReg(int reg, int value);
int YM2151ReadStatus(void);

void timer_callback_2151_a(void *param);
void timer_callback_2151_b(void *param);

//extern UINT8 *namco_soundregs;
//extern UINT8 *namco_wavedata;

//void namco_cus30_init(int clock, int rate, int num_voices);
//static void namco_cus30_sound_w(int offset, int data);

//void n63701x_reset(void);
//void n63701x_init(int clock, int rate);
//void n63701x_write(int offset, int data);

#endif /* NAMCOS86_SOUND_H */
/***************************************************************************

  ns86snd.c

  NAMCO SYSTEM86 Support interface

***************************************************************************/

#ifndef NAMCOS86_SOUND_INTERFACE_H
#define NAMCOS86_SOUND_INTERFACE_H

//#include "namcos86.h"

//void namcos86_sh_start(void);
//void namcos86_sh_stop(void);
//void namcos86_sh_reset(void);

READ8_HANDLER( YM2151_status_port_0_r );
WRITE8_HANDLER( YM2151_register_port_0_w );
WRITE8_HANDLER( YM2151_data_port_0_w );

READ8_HANDLER( namcos1_cus30_r );
WRITE8_HANDLER( namcos1_cus30_w );

WRITE8_HANDLER( namco_63701x_write );

#endif /* NAMCOS86_SOUND_INTERFACE_H */


