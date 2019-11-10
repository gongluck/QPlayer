#include "qffmpegsdlplayer.h"
#include <QQuickWindow>
#include <../g-ffmpeg/src/gutil.h>

QFFMPEGSDLPlayer::QFFMPEGSDLPlayer(QObject *parent) : QObject(parent)
{

}

QFFMPEGSDLPlayer::~QFFMPEGSDLPlayer()
{
    stop();
}

void QFFMPEGSDLPlayer::resizewnd()
{
    wnd_.setX(rect_->property("x").toInt());
    wnd_.setY(rect_->property("y").toInt());
    wnd_.setWidth(rect_->property("width").toInt());
    wnd_.setHeight(rect_->property("height").toInt());
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

    //sdlwnd_ = SDL_CreateWindowFrom(reinterpret_cast<QVariant*>(wnd_.winId()));
    sdlwnd_ = SDL_CreateWindow("test", 0 , 0, 400, 300, ::SDL_WINDOW_OPENGL | ::SDL_WINDOW_RESIZABLE);
    assert(sdlwnd_);
    sdlrender_ = SDL_CreateRenderer(sdlwnd_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assert(sdlrender_);

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
                        SDL_Texture* texture = nullptr;
                        byte* buf = nullptr;
                        switch (static_cast<AVPixelFormat>(frame->format))
                        {
                        case (AV_PIX_FMT_YUV420P):
                            texture = SDL_CreateTexture(sdlrender_, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, frame->width, frame->height);
                            buf = static_cast<byte*>(malloc(static_cast<size_t>(frame->width * frame->linesize[0] * 3 / 2)));
                            break;
                        default:
                            break;
                        }
                        if(texture != nullptr)
                        {
                            memcpy(buf, frame->data[0], static_cast<size_t>(frame->height * frame->linesize[0]));
                            memcpy(buf+frame->height*frame->linesize[0], frame->data[1], static_cast<size_t>(frame->height * frame->linesize[1] / 2));
                            memcpy(buf+frame->height*frame->linesize[0]*5/4, frame->data[2], static_cast<size_t>(frame->height * frame->linesize[2] / 2));
                            ret = SDL_UpdateTexture(texture, nullptr, buf, frame->linesize[0]);         
                            ret = SDL_RenderClear(sdlrender_);
                            ret = SDL_RenderCopy(sdlrender_, texture, nullptr, nullptr);
                            SDL_RenderPresent(sdlrender_);
                        }

                        if(texture != nullptr)
                        {
                            SDL_DestroyTexture(texture);
                        }
                        if(buf != nullptr)
                        {
                            free(buf);
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
    if(sdlwnd_ != nullptr)
    {
        SDL_DestroyWindow(sdlwnd_);
        sdlwnd_ = nullptr;
    }
    rect_->setVisible(true);
    wnd_.hide();

    return 0;
}
