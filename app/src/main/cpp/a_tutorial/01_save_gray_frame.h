//
// Created by aw on 2022/10/10.
//

#ifndef TESTFFMPEG_01_SAVE_GRAY_FRAME_H
#define TESTFFMPEG_01_SAVE_GRAY_FRAME_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include "z_default/log.h"

int startFuc(const char *filename, const char *cacheDir);

static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame,
                         const char *cacheDir);

static void save_gray_frame(unsigned char *buf, int wrap, int xSize, int ySize, char *filename);
}

#endif //TESTFFMPEG_01_SAVE_GRAY_FRAME_H
