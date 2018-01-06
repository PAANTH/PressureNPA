
//#include "math.h"
#include "i2c.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"
#include "string.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "pressure_sensor.h"

#define DEV_ADDR 0x00



  
      uint8_t offset = 0x20;
         uint8_t tempDat[3],j1=0, flag=0;



int main(void)
{
/////////////////////////////SENSORS////////////////////////////////// 
  //INIT I2C 
   

   config_I2C();
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
 
    

    
    while(1){

    }

}

