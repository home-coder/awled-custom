#include "Application_main.h"
#include "Application_KEY.h"
#include "Chip_AW9163.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef APPLICATION_WISTRON_ENABLE

static const BYTE aw9163_chipid_reg = 0x00;
static const BYTE aw9163_gcr_reg = 0x01;
static const BYTE aw9163_slpr_reg = 0x02;
static const BYTE aw9163_kinter_reg = 0x03;

static const BYTE aw9163_aksr_reg = 0x07;

static const BYTE aw9163_thr0_reg = 0x0a;
static const BYTE aw9163_thr1_reg = 0x0b;
static const BYTE aw9163_thr2_reg = 0x0c;
static const BYTE aw9163_thr3_reg = 0x0d;
//static const BYTE aw9163_thr4_reg = 0x0e;
//static const BYTE aw9163_thr5_reg = 0x0f;

static const BYTE aw9163_scfg1_reg = 0x11;
static const BYTE aw9163_scfg2_reg = 0x12;
static const BYTE aw9163_ofr1_reg = 0x13;
static const BYTE aw9163_ofr2_reg = 0x14;
//static const BYTE aw9163_ofr3_reg = 0x15;

static const BYTE aw9163_mptr_reg = 0x19;
static const BYTE aw9163_setcnt_reg = 0x1b;
static const BYTE aw9163_blcth_reg = 0x1c;

//static const BYTE aw9163_mcr_reg = 0x1e;
static const BYTE aw9163_rawst_reg = 0x30;
static const BYTE aw9163_kisr_reg = 0x32;

static const WORD aw9163_chipid_value = 0xb223;
static const WORD aw9163_softreset_value = 0x55aa;
static const WORD aw9163_gcr_dis_value = 0x0000;
static const WORD aw9163_gcr_enkey_value = 0x0002;	// Touch Mode
static const WORD aw9163_slpr_value = 0x0030; 	// touch key enable S1 - S4 only
static const WORD aw9163_scfg1_value = 0x0084;	// scan time setting
static const WORD aw9163_scfg2_value = 0x028f;	// bit0~3 is sense seting

static const WORD aw9163_ofr1_value = 0x0000;	// offset S1, S2
static const WORD aw9163_ofr2_value = 0x0000;	// offset S3, S4
//static const WORD aw9163_ofr3_value = 0x0000;	// offset S5, S6

static const WORD aw9163_thr0_value = 0x5064;	//S1 press thred setting
static const WORD aw9163_thr1_value = 0x5064;	//S2 press thred setting
static const WORD aw9163_thr2_value = 0x5064;	//S3 press thred setting
static const WORD aw9163_thr3_value = 0x5064;	//S4 press thred setting
//static const WORD aw9163_thr4_value = 0x0a0c;	//S5 press thred setting
//static const WORD aw9163_thr5_value = 0x0a0c;	//S6 press thred setting

static const WORD aw9163_mptr_value = 0x0010;	// MPTR
static const WORD aw9163_setcnt_value = 0x0202;	// debounce
static const WORD aw9163_blcth_value = 0x0402;	// base trace rate

static const WORD aw9163_aksr_value = 0x0000;	// AKS 
static const WORD aw9163_kinter_value = 0x000f;	// signel click interrupt

//static const WORD aw9163_mcr_value = 0x0003;	// rawdata of each channel

static void aw9163_read_register(BYTE aw9163_id, BYTE reg_addr, BYTE reg_num, WORD *p_value)
{
	AWCHIP_REGS_ACCESS_STRUCT reg_access_data;
	WORD reg_len = 0;
	BYTE *p_regs_buf = NULL;

	if ( aw9163_id >= AWCHIP_KEY_START 
		&& aw9163_id < AWCHIP_KEY_END )
	{
		reg_len = reg_num << 1; // 16 bits per value register
		// if data buffer is NULL, malloc a buffer for reading
		if ( NULL == p_value )
		{
			p_regs_buf = (BYTE *)Application_malloc(reg_len);
			if ( NULL != p_regs_buf )
			{
				memset( (void *)p_regs_buf, 0, reg_len );
			}
		}
		else
		{
			p_regs_buf = (BYTE *)p_value;
		}
		
		reg_access_data.device_commu = key_get_commu_group( aw9163_id );
		reg_access_data.device_addr = key_get_i2c_address( aw9163_id );
		reg_access_data.base_reg_addr = reg_addr;
		reg_access_data.regs_num = reg_num;

		APPLICATION_ReadRegisters( EVENT_ID_AWCHIP_REG_READ_16BIT, &reg_access_data, p_regs_buf );
		
		// if data buffer is NULL, 
		// we just read it to new buffer and will not use it.
		// sometimes we can use jtag to check the registers for debug
		if ( NULL == p_value )
		{
			Application_free( p_regs_buf );
			p_regs_buf = NULL;
		}
		else
		{
			memcpy( (void *)p_value, (const void *)p_regs_buf, reg_len );
		}
	}
}

