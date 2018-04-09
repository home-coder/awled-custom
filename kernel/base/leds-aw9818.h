#ifndef _AW9818_H_
#define _AW9818_H_

#define MAX_I2C_BUFFER_SIZE 65536

#define AW9818_ID 0x18

struct aw9818 {
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

