//
// repo:            scren_recorder
// file:            main_ffmpeg_test01.cpp
// path:            src/tests/ffmpeg_test01/main_ffmpeg_test01.cpp
// created on:      2025 Aug 10
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#include <qtutils/core/logger.hpp>
#include <cinternal/signals.h>
#include <cinternal/disable_compiler_warnings.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}
#include <qtutils/disable_utils_warnings.h>
#include <QImage>
#include <QDebug>
#include <QList>
#include <QScreen>
#include <QRect>
#include <QPixmap>
#include <QPainter>
#include <QCoreApplication>
#include <cinternal/undisable_compiler_warnings.h>


class FFmpegEncoder {
public:
    bool init(int w, int h, int fps, int bitrateKbps = 2500);
    void encodeQImage(const QImage& img) ;
    void flush() ;

    ~FFmpegEncoder() ;

private:
    struct FrameDeleter { void operator()(AVFrame* f) const { if (f) av_frame_free(&f); } };
    std::unique_ptr<AVFrame, FrameDeleter> yuv;

    AVCodecContext* c{nullptr};
    SwsContext* sws{nullptr};
    int width{0}, height{0}, frameRate{30};
    int64_t pts{0};
};


static int MakeAndHandleOneScreenshot(FFmpegEncoder* a_encoder_p);
int SendStream(const char* a_data, size_t a_dataSize);

static int s_loop = 0;

