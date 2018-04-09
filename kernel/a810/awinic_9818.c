/*************************************************************************
  > Filename   : awinic_9818.c
  > Author     : oneface - one_face@sina.com
  > Company    : 一尊还酹江月
  > Time       : 2018-04-08 13:34:50
 ************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/rtc.h>
#include <asm/gpio.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/semaphore.h>
#include <linux/sched.h>
#include <linux/string.h>

#define AW9818_DEBUG

#ifdef AW9818_DEBUG
#define AW9818_DEBUGP(...)        printk(__VA_ARGS__)
#else
#define AW9818_DEBUGP(...)
#endif

#define DEV_MAJOR   0		// meaning major is not set
#define DEV_MINOR   0		//meaning set minor to be 0
#define DEV_NAME    "xlj-aw9818"

#define AW9818_CHANNEL	2	//meaning use i2c-2, other i2c-0 i2c-1
#define LED_CHIP_NUMS   2	//2 aw9818 chips

struct aw9818_priv {
	struct i2c_client *i2c_cli[LED_CHIP_NUMS];	//i2c_cli[0]-->aw9818_1, i2c_cli[1]-->aw9818_2
	struct cdev cdev;
	dev_t devno;
	struct class *class;
	struct device *device;
	struct workqueue_struct *wqueue;
	struct work_struct work_startup;
};

struct aw9818_priv *g_aw9818 = NULL;

typedef unsigned char byte;
#define LED_CHIP_NUMS	2

#define IOCTL
#ifdef IOCTL //same as app
typedef enum {
	LEDS_EFFECT_NONE = 0,
	LEDS_EFFECT_STARTUP,	/*board power on */
	LEDS_EFFECT_COMPLETE,	/*board startup over */
	LEDS_EFFECT_AIRKISS_MODE,	/*in airkiss mode */
	LEDS_EFFECT_AIRKISS_CONFIG,	/*config airkiss */
	LEDS_EFFECT_AIRKISS_CONNECT,	/*airkiss connected */
	LEDS_EFFECT_WAKE_UP,	/*voice wake up */
	LEDS_EFFECT_COMMAND_FAIL,	/*voice command failed */
	LEDS_EFFECT_COMMAND_SUCCESS,	/*voice command success */
	LEDS_EFFECT_KEYMUTE,	/*key mute record */
	LEDS_EFFECT_KEYUNMUTE,	/*key cancel mute record */
	LEDS_EFFECT_BRIGHT_CHANGE,
	LEDS_EFFECT_COLOR_CHANGE,
	LEDS_EFFECT_IMAX_CHANGE,
	LEDS_EFFECT_TOTAL
} APP_LED_EFFECT_ENUM;

#define AW9818_IOC_MAGIC 'm'	//定义类型
#define AW9818_LEDS_EFFECT_COMPLETE _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_COMPLETE, int)
#endif

static byte effect_complete = 1;

//bright value
typedef struct {
	byte r;
	byte g;
	byte b;
} ledreg_data;

//color level
typedef enum {
	LED_COLOR_NONE = 0,
	LED_COLOR_HALF,
	LED_COLOR_FULL,
} LED_COLOR_LEVEL;

//--think
typedef struct {
	LED_COLOR_LEVEL r;
	LED_COLOR_LEVEL g;
	LED_COLOR_LEVEL b;
} ledcolor_info;

typedef struct {
	byte led_nums;
	ledreg_data *p_ledreg_data;
	ledcolor_info *p_ledcolor_info;
} ledif_info;

//hw--
typedef enum {
	AW9818_1 = 0,
	AW9818_2,
} CHIP_ID;

typedef struct {
	CHIP_ID chip_id;
	byte led_nums;
	const ledreg_data *ledreg_map;
} ledhw_info;
ledif_info *led_intf = NULL;

typedef enum {
	ELECTRIC_10mA = (0 << 4),
	ELECTRIC_20mA = (1 << 4),
	ELECTRIC_30mA = (2 << 4),
	ELECTRIC_40mA = (3 << 4),
} ELECTRIC;

#define AW981X_DEFAULT_IMAX    (ELECTRIC_40mA)

static const byte aw981x_sleep_reg = 0x01;
static const byte aw981x_reset_reg = 0x02;
static const byte aw981x_config_reg = 0x03;
static const byte aw981x_update_reg = 0x04;

