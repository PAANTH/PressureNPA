#include "pwm.h"
#include "stm32f4xx_tim.h"

void pwm_action(void){

    TIM_SetCounter(TIM4,0);
    TIM_SetCounter(TIM8,0);
    TIM8->CCR3=1435;//PWM_TIME;
    TIM8->CCR4=1435;//PWM_TIME;
    TIM_Cmd(TIM4,ENABLE);
    while(TIM_GetCounter(TIM4)<8750);
    TIM8->CCR3=0;
    TIM8->CCR4=0xFFFF;
    TIM_Cmd(TIM4,DISABLE);
}
