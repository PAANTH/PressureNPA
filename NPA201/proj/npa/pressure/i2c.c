#include "stm32f4xx_i2c.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_tim.h"


#include "misc.h"
#include "i2c.h"
#include "string.h"

#define I2C_GPIO_PORT   GPIOB
#define I2C_SCL         GPIO_Pin_6
#define I2C_SDA         GPIO_Pin_9
#define I2C_SCL_AF      GPIO_PinSource6
#define I2C_SDA_AF      GPIO_PinSource9
#define I2C_GPIO_AF     GPIO_AF_I2C1
#define I2C_PORT_RCC    RCC_AHB1Periph_GPIOB //AHB1
#define I2C_RCC         RCC_APB1Periph_I2C1  //APB1
#define I2C_AF_GPIO     GPIO_AF_I2C1
#define I2C_NUMBER      I2C1
#define DMA_RCC         RCC_AHB1Periph_DMA1  //AHB1
#define DMA_STREAM      DMA1_Stream5
#define DMA_CHANNEL     DMA_Channel_1
#define DMA_IRQ         DMA1_Stream5_IRQn
//On i2c change do not forget to change rcc AHB1/2 and dma irq-handler

uint8_t I2C_Rx_Buffer[8];
DMA_InitTypeDef  DMA_InitStructure;
void config_I2C(){
	//Arbeit
    RCC_AHB1PeriphClockCmd(I2C_PORT_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(I2C_RCC , ENABLE);

	GPIO_InitTypeDef gpio_init_structure;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_structure.GPIO_OType = GPIO_OType_OD;
    gpio_init_structure.GPIO_Pin = I2C_SCL | I2C_SDA;
    gpio_init_structure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(I2C_GPIO_PORT, &gpio_init_structure);

   GPIO_PinAFConfig(I2C_GPIO_PORT , I2C_SCL_AF, I2C_AF_GPIO);
   GPIO_PinAFConfig(I2C_GPIO_PORT , I2C_SDA_AF, I2C_AF_GPIO);

	I2C_InitTypeDef i2c_init_structure;
	i2c_init_structure.I2C_Ack = I2C_Ack_Enable;
	i2c_init_structure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    i2c_init_structure.I2C_ClockSpeed = 100000;
	i2c_init_structure.I2C_DutyCycle = I2C_DutyCycle_2;
        i2c_init_structure.I2C_Mode =I2C_Mode_I2C;
	i2c_init_structure.I2C_OwnAddress1 = 0x01;
    I2C_Init(I2C_NUMBER, &i2c_init_structure);

    //I2C_AnalogFilterCmd(I2C_NUMBER,  DISABLE);


    //DMA config
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(DMA_RCC, ENABLE);
    DMA_DeInit(DMA_STREAM); //reset DMA1 channe1 to default values;

    DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(I2C_NUMBER->DR);  //I2C_DR_Address; //=0x40005410 : address of data reading register of I2C1
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)I2C_Rx_Buffer; //variable to store data
    //DMA_InitStructure. = DMA_M2M_Disable; //channel will be used for peripheral to memory transfer
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	//setting normal mode (non circular)
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	//medium priority
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	//Location assigned to peripheral register will be source
    DMA_InitStructure.DMA_BufferSize =5;	//number of data to be transfered
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //automatic memory increment disable for peripheral
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//automatic memory increment enable for memory
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//source peripheral data size = 8bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//destination memory data size = 8bit
    DMA_InitStructure.DMA_Channel = DMA_CHANNEL;
    DMA_Init(DMA_STREAM, &DMA_InitStructure);
    DMA_ITConfig(DMA_STREAM, DMA_IT_TC, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = DMA_IRQ; //I2C1 connect to channel 1 Stream 5 of DMA1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
   
    I2C_Cmd(I2C_NUMBER , ENABLE);


}
int j=0;
uint8_t I2C_DMA_Read(uint8_t slaveAddr){
    uint8_t state=0;
     while(I2C_GetFlagStatus(I2C_NUMBER , I2C_FLAG_BUSY)){
         if(j<8000){
             j++;
         }
         else {
             return state=1;
         }
     }
     I2C_DMALastTransferCmd(I2C_NUMBER , ENABLE);

     j=0;
     I2C_GenerateSTART(I2C_NUMBER ,ENABLE);
     while(!I2C_CheckEvent(I2C_NUMBER , I2C_EVENT_MASTER_MODE_SELECT)){
         if(j<8000){
             j++;
         }
         else {
             return state=2;
         }
     }
     j=0;
     I2C_Send7bitAddress(I2C_NUMBER ,slaveAddr,I2C_Direction_Receiver);
     while (!I2C_CheckEvent(I2C_NUMBER , I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
         if(j<8000){
             j++;
         }
         else {
             return state=4;
         }
     }
     j=0;

     DMA_Cmd(DMA_STREAM, ENABLE);
     I2C_DMACmd(I2C_NUMBER , ENABLE);

     return state;
}

/**
  * @brief  This is the method witch writes a desired number of bytes to a slave.
  * Data are taken from a given variable
  *
  * @param  slaveAddr - Address of a slave to communicate in HEX(must be already left aligned)
  *
  * @param  numOfBytes - Amount of bytes to write to slave
  * @param  *writeData - Where  bytes to write are taken from
  *
  * For some slaves is needed to activate register increment
  */
uint8_t I2C_Write(uint8_t slaveAddr, uint8_t numOfBytes,  uint8_t *writeData){
	uint8_t state=0;
		int i=0,j=0;
        while(I2C_GetFlagStatus(I2C_NUMBER , I2C_FLAG_BUSY)){
                        if(j<8000){
                j++;
            }
            else {
                return state=1;
            }
		}
	    j=0;
        I2C_GenerateSTART(I2C_NUMBER ,ENABLE);
        while(!I2C_CheckEvent(I2C_NUMBER , I2C_EVENT_MASTER_MODE_SELECT)){
                        if(j<8000){
				j++;
			}
			else {
                return state=2;
			}
		}
	    j=0;

        I2C_Send7bitAddress(I2C_NUMBER, slaveAddr,I2C_Direction_Transmitter);
        while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
                        if(j<8000){
				j++;
			}
			else {
                return state=3;
			}
		}
	    j=0;

         while(i< numOfBytes){

             I2C_SendData(I2C_NUMBER, writeData[i]);
             while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
                        if(j<80000){
                            j++;
                        }
                        else {
                            return state=4;
                        }
             }
             j=0;


             i++;
         }
         I2C_GenerateSTOP(I2C_NUMBER, ENABLE);
		 return state;
}




