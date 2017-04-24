
/*  人体红外驱动程序  */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/input.h>

#include <asm/gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#define DEVICE_NAME		"hc-501"
#define HC_501_PGIO		EXYNOS4_GPX1(3)

//static struct semaphore hc_lock;
//static struct input_dev *hc_input_dev;

struct hc_input_dev_t{
	struct input_dev* input;
	struct semaphore hc_lock;
	struct timer_list *timer;
};

static struct hc_input_dev_t* hc_input_dev;


static void hc_timer(unsigned long _data){
	int tmp;
	tmp = gpio_get_value(HC_501_PGIO);
	
	input_report_key(hc_input_dev->input, KEY_1,tmp);
	input_sync(hc_input_dev->input);
	mod_timer(hc_input_dev->timer, jiffies + msecs_to_jiffies(40));
	
}


static int __init hc501_init(void)
{
	int ret;
	hc_input_dev = kmalloc(sizeof(struct hc_input_dev_t),GFP_KERNEL);
	if(!hc_input_dev){
		printk(KERN_INFO " error at hc501 start \n");
	}
	
	/* 1、申请gpio并设置成输入  */
	ret = gpio_request(HC_501_PGIO, DEVICE_NAME);
	if (ret) {
		printk(KERN_WARNING "request GPIO %d for %s failed\n", HC_501_PGIO,DEVICE_NAME);
		return ret;
	}
	//printk(KERN_INFO "gpio request success \n");
	s3c_gpio_cfgpin(HC_501_PGIO, S3C_GPIO_INPUT);
	/* 2、分配输入设备结构体 */
	hc_input_dev->input = input_allocate_device();
	if(!hc_input_dev->input){
		printk("alloc inputdevice error\n");
		gpio_free(HC_501_PGIO);
		return -1;
	}
	//printk(KERN_INFO "alloc input device success \n");
	/* 3、输入设备功能 */
	hc_input_dev->input->name = DEVICE_NAME;
	//hc_input_dev->input->evbit[BITS_TO_LONGS(EV_CNT)] = BIT_MASK(EV_KEY);
	set_bit(EV_KEY, hc_input_dev->input->evbit);
	/* 一直按下按键，反复上报数据  */
	set_bit(EV_REP, hc_input_dev->input->evbit);
	input_set_capability(hc_input_dev->input, EV_KEY, KEY_1);
	/* 4、注册输入设备  */
	ret = input_register_device(hc_input_dev->input);
	if (ret) {
		input_free_device(hc_input_dev->input);
		gpio_free(HC_501_PGIO);
		//printk("register input device error\n");
		return ret;
	}
	//printk(KERN_INFO "register input device success \n");
	/* 5\定时器，扫描红外传感器输入情况  */
	hc_input_dev->timer = (struct timer_list*)kmalloc(sizeof(struct timer_list),GFP_KERNEL);
	setup_timer(hc_input_dev->timer, hc_timer,0);
	add_timer(hc_input_dev->timer);
	
	printk(KERN_INFO "hc501 driver initialized\n");
	return 0;
}


static void __exit hc501_exit(void)
{
	del_timer_sync(hc_input_dev->timer);
	input_unregister_device(hc_input_dev->input);
	input_free_device(hc_input_dev->input);
	gpio_free(HC_501_PGIO);
}

module_init(hc501_init);
module_exit(hc501_exit);

MODULE_AUTHOR("zhangdalei");
MODULE_DESCRIPTION("HC-RS501 driver");
MODULE_LICENSE("GPL");





