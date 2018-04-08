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

#define DEV_MAJOR   0 // meaning major is not set
#define DEV_MINOR   0 //meaning set minor to be 0
#define DEV_NAME    "xlj-aw9818"

#define AW9818_CHANNEL	2

struct aw9818_priv {
	struct i2c_client *i2c;
	struct cdev cdev;
	dev_t devno;
	struct class *class;
	struct device *device;
};

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

static int aw9818_open(struct inode *inode, struct file *filp)
{
	struct aw9818_priv *aw9818 = container_of(inode->i_cdev,
						  struct aw9818_priv, cdev);

	filp->private_data = aw9818;

	return 0;
}

static long aw9818_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret;
	//TODO
	return ret;
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

static int __devinit aw9818_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct aw9818_priv *data = NULL;

	//check i2c-valid
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE | I2C_FUNC_I2C))
		return -EIO;

	//malloc priv-data
	data = kmalloc(sizeof(struct aw9818_priv), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	//i2c client
	i2c_set_clientdata(client, data);

	//char dev
	aw9818_setup_cdev(data);

	//all chips probe, then call function

	return 0;
}

static int aw9818_i2c_remove(struct i2c_client* client)
{
	struct tm1650_data* data = i2c_get_clientdata(client);

	kfree(data);
	data = NULL;
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
