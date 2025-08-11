

#define MAKE_REAL_SCREENSHOT
#define FRAME_RATE_TEST     25


#ifdef _MSC_VER
#pragma warning (disable:4996)
#endif

#include <qtutils/core/logger.hpp>
#include <cinternal/signals.h>
#include <cinternal/disable_compiler_warnings.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <objbase.h>
#ifdef MAKE_REAL_SCREENSHOT
struct timezone
{
    int tz_minuteswest;		/* Minutes west of GMT.  */
    int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
};
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
#if(_MSC_VER >= 1400)
#define InterlockedCompareExchangePointerNew InterlockedCompareExchangePointer
#else
#define InterlockedCompareExchangePointerNew InterlockedCompareExchange
#endif
static int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif
#else
#include <sys/time.h>
#endif

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>   // <-- added
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/error.h>
}

#include <qtutils/disable_utils_warnings.h>
#include <QImage>
#include <QDebug>
#include <QList>
#include <QScreen>
#include <QRect>
#include <QPixmap>
#include <QPainter>
#include <QApplication>
#include <QThread>

#include <cinternal/undisable_compiler_warnings.h>

static inline const char* ff_errstr(int err, char buf[AV_ERROR_MAX_STRING_SIZE]) {
    av_strerror(err, buf, AV_ERROR_MAX_STRING_SIZE);
    return buf;
}


class EncoderThread : public QThread {
private:
    void run() override ;
    void MainLoop();
};

struct SEncodeParams{
    AVCodecContext* ctx;
    AVFrame* frame;
    AVPacket* pkt;
    AVFormatContext* fmt_ctx;
    AVStream* st;
};

static void InitEncoderData(SEncodeParams* a_ep_p);
static void FillVideoWithAllFrames(SEncodeParams* a_ep_p);
static uint64_t GetMilisecondsFromEpoch();

static int s_loop = 0;

static void encode(AVCodecContext *enc_ctx,
                   AVFrame *frame,
                   AVPacket *pkt,
                   AVFormatContext* fmt_ctx,  // <-- changed
                   int stream_index)          // <-- changed
{
    int ret;

    if (frame)
        printf("Send frame %3" PRId64 "\n", frame->pts);

    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }

        // mux: set stream index and rescale timestamps to stream time_base
        pkt->stream_index = stream_index;
        av_packet_rescale_ts(pkt, enc_ctx->time_base, fmt_ctx->streams[stream_index]->time_base);

        printf("Write packet %3" PRId64 " (size=%5d)\n", pkt->pts, pkt->size);
        int wr = av_interleaved_write_frame(fmt_ctx, pkt);
        av_packet_unref(pkt);
        if (wr < 0) {
            char err[AV_ERROR_MAX_STRING_SIZE]{};
            fprintf(stderr, "Error writing packet: %s\n", ff_errstr(wr, err));
            exit(1);
        }
    }
}

int main(int a_argc, char** a_argv)
{
    QApplication aApp(a_argc,a_argv);
    EncoderThread aThread;

    s_loop = 1;

    aThread.start();

    CinternalSleepInterruptableMs(15000);

    s_loop = 0;

    aThread.quit();
    aThread.wait();

    return 0;
}



void EncoderThread::run()
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    MainLoop();

    CoUninitialize();
}


void EncoderThread::MainLoop()
{
    SEncodeParams aEp = {nullptr,nullptr,nullptr,nullptr,nullptr};

    InitEncoderData(&aEp);
    FillVideoWithAllFrames(&aEp);

    // flush
    encode(aEp.ctx, nullptr, aEp.pkt, aEp.fmt_ctx, aEp.st->index);

    // trailer / close
    av_write_trailer(aEp.fmt_ctx);
    if (!(aEp.fmt_ctx->oformat->flags & AVFMT_NOFILE))
        avio_closep(&aEp.fmt_ctx->pb);

    avcodec_free_context(&aEp.ctx);
    av_frame_free(&aEp.frame);
    av_packet_free(&aEp.pkt);
    avformat_free_context(aEp.fmt_ctx);
}


#ifdef MAKE_REAL_SCREENSHOT


static inline uint8_t clamp_u8(int v) {
    if (v < 0)   return 0;
    if (v > 255) return 255;
    return (uint8_t)v;
}


