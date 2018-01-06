#include "pressure_sensor.h"
#include "i2c.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "string.h"
#include "stm32f4xx_gpio.h"
#define PRESS_CHIP_ADDR 0x00

void config_Pressure(){

    //uint8_t buff[1]={0x04};
    //Start conversion
//    //I2C_Write(PRESS_CHIP_ADDR,PRESS_CONTROL_REG1, 1,1,buff);
//    int j;
//    for (j=0; j<1000000;j++);
//    buff[0]=0x31; //0x39 for 512ms
    //Set sample time 256ms and set active
    //I2C_Write(PRESS_CHIP_ADDR,PRESS_CONTROL_REG1, 1,1,buff);
    //while(!Get_Pressure());
}

uint32_t get_Pressure(){

   
    float pressure=0, temper=0;
    uint8_t val =0xAC;
    pressure=I2C_Write(PRESS_CHIP_ADDR,1,&val);
    for(int i=0; i<30000000;i++);//"kind of" 20ms
    I2C_DMA_Read(PRESS_CHIP_ADDR);
    while(i2c_dma_ready==0);
    pressure = (I2C_Rx_Buffer[1]<<8)|I2C_Rx_Buffer[2];
    pressure = pressure/65535*(1260-260)+260;
    
    temper = (I2C_Rx_Buffer[3]<<8)|I2C_Rx_Buffer[4];
    temper = temper/65535*(85+40)-40;
    return (uint32_t)pressure;
}

////////////////////////////////////////////////
///CHANGE ADDRESS
/// 

uint8_t commandData[4];
uint8_t dat[5], addr[23], sensMemLow[23],sensMemHigh[23],temp=0;


void init_Address_Vals(){
    //This values are the commands to write to sensor consequently
    commandData[0] = 0xA9;
    commandData[1] = 0x5E;
    commandData[2] = 0xAA;
    commandData[3] = 0xA8;
    
    
    //Pressure sensor memory word addresses
    addr[0]  = 0x00;
    addr[1]  = 0x01;
    addr[2]  = 0x02;
    addr[3]  = 0x03;
    addr[4]  = 0x04;
    addr[5]  = 0x05;
    addr[6]  = 0x06;
    addr[7]  = 0x07;
    addr[8]  = 0x08;
    addr[9]  = 0x09;
    addr[10] = 0x0A;
    addr[11] = 0x0B;
    addr[12] = 0x0C;
    addr[13] = 0x0D;
    addr[14] = 0x0E;
    addr[15] = 0x0F;
    addr[16] = 0x10;
    addr[17] = 0x11;
    addr[18] = 0x12;
    addr[19] = 0x13;
    addr[20] = 0x14;
    addr[21] = 0x15;
    addr[22] = 0x16;

    
    //DELAY TIMER INIT
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TimIS;
    TIM_TimeBaseStructInit(&TimIS);
    TimIS.TIM_CounterMode = TIM_CounterMode_Up;
    TimIS.TIM_Period = 0xFFFFF; 
    TimIS.TIM_Prescaler = 16800;
    TIM_TimeBaseInit(TIM2, &TimIS);
    
    
    
    //Power pin
    //RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef gpio_init_structure;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_IN;
	gpio_init_structure.GPIO_OType = GPIO_OType_PP;
    gpio_init_structure.GPIO_Pin = GPIO_Pin_2;
    gpio_init_structure.GPIO_PuPd = GPIO_PuPd_DOWN;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &gpio_init_structure);
    GPIO_SetBits (GPIOB, GPIO_Pin_2);
}


void power_Up(){
     while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2));
}



void power_Down(){
     while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2));
}
uint8_t read_All_Memory(){
    uint8_t j=0, retVal=0;
    for(int i =0; i<23; i++){
        
        retVal=Read_Mem(addr[i]);
        if(!(retVal==0)){
            return retVal;
        }
        sensMemHigh[j]=I2C_Rx_Buffer[1];
        sensMemLow[j++]=I2C_Rx_Buffer[2];
        memset(I2C_Rx_Buffer, 55, sizeof(I2C_Rx_Buffer));
       
        delayTIM(300);
    }
    return retVal;
}

uint8_t enter_Command_Mode(){
     uint8_t retVal=0;
     I2C_Write(PRESS_CHIP_ADDR,1,&commandData[0]);
     return retVal;
}


uint8_t exit_Command_Mode(){
     uint8_t retVal=0;
     I2C_Write(PRESS_CHIP_ADDR,1,&commandData[3]);
     return retVal;
}


uint8_t read_Status(){
    uint8_t retVal=0;
    retVal=I2C_Read (PRESS_CHIP_ADDR,I2C_Rx_Buffer,1);
    return retVal;
}

uint8_t create_Checksum(){
    uint8_t retVal=0;
    retVal=I2C_Write (PRESS_CHIP_ADDR,1,&commandData[2]);
    return retVal;
}


uint8_t increment_Page_Counter(){
    uint8_t retVal=0;
    retVal=I2C_Write (PRESS_CHIP_ADDR,1,&commandData[1]);
    return retVal;
}


uint8_t check_Page_Counter(){
    uint8_t retVal=0;
    retVal=I2C_Write (PRESS_CHIP_ADDR,1,&addr[0]);
    delayTIM (300);
    retVal=I2C_Read (PRESS_CHIP_ADDR,I2C_Rx_Buffer,3);
    return retVal;
}

void delayTIM(uint16_t val){
    //TIM delay
    TIM_Cmd(TIM2, ENABLE);
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < val);
    TIM_Cmd(TIM2, DISABLE);
    //
}

//0x2F
void set_New_Address(){
   sensMemLow[2]=0x2F; 
}


uint8_t write_Mem(){
    uint8_t temp[3];
    uint8_t retVal;
    temp[0]=0x42;//Address of new addrByte
    temp[1]=0x00;
    temp[2]=sensMemLow[2];
    retVal=I2C_Write(PRESS_CHIP_ADDR,3,temp);
    return retVal;
}

uint8_t write_All_Mem(){
    uint8_t retVal=0;
    uint8_t temp[3];
    for(int i=0;i<23;i++){
        
        temp[0]=addr[i]+0x40;//Address of new addrByte
        temp[1]=sensMemHigh[i];
        temp[2]=sensMemLow[i];
        retVal=I2C_Write(PRESS_CHIP_ADDR,3,temp);
        
    }
    
}
