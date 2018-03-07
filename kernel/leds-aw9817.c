/*
 * leds-aw9817.c   aw9817 led module
 *
 * Version: v1.0.0
 *
 * Copyright (c) 2017 AWINIC Technology CO., LTD
 *
 *  Author: Nick Li <liweilei@awinic.com.cn>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/firmware.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/debugfs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/leds.h>
#include <linux/leds-aw9817.h>
/******************************************************
 *
 * Marco
 *
 ******************************************************/
#define AW9817_I2C_NAME "aw9817_led"

#define AW9817_VERSION "v1.0.0"

#define AW_I2C_RETRIES 5
#define AW_I2C_RETRY_DELAY 5
#define AW_READ_CHIPID_RETRIES 5
#define AW_READ_CHIPID_RETRY_DELAY 5

#define REG_ID          0x00
#define REG_WORK_MODE   0x01
#define REG_SWRST       0x02
#define REG_LED_MODE    0x03
#define REG_UPDATE      0x04
#define REG_LA11        0x10
#define REG_LA21        0x11
#define REG_LA31        0x12
#define REG_LA41        0x13
#define REG_LA51        0x14
#define REG_LA61        0x15
#define REG_LA71        0x16
#define REG_LA81        0x17
#define REG_LA91        0x18
#define REG_LAA1        0x19
#define REG_LAB1        0x1a
#define REG_LA12        0x1b
#define REG_LA22        0x1c
#define REG_LA32        0x1d
#define REG_LA42        0x1e
#define REG_LA52        0x1f
#define REG_LA62        0x20
#define REG_LA72        0x21
#define REG_LA82        0x22
#define REG_LA92        0x23
#define REG_LAA2        0x24
#define REG_LAB2        0x25
#define REG_LA13        0x26
#define REG_LA23        0x27
#define REG_LA33        0x28
#define REG_LA43        0x29
#define REG_LA53        0x2a
#define REG_LA63        0x2b
#define REG_LA73        0x2c
#define REG_LA83        0x2d
#define REG_LA93        0x2e
#define REG_LAA3        0x2f
#define REG_LAB3        0x30
#define REG_LA14        0x31
#define REG_LA24        0x32
#define REG_LA34        0x33
#define REG_LA44        0x34
#define REG_LA54        0x35
#define REG_LA64        0x36
#define REG_LA74        0x37
#define REG_LA84        0x38
#define REG_LA94        0x39
#define REG_LAA4        0x3a
#define REG_LAB4        0x3b
#define REG_LEDON1      0x60
#define REG_LEDON2      0x61
#define REG_LEDON3      0x62
#define REG_LEDON4      0x63
#define REG_LEDON5      0x64
#define REG_LEDON6      0x65
#define REG_LEDON7      0x66
#define REG_LEDON8      0x67
#define REG_LEDON9      0x68
#define REG_LEDONA      0x69
#define REG_LEDONB      0x6A
#define REG_INTN_LA1    0x70
#define REG_INTN_LA2    0x71
#define REG_INTN_LA3    0x72
#define REG_INTN_LA4    0x73
#define REG_INTN_LA5    0x74
#define REG_INTN_LA6    0x75
#define REG_INTN_LA7    0x76
#define REG_INTN_LA8    0x77
#define REG_INTN_LA9    0x78
#define REG_INTN_LAA    0x79
#define REG_INTN_LAB    0x7A


/* aw9817 register read/write access*/
#define REG_NONE_ACCESS                 0
#define REG_RD_ACCESS                   1 << 0
#define REG_WR_ACCESS                   1 << 1
#define AW9817_REG_MAX                  0xFF