static const byte aw981x_en_value = 0x00;
static const byte aw981x_dis_value = 0x80;

static const byte aw981x_reset_value = 0x01;

// Set default-imax to 10mA, and select led matrix mode
static const byte aw981x_config_value = (0x02 | AW981X_DEFAULT_IMAX);

static const byte aw981x_update_value = 0x01;

static const byte gamma_brightness[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//9
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//19
	1, 1, 1, 1, 2, 2, 2, 3,	//27
	3, 3, 4, 4, 4, 5, 5, 5,	//35
	6, 6, 7, 7, 8, 9, 10, 11,	//43
	11, 12, 13, 14, 15, 16, 17, 18,	//51
	19, 20, 21, 22, 23, 24, 26, 27,	//59
	28, 29, 31, 32, 33, 35, 36, 38,	//67
	39, 41, 42, 44, 45, 47, 48, 50,	//75
	52, 54, 55, 57, 59, 61, 62, 63,	//83
};

static const byte led_bright_level[] = {
	10, 26, 42, 52, 60, 82,
};

typedef enum {
	none = 0,
	red,
	orange,
	yellow,
	green,
	blue,
	indigo,
	purple,
	LED_MAX_COLOR
} LED_COLOR;

typedef struct {
	byte cur_idx;
	byte bright_level;
	ledcolor_info background;
	ledcolor_info foward;
} ledeffect_info;
ledeffect_info *led_effect = NULL;

const ledcolor_info led_colors[LED_MAX_COLOR] = {
	{LED_COLOR_NONE, LED_COLOR_NONE, LED_COLOR_NONE},	// none
	{LED_COLOR_FULL, LED_COLOR_NONE, LED_COLOR_NONE},	// red
	{LED_COLOR_FULL, LED_COLOR_HALF, LED_COLOR_NONE},	// orange
	{LED_COLOR_FULL, LED_COLOR_FULL, LED_COLOR_NONE},	// yellow
	{LED_COLOR_NONE, LED_COLOR_FULL, LED_COLOR_NONE},	// green
	{LED_COLOR_NONE, LED_COLOR_NONE, LED_COLOR_FULL},	// blue
	{LED_COLOR_NONE, LED_COLOR_FULL, LED_COLOR_FULL},	//indigo
	{LED_COLOR_FULL, LED_COLOR_NONE, LED_COLOR_FULL},	//purple
};

const ledreg_data ledreg_map_9818_1[] = {
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
	{0x1, 0x2, 0x3},
	{0x5, 0x6, 0x7},
};

const ledreg_data ledreg_map_9818_2[] = {
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x0, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
	{0x9, 0x8, 0x7},
	{0x5, 0x4, 0x3},
};

const ledhw_info ledhw_info_struct[LED_CHIP_NUMS] = {
	{
	 AW9818_1,
	 18,
	 ledreg_map_9818_1,
	 },
	{
	 AW9818_2,
	 18,
	 ledreg_map_9818_2,
	 },
};

static const ledhw_info *get_ledhw_info(byte i)
{
	return &ledhw_info_struct[i];
}

static byte get_led_nums(byte i)
{
	const ledhw_info *p_ledhw_info = get_ledhw_info(i);
	return p_ledhw_info->led_nums;
}

static void set_ledif_info(ledif_info * ledif_info_struct)
{
	led_intf = ledif_info_struct;
}

static void ledif_info_init(void)
{
	byte i, len;

	ledif_info *ledif_info_struct = (ledif_info *) kmalloc(sizeof(ledif_info) * LED_CHIP_NUMS, GFP_KERNEL);
	if (NULL != ledif_info_struct) {
		for (i = 0; i < LED_CHIP_NUMS; i++) {
			ledif_info_struct[i].led_nums = get_led_nums(i);
			len = sizeof(ledreg_data) * (ledif_info_struct[i].led_nums);
			ledif_info_struct[i].p_ledreg_data = (ledreg_data *) kmalloc(len, GFP_KERNEL);
			if (NULL != ledif_info_struct[i].p_ledreg_data) {
				memset((void *)ledif_info_struct[i].p_ledreg_data, 0, len);
			} else {
				pr_err("ledif_info_struct[%d].p_ledreg_data is NULL\n", i);
			}
			len = sizeof(ledcolor_info) * (ledif_info_struct[i].led_nums);
			ledif_info_struct[i].p_ledcolor_info = (ledcolor_info *) kmalloc(len, GFP_KERNEL);
			if (NULL != ledif_info_struct[i].p_ledcolor_info) {
				memset((void *)ledif_info_struct[i].p_ledcolor_info, 0, len);
			} else {
				pr_err("ledif_info_struct[%d].p_ledcolor_info is NULL\n", i);
			}
		}
	}

	set_ledif_info(ledif_info_struct);
}

