#include "qffmpegsdlplayer.h"
#include <QQuickWindow>
#include <../g-ffmpeg/src/gutil.h>

QFFMPEGSDLPlayer::QFFMPEGSDLPlayer(QObject *parent) : QObject(parent)
{
    sdlwnd_ = SDL_CreateWindowFrom(reinterpret_cast<QVariant*>(wnd_.winId()));
    assert(sdlwnd_);
}

QFFMPEGSDLPlayer::~QFFMPEGSDLPlayer()
{
    stop();
    if(sdlwnd_ != nullptr)
    {
        SDL_DestroyWindow(sdlwnd_);
        sdlwnd_ = nullptr;
    }
}

void QFFMPEGSDLPlayer::resizewnd()
{
    wnd_.setX(rect_->property("x").toInt());
    wnd_.setY(rect_->property("y").toInt());
    wnd_.setWidth(rect_->property("width").toInt());
    wnd_.setHeight(rect_->property("height").toInt());
    // 标记窗口变化再重新创建renderer,
    // 也可以修改sdl2源码void SDL_OnWindowResized(SDL_Window * window)函数中
    // 发送SDL_WINDOWEVENT_SIZE_CHANGED消息改为发送SDL_WINDOWEVENT_RESIZED消息(本人验证过成功)
    // 但我不太想改sdl2的源码
    bresize = true;
}

void QFFMPEGSDLPlayer::setRect(QQuickItem* r)
{
    rect_ = r;
    wnd_.setParent(r->parentItem()->window());
    QObject::connect(r, SIGNAL(xChanged()), this, SLOT(resizewnd()));
    QObject::connect(r, SIGNAL(yChanged()), this, SLOT(resizewnd()));
    QObject::connect(r, SIGNAL(widthChanged()), this, SLOT(resizewnd()));
    QObject::connect(r, SIGNAL(heightChanged()), this, SLOT(resizewnd()));
    resizewnd();
    rect_->setVisible(true);
    wnd_.hide();
}

int QFFMPEGSDLPlayer::play(const QString& uri)
{
    int ret = stop();
    CHECKFFRET(ret);

    ret = demux_.open(uri.toStdString().c_str());
    CHECKFFRET(ret);

    ret = demux_.get_steam_index(videovec_, audiovec_);
    CHECKFFRET(ret);
    const AVCodecParameters* vpar = nullptr;
    AVRational vtimebase, atimebase;
    const AVCodecParameters* apar = nullptr;
    if(videovec_.size() > 0)
    {
        ret = demux_.get_stream_par(videovec_.at(0), vpar, vtimebase);
        CHECKFFRET(ret);
        ret = vdec_.copy_param(vpar);
        CHECKFFRET(ret);
    }
    if(audiovec_.size() > 0)
    {
        ret = demux_.get_stream_par(audiovec_.at(0), apar, atimebase);
        CHECKFFRET(ret);
        ret = adec_.copy_param(apar);
        CHECKFFRET(ret);
    }

    bstop_ = false;
    resizewnd();
    std::thread th([&]
    {
        int ret = 0;
        decltype(gff::GetPacket()) packet = nullptr;
        auto frame = gff::GetFrame();

        while(true)
        {
            if(bstop_)
            {
                break;
            }
            packet = gff::GetPacket();
            ret = demux_.readpacket(packet);
            CHECKFFRET(ret);
            if (static_cast<unsigned int>(packet->stream_index) == videovec_.at(0))
            {
                if (vdec_.decode(packet, frame) >= 0)
                {
                    do
                    {  
                        if(bresize)
                        {
                            if(sdlrender_ != nullptr)
                            {
                                SDL_DestroyRenderer(sdlrender_);
                                sdlrender_ = nullptr;
                            }
                            assert(sdlwnd_);
                            sdlrender_ = SDL_CreateRenderer(sdlwnd_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                            assert(sdlrender_);
                            bresize = false;
                        }

                        SDL_Texture* texture = nullptr;
                        switch (static_cast<AVPixelFormat>(frame->format))
                        {
                        case (AV_PIX_FMT_YUV420P):
                            texture = SDL_CreateTexture(sdlrender_, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, frame->width, frame->height);
                            break;
                        default:
                            break;
                        }
                        if(texture != nullptr)
                        {      
                            ret = SDL_UpdateYUVTexture(texture, nullptr, frame->data[0], frame->linesize[0], frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
                            ret = SDL_RenderClear(sdlrender_);
                            ret = SDL_RenderCopy(sdlrender_, texture, nullptr, nullptr);
                            SDL_RenderPresent(sdlrender_);
                            SDL_DestroyTexture(texture);
                        }
                    } while (vdec_.decode(nullptr, frame) >= 0);
                }
            }
            else if (static_cast<unsigned int>(packet->stream_index) == audiovec_.at(0))
            {
                if (adec_.decode(packet, frame) >= 0)
                {
                    do
                    {
                    } while (adec_.decode(nullptr, frame) >= 0);
                }
            }
        }

        return 0;
    }
    );
    demuxth_.swap(th);

    rect_->setVisible(false);
    wnd_.show();

    return 0;
}

int QFFMPEGSDLPlayer::stop()
{
    bstop_ = true;
    if(demuxth_.joinable())
    {
        demuxth_.join();
    }
    vdec_.cleanup();
    adec_.cleanup();
    demux_.cleanup();

    if(sdlrender_ != nullptr)
    {
        SDL_DestroyRenderer(sdlrender_);
        sdlrender_ = nullptr;
    }
    rect_->setVisible(true);
    wnd_.hide();

    return 0;
}