const unsigned char aw9817_reg_access[AW9817_REG_MAX] = {
	[REG_ID       ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_WORK_MODE] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_SWRST    ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LED_MODE ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_UPDATE   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LA11     ] = REG_WR_ACCESS,
	[REG_LA21     ] = REG_WR_ACCESS,
	[REG_LA31     ] = REG_WR_ACCESS,
	[REG_LA41     ] = REG_WR_ACCESS,
	[REG_LA51     ] = REG_WR_ACCESS,
	[REG_LA61     ] = REG_WR_ACCESS,
	[REG_LA71     ] = REG_WR_ACCESS,
	[REG_LA81     ] = REG_WR_ACCESS,
	[REG_LA91     ] = REG_WR_ACCESS,
	[REG_LAA1     ] = REG_WR_ACCESS,
	[REG_LAB1     ] = REG_WR_ACCESS,
	[REG_LA12     ] = REG_WR_ACCESS,
	[REG_LA22     ] = REG_WR_ACCESS,
	[REG_LA32     ] = REG_WR_ACCESS,
	[REG_LA42     ] = REG_WR_ACCESS,
	[REG_LA52     ] = REG_WR_ACCESS,
	[REG_LA62     ] = REG_WR_ACCESS,
	[REG_LA72     ] = REG_WR_ACCESS,
	[REG_LA82     ] = REG_WR_ACCESS,
	[REG_LA92     ] = REG_WR_ACCESS,
	[REG_LAA2     ] = REG_WR_ACCESS,
	[REG_LAB2     ] = REG_WR_ACCESS,
	[REG_LA13     ] = REG_WR_ACCESS,
	[REG_LA23     ] = REG_WR_ACCESS,
	[REG_LA33     ] = REG_WR_ACCESS,
	[REG_LA43     ] = REG_WR_ACCESS,
	[REG_LA53     ] = REG_WR_ACCESS,
	[REG_LA63     ] = REG_WR_ACCESS,
	[REG_LA73     ] = REG_WR_ACCESS,
	[REG_LA83     ] = REG_WR_ACCESS,
	[REG_LA93     ] = REG_WR_ACCESS,
	[REG_LAA3     ] = REG_WR_ACCESS,
	[REG_LAB3     ] = REG_WR_ACCESS,
	[REG_LA14     ] = REG_WR_ACCESS,
	[REG_LA24     ] = REG_WR_ACCESS,
	[REG_LA34     ] = REG_WR_ACCESS,
	[REG_LA44     ] = REG_WR_ACCESS,
	[REG_LA54     ] = REG_WR_ACCESS,
	[REG_LA64     ] = REG_WR_ACCESS,
	[REG_LA74     ] = REG_WR_ACCESS,
	[REG_LA84     ] = REG_WR_ACCESS,
	[REG_LA94     ] = REG_WR_ACCESS,
	[REG_LAA4     ] = REG_WR_ACCESS,
	[REG_LAB4     ] = REG_WR_ACCESS,
	[REG_LEDON1   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDON2   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDON3   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDON4   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDON5   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDON6   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDON7   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDON8   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDON9   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDONA   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_LEDONB   ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LA1 ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LA2 ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LA3 ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LA4 ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LA5 ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LA6 ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LA7 ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LA8 ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LA9 ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LAA ] = REG_RD_ACCESS|REG_WR_ACCESS,
	[REG_INTN_LAB ] = REG_RD_ACCESS|REG_WR_ACCESS,
};


/******************************************************
 *
 * aw9817 i2c write/read
 *
 ******************************************************/
static int aw9817_i2c_write(struct aw9817 *aw9817, 
		unsigned char reg_addr, unsigned char reg_data)
{
	int ret = -1;
	unsigned char cnt = 0;

	while(cnt < AW_I2C_RETRIES) {
		ret = i2c_smbus_write_byte_data(aw9817->i2c, reg_addr, reg_data);
		if(ret < 0) {
			pr_err("%s: i2c_write cnt=%d error=%d\n", __func__, cnt, ret);
		} else {
			break;
		}
		cnt ++;
		msleep(AW_I2C_RETRY_DELAY);
	}

	return ret;
}

static int aw9817_i2c_read(struct aw9817 *aw9817, 
		unsigned char reg_addr, unsigned char *reg_data)
{
	int ret = -1;
	unsigned char cnt = 0;

	while(cnt < AW_I2C_RETRIES) {
		ret = i2c_smbus_read_byte_data(aw9817->i2c, reg_addr);
		if(ret < 0) {
			pr_err("%s: i2c_read cnt=%d error=%d\n", __func__, cnt, ret);
		} else {
			*reg_data = ret;
			break;
		}
		cnt ++;
		msleep(AW_I2C_RETRY_DELAY);
	}

	return ret;
}

static int aw9817_i2c_write_bits(struct aw9817 *aw9817, 
		unsigned char reg_addr, unsigned char mask, unsigned char reg_data)
{
	unsigned char reg_val;

	aw9817_i2c_read(aw9817, reg_addr, &reg_val);
	reg_val &= mask;
	reg_val |= reg_data;
	aw9817_i2c_write(aw9817, reg_addr, reg_val);

	return 0;
}


/******************************************************
 *
 * aw9817 led
 *
 ******************************************************/