static void set_ledeffect_info(ledeffect_info * ledeffect_info_struct)
{
	led_effect = ledeffect_info_struct;
}

static ledeffect_info *get_led_effect(void)
{
	return led_effect;
}

static void ledeffect_info_init(void)
{
	ledeffect_info *ledeffect_info_struct = NULL;

	ledeffect_info_struct = (ledeffect_info *) kmalloc(sizeof(ledeffect_info), GFP_KERNEL);
	if (ledeffect_info_struct) {
		ledeffect_info_struct->cur_idx = 0;
		ledeffect_info_struct->background = led_colors[none];
		ledeffect_info_struct->foward = led_colors[none];
		ledeffect_info_struct->bright_level = led_bright_level[0];
		set_ledeffect_info(ledeffect_info_struct);
	}

	set_ledeffect_info(ledeffect_info_struct);
}

static int aw9818_read(u8 reg, u8 * rt_value, struct i2c_client *client)
{
	int ret;
	u8 read_cmd[3] = { 0 };
	u8 cmd_len = 0;

	read_cmd[0] = reg;
	cmd_len = 1;

	if (client->adapter == NULL)
		pr_err("aw9818_read client->adapter==NULL\n");

	ret = i2c_master_send(client, read_cmd, cmd_len);
	if (ret != cmd_len) {
		pr_err("aw9818_read error1\n");
		return -1;
	}

	ret = i2c_master_recv(client, rt_value, 1);
	if (ret != 1) {
		pr_err("aw9818_read error2, ret = %d.\n", ret);
		return -1;
	}

	return 0;
}

int aw9818_write(u8 reg, unsigned char value, struct i2c_client *client)
{
	int ret = 0;
	u8 write_cmd[2] = { 0 };

	write_cmd[0] = reg;
	write_cmd[1] = value;

	ret = i2c_master_send(client, write_cmd, 2);
	if (ret != 2) {
		pr_err("aw9818_write error->[REG-0x%02x,val-0x%02x]\n", reg, value);
		return -1;
	}

	return 0;
}

static void aw981x_write_register(CHIP_ID chip_id, byte reg, byte data)
{
	pr_err("id 0x%x, reg 0x%x, data 0x%x\n", chip_id, reg, data);

	aw9818_write(reg, data, g_aw9818->i2c_cli[chip_id]);
}

static void aw981x_enable_chip(void)
{
	byte aw981x_id;

	for (aw981x_id = 0; aw981x_id < LED_CHIP_NUMS; aw981x_id++) {
		// enable chip
		aw981x_write_register(aw981x_id, aw981x_sleep_reg, aw981x_en_value);
	}
}

static void led_default_setup(void)
{
	byte aw981x_id;
	pr_err("%s\n", __func__);

	for (aw981x_id = 0; aw981x_id < LED_CHIP_NUMS; aw981x_id++) {
		// reset chip
		aw981x_write_register(aw981x_id, aw981x_reset_reg, aw981x_reset_value);
		// configure Imax and mode
		aw981x_write_register(aw981x_id, aw981x_config_reg, aw981x_config_value);
	}

	aw981x_enable_chip();
}

static void led_init(void)
{
	ledif_info_init();
	ledeffect_info_init();
	led_default_setup();
}

static int aw9818_open(struct inode *inode, struct file *filp)
{
	struct aw9818_priv *aw9818 = container_of(inode->i_cdev,
						  struct aw9818_priv, cdev);

	filp->private_data = aw9818;

	return 0;
}

/*
 *
 *eg: a signal like "startup-complete"
 */