static inline void encoderCheckSize(QImage* a_img_p, const SEncodeParams* a_ep_p) {
    const int newImageWidth = a_img_p->width();
    const int newImageHeight = a_img_p->height();
    if((newImageWidth!=(a_ep_p->ctx->width))||(newImageHeight!=(a_ep_p->ctx->height))){
        *a_img_p = a_img_p->scaled(a_ep_p->ctx->width,a_ep_p->ctx->height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }  //  if((newImageWidth!=encoderWidthCached)||(newImageHeight!=encoderHeightCached)){
}


static inline QPixmap ConcatenatePixmaps(
    QRect* CPPUTILS_ARG_NN a_pReturnRect,
    const QPixmap& a_pixmap1, const QPixmap& a_pixmap2,
    QRect a_rect1, QRect a_rect2)
{
    // Find the bounding rectangle that contains both a_rect1 and a_rect2
    *a_pReturnRect = a_rect1.united(a_rect2);

    // Create the result pixmap with the size of the united rectangle
    QPixmap result(a_pReturnRect->size());
    result.fill(Qt::transparent); // Fill the result pixmap with transparency

    // Create a QPainter to draw the pixmaps on the result pixmap
    QPainter painter(&result);

    // Draw the first pixmap at its designated position according to a_rect1
    painter.drawPixmap(a_rect1.topLeft() - a_pReturnRect->topLeft(), a_pixmap1);

    // Draw the second pixmap at its designated position according to a_rect2
    painter.drawPixmap(a_rect2.topLeft() - a_pReturnRect->topLeft(), a_pixmap2);

    return result;
}


static inline QImage GetScreenshot()
{
    static int snIteration = 0;
    QScreen* pScreen=nullptr;
    QRect aRectAll, aRect;
    QPixmap aPixMapAll;
    const QList<QScreen*> allScreens = QGuiApplication::screens();
    const qsizetype screensCount = allScreens.size();
    qsizetype i;

    QtUtilsDebug()<<++snIteration<<".  screens count:"<<screensCount;

    for(i=0; i<screensCount;++i){
        pScreen = allScreens.at(i);
        if(pScreen){
            ++i;
            aRectAll = pScreen->geometry();
            aPixMapAll = pScreen->grabWindow(0);
            break;
        }  //  if(pScreen){
    }  //  for(i=0; i<screensCount;++i){

    if(pScreen){
        for(; i<screensCount;++i){
            pScreen = allScreens.at(i);
            if(pScreen){
                aRect = pScreen->geometry();
                aPixMapAll = ConcatenatePixmaps(&aRectAll,aPixMapAll,pScreen->grabWindow(0),aRectAll,aRect);
            }  //  if(pScreen){
        }  //  for(; i<cnCount;++i){
    }  //  if(pScreen){

    if (aPixMapAll.isNull()) return QImage();
    const QImage img = aPixMapAll.toImage().convertToFormat(QImage::Format_ARGB32);
    //encoderCheckSize(&img,a_ep_p);
    //a_encoder_p->encodeQImage(img);

    return img;
}


static uint64_t GetMilisecondsFromEpoch()
{
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    return ((uint64_t)tv.tv_sec)*1000 + (uint64_t)tv.tv_usec/1000;
}


#endif


static void InitEncoderData(SEncodeParams* a_ep_p)
{

    const char *filename, *codec_name;
    const AVCodec *codec;
    int ret;

    filename   = "out.mp4";
    codec_name = "h264_mf"; // keep your Media Foundation encoder (or "libx264" if preferred)

    // find encoder
    codec = avcodec_find_encoder_by_name(codec_name);
    if (!codec) {
        fprintf(stderr, "Codec '%s' not found\n", codec_name);
        exit(1);
    }

    a_ep_p->ctx = avcodec_alloc_context3(codec);
    if (!(a_ep_p->ctx)) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

#ifdef MAKE_REAL_SCREENSHOT

    const QImage img = GetScreenshot();
    if(img.isNull()){
        exit(1);
    }
    a_ep_p->ctx->width = img.width();
    a_ep_p->ctx->height = img.height();

#else

    // basic encoding params
    a_ep_p->ctx->width    = 352;
    a_ep_p->ctx->height   = 288;

#endif

    a_ep_p->ctx->bit_rate = a_ep_p->ctx->width * a_ep_p->ctx->height * (100/FRAME_RATE_TEST);
    a_ep_p->ctx->time_base = AVRational{1, FRAME_RATE_TEST};
    a_ep_p->ctx->framerate = AVRational{FRAME_RATE_TEST, 1};
    a_ep_p->ctx->gop_size  = 10;
    a_ep_p->ctx->max_b_frames = 1;
    a_ep_p->ctx->pix_fmt   = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(a_ep_p->ctx->priv_data, "preset", "slow", 0);

    // ---------- container (very small addition) ----------
    ret = avformat_alloc_output_context2(&(a_ep_p->fmt_ctx), nullptr, nullptr, filename); // auto-detect MP4 by extension
    if (ret < 0 || !a_ep_p->fmt_ctx) {
        char err[AV_ERROR_MAX_STRING_SIZE]{};
        fprintf(stderr, "Could not alloc output context: %s\n", ff_errstr(ret, err));
        exit(1);
    }

    // MP4 often wants global headers
    if (a_ep_p->fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
        a_ep_p->ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // create one stream and copy codec params
    a_ep_p->st = avformat_new_stream(a_ep_p->fmt_ctx, nullptr);
    if (!a_ep_p->st) { fprintf(stderr, "Could not create stream\n"); exit(1); }
    a_ep_p->st->time_base = AVRational{1, 25}; // stream time base

    // open encoder after setting ctx flags
    ret = avcodec_open2(a_ep_p->ctx, codec, nullptr);
    if (ret < 0) {
        char err[AV_ERROR_MAX_STRING_SIZE]{};
        fprintf(stderr, "Could not open codec: %s\n", ff_errstr(ret, err));
        exit(1);
    }

    ret = avcodec_parameters_from_context(a_ep_p->st->codecpar, a_ep_p->ctx);
    if (ret < 0) {
        fprintf(stderr, "Could not copy codec parameters to stream\n");
        exit(1);
    }

    // open the output file (MP4)
    if (!(a_ep_p->fmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&a_ep_p->fmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            char err[AV_ERROR_MAX_STRING_SIZE]{};
            fprintf(stderr, "Could not open output file: %s\n", ff_errstr(ret, err));
            exit(1);
        }
    }

    // write header
    ret = avformat_write_header(a_ep_p->fmt_ctx, nullptr);
    if (ret < 0) {
        char err[AV_ERROR_MAX_STRING_SIZE]{};
        fprintf(stderr, "Error occurred when opening output file: %s\n", ff_errstr(ret, err));
        exit(1);
    }
    // -----------------------------------------------------

    a_ep_p->frame = av_frame_alloc();
    if (!a_ep_p->frame) { fprintf(stderr, "Could not allocate video frame\n"); exit(1); }
    a_ep_p->frame->format = a_ep_p->ctx->pix_fmt;
    a_ep_p->frame->width  = a_ep_p->ctx->width;
    a_ep_p->frame->height = a_ep_p->ctx->height;

    ret = av_frame_get_buffer(a_ep_p->frame, 0);
    if (ret < 0) { fprintf(stderr, "Could not allocate the video frame data\n"); exit(1); }

    a_ep_p->pkt = av_packet_alloc();
    if (!a_ep_p->pkt) exit(1);

}



static void FillVideoWithAllFrames(SEncodeParams* a_ep_p)
{

#ifdef MAKE_REAL_SCREENSHOT

    const int w = a_ep_p->ctx->width;
    const int h = a_ep_p->ctx->height;
    const uint64_t startTime = GetMilisecondsFromEpoch();
    const uint64_t intervalPerTimeUnitMs = 1000 / FRAME_RATE_TEST;
    uint64_t currentTime;

    while (s_loop) {
        QImage img = GetScreenshot();
        if (img.isNull()) {
            // no frame this tick — skip, keep pts moving
            continue;
        }

        currentTime = GetMilisecondsFromEpoch();

        // Ensure format + size expected by encoder
        if (img.format() != QImage::Format_ARGB32)
            img = img.convertToFormat(QImage::Format_ARGB32);
        if (img.width() != w || img.height() != h)
            img = img.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        // Frame must be writable before filling
        int ret = av_frame_make_writable(a_ep_p->frame);
        if (ret < 0) {
            fprintf(stderr, "av_frame_make_writable failed\n");
            return;
        }

        uint8_t* y_plane = a_ep_p->frame->data[0];
        uint8_t* u_plane = a_ep_p->frame->data[1];
        uint8_t* v_plane = a_ep_p->frame->data[2];
        const int y_stride = a_ep_p->frame->linesize[0];
        const int u_stride = a_ep_p->frame->linesize[1];
        const int v_stride = a_ep_p->frame->linesize[2];

        // --- Luma (Y): per pixel ---
        for (int y = 0; y < h; ++y) {
            const uint8_t* src = img.constScanLine(y);
            uint8_t* y_out = y_plane + y * y_stride;

            for (int x = 0; x < w; ++x) {
                // QImage ARGB32 on little endian: B, G, R, A
                const int B = src[0];
                const int G = src[1];
                const int R = src[2];

                // BT.601 limited range
                // Y = ( 66R + 129G + 25B + 128) >> 8) + 16
                int Y = (( 66 * R + 129 * G +  25 * B + 128) >> 8) + 16;
                *y_out++ = clamp_u8(Y);

                src += 4;
            }
        }

        // --- Chroma (U,V): 4:2:0, average over 2x2 block ---
        for (int y = 0; y < h; y += 2) {
            const uint8_t* s0 = img.constScanLine(y);
            const uint8_t* s1 = (y + 1 < h) ? img.constScanLine(y + 1) : img.constScanLine(y);

            uint8_t* u_out = u_plane + (y / 2) * u_stride;
            uint8_t* v_out = v_plane + (y / 2) * v_stride;

            for (int x = 0; x < w; x += 2) {
                // Fetch 2x2 pixels, average R/G/B
                const uint8_t* p00 = s0 + (x + 0) * 4;
                const uint8_t* p01 = (x + 1 < w) ? (s0 + (x + 1) * 4) : p00;
                const uint8_t* p10 = s1 + (x + 0) * 4;
                const uint8_t* p11 = (x + 1 < w) ? (s1 + (x + 1) * 4) : p10;

                int Rsum = p00[2] + p01[2] + p10[2] + p11[2];
                int Gsum = p00[1] + p01[1] + p10[1] + p11[1];
                int Bsum = p00[0] + p01[0] + p10[0] + p11[0];

                // round-to-nearest average
                int R = (Rsum + 2) >> 2;
                int G = (Gsum + 2) >> 2;
                int B = (Bsum + 2) >> 2;

                // BT.601 limited range
                // U = ((-38R - 74G + 112B + 128) >> 8) + 128
                // V = (( 112R - 94G - 18B + 128) >> 8) + 128
                int U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
                int V = (( 112 * R - 94 * G -  18 * B + 128) >> 8) + 128;

                *u_out++ = clamp_u8(U);
                *v_out++ = clamp_u8(V);
            }
        }

        // PTS in codec time_base (1/FRAME_RATE_TEST)
        a_ep_p->frame->pts = (currentTime-startTime) / intervalPerTimeUnitMs;

        // encode + mux
        encode(a_ep_p->ctx, a_ep_p->frame, a_ep_p->pkt, a_ep_p->fmt_ctx, a_ep_p->st->index);
        CinternalSleepInterruptableMs(25);
    }


#else

    int i, ret, x, y;

    // encode ~10 seconds
    for (i = 0; i < 250; i++) {
        ret = av_frame_make_writable(a_ep_p->frame);
        if (ret < 0) exit(1);

        // dummy pattern
        for (y = 0; y < a_ep_p->ctx->height; y++)
            for (x = 0; x < a_ep_p->ctx->width; x++)
                a_ep_p->frame->data[0][y * a_ep_p->frame->linesize[0] + x] = x + y + i * 3;

        for (y = 0; y < a_ep_p->ctx->height/2; y++)
            for (x = 0; x < a_ep_p->ctx->width/2; x++) {
                a_ep_p->frame->data[1][y * a_ep_p->frame->linesize[1] + x] = 128 + y + i * 2;
                a_ep_p->frame->data[2][y * a_ep_p->frame->linesize[2] + x] =  64 + x + i * 5;
            }

        a_ep_p->frame->pts = i; // in codec time_base (1/25)

        encode(a_ep_p->ctx, a_ep_p->frame, a_ep_p->pkt, a_ep_p->fmt_ctx, a_ep_p->st->index);
    }

#endif

}


#ifdef _WIN32
#ifdef MAKE_REAL_SCREENSHOT

static int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    static void* spPointer = NULL;
    FILETIME ft;
    unsigned __int64 tmpres = 0;
#if(_MSC_VER >= 1400)
    long lnTzTemp;
    int nldTemp;
#endif

    if (tv)
    {
        GetSystemTimeAsFileTime(&ft);

        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

        tmpres /= 10;  /*convert into microseconds*/
        /*converting file time to unix epoch*/
        tmpres -= DELTA_EPOCH_IN_MICROSECS;
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }

    if (tz)
    {
        // (setting timezone environmental variable) once
        if (InterlockedCompareExchangePointerNew(&spPointer, (void*)1, NULL) == NULL)
        {
            _tzset();
        }
#if(_MSC_VER >= 1400)
        _get_timezone(&lnTzTemp);
        tz->tz_minuteswest = lnTzTemp / 60;
        _get_daylight(&nldTemp);
        tz->tz_dsttime = nldTemp;
#else
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
#endif
    }

    return 0;
}

#endif
#endif