static void aw9817_brightness_work(struct work_struct *work)
{
	struct aw9817 *aw9817 = container_of(work, struct aw9817,
			brightness_work);

	unsigned char i;

	if(aw9817->cdev.brightness > aw9817->cdev.max_brightness) {
		aw9817->cdev.brightness = aw9817->cdev.max_brightness;
	}

	aw9817_i2c_write(aw9817, 0x02, 0x01);               // software reset

	if(aw9817->cdev.brightness) {
		aw9817_i2c_write(aw9817, 0x01, 0x00);           // normal mode
		aw9817_i2c_write(aw9817, 0x03,
				(aw9817->imax<<4) | (0x01<<2) |
				0x02);                                  // martix mode

		for(i=0; i<44; i++) {
			aw9817_i2c_write(aw9817, 0x10+i,
					aw9817->cdev.brightness);               // led dimming
		}
		aw9817_i2c_write(aw9817, 0x04, 0x01);           // update led dimming
	}
}

static void aw9817_set_brightness(struct led_classdev *cdev,
		enum led_brightness brightness)
{
	struct aw9817 *aw9817 = container_of(cdev, struct aw9817, cdev);

	aw9817->cdev.brightness = brightness;

	schedule_work(&aw9817->brightness_work);
}

static void aw9817_led_blink(struct aw9817 *aw9817, unsigned char blink)
{
	unsigned char i;

	aw9817_i2c_write(aw9817, 0x02, 0x01);               // software reset

	if(blink) {
		if(aw9817->cdev.brightness > aw9817->cdev.max_brightness) {
			aw9817->cdev.brightness = aw9817->cdev.max_brightness;
		}

		aw9817_i2c_write(aw9817, 0x01, 0x00);           // normal mode
		aw9817_i2c_write(aw9817, 0x03,
				(aw9817->imax<<4) | (0x01<<2) |
				0x01);                                  // blink mode

		for(i=0; i<44; i++) {
			aw9817_i2c_write(aw9817, 0x10+i,
					(aw9817->fade_time<<6) |
					aw9817->cdev.brightness);               // led dimming
		}
		aw9817_i2c_write(aw9817, 0x04, 0x01);           // update led dimming

		for(i=0; i<11; i++) {
			aw9817_i2c_write(aw9817, 0x60+i, 0x0f);       // led blink
		}
	}
}

/*****************************************************
 *
 * device tree
 *
 *****************************************************/
static int aw9817_parse_dt(struct device *dev, struct aw9817 *aw9817,
		struct device_node *np)
{
	aw9817->reset_gpio = of_get_named_gpio(np, "reset-gpio", 0);
	if (aw9817->reset_gpio < 0) {
		dev_err(dev, "%s: no reset gpio provided, will not HW reset device\n", __func__);
		return -1;
	} else {
		dev_info(dev, "%s: reset gpio provided ok\n", __func__);
	}

	return 0;
}

static int aw9817_hw_reset(struct aw9817 *aw9817)
{
	pr_info("%s enter\n", __func__);

	if (aw9817 && gpio_is_valid(aw9817->reset_gpio)) {
		gpio_set_value_cansleep(aw9817->reset_gpio, 0);
		msleep(1);
		gpio_set_value_cansleep(aw9817->reset_gpio, 1);
		msleep(1);
	} else {
		dev_err(aw9817->dev, "%s:  failed\n", __func__);
	}
	return 0;
}

static int aw9817_hw_off(struct aw9817 *aw9817)
{
	pr_info("%s enter\n", __func__);

	if (aw9817 && gpio_is_valid(aw9817->reset_gpio)) {
		gpio_set_value_cansleep(aw9817->reset_gpio, 0);
		msleep(1);
	} else {
		dev_err(aw9817->dev, "%s:  failed\n", __func__);
	}
	return 0;
}

/*****************************************************
 *
 * check chip id
 *
 *****************************************************/
static int aw9817_read_chipid(struct aw9817 *aw9817)
{
	int ret = -1;
	unsigned char cnt = 0;
	unsigned char reg_val = 0;

	while(cnt < AW_READ_CHIPID_RETRIES) {
		ret = aw9817_i2c_read(aw9817, REG_ID, &reg_val);
		if (ret < 0) {
			dev_err(aw9817->dev,
					"%s: failed to read register AW9817_REG_ID: %d\n",
					__func__, ret);
			return -EIO;
		}
		switch (reg_val) {
			case AW9817_ID:
				pr_info("%s aw9817 detected\n", __func__);
				aw9817->chipid = AW9817_ID;
				return 0;
			default:
				pr_info("%s unsupported device revision (0x%x)\n",
						__func__, reg_val );
				break;
		}
		cnt ++;

		msleep(AW_READ_CHIPID_RETRY_DELAY);
	}

	return -EINVAL;
}

/******************************************************
 *
 * sys group attribute: reg
 *
 ******************************************************/
