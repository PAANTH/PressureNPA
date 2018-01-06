#ifndef __NETWORK_H
#define __NETWORK_H
#include "core/net.h"
//#include "drivers/lan8720.h"

#define MAC_ADDR "00-AB-HF-EF-04-07"
#define IPV4_HOST_ADDR "192.168.1.4"
#define IPV4_SUBNET_MASK "255.255.255.0"
#define IPV4_DEFAULT_GATEWAY "192.168.1.1"

#define IPV4_PRIMARY_DNS "8.8.8.8"
#define IPV4_SECONDARY_DNS "8.8.4.4"


void reset_HW_PHY();
void init_Network(void);
void periodic_Toggle(void);
//extern IpAddr ipAddrRemote;

#endif
