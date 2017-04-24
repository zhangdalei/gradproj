
/* jni for hc501 servie zhangdalei's gradugate project */


#define LOG_TAG "hcservice"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/hc501_hal.h>

#include <stdio.h>

namespace android
{

struct hc_device_t *hcdev;


static jint native_hcOpen(JNIEnv *env, jobject clazz)
{
	int err;
    hw_module_t* module;
	hw_device_t* device;
	ALOGI("open hc module hc in jni\n");
	err = hw_get_module(HC_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
	if(err == 0 )
	{
		err = module->methods->open(module,NULL,&device);
		if (err == 0) {
			 hcdev = (struct hc_device_t*)device;	//将hw_device_t转换为设备自定义结构体
			 if(!hcdev){
				ALOGE("hcdev is null! in jni \n");
				return 0;
			 }
			 err = hcdev->hc_open(hcdev);
			 if(err < 0){
				ALOGI("open hc501 device error in jni\n");
				return -1;
			 }
			 ALOGI("open hc501 device success in jni\n");
			} 
		else{
			ALOGI("open hc501 module error in jni\n");
		}
	}
	else {
			ALOGI("get hc501 module errors in jni\n");
			return -2;
		}
	return 1;
}

static jint native_hcRead(JNIEnv *env, jobject clazz)
{
	return hcdev->hc_read(hcdev);
}

static JNINativeMethod method_table[] = {
    { "native_hcOpen", "()I", (void*)native_hcOpen },
    { "native_hcRead", "()I", (void*)native_hcRead }
};

int register_android_server_HcService(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, "com/android/server/HcService",
            method_table, NELEM(method_table));
}

};