static ssize_t aw9817_reg_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct aw9817 *aw9817 = container_of(led_cdev, struct aw9817, cdev);

	unsigned int databuf[2] = {0, 0};

	if(2 == sscanf(buf, "%x %x", &databuf[0], &databuf[1])) {
		aw9817_i2c_write(aw9817, (unsigned char)databuf[0], (unsigned char)databuf[1]);
	}

	return count;
}

static ssize_t aw9817_reg_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct aw9817 *aw9817 = container_of(led_cdev, struct aw9817, cdev);
	ssize_t len = 0;
	unsigned char i = 0;
	unsigned char reg_val = 0;
	for(i = 0; i < AW9817_REG_MAX; i ++) {
		if(!(aw9817_reg_access[i]&REG_RD_ACCESS))
			continue;
		aw9817_i2c_read(aw9817, i, &reg_val);
		len += snprintf(buf+len, PAGE_SIZE-len, "reg:0x%02x=0x%02x \n", i, reg_val);
	}
	return len;
}

static ssize_t aw9817_hwen_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct aw9817 *aw9817 = container_of(led_cdev, struct aw9817, cdev);

	unsigned int databuf[1] = {0};

	if(1 == sscanf(buf, "%x", &databuf[0])) {
		if(1 == databuf[0]) {
			aw9817_hw_reset(aw9817);
		} else {
			aw9817_hw_off(aw9817);
		}
	}

	return count;
}

static ssize_t aw9817_hwen_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct aw9817 *aw9817 = container_of(led_cdev, struct aw9817, cdev);
	ssize_t len = 0;
	len += snprintf(buf+len, PAGE_SIZE-len, "hwen=%d\n",
			gpio_get_value(aw9817->reset_gpio));

	return len;
}

static ssize_t aw9817_blink_store(struct device* dev, struct device_attribute *attr,
		const char* buf, size_t len)
{
	unsigned int databuf[1];
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct aw9817 *aw9817 = container_of(led_cdev, struct aw9817, cdev);

	sscanf(buf,"%d",&databuf[0]);
	aw9817_led_blink(aw9817, databuf[0]);

	return len;
}

static ssize_t aw9817_blink_show(struct device* dev,struct device_attribute *attr, char* buf)
{
	ssize_t len = 0;
	len += snprintf(buf+len, PAGE_SIZE-len, "aw9817_blink()\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 0 > blink\n");
	len += snprintf(buf+len, PAGE_SIZE-len, "echo 1 > blink\n");

	return len;
}

static DEVICE_ATTR(reg, S_IWUSR | S_IRUGO, aw9817_reg_show, aw9817_reg_store);
static DEVICE_ATTR(hwen, S_IWUSR | S_IRUGO, aw9817_hwen_show, aw9817_hwen_store);
static DEVICE_ATTR(blink, S_IWUSR | S_IRUGO, aw9817_blink_show, aw9817_blink_store);

static struct attribute *aw9817_attributes[] = {
	&dev_attr_reg.attr,
	&dev_attr_hwen.attr,
	&dev_attr_blink.attr,
	NULL
};

static struct attribute_group aw9817_attribute_group = {
	.attrs = aw9817_attributes
};


/******************************************************
 *
 * led class dev
 *
 ******************************************************/
static int aw9817_parse_led_cdev(struct aw9817 *aw9817,
		struct device_node *np)
{
	struct device_node *temp;
	int ret = -1;

	for_each_child_of_node(np, temp) {
		ret = of_property_read_string(temp, "aw9817,name",
				&aw9817->cdev.name);
		if (ret < 0) {
			dev_err(aw9817->dev,
					"Failure reading led name, ret = %d\n", ret);
			goto free_pdata;
		}
		ret = of_property_read_u32(temp, "aw9817,imax",
				&aw9817->imax);
		if (ret < 0) {
			dev_err(aw9817->dev,
					"Failure reading imax, ret = %d\n", ret);
			goto free_pdata;
		}
		ret = of_property_read_u32(temp, "aw9817,brightness",
				&aw9817->cdev.brightness);
		if (ret < 0) {
			dev_err(aw9817->dev,
					"Failure reading brightness, ret = %d\n", ret);
			goto free_pdata;
		}
		ret = of_property_read_u32(temp, "aw9817,max_brightness",
				&aw9817->cdev.max_brightness);
		if (ret < 0) {
			dev_err(aw9817->dev,
					"Failure reading max brightness, ret = %d\n", ret);
			goto free_pdata;
		}
		ret = of_property_read_u32(temp, "aw9817,fade_time",
				&aw9817->fade_time);
		if (ret < 0) {
			dev_err(aw9817->dev,
					"Failure reading fade_time, ret = %d\n", ret);
			goto free_pdata;
		}
	}

