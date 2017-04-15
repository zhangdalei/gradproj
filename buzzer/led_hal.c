#include <hardware/led_hal.h>
#include <hardware/hardware.h>

#include <cutils/log.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
/*  实现hw_module_t结构体**/
#define LOG_TAG "ledhal"
#define LED "/dev/leds"
int fd=-1;


static int led_close(hw_device_t *device)
{
    free(device);
    return 0;
}
static int led_open(struct led_device* leddev)
{
	fd = open(LED,O_RDWR);
	if(fd < 0)
	{
		ALOGD("jni open led fail.\n");
		return -1;
	}
	ALOGD("jni open led sucess.\n");
	return 0;
}
static int led_ctrl(struct led_device* leddev,int which,int status)
{
	jint ret = -1;
	ret = ioctl(fd,status,which);
	if(ret < 0)
	{
		ALOGD("jni ctrl led fail.\n");
		return ret;
	}
	ALOGD("which:%d,,,status:%d\n",which,status);
	return ret;
}

static int led_dev_open(const hw_module_t* module, const char* id __unused,
                      hw_device_t** device __unused)
{
	struct led_device led = (struct led_device*)malloc(sizeof(struct led_device));
	if (!led) {
        ALOGE("Can not allocate memory for the led device");
        return -ENOMEM;
    }
    led->common.module = (hw_module_t *) module;
    led->common.close = led_close;
	led->led_open = led_open;
	led->led_ctrl = led_ctrl;
	device = (hw_device_t*)led;
	return 0;
}


static struct hw_module_methods_t led_module_methods = {
    .open = led_dev_open,
};

struct hw_module_t HAL_MODULE_INFO_SYM = {
    .module_api_version = VIBRATOR_API_VERSION,
    .hal_api_version = HARDWARE_HAL_API_VERSION,
    .id = "led",
    .name = "Default vibrator HAL",
    .author = "The Android Open Source Project",
    .methods = &vibrator_module_methods,
};

