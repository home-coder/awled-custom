#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
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
#include <linux/wait.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/kthread.h>

#define debug()   printk("%s, %d \n", __func__, __LINE__)
#define DEV_MAJOR   0		// meaning major is not set
#define DEV_MINOR   0		//meaning set minor to be 0
#define DEV_NAME    "xlj-aw9818"

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
} LED_EFFECT_ENUM;

#define AW9818_IOC_MAGIC 'm'	//定义类型
#define AW9818_LEDS_EFFECT_STARTUP			_IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_STARTUP, int)
#define AW9818_LEDS_EFFECT_COMPLETE			_IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_COMPLETE, int)
#define AW9818_LEDS_EFFECT_AIRKISS_MODE		_IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_AIRKISS_MODE, int)
#define AW9818_LEDS_EFFECT_AIRKISS_CONFIG	_IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_AIRKISS_CONFIG, int)
#define AW9818_LEDS_EFFECT_AIRKISS_CONNECT  _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_AIRKISS_CONNECT, int)
#define AW9818_LEDS_EFFECT_WAKE_UP			_IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_WAKE_UP, int)
#define AW9818_LEDS_EFFECT_COMMAND_FAIL		_IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_COMMAND_FAIL, int)
#define AW9818_LEDS_EFFECT_COMMAND_SUCCESS  _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_COMMAND_SUCCESS, int)
#define AW9818_LEDS_EFFECT_KEYMUTE		    _IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_KEYMUTE, int)
#define AW9818_LEDS_EFFECT_KEYUNMUTE		_IOW(AW9818_IOC_MAGIC, LEDS_EFFECT_KEYUNMUTE, int)

typedef enum {
	LED_THREAD_ACTIVE = 0,
	LED_THREAD_INACTIVE,
} LED_THREAD_STATUS;

struct aw9818_priv {
	struct cdev cdev;
	dev_t devno;
	struct class *class;
	struct device *device;
	struct mutex effect_lock;
	struct semaphore condition_lock;
	wait_queue_head_t notify_led_event;
	struct task_struct *led_cntrl_threadid;
	LED_THREAD_STATUS led_thread_running;
	bool wait_condtion, led_thread_sleep;
};

struct aw9818_priv *g_aw9818 = NULL;

typedef unsigned char byte;
typedef struct {
	int state;
	bool is_mute;
	byte orientation;
}ledeffect_info;
ledeffect_info *ledeffect = NULL;

static ledeffect_info *get_led_effect(void)
{
	if (ledeffect == NULL) {
		ledeffect = (ledeffect_info *)kzalloc(sizeof(ledeffect_info), GFP_KERNEL);
		if (!ledeffect) {
			pr_err("ENOMEM\n");
		}
	}
	return ledeffect;
}

static void led_effect_close(void)
{
	printk("%s...\n", __func__);
}

static void led_effect_startup(void)
{
	printk("%s...\n", __func__);

}

static void led_effect_complete(void)
{
	printk("%s...\n", __func__);

}

static void led_effect_airkiss_mode(void)
{
	printk("%s...\n", __func__);
	while (g_aw9818->led_thread_running == LED_THREAD_ACTIVE) {
		printk("%s...\n", __func__);
		msleep(50);
	}

}

static void led_effect_airkiss_config(void)
{
	printk("%s...\n", __func__);
	while (g_aw9818->led_thread_running == LED_THREAD_ACTIVE) {
		printk("%s...\n", __func__);
		msleep(50);
	}

}

static void led_effect_airkiss_connect(void)
{
	printk("%s...\n", __func__);

}

static void led_effect_wake_up(byte orientation)
{
	printk("%s...\n", __func__);
	//FIXME orientation assert
	while (g_aw9818->led_thread_running == LED_THREAD_ACTIVE) {
		printk("%s..., orientation--->%u\n", __func__, orientation);
		msleep(50);
	}

}

static void led_effect_command_fail(void)
{
	printk("%s...\n", __func__);

}

static void led_effect_command_success(void)
{
	printk("%s...\n", __func__);

}

static void led_effect_keymute(void)
{
	printk("%s...\n", __func__);

}

static void led_effect_keyunmute(void)
{
	printk("%s...\n", __func__);

}