	INIT_WORK(&aw9817->brightness_work, aw9817_brightness_work);

	aw9817->cdev.brightness_set = aw9817_set_brightness;
	ret = led_classdev_register(aw9817->dev, &aw9817->cdev);
	if (ret) {
		dev_err(aw9817->dev,
				"unable to register led ret=%d\n", ret);
		goto free_pdata;
	}

	ret = sysfs_create_group(&aw9817->cdev.dev->kobj,
			&aw9817_attribute_group);
	if (ret) {
		dev_err(aw9817->dev, "led sysfs ret: %d\n", ret);
		goto free_class;
	}

	return 0;

free_class:
	led_classdev_unregister(&aw9817->cdev);
free_pdata:
	return ret;
}

/******************************************************
 *
 * i2c driver
 *
 ******************************************************/
static int aw9817_i2c_probe(struct i2c_client *i2c, const struct i2c_device_id *id)
{
	struct aw9817 *aw9817;
	struct device_node *np = i2c->dev.of_node;
	int ret;

	pr_info("%s enter\n", __func__);

	if (!i2c_check_functionality(i2c->adapter, I2C_FUNC_I2C)) {
		dev_err(&i2c->dev, "check_functionality failed\n");
		return -EIO;
	}

	aw9817 = devm_kzalloc(&i2c->dev, sizeof(struct aw9817), GFP_KERNEL);
	if (aw9817 == NULL)
		return -ENOMEM;

	aw9817->dev = &i2c->dev;
	aw9817->i2c = i2c;

	i2c_set_clientdata(i2c, aw9817);

	/* aw9817 rst & int */
	if (np) {
		ret = aw9817_parse_dt(&i2c->dev, aw9817, np);
		if (ret) {
			dev_err(&i2c->dev, "%s: failed to parse device tree node\n", __func__);
			goto err;
		}
	} else {
		aw9817->reset_gpio = -1;
	}

	if (gpio_is_valid(aw9817->reset_gpio)) {
		ret = devm_gpio_request_one(&i2c->dev, aw9817->reset_gpio,
				GPIOF_OUT_INIT_LOW, "aw9817_rst");
		if (ret){
			dev_err(&i2c->dev, "%s: rst request failed\n", __func__);
			goto err;
		}
	}

	/* hardware reset */
	aw9817_hw_reset(aw9817);

	/* aw9817 chip id */
	ret = aw9817_read_chipid(aw9817);
	if (ret < 0) {
		dev_err(&i2c->dev, "%s: aw9817_read_chipid failed ret=%d\n", __func__, ret);
		goto err_id;
	}

	dev_set_drvdata(&i2c->dev, aw9817);

	aw9817_parse_led_cdev(aw9817, np);
	if (ret < 0) {
		dev_err(&i2c->dev, "%s error creating led class dev\n", __func__);
		goto err_sysfs;
	}

	pr_info("%s probe completed successfully!\n", __func__);

	return 0;

err_sysfs:
err_id:
err:
	return ret;
}

static int aw9817_i2c_remove(struct i2c_client *i2c)
{
	struct aw9817 *aw9817 = i2c_get_clientdata(i2c);

	pr_info("%s enter\n", __func__);

	if (gpio_is_valid(aw9817->reset_gpio))
		devm_gpio_free(&i2c->dev, aw9817->reset_gpio);

	return 0;
}

static const struct i2c_device_id aw9817_i2c_id[] = {
	{ AW9817_I2C_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, aw9817_i2c_id);

static struct of_device_id aw9817_dt_match[] = {
	{ .compatible = "awinic,aw9817_led" },
	{ },
};

static struct i2c_driver aw9817_i2c_driver = {
	.driver = {
		.name = AW9817_I2C_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(aw9817_dt_match),
	},
	.probe = aw9817_i2c_probe,
	.remove = aw9817_i2c_remove,
	.id_table = aw9817_i2c_id,
};


static int __init aw9817_i2c_init(void)
{
	int ret = 0;

	pr_info("aw9817 driver version %s\n", AW9817_VERSION);

	ret = i2c_add_driver(&aw9817_i2c_driver);
	if(ret){
		pr_err("fail to add aw9817 device into i2c\n");
		return ret;
	}

	return 0;
}
module_init(aw9817_i2c_init);


static void __exit aw9817_i2c_exit(void)
{
	i2c_del_driver(&aw9817_i2c_driver);
}
module_exit(aw9817_i2c_exit);


MODULE_DESCRIPTION("AW9817 LED Driver");
MODULE_LICENSE("GPL v2");
