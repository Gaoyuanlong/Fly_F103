#ifndef __BLE_H__
#define __BLE_H__
#include "BSP.h"
#include <string.h>
#define BLE_CMD_NONE        255
#define BLE_WAIT_CONNECT    254
#define BLE_CONNECT_DOWN    253
#define BLE_ERR_FORMAT      252
#define BLE_RESET           0
#define BLE_CFG_BAUD_115200 1
#define BLE_CFG_BAUD_460800 2
#define BLE_CFG_BAUD_921600 3
#define BLE_CFG_RTS_CTS_EN  4
#define BLE_CFG_CPU_32_MHZ  5
#define BLE_CFG_CPU_64_MHZ  6
#define BLE_CFG_HCIB_921600 7
#define BLE_CFG_HCIB_2MHZ   8

typedef struct BLE_
{
	USART_* Usart;
}BLE_;


typedef struct BLE_OPS_
{
	void (*Init)(BLE_* Aim);
	BOOL (*Ctrl)(u8 Cmd,BOOL IsWait,BLE_* Aim);
	BOOL (*Send)(u8* BUF,u8 Cnt,BLE_* Aim);
	BOOL (*Recv)(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait,BLE_* Aim);
}BLE_OPS_;

extern BLE_OPS_ BLE_OPS;
#endif
