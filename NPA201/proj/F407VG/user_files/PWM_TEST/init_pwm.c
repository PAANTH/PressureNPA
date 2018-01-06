#include "init_pwm.h"
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

//#define TIMER_PRESCALER 12000  //10000 timer ticks per second
#define PWM_TIME 1435//1025
void Init_pwm_V(){
    //Init GPIO PC8/9
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.GPIO_Speed=GPIO_Speed_100MHz;
    gpio_init_struct.GPIO_Mode=GPIO_Mode_AF;
    gpio_init_struct.GPIO_OType=GPIO_OType_PP;
    gpio_init_struct.GPIO_PuPd=GPIO_PuPd_DOWN;
    gpio_init_struct.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_Init(GPIOC,&gpio_init_struct);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,GPIO_AF_TIM8);

    //Init timers TIM8_CH3/4
    //tim init
    TIM_TimeBaseInitTypeDef tim_init_struct;
    tim_init_struct.TIM_ClockDivision=TIM_CKD_DIV1;
    tim_init_struct.TIM_CounterMode=TIM_CounterMode_Up;
    tim_init_struct.TIM_Period=2870;//2050;
    tim_init_struct.TIM_Prescaler=0;
    tim_init_struct.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM8, &tim_init_struct);
    //pwm init
    TIM_OCInitTypeDef pwm_init_struct;
    TIM_OCStructInit(&pwm_init_struct);
    pwm_init_struct.TIM_OCMode=TIM_OCMode_PWM2;
    pwm_init_struct.TIM_OutputState=TIM_OutputState_Enable;
    pwm_init_struct.TIM_Pulse=PWM_TIME;

    pwm_init_struct.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC3Init(TIM8, &pwm_init_struct);
    pwm_init_struct.TIM_OCPolarity=TIM_OCPolarity_Low;
    TIM_OC4Init(TIM8, &pwm_init_struct);


    TIM_TimeBaseStructInit(&tim_init_struct);
    tim_init_struct.TIM_Prescaler=0;
    tim_init_struct.TIM_Period=0xFFFF;
    TIM_TimeBaseInit(TIM4, &tim_init_struct);

    //Start transmitters

    //reset transmitters
    TIM8->CCR3=0;
    TIM8->CCR4=0xFFFF;

    TIM_Cmd(TIM8,ENABLE);
    TIM_CtrlPWMOutputs(TIM8,ENABLE);

    //reset transmitters
    TIM8->CCR3=0;
    TIM8->CCR4=0xFFFF;
}
