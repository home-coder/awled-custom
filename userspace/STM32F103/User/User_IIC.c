#include "User_IIC.h"
#include "UserProgram.h"

//*****************************************
//������IIC��ʼ��
//������I2Cx
//����ֵ����
//*****************************************
static void IIC_Init(I2C_TypeDef* I2Cx)
{
	I2C_InitTypeDef I2C_InitStructure;
	
	assert_param( NULL != I2Cx );
	
	// I2Cx RCC Interface Reset
	I2C_DeInit( I2Cx );

	// I2Cx Module Reset
	I2C_SoftwareResetCmd(I2Cx, ENABLE);
	I2C_SoftwareResetCmd(I2Cx, DISABLE);
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	
	I2C_Init(I2Cx,&I2C_InitStructure);
	
	I2C_Cmd(I2Cx,ENABLE);
}

//*****************************************
//������IIC1��ʼ��
//��������
//����ֵ����
//*****************************************
void IIC1_Init(void)
{
	IIC_Init(I2C1);
}

//*****************************************
//������IIC2��ʼ��
//��������
//����ֵ����
//*****************************************
void IIC2_Init(void)
{
	IIC_Init(I2C2);
}


//*****************************************
//������IICд�ֽ�
//������iicaddr -- IIC�ӻ���ַ
//		regaddr -- ����д�Ĵ�����ʼ��ַ
//		num		-- ����д�Ĵ������ݸ���
//		pBuf	-- ����д�Ĵ������ݴ������
//����ֵ���Ƿ���ȷ��������
//		0 -- �������ݳ���
//		1 -- �������ݳɹ�
//*****************************************
static BYTE IIC_WriteBytes(I2C_TypeDef* I2Cx, I2C_DATA_STRUCT *p_i2c_data)
{
	uint8 i = 0;
	uint16 time = 0x500;
	
	assert_param( NULL != I2Cx );
	assert_param( NULL != p_i2c_data );
	assert_param( NULL != p_i2c_data->pBuf );
	
	while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY) == SET)
	{
		if((time--) == 0)
		{
			return 0;
		}	
	}		
	
	I2C_GenerateSTART(I2Cx,ENABLE);
	time = 0x500;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) == RESET)
	{
		if((time--) == 0)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return 0;
		}	
	}
	I2C_ReadRegister(I2Cx,I2C_Register_SR1);
	I2C_SendData(I2Cx,(p_i2c_data->iicaddr << 1));
	
	time = 0x500;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) == RESET)
	{
		if((time--) == 0)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return 0;
		}	
	}
	I2C_ReadRegister(I2Cx,I2C_Register_SR1);
	I2C_ReadRegister(I2Cx,I2C_Register_SR2);
		
	I2C_SendData(I2Cx,p_i2c_data->regaddr);
	time = 0x500;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == RESET)
	{
		if((time--) == 0)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return 0;
		}	
	}
	
	for(i = 0;i < p_i2c_data->num;i++)
	{
		I2C_SendData(I2Cx,p_i2c_data->pBuf[i]);
		time = 0x500;
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == RESET)
		{
			if((time--) == 0)
			{
				I2C_GenerateSTOP(I2Cx,ENABLE);
				return 0;
			}	
		}

	}
	I2C_GenerateSTOP(I2Cx,ENABLE);
	return 1;
}

//*****************************************
//������IIC1д�ֽ�
//������iicaddr -- IIC�ӻ���ַ
//		regaddr -- ����д�Ĵ�����ʼ��ַ
//		num		-- ����д�Ĵ������ݸ���
//		pBuf	-- ����д�Ĵ������ݴ������
//����ֵ���Ƿ���ȷ��������
//		0 -- �������ݳ���
//		1 -- �������ݳɹ�
//*****************************************
BYTE IIC1_WriteBytes(BYTE iicaddr,BYTE regaddr,BYTE num,BYTE * pBuf)
{
	I2C_TypeDef* I2Cx = I2C1;
	I2C_DATA_STRUCT i2c_data;
	BYTE re = 0;
	
	assert_param( NULL != pBuf );
	
	i2c_data.iicaddr = iicaddr;
	i2c_data.regaddr = regaddr;
	i2c_data.num = num;
	i2c_data.pBuf = pBuf;
	
	re = IIC_WriteBytes( I2Cx, &i2c_data );
	if ( 0 == re )
	{
		// I2C initialize again
		IIC_Init( I2Cx );
		// Read one more time
		re = IIC_WriteBytes( I2Cx, &i2c_data );
	}
	
	return re;
}

//*****************************************
//������IIC2д�ֽ�
//������iicaddr -- IIC�ӻ���ַ
//		regaddr -- ����д�Ĵ�����ʼ��ַ
//		num		-- ����д�Ĵ������ݸ���
//		pBuf	-- ����д�Ĵ������ݴ������
//����ֵ���Ƿ���ȷ��������
//		0 -- �������ݳ���
//		1 -- �������ݳɹ�
//*****************************************
BYTE IIC2_WriteBytes(BYTE iicaddr,BYTE regaddr,BYTE num,BYTE * pBuf)
{
	I2C_TypeDef* I2Cx = I2C2;
	I2C_DATA_STRUCT i2c_data;
	BYTE re = 0;
	
	assert_param( NULL != pBuf );
	
	i2c_data.iicaddr = iicaddr;
	i2c_data.regaddr = regaddr;
	i2c_data.num = num;
	i2c_data.pBuf = pBuf;
	
	re = IIC_WriteBytes( I2Cx, &i2c_data );
	if ( 0 == re )
	{
		// I2C initialize again
		IIC_Init( I2Cx );
		// Read one more time
		re = IIC_WriteBytes( I2Cx, &i2c_data );
	}
	
	return re;
}


