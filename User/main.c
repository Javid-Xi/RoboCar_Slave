/**
************************************************************
* @file         main.c
* @brief        µÆÐ§¿ØÖÆ
* @author       Javid
* @date         2020-05-16
* @version      1.0
*
***********************************************************/
#include "sys.h"
#include "delay.h"
#include "spi.h"

int main()
{
    delay_init();
    SPI2_Init();
    delay_ms(100);

    while(1)
    {
        LED_Circulate();
    }
}