static void led_event_cntrl_thread(struct aw9818_priv *data)
{
	ledeffect_info *p_led_effect = NULL;
	g_aw9818->wait_condtion = false;
	g_aw9818->led_thread_running = LED_THREAD_INACTIVE;
	g_aw9818->led_thread_sleep = true;

	while (true) {
		p_led_effect = get_led_effect();
		if (NULL != p_led_effect) {
			//TODO check state
			wait_event_interruptible(data->notify_led_event, g_aw9818->wait_condtion == true || kthread_should_stop());	//FIXME case is 1:interrup coming
			down(&g_aw9818->condition_lock);
			g_aw9818->wait_condtion = false;
			g_aw9818->led_thread_sleep = false;
			g_aw9818->led_thread_running = LED_THREAD_ACTIVE;

			switch (p_led_effect->state) {
				case AW9818_LEDS_EFFECT_STARTUP:
					led_effect_startup();
					break;
				case AW9818_LEDS_EFFECT_COMPLETE:
					led_effect_complete();
					break;
				case AW9818_LEDS_EFFECT_AIRKISS_MODE:
					led_effect_airkiss_mode();
					break;
				case AW9818_LEDS_EFFECT_AIRKISS_CONFIG:
					led_effect_airkiss_config();
					break;
				case AW9818_LEDS_EFFECT_AIRKISS_CONNECT:
					led_effect_airkiss_connect();
					break;
				case AW9818_LEDS_EFFECT_WAKE_UP:
					led_effect_wake_up(p_led_effect->orientation);
					break;
				case AW9818_LEDS_EFFECT_COMMAND_FAIL:
					led_effect_command_fail();
					break;
				case AW9818_LEDS_EFFECT_COMMAND_SUCCESS:
					led_effect_command_success();
					break;
				case AW9818_LEDS_EFFECT_KEYMUTE:
					p_led_effect->is_mute = true;
					led_effect_keymute();
					break;
				case AW9818_LEDS_EFFECT_KEYUNMUTE:
					p_led_effect->is_mute = false;
					led_effect_keyunmute();
					break;
				default:
					break;
			}
			if (p_led_effect->state != AW9818_LEDS_EFFECT_KEYMUTE) {
				if (p_led_effect->is_mute) {
					//mute color
					led_effect_keymute();
				} else {
					//close
					led_effect_close();
				}
			}

			g_aw9818->led_thread_running = LED_THREAD_INACTIVE;

			g_aw9818->led_thread_sleep = true;

			up(&g_aw9818->condition_lock);
		}
	}
}

static void led_init(void)
{
	mutex_init(&g_aw9818->effect_lock); 
	sema_init(&g_aw9818->condition_lock, 1);

	init_waitqueue_head(&g_aw9818->notify_led_event);
	g_aw9818->led_cntrl_threadid = kthread_run((int (*)(void *))led_event_cntrl_thread, g_aw9818, "led_control_thread");
	if (IS_ERR(g_aw9818->led_cntrl_threadid)) {
		pr_err("Not able to spawn kernel thread\n");
	}
}

static int aw9818_open(struct inode *inode, struct file *filp)
{
	struct aw9818_priv *aw9818 = container_of(inode->i_cdev,
									  struct aw9818_priv, cdev);

	filp->private_data = aw9818;

	return 0;
}

static void do_ctrl_event(unsigned long cmd)
{
	ledeffect_info *p_led_effect = get_led_effect();
	int timeout = 4;
	printk("%s...\n", __func__);
	if (NULL != p_led_effect) {
		mutex_lock(&g_aw9818->effect_lock);
		p_led_effect->state = cmd;
		//halt current thread
		if (g_aw9818->led_thread_running == LED_THREAD_ACTIVE) {
			g_aw9818->led_thread_running = LED_THREAD_INACTIVE;
		}
		g_aw9818->wait_condtion = true;

		//wait led_thread_running status is TASK_UNINTERRUPTIBLE
		while (true) {
			if (g_aw9818->led_thread_sleep == true) {
				mdelay(50);
				wake_up(&g_aw9818->notify_led_event);
				break;
			} else {
				if (timeout--) {
					mdelay(50);
				} else {
					break;
				}
			}
		}
		mutex_unlock(&g_aw9818->effect_lock);
	}

}

static long aw9818_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)//FIXME if not only orientation, it should be a struct
{
	ledeffect_info *p_led_effect = get_led_effect();
	if (NULL != p_led_effect) {
		get_user(p_led_effect->orientation, (unsigned long __user *)arg);

		do_ctrl_event(cmd);
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

static __init int module_mechanism_init(void)
{
	static struct aw9818_priv *data = NULL;
	printk("Hello World\n");

	//malloc priv-data
	data = kzalloc(sizeof(struct aw9818_priv), GFP_KERNEL);
	if (!data)
		return -ENOMEM;
	g_aw9818 = data;
	led_init();
	aw9818_setup_cdev(data);

	return 0;
}

static __exit void module_mechanism_exit(void)
{
	printk("Byebye\n");
	device_destroy(g_aw9818->class, g_aw9818->devno);
	class_destroy(g_aw9818->class);
	cdev_del(&g_aw9818->cdev);
	unregister_chrdev_region(g_aw9818->devno, 1);


	kfree(g_aw9818);
}

//insmod xxx.ko
module_init(module_mechanism_init);
//rmmod xxx.ko
module_exit(module_mechanism_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("oneface"); 
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("mechanism led");
