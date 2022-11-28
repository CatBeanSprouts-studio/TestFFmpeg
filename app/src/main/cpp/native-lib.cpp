#include <jni.h>
#include <string>

extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <android/log.h>
#include "z_default/log.h"

JNIEXPORT jstring JNICALL
Java_com_mk_testffmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject,
        jstring filename,
        jstring cacheDir) {
    return env->NewStringUTF("hello from native");
}
}