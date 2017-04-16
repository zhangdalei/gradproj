
/* jni for buzzer servie zhangdalei's project */


#define LOG_TAG "buzzerservice"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/buzzer_hal.h>

#include <stdio.h>

namespace android
{

struct buzzer_device_t *buzzerdev;


static jint native_buzzerOpen(JNIEnv *env, jobject clazz)
{
	int err;
    hw_module_t* module;
	hw_device_t* device;
	ALOGI("open buzzer in jni\n");
	err = hw_get_module(BUZZER_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
	if(err ==0 )
	{
		err = module->methods->open(module,NULL,&device);
		if (err == 0) {
			 buzzerdev = (struct buzzer_device_t*)device;	//将hw_device_t转换为设备自定义结构体
			 if(!buzzerdev){
				ALOGE("buzzerdev is null! in jni \n");
			 }
			 buzzerdev->buzzer_open(buzzerdev);
			 ALOGI("open buzzer success in jni\n");
			} 
		else {
			ALOGI("open buzzer errors in jni\n");
			return 0;
		}
	}
	return 1;
}

static jint native_buzzerCtrl(JNIEnv *env, jobject clazz, jint cmd, jlong frec)
{
	return buzzerdev->buzzer_ioclt(buzzerdev, cmd, frec);
}

static JNINativeMethod method_table[] = {
    { "native_buzzerOpen", "()I", (void*)native_buzzerOpen },
    { "native_buzzerCtrl", "(IJ)I", (void*)native_buzzerCtrl }
};

int register_android_server_BuzzerService(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, "com/android/server/BuzzerService",
            method_table, NELEM(method_table));
}

};