BOOL aw9163_check_chipid(BYTE aw9163_id)
{
	BOOL re = FALSE;
	WORD chip_id = 0;

	aw9163_read_register( aw9163_id, aw9163_chipid_reg, 1, &chip_id );
	if ( aw9163_chipid_value == chip_id )
	{
		re = TRUE;
	}
	
	return re;
}

void aw9163_get_regs_cfg(BYTE aw9163_id)
{
	aw9163_read_register( aw9163_id, aw9163_chipid_reg, 0x1d, NULL );
}

// change Little-Endian to Big-Endian
static BYTE * aw9163_word_to_bytes(WORD value)
{
	static BYTE regs_value[2];

	regs_value[0] = (BYTE)((value & 0xFF00)>>8);
	regs_value[1] = (BYTE)(value & 0x00FF);

	return regs_value;
}

static void aw9163_write_register(BYTE aw9163_id, BYTE reg_addr, WORD value)
{
	AWCHIP_REGS_ACCESS_STRUCT reg_access_data;

	if ( aw9163_id >= AWCHIP_KEY_START 
		&& aw9163_id < AWCHIP_KEY_END )
	{
		reg_access_data.device_commu = key_get_commu_group( aw9163_id );
		reg_access_data.device_addr = key_get_i2c_address( aw9163_id );
		reg_access_data.base_reg_addr = reg_addr;
		reg_access_data.regs_num = 1;

		APPLICATION_WriteRegisters( EVENT_ID_AWCHIP_REG_WRITE_16BIT, 
							&reg_access_data, aw9163_word_to_bytes(value) );
	}
}

void aw9163_regs_init_key(BYTE aw9163_id)
{
	// disable chip
	aw9163_write_register( aw9163_id, aw9163_gcr_reg, aw9163_gcr_dis_value );

	// touch key enable
	aw9163_write_register( aw9163_id, aw9163_slpr_reg, aw9163_slpr_value );

	// scan time setting
	aw9163_write_register( aw9163_id, aw9163_scfg1_reg, aw9163_scfg1_value );
	// bit0~3 is sense setting
	aw9163_write_register( aw9163_id, aw9163_scfg2_reg, aw9163_scfg2_value );

	// offset S1 - S4
	aw9163_write_register( aw9163_id, aw9163_ofr1_reg, aw9163_ofr1_value );
	aw9163_write_register( aw9163_id, aw9163_ofr2_reg, aw9163_ofr2_value );

	// press thred setting S1 - S4
	aw9163_write_register( aw9163_id, aw9163_thr0_reg, aw9163_thr0_value );
	aw9163_write_register( aw9163_id, aw9163_thr1_reg, aw9163_thr1_value );
	aw9163_write_register( aw9163_id, aw9163_thr2_reg, aw9163_thr2_value );
	aw9163_write_register( aw9163_id, aw9163_thr3_reg, aw9163_thr3_value );

	//press max time reset base
	aw9163_write_register( aw9163_id, aw9163_mptr_reg, aw9163_mptr_value );
	// debounce
	aw9163_write_register( aw9163_id, aw9163_setcnt_reg, aw9163_setcnt_value );
	// base trace rate : increment 8 or decrement 16 in 1s
	aw9163_write_register( aw9163_id, aw9163_blcth_reg, aw9163_blcth_value );

	// AKS 
	aw9163_write_register( aw9163_id, aw9163_aksr_reg, aw9163_aksr_value );

	// signle click interrupt 
	aw9163_write_register( aw9163_id, aw9163_kinter_reg, aw9163_kinter_value );

	// mcr configure for rawdata
	//aw9163_write_register( aw9163_id, aw9163_mcr_reg, aw9163_mcr_value );

	// enable chip
	aw9163_write_register( aw9163_id, aw9163_gcr_reg, aw9163_gcr_enkey_value );
}

void aw9163_enter_standby_state(BYTE aw9163_id)
{
	// disable chip
	aw9163_write_register( aw9163_id, aw9163_gcr_reg, aw9163_gcr_dis_value );
}

void aw9163_softreset(BYTE aw9163_id)
{
	aw9163_write_register( aw9163_id, aw9163_chipid_reg, aw9163_softreset_value );
}

void aw9163_get_key_interrupt(BYTE aw9163_id, BYTE key_nums, BYTE *p_kisr, BYTE *p_rawst)
{
	WORD kisr = 0x0000;
	WORD rawst = 0x0000;
	BYTE i = 0;

	if ( key_nums > 0 
		&& NULL != p_kisr && NULL != p_rawst )
	{
		// read the key isr
		aw9163_read_register( aw9163_id, aw9163_kisr_reg, 1, &kisr );
		// read the key raw data
		aw9163_read_register( aw9163_id, aw9163_rawst_reg, 1, &rawst );

		for ( i = 0; i < key_nums; i++ )
		{
			p_kisr[i] = (kisr>>i)&0x0001;
			p_rawst[i] = (rawst>>i)&0x0001;
		}
	}
}

#endif /* APPLICATION_WISTRON_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
