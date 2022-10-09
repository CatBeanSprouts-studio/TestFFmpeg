#include <jni.h>
#include <string>

extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <android/log.h>
#include "z_default/log.h"

int startFuc(const char *filename, const char *cacheDir);
static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame,
                         const char *cacheDir);
static void save_gray_frame(unsigned char *buf, int wrap, int xSize, int ySize, char *fileName);

JNIEXPORT jstring JNICALL
Java_com_mk_testffmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject,
        jstring filename,
        jstring cacheDir) {
    char info[10000] = {0};
    const char *pId = env->GetStringUTFChars(filename, nullptr);
    const char *cacheDirId = env->GetStringUTFChars(cacheDir, nullptr);

    if (-1 == startFuc(pId, cacheDirId)) {
        error("ERROR start Func")
    } else {
        debug("start func successful")
    }
    sprintf(info, "Hello ffmpeg\n%s", avcodec_configuration());
    return env->NewStringUTF(info);
}

int startFuc(const char *filename, const char *cacheDir) {
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (!pFormatContext) {
        error(" ------> Error could not allocate memory for Format Context")
        return -1;
    }

    if (avformat_open_input(&pFormatContext, filename, nullptr, nullptr) != 0) {
        error(" ------> Error could not open the file")
        return -1;
    }

    info(" ------> Format: %s, Duration: %ld us, Bit_rate: %ld", pFormatContext->iformat->name,
         pFormatContext->duration, pFormatContext->bit_rate)

    info(" ------> Finding stream info from format")

    if (avformat_find_stream_info(pFormatContext, nullptr) < 0) {
        error(" ------> ERROR could not get the stream info")
        return -1;
    }

    const AVCodec *pCodec = nullptr;
    int video_stream_index = -1;
    AVCodecParameters *pCodecParameters = nullptr;

    for (int i = 0; i < pFormatContext->nb_streams; i++) {
        AVCodecParameters *pLocalCodecParameters = pFormatContext->streams[i]->codecpar;

        info(" ------> AVStream->time_base before open coded %d  %d",
             pFormatContext->streams[i]->time_base.num,
             pFormatContext->streams[i]->time_base.den)

        info(" ------> AVStream->r_frame before open coded %d  %d",
             pFormatContext->streams[i]->r_frame_rate.num,
             pFormatContext->streams[i]->r_frame_rate.den)

        info(" ------> AVStream->start_time %" PRId64,
             pFormatContext->streams[i]->start_time)

        info(" ------> AVStream->duration %" PRId64,
             pFormatContext->streams[i]->duration)

        info(" ------> finding the proper decoder (CODEC)")

        const AVCodec *pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);
        if (pLocalCodec == nullptr) {
            error(" ------> ERROR unsupported codec!")
            continue;
        }

        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            if (video_stream_index == -1) {
                video_stream_index = i;
                pCodec = pLocalCodec;
                pCodecParameters = pLocalCodecParameters;
            }

            info(" ------> Video Codec: resolution %d x %d",
                 pLocalCodecParameters->width,
                 pLocalCodecParameters->height)

        } else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {

            info(" ------> Audio Codec: %d channels, sample rate %d",
                 pLocalCodecParameters->channels,
                 pLocalCodecParameters->sample_rate)
        }

        info("\tCodec %s ID %d bit_rate %ld", pLocalCodec->name, pLocalCodec->id,
             pLocalCodecParameters->bit_rate)
    }
    if (video_stream_index == -1) {
        error(" ------> File %s does not contain a video stream!", filename)
        return -1;
    }

    AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
    if (!pCodecContext) {
        error(" ------> failed to allocated memory for AVCodecContext")
        return -1;
    }

    if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0) {
        error(" ------> failed to copy codec params to codec context")
        return -1;
    }

    if (avcodec_open2(pCodecContext, pCodec, nullptr) < 0) {
        error(" ------> failed to open codec through avcodec_open2")
        return -1;
    }

    AVFrame *pFrame = av_frame_alloc();
    if (!pFrame) {
        error(" ------> failed to allocate memory for AVFrame")
        return -1;
    }

    AVPacket *pPacket = av_packet_alloc();
    if (!pPacket) {
        error(" ------> failed to allocate memory for AVPacket")
        return -1;
    }

    int response;
    int how_many_packets_to_process = 8;

    while (av_read_frame(pFormatContext, pPacket) >= 0) {

        if (pPacket->stream_index == video_stream_index) {
            info(" ------> AVPacket->pts %" PRId64, pPacket->pts)
            response = decode_packet(pPacket, pCodecContext, pFrame, cacheDir);
            if (response < 0)
                break;

            if (--how_many_packets_to_process <= 0) break;
        }

        av_packet_unref(pPacket);
    }

    info(" ------> releasing all the resources")

    avformat_close_input(&pFormatContext);
    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecContext);

    return 1;
}

static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame,
                         const char *cacheDir) {
    int response = avcodec_send_packet(pCodecContext, pPacket);

    if (response < 0) {
        error(" ------> Error while sending a packet to the decoder: %s", av_err2str(response))
        return response;
    }

    while (response >= 0) {
        response = avcodec_receive_frame(pCodecContext, pFrame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            break;
        } else if (response < 0) {
            error(" ------> Error while sending a packet to the decoder: %s", av_err2str(response))
            return response;
        }

        if (response >= 0) {
            info(" ------> Frame %d (type = %c, size = %d bytes, format = %d) pts %ld key_frame %d [DTS %d]",
                 pCodecContext->frame_number,
                 av_get_picture_type_char(pFrame->pict_type),
                 pFrame->pkt_size,
                 pFrame->format,
                 pFrame->pts,
                 pFrame->key_frame,
                 pFrame->coded_picture_number)

            char frame_filename[1024];
            snprintf(frame_filename, sizeof(frame_filename), "%s/%s-%d.pgm", cacheDir,
                     "frame", pCodecContext->frame_number);

            if (pFrame->format != AV_PIX_FMT_YUV420P) {
                info(" ------> Warning: the generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB")
            }

            save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height,
                            frame_filename);
        }
    }

    return 0;
}

static void save_gray_frame(unsigned char *buf, int wrap, int xSize, int ySize, char *fileName) {
    FILE *f;
    int index;
    f = fopen(fileName, "w");

    if (f == nullptr) {
        error(" ------> error open file: %s", fileName)
        return;
    }

    fprintf(f, "P5\n%d %d\n%d\n", xSize, ySize, 255);

    for (index = 0; index < ySize; index++) {
        fwrite(buf + index * wrap, 1, xSize, f);
    }
    fclose(f);
}
}