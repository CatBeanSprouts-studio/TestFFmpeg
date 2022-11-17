# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# so名称
project("testffmpeg")

# 头文件位置
set(LIB_PATH ${PROJECT_SOURCE_DIR}/../../../libs)
set(DIR ${LIB_PATH}/${ANDROID_ABI})

# 添加编码库
add_library(avcodec
        SHARED
        IMPORTED)
set_target_properties(
        avcodec
        PROPERTIES IMPORTED_LOCATION
        ${DIR}/libavcodec.so)

# 添加滤镜特效处理库
add_library(avfilter
        SHARED
        IMPORTED)
set_target_properties(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        ${DIR}/libavfilter.so
)

# 添加封装格式处理库
add_library(avformat
        SHARED
        IMPORTED)
set_target_properties(
        avformat
        PROPERTIES IMPORTED_LOCATION
        ${DIR}/libavformat.so
)

# 添加工具库
add_library(avutil
        SHARED
        IMPORTED)
set_target_properties(
        avutil
        PROPERTIES IMPORTED_LOCATION
        ${DIR}/libavutil.so
)


# 添加音频采样数据格式转换库
add_library(swresample
        SHARED
        IMPORTED)
set_target_properties(
        swresample
        PROPERTIES IMPORTED_LOCATION
        ${DIR}/libswresample.so
)

# 添加音频采样数据格式转换库
add_library(swscale
        SHARED
        IMPORTED)
set_target_properties(
        swscale
        PROPERTIES IMPORTED_LOCATION
        ${DIR}/libswscale.so
)

# 源文件
include_directories(
        ${LIB_PATH}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/a_tutorial
        ${CMAKE_CURRENT_SOURCE_DIR}/z_default
)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} MAIN_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/a_tutorial A_TUTORIAL_LIST)

add_library(
        testffmpeg
        SHARED
        ${MAIN_LIST}
        ${A_TUTORIAL_LIST}
)

find_library(
        log-lib
        log)

target_link_libraries(
        testffmpeg
        avcodec
        avfilter
        avformat
        avutil
        swresample
        swscale
        ${log-lib})