static long aw9818_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case AW9818_LEDS_EFFECT_COMPLETE:
		effect_complete = 0;
		//TODO set unblank
		break;
	default:
		break;
	}

	return 0;
}

static int aw9818_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static struct file_operations aw9818_fops = {
	.owner = THIS_MODULE,
	.open = aw9818_open,
	.unlocked_ioctl = aw9818_unlocked_ioctl,
	.release = aw9818_release,
};

/*
 *
 *this function is a typical cdev module
 */
static int aw9818_setup_cdev(struct aw9818_priv *data)
{
	int ret;
	static int dev_major = DEV_MAJOR;
	static int dev_minor = DEV_MINOR;

	data->devno = MKDEV(dev_major, dev_minor);
	if (dev_major) {
		ret = register_chrdev_region(data->devno, 1, DEV_NAME);
	} else {
		ret = alloc_chrdev_region(&data->devno, dev_minor, 1, DEV_NAME);
	}
	if (ret < 0)
		return ret;

	cdev_init(&data->cdev, &aw9818_fops);
	ret = cdev_add(&data->cdev, data->devno, 1);
	if (ret)
		pr_err("error add a char dev");
	data->class = class_create(THIS_MODULE, DEV_NAME);
	data->device = device_create(data->class, NULL, data->devno, NULL, DEV_NAME);

	return 0;
}

static byte convert_data(byte brightness, LED_COLOR_LEVEL color)
{
	switch (color) {
	case LED_COLOR_NONE:
		brightness &= 0;
		break;
	case LED_COLOR_HALF:
		brightness <<= 1;
		break;
	case LED_COLOR_FULL:
		brightness = brightness;
		break;
	default:
		break;
	}

	return brightness;
}

static void led_update_data(byte led_index, ledreg_data * p_ledreg_data)
{
	const ledhw_info *p_ledhw_info = NULL;
	const ledreg_data *lreg = NULL;

	//get regmap(r,g,b) and data(r,g,b)
	if (led_index < get_led_nums(0)) {
		p_ledhw_info = get_ledhw_info(0);
	} else {
		p_ledhw_info = get_ledhw_info(1);
		led_index -= get_led_nums(0);
	}
	lreg = p_ledhw_info->ledreg_map + led_index;

	aw981x_write_register(p_ledhw_info->chip_id, lreg->r, p_ledreg_data->r);
	aw981x_write_register(p_ledhw_info->chip_id, lreg->g, p_ledreg_data->g);
	aw981x_write_register(p_ledhw_info->chip_id, lreg->b, p_ledreg_data->b);
}

static void led_set_bright_color(byte led_index, byte brightness, const ledcolor_info color)
{
	ledif_info *ledif_info_struct = NULL;

	if (led_index < get_led_nums(0)) {
		ledif_info_struct = &led_intf[0];
	} else {
		ledif_info_struct = &led_intf[1];
	}

	//get brightness by level
	brightness = gamma_brightness[brightness];

	if (NULL != ledif_info_struct && NULL != ledif_info_struct->p_ledreg_data) {
		ledif_info_struct->p_ledreg_data->r = convert_data(brightness, color.r);
		ledif_info_struct->p_ledreg_data->g = convert_data(brightness, color.g);
		ledif_info_struct->p_ledreg_data->b = convert_data(brightness, color.b);

		led_update_data(led_index, ledif_info_struct->p_ledreg_data);
	} else if (ledif_info_struct == NULL) {
		pr_err("some struct is NULL\n");
	}

}

static void led_set_all_bright_color(byte led_nums, byte brightness, const ledcolor_info background)
{
	byte i;
	for (i = 0; i < led_nums; i++) {
		pr_err("led %u, led_set_bright_color\n", i);
		led_set_bright_color(i, brightness, background);
	}
}

static void led_set_comet(byte cur_idx, const ledcolor_info foward)
{
	byte comet_nums, i;

	comet_nums = sizeof(led_bright_level) / sizeof(led_bright_level[0]);
	for (i = 0; i < comet_nums; i++) {
		led_set_bright_color(cur_idx++, led_bright_level[i], foward);
	}

}

static void effect_comet_function(byte led_nums, byte cur_idx, byte brightness, const ledcolor_info background, const ledcolor_info foward)
{
	//set all background
	led_set_all_bright_color(led_nums, brightness, background);

	//set comet
	led_set_comet(cur_idx, foward);

	//enable
	aw981x_enable_chip();
}

