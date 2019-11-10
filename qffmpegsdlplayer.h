#ifndef QFFMPEGSDLPLAYER_H
#define QFFMPEGSDLPLAYER_H

#include <QObject>
#include <QWindow>
#include <QQuickItem>

#include <../g-ffmpeg/src/gdemux.h>
#include <../g-ffmpeg/src/gdec.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <thread>

class QFFMPEGSDLPlayer : public QObject
{
    Q_OBJECT
public:
    explicit QFFMPEGSDLPlayer(QObject *parent = nullptr);
    ~QFFMPEGSDLPlayer();

    // 设置渲染区域
    void setRect(QQuickItem* r);


signals:

public slots:
    // 调整窗口大小
    void resizewnd();

    int play(const QString& uri);
    int stop();

private:
    QWindow wnd_;
    QQuickItem* rect_ = nullptr;

    SDL_Window* sdlwnd_ = nullptr;
    SDL_Renderer* sdlrender_ = nullptr;

    std::vector<unsigned int> videovec_, audiovec_;
    gff::gdemux demux_;
    gff::gdec vdec_;
    gff::gdec adec_;

    bool bstop_ = false;
    std::thread demuxth_;
};

#endif // QFFMPEGSDLPLAYER_H
