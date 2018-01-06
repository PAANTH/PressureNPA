#ifndef __UDP_F_H
#define __UDP_F_H
#include "stdint.h"
#include "core/net.h"

Socket* socketIni(IpAddr *ipToBind, uint16_t portToBind);
void initSockets(void);
Socket  *socket_receive, *socket_send;

#endif
