
/**  毕业设计	hc-rs501 hal代码
  *	 by zhangdalei
  */
#define LOG_TAG "hc501"

#include <hardware/hardware.h>
#include <hardware/hc501_hal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

#include <cutils/log.h>

static int hc_open(struct hc_device_t* hc_dev)
{
	hc_dev->fd = open(HC_501, O_RDWR);
	if(hc_dev->fd < 0){
		ALOGE("open %s error",HC_501);
		return -1;
	}
	ALOGI("open %s success",HC_501);
	return 0;
}


static int hc_close(struct hw_device_t* device)
{
	struct hc_device_t* hc_dev = (struct hc_device_t*)device;
	close(hc_dev->fd);
	free(hc_dev);
	return 0;
}

int hc_read(struct hc_device_t* hc_dev)
{
	int ret = 0;
	char buf[2];
	read(hc_dev->fd, buf, 1);
	if(buf[0] == '1')
	{
		ALOGI("read hc data %s in hal\n", buf);
		return 1;
	}
	if(buf[0] == '0')
	{
		ALOGI("read hc data %s in hal\n",buf);
		return 0;
	}
	return 2;
}


/* 初始化buzzer_device_t */
static int hcdev_open(const hw_module_t* module, const char* id __unused,
                      hw_device_t** device __unused) {

    struct hc_device_t *hcdev = (struct hc_device_t *)calloc(1, sizeof(struct hc_device_t));

    if (!hcdev) {
        ALOGE("Can not allocate memory for the hc501 device");
        return -ENOMEM;
    }

    hcdev->common.tag = HARDWARE_DEVICE_TAG;
    hcdev->common.module = (hw_module_t *) module;
    hcdev->common.version = HARDWARE_DEVICE_API_VERSION(1,0);
    hcdev->common.close = hc_close;

    hcdev->hc_open= hc_open;
    hcdev->hc_read= hc_read;

    *device = (hw_device_t *) hcdev;

    return 0;
}

/*===========================================================================*/
/* Default buzzer HW module interface definition                           */
/*===========================================================================*/

static struct hw_module_methods_t hc_module_methods = {
    .open = hcdev_open,
};

// HAL_MODULE_INFO_SYM结构体
struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .module_api_version = HC_API_VERSION,
    .hal_api_version = HARDWARE_HAL_API_VERSION,
    .id = HC_HARDWARE_MODULE_ID,
    .name = "Default hc HAL",
    .author = "zhangdalei",
    .methods = &hc_module_methods,
};




