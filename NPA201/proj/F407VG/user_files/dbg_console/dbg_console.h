#ifndef __DBG_CONSOLE_H
#define __DBG_CONSOLE_H
#include "inttypes.h"
#define NEWLINE "\r\n"

void dbg_console_init(unsigned int baudrate);
int  dbg_msg(const char *str);
void dbg_backspace(int count);
extern uint32_t counterFaultR;
#endif
