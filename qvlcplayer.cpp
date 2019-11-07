#include "qvlcplayer.h"
#include <QQuickWindow>

QVLCPlayer::QVLCPlayer(QObject *parent) : QObject(parent)
{

}

QVLCPlayer::~QVLCPlayer()
{
    stop();
}

void QVLCPlayer::resizewnd()
{
    wnd_.setX(rect_->property("x").toInt());
    wnd_.setY(rect_->property("y").toInt());
    wnd_.setWidth(rect_->property("width").toInt());
    wnd_.setHeight(rect_->property("height").toInt());
    wnd_.show();
}

void QVLCPlayer::setRect(QQuickItem* r)
{
    rect_ = r;
    wnd_.setParent(r->parentItem()->window());
    QObject::connect(r, SIGNAL(xChanged()), this, SLOT(resizewnd()));
    QObject::connect(r, SIGNAL(yChanged()), this, SLOT(resizewnd()));
    QObject::connect(r, SIGNAL(widthChanged()), this, SLOT(resizewnd()));
    QObject::connect(r, SIGNAL(heightChanged()), this, SLOT(resizewnd()));
}

int QVLCPlayer::play(const QString& uri)
{
    stop();
    inst_ = libvlc_new(0, nullptr);
    if(inst_ == nullptr)
    {
        qDebug("libvlc_new");
        return -1;
    }
    media_ = libvlc_media_new_path(inst_, uri.toStdString().c_str());
    if(media_ == nullptr)
    {
        qDebug("libvlc_media_new_path");
        return -2;
    }
    player_ = libvlc_media_player_new_from_media(media_);
    if(player_ == nullptr)
    {
        qDebug("libvlc_media_player_new_from_media");
        return -3;
    }

    // 设置窗口句柄
    libvlc_media_player_set_hwnd(player_, reinterpret_cast<QVariant*>(wnd_.winId()));

    // 播放文件
    libvlc_media_player_play(player_);

    wnd_.show();
    resizewnd();

    return 0;
}

int QVLCPlayer::stop()
{
    if(player_ != nullptr)
    {
        libvlc_media_player_stop(player_);
        libvlc_media_player_release(player_);
    }
    if(media_ != nullptr)
    {
        libvlc_media_release(media_);
    }
    if(inst_ != nullptr)
    {
        libvlc_release(inst_);
    }

    player_ = nullptr;
    media_ = nullptr;
    inst_ = nullptr;

    return 0;
}
