#ifndef __P_SENSOR_H
#define __P_SENSOR_H
#include "stdint.h"

void config_Pressure();
uint32_t get_Pressure();
extern uint8_t I2C_Rx_Buffer[8];

void delayTIM(uint16_t val);
uint8_t enter_Command_Mode();
uint8_t exit_Command_Mode();
uint8_t read_Status();
uint8_t create_Checksum();
uint8_t increment_Page_Counter();
uint8_t check_Page_Counter();
uint8_t read_All_Memory();
uint8_t write_Mem();
uint8_t write_All_Mem();
void init_Address_Vals();
void power_Up();
void power_Down();
void set_New_Address();
#endif
