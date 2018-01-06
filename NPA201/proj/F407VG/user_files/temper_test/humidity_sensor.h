#ifndef __H_SENSOR_H
#define __H_SENSOR_H
#include "stdint.h"


uint16_t get_humidity();
void humidity_start_conv();
extern uint8_t I2C_Rx_Buffer[8];
#endif
