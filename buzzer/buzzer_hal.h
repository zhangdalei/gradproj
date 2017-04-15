
#ifndef _BUZZER_HAL
#define _BUZZER_HAL

#include <hardware/hardware.h>

#define PWM_IOCTL_SET_FREQ		1
#define PWM_IOCTL_STOP			0
#define BUZZER "/dev/pwm"

// 设备id,用于匹配设备
#define BUZZER_HARDWARE_MODULE_ID "buzzer"

#define BUZZER_API_VERSION HARDWARE_MODULE_API_VERSION(1,0)


struct buzzer_device_t
{
	struct hw_device_t common;
	int fd;
	int (*buzzer_open)(struct buzzer_device_t* buzzer_dev);
	int (*buzzer_ioclt)(struct buzzer_device_t* buzzer_dev,int cmd,unsigned long frec);
};



#endif

