#ifndef _AW9817_H_
#define _AW9817_H_

#define MAX_I2C_BUFFER_SIZE 65536

#define AW9817_ID 0x18

struct aw9817 {
    struct i2c_client *i2c;
    struct device *dev;
    struct led_classdev cdev;
    struct work_struct brightness_work;

    int reset_gpio;

    unsigned char chipid;

    int imax;
    int fade_time;
};

#endif

