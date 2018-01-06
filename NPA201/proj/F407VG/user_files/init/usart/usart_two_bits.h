#ifndef __USARTT_H
#define __USARTT_H
#include "inttypes.h"
#define NEWLINE "\r\n"

void usart_initt(unsigned int baudrate);
int  usart_msg(const char *str);
void usart_backspace(int count);
void send_stuff();
#endif
