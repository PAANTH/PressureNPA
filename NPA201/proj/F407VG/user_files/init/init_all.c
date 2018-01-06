#include "init_all.h"
#include "rs.h"
#include "init_pwm.h"
//#include "usart_config.h"
#include "usart_two_bits.h"
#include "stm32f4xx_i2c.h"
#include "i2c.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_i2c.h"
#include "misc.h"
#include "pressure_sensor.h"
#include "stm32f4xx_usart.h"
#include "network.h"
#include "udp_file.h"
void init_all(){
    //init_usb();
    //Init_pwm_V();
    //Config_usart_periph();

//    //Arbeit
   // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//////    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//    SysTick_Config(16800);
//    init_Network ();
//    GPIO_InitTypeDef gpio_init_structure;
    
//    gpio_init_structure.GPIO_Mode = GPIO_Mode_IN;
//   // gpio_init_structure.GPIO_OType = GPIO_OType_PP;
//    gpio_init_structure.GPIO_Pin =  GPIO_Pin_0;
//   // gpio_init_structure.GPIO_PuPd = GPIO_PuPd_DOWN;
//    gpio_init_structure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_Init(GPIOA, &gpio_init_structure);
    
    
//    gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;
//    gpio_init_structure.GPIO_OType = GPIO_OType_PP;
//    gpio_init_structure.GPIO_Pin =  GPIO_Pin_15;
//    gpio_init_structure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    gpio_init_structure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_Init(GPIOD, &gpio_init_structure);

    
  
//////    GPIO_ResetBits(GPIOB,GPIO_Pin_7);

//////    GPIO_SetBits(GPIOB,GPIO_Pin_7);
   
    
    //TIM_ITConfig (TIM2, TIM_IT_Update, ENABLE);
    //NVIC_EnableIRQ (TIM2_IRQn);
    //TIM_Cmd (TIM2, ENABLE);
   
        config_I2C();

//    //config_Pressure();

//        /////USART
//rs_init();
//       //USART_SendData (USART6, 0x3E);
//        /// 
     // Init_pwm_V ();
       
}
void SysTick_Handler(){
    systemTicks++;
}

//uint8_t temp1 = 33;
//void TIM2_IRQHandler(){
//    GPIO_ResetBits (GPIOD,GPIO_Pin_11);
//    TIM_ClearITPendingBit (TIM2, TIM_IT_Update);
    
//    //GPIO_ToggleBits (GPIOD, GPIO_Pin_15);
//    temp1 = temp1 >>1;
    
//}