///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////HUMIDITY SPECIALS///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


///
/// \brief I2C_humidity_measurement_request call it 37ms before getting data
/// Slave addres is 0x4E (already left aligned)
///
/// \return
///
uint8_t I2C_humidity_measurement_request(){
    uint8_t state=0,slaveAddr=0x4E;

    while(I2C_GetFlagStatus(I2C_NUMBER,  I2C_FLAG_BUSY)){
        if(j<8000){
            j++;
        }
        else {
            return state=1;
        }
    }
    j=0;
    I2C_GenerateSTART(I2C_NUMBER, ENABLE);
    while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_MODE_SELECT)){
        if(j<8000){
            j++;
        }
        else {
            return state=1;
        }
    }
    j=0;

    I2C_Send7bitAddress(I2C_NUMBER, slaveAddr,I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
        if(j<8000){
            j++;
        }
        else {
            return state=1;
        }
    }
    j=0;


     I2C_GenerateSTOP(I2C_NUMBER, ENABLE);
     return state;


}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////READ BYTES NO DMA///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

uint8_t I2C_Read(uint8_t slaveAddr, uint8_t *receivedData, uint8_t _numOfBytes){
    uint8_t state=0,numOfBytes=_numOfBytes;
    
    if(numOfBytes==0){
        return state=5;
    }
   
    while(I2C_GetFlagStatus(I2C_NUMBER,  I2C_FLAG_BUSY)){
        if(j<8000){
            j++;
        }
        else {
            return state=1;
        }
    }
    j=0;
    I2C_GenerateSTART(I2C_NUMBER, ENABLE);
    while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_MODE_SELECT)){
        if(j<8000){
            j++;
        }
        else {
            return state=2;
        }
    }
    j=0;
    I2C_AcknowledgeConfig(I2C_NUMBER,  ENABLE);
    I2C_Send7bitAddress(I2C_NUMBER, slaveAddr,I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
        if(j<80000){
            j++;
        }
        else {
            return state=3;
        }
    }
    j=0;

    int i=0;
    if(numOfBytes==1){
        
        I2C_AcknowledgeConfig(I2C_NUMBER, DISABLE); 
        while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_BYTE_RECEIVED));
        I2C_NACKPositionConfig(I2C_NUMBER,  I2C_NACKPosition_Current);
        receivedData[i]=I2C_ReceiveData(I2C_NUMBER);
        
    }
    else{
        
        while(i< numOfBytes){
            while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_BYTE_RECEIVED));
             if(numOfBytes-i==2){
               I2C_AcknowledgeConfig(I2C_NUMBER, DISABLE);
               I2C_NACKPositionConfig(I2C_NUMBER,  I2C_NACKPosition_Next);
             }
             receivedData[i]=I2C_ReceiveData(I2C_NUMBER);
             i++;
         }
    }

    I2C_GenerateSTOP(I2C_NUMBER, ENABLE);
    return state;

}



