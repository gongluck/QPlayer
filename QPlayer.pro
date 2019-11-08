QT += quick widgets
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../g-ffmpeg/src/gdec.cpp \
        ../g-ffmpeg/src/gdemux.cpp \
        ../g-ffmpeg/src/genc.cpp \
        ../g-ffmpeg/src/gmux.cpp \
        ../g-ffmpeg/src/gswr.cpp \
        ../g-ffmpeg/src/gsws.cpp \
        ../g-ffmpeg/src/gutil.cpp \
        main.cpp \
        qffmpegsdlplayer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../g-ffmpeg/src/gavbase.h \
    ../g-ffmpeg/src/gdec.h \
    ../g-ffmpeg/src/gdemux.h \
    ../g-ffmpeg/src/genc.h \
    ../g-ffmpeg/src/gmux.h \
    ../g-ffmpeg/src/gswr.h \
    ../g-ffmpeg/src/gsws.h \
    ../g-ffmpeg/src/gutil.h \
    qffmpegsdlplayer.h

win32: LIBS += -L$$PWD/../g-ffmpeg/ffmpeg/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale

INCLUDEPATH += $$PWD/../g-ffmpeg/ffmpeg/inc
DEPENDPATH += $$PWD/../g-ffmpeg/ffmpeg/inc

win32: LIBS += -L$$PWD/SDL2/lib/x86 -lSDL2

INCLUDEPATH += $$PWD/SDL2/include
DEPENDPATH += $$PWD/SDL2/include
