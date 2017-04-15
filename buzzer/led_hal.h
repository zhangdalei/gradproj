#ifndef LED_HAL_H
#define LED_HAL_H

typedef struct led_device
{
	struct hw_device_t common;
	int (*led_open)(struct led_device* leddev);
	int (*led_ctrl)(struct led_device* leddev,int which,int status);
}



#endif