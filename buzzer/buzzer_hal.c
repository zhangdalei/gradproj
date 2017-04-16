
/**  毕业设计	buzzer hal代码
  *	 by zhangdalei
  */
#define LOG_TAG "buzzer"

#include <hardware/hardware.h>
#include <hardware/buzzer_hal.h>

#include <cutils/log.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

//#include <errno.h>


//int fd = -1;

static int buz_open(struct buzzer_device_t* buzzer_dev)
{
	buzzer_dev->fd = open(BUZZER, O_RDWR);
	if(buzzer_dev < 0){
		ALOGE("open %s error",BUZZER);
		return -1;
	}
	ALOGI("open %s success",BUZZER);
	return 0;
}


static int buz_close(struct hw_device_t* device)
{
	struct buzzer_device_t* buzzer_dev = (struct buzzer_device_t*)device;
	close(buzzer_dev->fd);
	return 0;
}

static int buz_ioclt (struct buzzer_device_t* buzzer_dev, int cmd, unsigned long frec){
	int res;
	if(frec == 0){
		res = ioctl(buzzer_dev->fd, PWM_IOCTL_STOP);
		return 0;
	}
	switch(cmd){
		case PWM_IOCTL_SET_FREQ:
			res = ioctl(buzzer_dev->fd, PWM_IOCTL_SET_FREQ, frec);
			break;
		case PWM_IOCTL_STOP:
		default:
			res = ioctl(buzzer_dev->fd, PWM_IOCTL_STOP);
			break;	
	}
	
	return res;
}

/* 初始化buzzer_device_t */
static int buzzer_open(const hw_module_t* module, const char* id __unused,
                      hw_device_t** device __unused) {

    struct buzzer_device_t *buzzerdev = (struct buzzer_device_t *)calloc(1, sizeof(struct buzzer_device_t));

    if (!buzzerdev) {
        ALOGE("Can not allocate memory for the buzzer device");
        return -ENOMEM;
    }

    buzzerdev->common.tag = HARDWARE_DEVICE_TAG;
    buzzerdev->common.module = (hw_module_t *) module;
    buzzerdev->common.version = HARDWARE_DEVICE_API_VERSION(1,0);
    buzzerdev->common.close = buz_close;

    buzzerdev->buzzer_open = buz_open;
    buzzerdev->buzzer_ioclt = buz_ioclt;

	

    *device = (hw_device_t *) buzzerdev;

    return 0;
}

/*===========================================================================*/
/* Default buzzer HW module interface definition                           */
/*===========================================================================*/

static struct hw_module_methods_t buzzer_module_methods = {
    .open = buzzer_open,
};

// HAL_MODULE_INFO_SYM结构体
struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .module_api_version = BUZZER_API_VERSION,
    .hal_api_version = HARDWARE_HAL_API_VERSION,
    .id = BUZZER_HARDWARE_MODULE_ID,
    .name = "Default buzzer HAL",
    .author = "zhangdalei",
    .methods = &buzzer_module_methods,
};






