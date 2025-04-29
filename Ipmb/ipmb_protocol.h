#ifndef __IPMB_PROTOCOL_H
#define __IPMB_PROTOCOL_H

#include <stdint.h>

void IPMB_Process_Command(uint8_t* rx_buf, uint8_t* tx_buf, uint8_t* tx_len);

#endif