/**
 * Read memory word from pressure sensor
 * @bts - is an address of the word we are gonna read
 * Puts results in to a I2C_Rx_Buffer
 */
uint8_t Read_Mem(uint8_t bts){
    uint8_t retVal=0;
    //Check page procedure
    retVal=I2C_Write (0x00,1,&bts);
    if(!(retVal==0)){
        return retVal;
    }
    //TIM delay
    TIM_Cmd(TIM2, ENABLE);
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < 300);
    TIM_Cmd(TIM2, DISABLE);
    ////
   
    ///
    retVal=I2C_Read (0x00,I2C_Rx_Buffer,3);
    if(!(retVal==0)){
        return retVal;
    }
    
    return retVal;
//    uint8_t state=0;
//     while(I2C_GetFlagStatus(I2C_NUMBER,  I2C_FLAG_BUSY)){
//         if(j<8000){
//             j++;
//         }
//         else {
//             return state=1;
//         }
//     }
//     j=0;
//     I2C_GenerateSTART(I2C_NUMBER, ENABLE);
//     while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_MODE_SELECT)){
//         if(j<8000){
//             j++;
//         }
//         else {
//             return state=1;
//         }
//     }
//     j=0;


//     /* Send press sensor address for write */
//     I2C_Send7bitAddress(I2C_NUMBER , 0x00, I2C_Direction_Transmitter);
//     j=0;
//     /* Test on EV6 and clear it */
//     while(!I2C_CheckEvent(I2C_NUMBER , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
//         if(j<8000){
//             j++;
//         }
//         else {
//             return state=1;
//         }
//     }

//     /* Clear EV6 by setting again the PE bit */
//    // I2C_Cmd(I2C_NUMBER , ENABLE);

//     /* Send  internal address to write to */

//     I2C_SendData(I2C1,bts);
//     while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
//         if(j<80000){
//             j++;
//         }
//         else {
//             return state=2;
//         }
//     }

//     //DELAY
//      //TIM delay
//             TIM_Cmd(TIM2, ENABLE);
//             TIM_SetCounter(TIM2, 0);
//             while (TIM_GetCounter(TIM2) < 300);
//             TIM_Cmd(TIM2, DISABLE);
//             ////
     
//     //Repeated start
//     I2C_GenerateSTART(I2C_NUMBER, ENABLE);
//     while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_MODE_SELECT)){
//         if(j<8000){
//             j++;
//         }
//         else {
//             return state=1;
//         }
//     }
//     j=0;

//     I2C_Send7bitAddress(I2C_NUMBER, 0x00,I2C_Direction_Receiver);
//     while (!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
//         if(j<80000){
//             j++;
//         }
//         else {
//             return state=1;
//         }
//     }
//     j=0;

//     DMA_Cmd(DMA_STREAM, ENABLE);
//     I2C_DMACmd(I2C_NUMBER , ENABLE);



//     int i=0;
//     I2C_AcknowledgeConfig(I2C_NUMBER, ENABLE);
//     while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_BYTE_RECEIVED));
//     I2C_Rx_Buffer[i++]=I2C_ReceiveData(I2C_NUMBER);

//     while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_BYTE_RECEIVED));
//     I2C_Rx_Buffer[i++]=I2C_ReceiveData(I2C_NUMBER);

//     I2C_AcknowledgeConfig(I2C_NUMBER, DISABLE);
//     while(!I2C_CheckEvent(I2C_NUMBER,  I2C_EVENT_MASTER_BYTE_RECEIVED));
//     I2C_NACKPositionConfig(I2C_NUMBER,  I2C_NACKPosition_Current);
//     I2C_Rx_Buffer[i]=I2C_ReceiveData(I2C_NUMBER);


//     I2C_GenerateSTOP(I2C_NUMBER, ENABLE);
     //return state;

}




///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////DMA INTERRUPT////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void DMA1_Stream5_IRQHandler(void)
{
    //i2c_dma_ready=1;//time_for_i2c=1;
    DMA_ClearITPendingBit(DMA_STREAM,DMA_IT_TC);
    DMA_ClearFlag(DMA_STREAM, DMA_FLAG_TCIF5);
    I2C_DMACmd(I2C_NUMBER,  DISABLE);
    /* Send I2C1 STOP Condition */
    I2C_GenerateSTOP(I2C_NUMBER,  ENABLE);
    /* Disable DMA channel*/
    DMA_Cmd(DMA1_Stream5, DISABLE);

}
















