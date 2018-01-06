#ifndef __PWM_H
#define __PWM_H
#include "inttypes.h"

void send_data_rs(void);
void rs_init();
void change_data_rs();

typedef struct{
    uint16_t RawCurrent;
    uint16_t RawVoltage;
    uint16_t RawTemperature;
    uint16_t Reserved_0;
    float Temperature;
    float Amperage;
    float FilteredAmperage;
}TelemetryDriver_t;

typedef struct{
    uint32_t A5:1;
    uint32_t AN5:1;
    uint32_t A15:1;
    uint32_t AN5CO:1;
    uint32_t A15Forward:1;
    uint32_t A15Reverse:1;
    uint32_t AN5Forward:1;
    uint32_t AN5Reverse:1;
    uint32_t AForwardDriver:1;
    uint32_t AReverseDriver:1;
    uint32_t APwr:1;
    uint32_t Board27v:1;
    uint32_t Board5v:1;
    uint32_t SoftOvercurrentFD:1;
    uint32_t SoftOvercurrentRD:1;
    uint32_t SoftCommonOvercurrent:1;
}TelemetryErrorState_t;

typedef struct{
    uint16_t Counter;
    uint16_t RawPwr;
    TelemetryErrorState_t Errors;
    TelemetryDriver_t ForwardDriver;
    TelemetryDriver_t ReverseDriver;
    uint16_t RawInputAnalogControl;
    uint16_t Reserved_1;
    float Amperage;
    float RawControlVoltage;
    float CalculatedControlVoltage;
    float ControlVoltage;
    float FeedBackVoltage;
    int32_t RawOutputControl;
    float FilteredAmperage;
}TelemetryProtocol_t;


#endif