//*****************************************
//������IIC���ֽ�
//������iicaddr -- IIC�ӻ���ַ
//		regaddr -- �������Ĵ�����ʼ��ַ
//		num		-- �������Ĵ������ݸ���
//		pBuf	-- �������Ĵ������ݴ������
//����ֵ���Ƿ���ȷ��ȡ����
//		0 -- �������ݳ���
//		1 -- �������ݳɹ�
//*****************************************
static BYTE IIC_ReadBytes(I2C_TypeDef* I2Cx, I2C_DATA_STRUCT *p_i2c_data)
{
	uint8 i = 0;
	uint16 time = 0x500;
	
	assert_param( NULL != I2Cx );
	assert_param( NULL != p_i2c_data );
	assert_param( NULL != p_i2c_data->pBuf );	
	
	while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY) == SET)
	{
		if((time--) == 0)
		{
			return 0;
		}	
	}	
	
	I2C_GenerateSTART(I2Cx,ENABLE);
	time = 0x500;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) == RESET)
	{
		if((time--) == 0)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return 0;
		}	
	}	
	
	I2C_ReadRegister(I2Cx,I2C_Register_SR1);
	I2C_SendData(I2Cx,(p_i2c_data->iicaddr << 1));
	time = 0x500;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) == RESET)
	{
		if((time--) == 0)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return 0;
		}	
	}
	
	I2C_ReadRegister(I2Cx,I2C_Register_SR1);
	I2C_ReadRegister(I2Cx,I2C_Register_SR2);
	
	I2C_SendData(I2Cx,p_i2c_data->regaddr);
	time = 0x500;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == RESET)
	{
		if((time--) == 0)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return 0;
		}	
	}
	I2C_GenerateSTOP(I2Cx,ENABLE);
	
	
	I2C_GenerateSTART(I2Cx,ENABLE);					//��ʼ����
	time = 0x500;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) == RESET)
	{
		if((time--) == 0)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return 0;
		}	
	}
	
	I2C_ReadRegister(I2Cx,I2C_Register_SR1);
	I2C_SendData(I2Cx,(p_i2c_data->iicaddr << 1)+1);
	time = 0x500;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) == RESET)
	{
		if((time--) == 0)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return 0;
		}	
	}
	
	I2C_ReadRegister(I2Cx,I2C_Register_SR1);
	I2C_ReadRegister(I2Cx,I2C_Register_SR2);

	for(i = 0;i < p_i2c_data->num - 1;i++)
	{
		time = 0x500;
		while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_RXNE) == RESET)
		{
			if((time--) == 0)
			{
				I2C_GenerateSTOP(I2Cx,ENABLE);
				return 0;
			}	
		}
		p_i2c_data->pBuf[i] = I2C_ReceiveData(I2Cx);
	}
		
	I2C_AcknowledgeConfig(I2Cx,DISABLE);
	I2C_GenerateSTOP(I2Cx,ENABLE);
	
	time = 0x500;
	while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_RXNE) == RESET)
	{
		if((time--) == 0)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			I2C_AcknowledgeConfig(I2Cx,ENABLE);
			return 0;
		}	
	}
	p_i2c_data->pBuf[p_i2c_data->num-1] = I2C_ReceiveData(I2Cx);
	//I2C_GenerateSTOP(I2Cx,ENABLE);
	
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
	return 1;
}


//*****************************************
//������IIC1���ֽ�
//������iicaddr -- IIC�ӻ���ַ
//		regaddr -- �������Ĵ�����ʼ��ַ
//		num		-- �������Ĵ������ݸ���
//		pBuf	-- �������Ĵ������ݴ������
//����ֵ���Ƿ���ȷ��ȡ����
//		0 -- �������ݳ���
//		1 -- �������ݳɹ�
//*****************************************
BYTE IIC1_ReadBytes(BYTE iicaddr,BYTE regaddr,BYTE num,BYTE * pBuf)
{
	I2C_TypeDef* I2Cx = I2C1;
	I2C_DATA_STRUCT i2c_data;
	BYTE re = 0;
	
	assert_param( NULL != pBuf );
	
	i2c_data.iicaddr = iicaddr;
	i2c_data.regaddr = regaddr;
	i2c_data.num = num;
	i2c_data.pBuf = pBuf;
	
	re = IIC_ReadBytes( I2Cx, &i2c_data );
	if ( 0 == re )
	{
		// I2C initialize again
		IIC_Init( I2Cx );
		// Read one more time
		re = IIC_ReadBytes( I2Cx, &i2c_data );
	}
	
	return re;
}

//*****************************************
//������IIC2���ֽ�
//������iicaddr -- IIC�ӻ���ַ
//		regaddr -- �������Ĵ�����ʼ��ַ
//		num		-- �������Ĵ������ݸ���
//		pBuf	-- �������Ĵ������ݴ������
//����ֵ���Ƿ���ȷ��ȡ����
//		0 -- �������ݳ���
//		1 -- �������ݳɹ�
//*****************************************
BYTE IIC2_ReadBytes(BYTE iicaddr,BYTE regaddr,BYTE num,BYTE * pBuf)
{
	I2C_TypeDef* I2Cx = I2C2;
	I2C_DATA_STRUCT i2c_data;
	BYTE re = 0;
	
	assert_param( NULL != pBuf );
	
	i2c_data.iicaddr = iicaddr;
	i2c_data.regaddr = regaddr;
	i2c_data.num = num;
	i2c_data.pBuf = pBuf;
	
	re = IIC_ReadBytes( I2Cx, &i2c_data );
	if ( 0 == re )
	{
		// I2C initialize again
		IIC_Init( I2Cx );
		// Read one more time
		re = IIC_ReadBytes( I2Cx, &i2c_data );
	}
	
	return re;
}
