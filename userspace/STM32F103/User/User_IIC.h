#ifndef _USER_IIC_H_
#define _USER_IIC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f10x_i2c.h"
#include "type.h"
	
typedef struct{
	BYTE iicaddr;
	BYTE regaddr;
	BYTE num;
	BYTE *pBuf;
}I2C_DATA_STRUCT;

void IIC1_Init(void);
void IIC2_Init(void);
BYTE IIC1_WriteBytes(BYTE iicaddr,BYTE regaddr,BYTE num,BYTE * pBuf);
BYTE IIC2_WriteBytes(BYTE iicaddr,BYTE regaddr,BYTE num,BYTE * pBuf);
BYTE IIC1_ReadBytes(BYTE iicaddr,BYTE regaddr,BYTE num,BYTE * pBuf);
BYTE IIC2_ReadBytes(BYTE iicaddr,BYTE regaddr,BYTE num,BYTE * pBuf);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
