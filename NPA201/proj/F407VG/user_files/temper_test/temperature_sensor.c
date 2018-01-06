#include "temperature_sensor.h"
#include "i2c.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "misc.h"



uint16_t get_Temperature(){

    //normal representation, MSB bit is sign  -127...+127 , 1st byte -integer 2nd -fractional
    uint16_t temperature = 0;
    uint8_t intg = 0;
    uint8_t frac = 0;
    uint8_t sign = 0, temp=0;

    //temp=I2C_DMA_Read(0x90, 1);
    while(i2c_dma_ready==0);

    if((I2C_Rx_Buffer[0] & 0x80) !=0 ){
        sign=1;
        uint16_t b = ~((0xE000 | (((I2C_Rx_Buffer[0]<<8)+I2C_Rx_Buffer[1])>>3))-1);
        intg = b>>4;
        frac = 0x0F & b;
    }
    else {
        sign=0;
        uint16_t b = ((I2C_Rx_Buffer[0]<<8)+I2C_Rx_Buffer[1])>>3;
        intg = b>>4;
        frac = (0x0F & b)*625/100;
    }

    intg = (sign<<7)| intg;
    temperature = (intg<<8) | frac;

    return temperature;
}






















