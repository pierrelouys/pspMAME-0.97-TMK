

#ifndef _MY_PG_H_
#define _MY_PG_H_

#include "driver.h"

extern void psp_print(UINT16 x, UINT16 y, UINT16 color, const char *text, ...);
extern void psp_printf_bbb(                             const char *text, ...);
extern int printfnw_(const char *format);

extern void psp_print_color(UINT16 set_color);
extern void psp_print_locate(UINT16 x, UINT16 y);


#define v_sync sceDisplayWaitVblankStart
extern void psp_clear_screen(void);
#endif /* _MY_PG_H_ */

