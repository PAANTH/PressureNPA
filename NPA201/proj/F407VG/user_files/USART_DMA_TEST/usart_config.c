#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "rs.h"
#include "stm32f4xx_gpio.h"
#include "usart_config.h"
void Config_usart_periph(void){


    //rs_init();

    //Button Init
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef port;
    GPIO_StructInit(&port);



    port.GPIO_Mode=GPIO_Mode_OUT;
    port.GPIO_OType=GPIO_OType_PP;
    port.GPIO_Pin=GPIO_Pin_15; //LED_PIN
    port.GPIO_PuPd=GPIO_PuPd_NOPULL;
    port.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&port);

    port.GPIO_Mode=GPIO_Mode_IN;
    port.GPIO_Pin=GPIO_Pin_0;//BUTTON_PIN;
    port.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&port);



}










