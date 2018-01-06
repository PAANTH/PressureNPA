#include "udp_file.h"


Socket* socketIni(IpAddr *ipToBind, uint16_t portToBind){
    Socket *socket_out;

    socket_out = socketOpen(SOCKET_TYPE_DGRAM, SOCKET_IP_PROTO_UDP);
    socketBind(socket_out, ipToBind, portToBind);
    return  socket_out;
}


void initSockets(void){

    IpAddr ipAddr_local;
    ipStringToAddr("192.168.1.3", &ipAddr_local);

    socket_receive=socketIni(&ipAddr_local,51000);
    socket_send=socketIni(&ipAddr_local,51001);
}
