
#ifndef _HC501_HAL_H
#define _HC501_HAL_H

#include <hardware/hardware.h>

#define HC_501 "/dev/hc_501"


#define HC_HARDWARE_MODULE_ID "hc501"

#define HC_API_VERSION HARDWARE_MODULE_API_VERSION(1,0)

struct hc_device_t {
	struct hw_device_t common;
	int fd;
	int (*hc_open)(struct hc_device_t* hc_dev);
	int (*hc_read)(struct hc_device_t* hc_dev);
};




#endif


