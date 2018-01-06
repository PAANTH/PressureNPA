#include "network.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "udp_file.h"
#include "os_port.h"

#include "drivers/stm32f4x7_eth.h"
//#include "drivers/ksz8081.h"
#include "drivers/lan8720.h"
#include "inttypes.h"
#define phy_rst_clk_config() RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define PIN_RST 	GPIO_Pin_8
#define PORT_RST 	GPIOA

void reset_HW_PHY(){
    GPIO_InitTypeDef gpio_init_struct;

    phy_rst_clk_config();

    //Configure PA8 (PHY_RST) pin as an output
    gpio_init_struct.GPIO_Pin = PIN_RST;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init_struct.GPIO_OType = GPIO_OType_PP;
    gpio_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_RST, &gpio_init_struct);

    //Reset PHY transceiver (hard reset)
    GPIO_ResetBits(PORT_RST, PIN_RST);
    sleep(10);
    GPIO_SetBits(PORT_RST, PIN_RST);
    sleep(10);
}
void init_Network(void){
    Ipv4Addr ipv4Addr;
    MacAddr macAddr;
    NetInterface *interface;

    //phy_hw_reset();

    netInit();

    interface = &netInterface[0];

    netSetInterfaceName(interface, "eth0");
    netSetHostname(interface, "SIPA-01");
    netSetDriver(interface, &stm32f4x7EthDriver);

    netSetPhyDriver(interface, &lan8720PhyDriver);//&ksz8081PhyDriver);//

    macStringToAddr(MAC_ADDR, &macAddr);
    netSetMacAddr(interface, &macAddr);

    netConfigInterface(interface);

    ipv4StringToAddr(IPV4_HOST_ADDR, &ipv4Addr);
    ipv4SetHostAddr(interface, ipv4Addr);

    ipv4StringToAddr(IPV4_SUBNET_MASK, &ipv4Addr);
    ipv4SetSubnetMask(interface, ipv4Addr);

    ipv4StringToAddr(IPV4_DEFAULT_GATEWAY, &ipv4Addr);
    ipv4SetDefaultGateway(interface, ipv4Addr);

    ipv4StringToAddr(IPV4_PRIMARY_DNS, &ipv4Addr);
    ipv4SetDnsServer(interface, 0, ipv4Addr);
    ipv4StringToAddr(IPV4_SECONDARY_DNS, &ipv4Addr);
    ipv4SetDnsServer(interface, 1, ipv4Addr);

    //init sockets
    initSockets();
    //ipStringToAddr("192.168.1.255", &ipAddrRemote);
}

void  periodic_Toggle(void){
 netTask();
}
