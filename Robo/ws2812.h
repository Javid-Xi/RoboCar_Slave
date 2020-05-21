#ifndef __WS2812_H
#define __WS2812_H
#include "sys.h"

#define DI PBout(7)

#define SNUM  8

void WS2812_Init(void);
void ResetDataFlow(void);
void SendOnePix(unsigned char *ptr);
void SendOneFrame(unsigned char *ptr);
void SendSameColor(unsigned char *ptr, unsigned char cnt);
void SendOneFrameFrom(unsigned char i, unsigned char *ptr);
void SendOneFrameSince(unsigned char i, unsigned char *ptr);



#endif