int main(int a_argc, char* a_argv[]) {
    QGuiApplication app(a_argc, a_argv);

    s_loop = 1;

    FFmpegEncoder encoder;
    int fps = 20;

    if (!encoder.init(1280, 920, fps)) {
        qWarning() << "Encoder init failed";
        return 1;
    }

    // Timer interval
    //int intervalMs = qMax(1, 1000 / fps);
    //connect(&timer, &QTimer::timeout, this, &ScreenStreamer::captureAndSend);
    //timer.start(intervalMs);
    //return true;

    while(s_loop){
        MakeAndHandleOneScreenshot(&encoder);
        CinternalSleepInterruptableMs(1000);
    }  //  while(s_loop){

    return 0;
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


static inline void encoderCheckSize(QImage* a_img_p) {
    static int encoderWidthCached{0};
    static int encoderHeightCached{0};
    const int newImageWidth = a_img_p->width();
    const int newImageHeight = a_img_p->height();
    if((newImageWidth!=encoderWidthCached)||(newImageHeight!=encoderHeightCached)){
        bool rescale = false;
        if(encoderWidthCached){
            rescale = true;
        }
        else{
            encoderWidthCached = newImageWidth;
        }

        if(encoderHeightCached){
            rescale = true;
        }
        else{
            encoderHeightCached = newImageHeight;
        }

        if(rescale){
            *a_img_p = a_img_p->scaled(encoderWidthCached, encoderHeightCached, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }

    }  //  if((newImageWidth!=encoderWidthCached)||(newImageHeight!=encoderHeightCached)){
}


static int MakeAndHandleOneScreenshot(FFmpegEncoder* a_encoder_p)
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

    if (aPixMapAll.isNull()) return -1;
    QImage img = aPixMapAll.toImage().convertToFormat(QImage::Format_ARGB32);
    encoderCheckSize(&img);
    a_encoder_p->encodeQImage(img);

    return 0;
}


static inline const char* ff_errstr(int err, char buf[AV_ERROR_MAX_STRING_SIZE]) {
    av_strerror(err, buf, AV_ERROR_MAX_STRING_SIZE);
    return buf;
}


static void log_ffmpeg_config_and_h264_encoders() {
    qInfo() << "FFmpeg config:" << avcodec_configuration();
    void* it = nullptr;
    const AVCodec* c = nullptr;
    qInfo() << "Available H.264 encoders:";
    while ((c = av_codec_iterate(&it))) {
        if (!av_codec_is_encoder(c)) continue;
        if (c->type != AVMEDIA_TYPE_VIDEO) continue;
        if (strstr(c->name, "264")) qInfo() << "  -" << c->name;
    }
}

//static AVPixelFormat choose_pix_fmt_for_encoder(const AVCodec* codec) {
//    // If encoder advertises supported pix_fmts, pick the first yuv420-compatible one.
//    if (codec->pix_fmts) {
//        for (const AVPixelFormat* p = codec->pix_fmts; *p != AV_PIX_FMT_NONE; ++p) {
//            if (*p == AV_PIX_FMT_YUV420P || *p == AV_PIX_FMT_NV12 || *p == AV_PIX_FMT_P010) return *p;
//        }
//        // fallback to first advertised
//        return codec->pix_fmts[0];
//    }
//    // Many external encoders (libx264) don’t fill pix_fmts; yuv420p is safe.
//    return AV_PIX_FMT_YUV420P;
//}


bool FFmpegEncoder::init(int, int, int, int ) {

    const AVCodec* codec = avcodec_find_encoder_by_name("h264_mf");
    if (!codec) {
        fprintf(stderr, "Codec '%s' not found\n", "h264_mf");
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = 352;
    c->height = 288;
    /* frames per second */
    c->time_base = {1, 25};
    c->framerate = {25, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);

    /* open it */
    int ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec: %s\n", "hi");
        exit(1);
    }

    return true;


#if 0

    // enforce even for yuv420p
    w &= ~1; h &= ~1;

    // remember in members (used elsewhere)
    width = w; height = h; frameRate = fps;
    //log_ffmpeg_config_and_h264_encoders();

    //av_register_all();

    // Prefer libx264; else try NVENC; else native "h264" (experimental)
    //const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    const AVCodec* codec = avcodec_find_encoder_by_name("h264_mf");
    //if (!codec) codec = avcodec_find_encoder_by_name("h264_nvenc");
    //if (!codec) codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) { qWarning() << "No H.264 encoder available"; return false; }

    qInfo() << "Using encoder:" << codec->name;

    //ctx = avcodec_alloc_context3(codec);
    //if (!ctx) return false;
    //
    //ctx->codec_type  = AVMEDIA_TYPE_VIDEO;
    //ctx->width       = width;
    //ctx->height      = height;
    //ctx->time_base   = AVRational{1, frameRate};
    //ctx->framerate   = AVRational{frameRate, 1};
    //ctx->pix_fmt     = AV_PIX_FMT_YUV420P;
    //ctx->gop_size    = frameRate;     // ~1s GOP
    //ctx->max_b_frames= 0;             // low latency
    //ctx->bit_rate    = (int64_t)bitrateKbps * 1000;
    //ctx->thread_count= 1;
    //
    //// If native encoder, allow experimental
    //if (std::string(codec->name) == "h264") {
    //    ctx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
    //}
    //
    //// Only set x264 private options when we really use libx264
    //if (std::string(codec->name) == "libx264" && ctx->priv_data) {
    //    av_opt_set(ctx->priv_data, "preset", "veryfast", 0);
    //    av_opt_set(ctx->priv_data, "tune",   "zerolatency", 0);
    //    // av_opt_set(ctx->priv_data, "profile","baseline", 0); // optional
    //    // don't set "annexb" here
    //}

    ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* put sample parameters */
    ctx->bit_rate = 400000;
    /* resolution must be a multiple of two */
    ctx->width = 352;
    ctx->height = 288;
    /* frames per second */
    ctx->time_base = {1, 25};
    ctx->framerate = {25, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    ctx->gop_size = 10;
    ctx->max_b_frames = 1;
    ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(ctx->priv_data, "preset", "slow", 0);

    /* open it */
    int ret = avcodec_open2(ctx, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec: %s\n", "hi");
        exit(1);
    }


    char err[AV_ERROR_MAX_STRING_SIZE]{};
    int rc = avcodec_open2(ctx, codec, nullptr);
    if (rc < 0) {
        qWarning() << "avcodec_open2 failed:" << ff_errstr(rc, err);
        avcodec_free_context(&ctx);
        return false;
    }

    // Allocate YUV frame buffer
    yuv.reset(av_frame_alloc());
    if (!yuv) return false;
    yuv->format = ctx->pix_fmt;
    yuv->width  = ctx->width;
    yuv->height = ctx->height;
    if (av_frame_get_buffer(yuv.get(), 32) < 0) return false;

    // BGRA (QImage) -> YUV420P scaler
    sws = sws_getContext(width, height, AV_PIX_FMT_BGRA,
                         width, height, AV_PIX_FMT_YUV420P,
                         SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws) return false;

    return true;

#endif

}


void FFmpegEncoder::encodeQImage(const QImage& img) {
    if (!c || !sws) return;

    // Ensure we have ARGB32/BGRA format for a predictable layout
    QImage frame = img.convertToFormat(QImage::Format_ARGB32); // ARGB32 in Qt uses BGRA byte order
    const uint8_t* srcData[4] = { frame.bits(), nullptr, nullptr, nullptr };
    int srcLinesize[4] = { (int)frame.bytesPerLine(), 0, 0, 0 };

    // Make sure our YUV frame buffer is writable
    if (av_frame_make_writable(yuv.get()) < 0) return;

    // Convert
    sws_scale(sws, srcData, srcLinesize, 0, height, yuv->data, yuv->linesize);

    // PTS
    yuv->pts = pts++;

    // Encode
    if (avcodec_send_frame(c, yuv.get()) < 0) return;

    AVPacket* pkt = av_packet_alloc();
    if (!pkt) return;
    while (true) {
        int ret = avcodec_receive_packet(c, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) { av_packet_free(&pkt); break; }
        if (ret < 0) { av_packet_free(&pkt); break; }

        // Send out encoded Annex B NAL units
        SendStream(reinterpret_cast<const char*>(pkt->data), static_cast<size_t>(pkt->size));
        av_packet_unref(pkt);
    }
}


void FFmpegEncoder::flush() {
    if (!c) return;
    avcodec_send_frame(c, nullptr);
    AVPacket* pkt = av_packet_alloc();
    if (!pkt) return;
    while (avcodec_receive_packet(c, pkt) == 0) {
        SendStream(reinterpret_cast<const char*>(pkt->data), static_cast<size_t>(pkt->size));
        av_packet_unref(pkt);
    }
    av_packet_free(&pkt);
}

FFmpegEncoder::~FFmpegEncoder() {
    if (c) {
        flush();
        avcodec_free_context(&c);
    }
    if (sws) sws_freeContext(sws);
}


int SendStream(const char* a_data, size_t a_dataSize)
{
    (void)a_data;
    (void)a_dataSize;
    return 0;
}
