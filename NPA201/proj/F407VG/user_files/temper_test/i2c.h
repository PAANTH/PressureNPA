#ifndef __I2C_H
#define __I2C_H
#include "inttypes.h"
//#include "defs.h"
void config_I2C();
uint8_t I2C_Write(uint8_t slaveAddr,  uint8_t numOfBytes, uint8_t *writeData);
//uint8_t I2C_Read(uint8_t slaveAddr, uint8_t regAddr, uint8_t numOfBytes, uint8_t maxSize, uint8_t *receivedData);
uint8_t I2C_humidity_measurement_request();
uint8_t I2C_Read(uint8_t slaveAddr, uint8_t *receivedData, uint8_t _numOfBytes);
uint8_t I2C_Read_Pressure(uint8_t slaveAddr, uint8_t *receivedData);
uint8_t I2C_DMA_Read(uint8_t slaveAddr);
uint8_t Read_Mem(uint8_t bts);

extern uint8_t  i2c_dma_ready;
#endif
