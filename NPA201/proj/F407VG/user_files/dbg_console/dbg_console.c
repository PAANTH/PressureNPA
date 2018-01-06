#include "dbg_console.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

#define gpio_clk_config()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define usart_clk_config()    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE)
#define PIN_TX 		GPIO_Pin_6
#define PIN_RX 		GPIO_Pin_7
#define PIN_PORT 	GPIOC
#define PIN_SRC_TX      GPIO_PinSource6
#define PIN_SRC_RX      GPIO_PinSource7
#define USART_NUM	USART6
#define USART_AF        GPIO_AF_USART6

void dbg_console_init(unsigned int baudrate){
	gpio_clk_config();

	GPIO_InitTypeDef gpio_init_struct;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_Pin = PIN_TX | PIN_RX;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(PIN_PORT, &gpio_init_struct);

        GPIO_PinAFConfig(PIN_PORT, PIN_SRC_RX, USART_AF);
        GPIO_PinAFConfig(PIN_PORT, PIN_SRC_TX, USART_AF);

	usart_clk_config();

	USART_InitTypeDef usart_init_struct;
	usart_init_struct.USART_BaudRate = baudrate;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_init_struct.USART_Parity = USART_Parity_No;
	usart_init_struct.USART_StopBits = USART_StopBits_1;
	usart_init_struct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART_NUM, &usart_init_struct);

	USART_Cmd(USART_NUM, ENABLE);

}

int dbg_msg(const char *str){
	int count = 0;
	while(USART_GetFlagStatus(USART_NUM, USART_FLAG_TXE) != SET);
	while(*str!= '\0'){
		USART_SendData(USART_NUM, *str++);
		while(USART_GetFlagStatus(USART_NUM, USART_FLAG_TC) != SET);
		count++;
	}
	return count;
}

void dbg_backspace(int count){
	int i;

	while(USART_GetFlagStatus(USART_NUM, USART_FLAG_TXE) != SET);
	for(i=0; i<count; i++){
		USART_SendData(USART_NUM, '\b');
		while(USART_GetFlagStatus(USART_NUM, USART_FLAG_TC) != SET);
	}
	for(i=0; i<count; i++){
		USART_SendData(USART_NUM, ' ');
		while(USART_GetFlagStatus(USART_NUM, USART_FLAG_TC) != SET);
	}
	for(i=0; i<count; i++){
		USART_SendData(USART_NUM, '\b');
		while(USART_GetFlagStatus(USART_NUM, USART_FLAG_TC) != SET);
	}
}









