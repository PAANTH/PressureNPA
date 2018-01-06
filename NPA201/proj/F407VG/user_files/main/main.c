#include "init_all.h"
//#include "math.h"
#include "i2c.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"
#include "string.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
//#include "temperature_sensor.h"
//#include "humidity_sensor.h"
#include "pressure_sensor.h"
//#include "udp_file.h"
//#include "network.h"
//#include "core/net.h"
//#include "rs.h"
#include "pwm.h"
#define DEV_ADDR 0x00

#include "usart_dma_sipa.h"

  
      uint8_t offset = 0x20;
         uint8_t tempDat[3],j1=0, flag=0;

void ch_adr(){
    //uint32_t t=0;
  //  t=I2C_Read(DEV_ADDR,dat,5);

//ADDR CHANGE PROCEDURE
   
//    //Enter command mode
//    t=I2C_Write(DEV_ADDR,1,&commandData[0]);//commandData);
// memset(dat, 55, sizeof(dat));
////    //Technical delay may be unnesessary
//    for(int i=0;i<10000;i++);
//    t=I2C_Read(DEV_ADDR,dat,1);
//      memset(dat, 55, sizeof(dat));
//     t=I2C_Read(DEV_ADDR,dat,5);
////    //Reading memory words cycle
//    memset(I2C_Rx_Buffer, 55, sizeof(I2C_Rx_Buffer));
////    for(int i =0; i<23; i++){
//         for(int i=0;i<10000;i++);
//        t=Read_Mem(addr[i]);
//        sensMem[j1++]=I2C_Rx_Buffer[1];
//        sensMem[j1++]=I2C_Rx_Buffer[2];
//        memset(I2C_Rx_Buffer, 55, sizeof(I2C_Rx_Buffer));
//    }

//    //Change address data
//    temp=sensMem[5];
//    temp=temp&0xC0;
//    temp=temp|0x2F;
//    sensMem[5]=temp;

//    //Increment memory page counter
//    t=I2C_Write(DEV_ADDR,1,&commandData[1]);

//    //Write data words back to the sensor
//    j1=0;
//    for(int i=0; i<23;i++){
//        tempDat[0]=addr[i]+offset;
//        tempDat[1]=sensMem[j1++];
//        tempDat[2]=sensMem[j1++];
//        t=I2C_Write(DEV_ADDR,3,tempDat);
//    }
//    t=I2C_Write(DEV_ADDR,1,&commandData[2]);
//    t=I2C_Write(DEV_ADDR,1,&commandData[3]);

//////////////////////////////////////
}

int main(void)
{
/////////////////////////////SENSORS////////////////////////////////// 
  //INIT I2C 
   

   init_all();
   init_Address_Vals ();
    
 
   memset (I2C_Rx_Buffer,5, sizeof(I2C_Rx_Buffer));
   power_Up ();
   delayTIM (1000);
  read_Status ();
  
//   power_Up ();
//   delayTIM (10);
//   enter_Command_Mode ();
//   delayTIM (500);
//   read_Status ();
//   memset (I2C_Rx_Buffer,7, sizeof(I2C_Rx_Buffer));
//   delayTIM (500);
//   read_All_Memory ();
//   delayTIM (500);
//   increment_Page_Counter ();
//   delay(500);
//   set_New_Address ();
//   write_All_Mem ();
//   delayTIM (500);
//   create_Checksum ();
//   delayTIM (500);
//   exit_Command_Mode ();
//   delayTIM (500);
//   read_Status ();
//   delayTIM (500);
   power_Down ();
////////////////////END SENSORS/////////////////////////////////////////////  
 
    
///////////////////SIPA GPS DMA_USART/////////////////////////////////////////    

    
    
    
    //usart_dma_sipa_init();
    
        
    
    while(1){
         //handle_usart_dma_sipa();
    }
//////////////////END SIPA GPS DMA_USART/////////////////////////////////////////
}

