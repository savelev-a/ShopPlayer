#-------------------------------------------------
#
# Project created by QtCreator 2017-08-10T13:22:23
#
#-------------------------------------------------

QT       += core gui widgets multimedia network

TARGET = ShopPlayer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += src/main.cpp\
        src/ui/mainwindow.cpp \
    src/mplayer/mediaplayer.cpp \
    src/application.cpp \
    src/ui/positionslider.cpp \
    src/model/audioclip.cpp \
    src/model/playlistmodel.cpp \
    src/ui/settingswindow.cpp \
    src/ui/volumebutton.cpp \
    src/network/networkclient.cpp \
    src/ui/downloadframe.cpp

HEADERS  += src/ui/mainwindow.h \
    src/mplayer/mediaplayer.h \
    src/application.h \
    src/ui/positionslider.h \
    src/model/audioclip.h \
    src/model/playlistmodel.h \
    src/ui/settingswindow.h \
    src/ui/volumebutton.h \
    src/network/networkclient.h \
    src/ui/downloadframe.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/settingswindow.ui

win32:RC_ICONS += icon.ico

RESOURCES += \
    res.qrc
