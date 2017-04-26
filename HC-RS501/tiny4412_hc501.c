
/*  人体红外驱动程序  */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>

#include <mach/map.h>
#include <mach/gpio.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>
#include <asm/gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>


#define DEVICE_NAME		"hc_501"
#define HC501_PGIO		EXYNOS4_GPX1(0)	// EINT8
static int hc_irq, flag = 0;
struct timer_list hc_timer;

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

static irqreturn_t hc501_interrupt(int irq, void * data)
{
	printk(KERN_INFO "int the function hc501_interrupt");
	//mod_timer(&hc_timer, jiffies + msecs_to_jiffies(40));
	flag = 1;
	wake_up_interruptible(&button_waitq);
	return IRQ_HANDLED;
}

static int hc_open (struct inode *inode, struct file *filp)
{
	int ret;
	/* 1、申请gpio并设置成输入  */
	ret = gpio_request(HC501_PGIO, DEVICE_NAME);
	if (ret) {
		printk(KERN_WARNING "request GPIO %d for %s failed\n", HC501_PGIO,DEVICE_NAME);
		return ret;
	}
	printk(KERN_INFO "gpio request success \n");
	s3c_gpio_cfgpin(HC501_PGIO, S3C_GPIO_INPUT);
	s3c_gpio_setpull(HC501_PGIO, S3C_GPIO_PULL_DOWN);
	/*  2、申请定时器  */
	//setup_timer(&hc_timer, tiny4412_hc_timer,1);
	/*  3、申请中断  */
	hc_irq = gpio_to_irq(HC501_PGIO);
	ret = request_irq(hc_irq, hc501_interrupt, IRQ_TYPE_EDGE_BOTH, 
				DEVICE_NAME, &hc_irq);
	if (ret)
		{
			printk(KERN_ERR "request hc irq error\n");
			gpio_free(HC501_PGIO);
			return -1;
		}
	
	printk(KERN_INFO "hc open success\n");
	return 0;
}

static int hc_close(struct inode *inode, struct file *file)
{
	free_irq(hc_irq,&hc_irq);
	gpio_free(HC501_PGIO);
	return 0;
}

static ssize_t hc_read (struct file* file, char __user *buff, 
	size_t count, loff_t *offp)
{
	char *data;
	unsigned tmp;
	printk(KERN_INFO "in %s\n",__FUNCTION__);
	wait_event_interruptible(button_waitq,flag);
	//s3c_gpio_cfgpin(HC501_PGIO, S3C_GPIO_OUTPUT);
	tmp = gpio_get_value(HC501_PGIO);
	//s3c_gpio_cfgpin(HC501_PGIO, S3C_GPIO_INPUT);
	flag = 0;
	/* EINT8悬空是被上拉电阻拉高,接上下拉电阻 */
	if(tmp){
		data = "1";
		//printk(KERN_INFO "read data is 1\n");
	}else{
		data = "0";
		//printk(KERN_INFO "read data is 0\n");
		
	}
	if(copy_to_user((void *)buff, (const void *)(data),count)){
		printk(KERN_ERR "copt_to_user error. \n");
		return -1;
	}
	return 0;
	
}

static ssize_t data_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	
	//char *data;
	unsigned tmp;
	printk(KERN_INFO "in %s\n",__FUNCTION__);
	//wait_event_interruptible(button_waitq,flag);
	tmp = gpio_get_value(HC501_PGIO);
	flag = 0;
	if(tmp){
		//data = "1";
		printk(KERN_INFO "read data is 1\n");
	}else{
		//data = "0";
		printk(KERN_INFO "read data is 0\n");
		
	}
	/*
	if(copy_to_user((void *)buff, (const void *)(data),count)){
		printk(KERN_ERR "copt_to_user error. \n");
		return -1;
	}
	*/
	return 0;
}

static DEVICE_ATTR(data, S_IRUGO , data_show, NULL);
static struct attribute *hc_attr[] = {
	&dev_attr_data.attr,
	NULL,
};

static struct attribute_group hc_attribute_group = {
	.attrs = hc_attr,
};


static struct file_operations dev_fops = {
	.owner		= THIS_MODULE,
	.open		= hc_open,
	.release	= hc_close, 
	.read		= hc_read,
	//.poll		= tiny4412_buttons_poll,
};

static struct miscdevice misc = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= DEVICE_NAME,
	.fops		= &dev_fops,
};

static int __init hc501_init(void)
{
	int ret;
	ret = misc_register(&misc);
	if ((ret = sysfs_create_group(&misc.this_device->kobj, &hc_attribute_group)))
	{
		sysfs_remove_group(&misc.this_device->kobj, &hc_attribute_group);
	}
	printk(DEVICE_NAME"\tinitialized\n");
	return ret;
}

static void __exit hc501_exit(void)
{
	sysfs_remove_group(&misc.this_device->kobj, &hc_attribute_group);
	misc_deregister(&misc);
//	del_timer_sync(hc_input_dev->timer);
//	gpio_free(HC_501_PGIO);
}

module_init(hc501_init);
module_exit(hc501_exit);

MODULE_AUTHOR("zhangdalei");
MODULE_DESCRIPTION("HC-RS501 driver");
MODULE_LICENSE("GPL");