/*
 *
 * 一圈浅蓝色的灯亮起，浅红色的灯以 彗星 模式转动
 */
static void aw9818_startup(struct work_struct *ws)
{
	byte led_nums, comet_nums;
	ledeffect_info *p_led_effect = get_led_effect();
	pr_err("%s\n", __func__);

	if (NULL != p_led_effect) {
		led_nums = get_led_nums(0) + get_led_nums(1);
		comet_nums = sizeof(led_bright_level) / sizeof(led_bright_level[0]);

		p_led_effect->background = led_colors[none];
		p_led_effect->foward = led_colors[yellow];
		p_led_effect->cur_idx = 0;
		p_led_effect->bright_level = led_bright_level[comet_nums - 1];	//故意设置背景色比comet最大值暗一些

		while (p_led_effect->cur_idx < led_nums - comet_nums) {
			effect_comet_function(led_nums, p_led_effect->cur_idx, p_led_effect->bright_level, p_led_effect->background,
					      p_led_effect->foward);
			p_led_effect->cur_idx++;
		}
	}
}

/*
 *loop waitting "start compete sigal" from userspace !
 */
static void led_effect_startup(void)
{
	struct aw9818_priv *awdata = g_aw9818;

	if (awdata) {
		INIT_WORK(&awdata->work_startup, aw9818_startup);
		if (!work_pending(&awdata->work_startup)) {
			queue_work(awdata->wqueue, &awdata->work_startup);
		}
	}
}

static int __devinit aw9818_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	static struct aw9818_priv *data = NULL;

	//check i2c-valid
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE | I2C_FUNC_I2C))
		return -EIO;

	//malloc priv-data
	if (data == NULL) {
		data = devm_kzalloc(&client->dev, sizeof(struct aw9818_priv), GFP_KERNEL);
		if (!data)
			return -ENOMEM;
		g_aw9818 = data;
	}
	//i2c client can get priv somewhere i2c_set_clientdata(client, data);

	if (id->driver_data == 1) {	//all chips probe, then call function
		data->i2c_cli[1] = client;
		led_init();
		aw9818_setup_cdev(data);
		led_effect_startup();
	} else if (id->driver_data == 0) {
		data->i2c_cli[0] = client;
	}

	return 0;
}

static int aw9818_i2c_remove(struct i2c_client *client)
{
	//data auto-free by devm_kzalloc
	return 0;
}

static struct i2c_board_info aw9818_i2c_board_info[] = {
	{I2C_BOARD_INFO("aw9818_1", 0x3a),},
	{I2C_BOARD_INFO("aw9818_2", 0x3b),},
};

static const struct i2c_device_id aw9818_i2c_id[] = {
	{"aw9818_1", 0},
	{"aw9818_2", 1},
	{},
};

MODULE_DEVICE_TABLE(i2c, aw9818_i2c_id);

static struct i2c_driver aw9818_i2c_driver = {
	.driver = {
		   .name = "hello aw9818",
		   },
	.probe = aw9818_i2c_probe,
	.id_table = aw9818_i2c_id,
	.remove = aw9818_i2c_remove,
};

static __init int module_aw9818_init(void)
{
	struct i2c_adapter *adapter = NULL;
	struct i2c_client *client = NULL;
	int i;

	printk("Hello World\n");
	adapter = i2c_get_adapter(AW9818_CHANNEL);
	if (!adapter)
		return -ENODEV;
	for (i = 0; i < sizeof(aw9818_i2c_board_info) / sizeof(aw9818_i2c_board_info[0]); i++) {
		client = NULL;
		client = i2c_new_device(adapter, &aw9818_i2c_board_info[i]);
		if (!client)
			return -ENODEV;
	}
	i2c_put_adapter(adapter);

	return i2c_add_driver(&aw9818_i2c_driver);
}

static __exit void module_aw9818_exit(void)
{
	i2c_del_driver(&aw9818_i2c_driver);
	printk("Byebye\n");
}

//insmod xxx.ko
module_init(module_aw9818_init);
//rmmod xxx.ko
module_exit(module_aw9818_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("oneface");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("aw9818 i2c led");
