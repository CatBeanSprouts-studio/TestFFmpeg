//
// Created by aw on 2022/10/9.
//
#include "01_save_gray_frame.h"

int startFuc(const char *filename, const char *cacheDir) {
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (!pFormatContext) {
        error("Error could not allocate memory for Format Context.")
        return -1;
    }

    if (avformat_open_input(&pFormatContext, filename, nullptr, nullptr)) {
        error("Error could not open the file.")
        return -1;
    }

    if (avformat_find_stream_info(pFormatContext, nullptr) < 0) {
        error("Error could not get the stream info.")
        return -1;
    }

    const AVCodec *pCodec = nullptr;
    int video_stream_index = -1;

    for (int i = 0; i < pFormatContext->nb_streams; i++) {
        AVCodecParameters *pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        const AVCodec *pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);
        if (pLocalCodec == nullptr) {
            error("")
        }
    }

    return 1;
}