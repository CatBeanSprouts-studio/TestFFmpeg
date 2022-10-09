//
// Created by aw on 2022/10/9.
//

#ifndef TESTFFMPEG_LOG_H
#define TESTFFMPEG_LOG_H

#define TAG "JNITag ------> "
#define info(...)    __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);
#define debug(...)   __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__);
#define error(...)   __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__);

#endif //TESTFFMPEG_LOG_H
