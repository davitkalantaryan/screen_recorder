#   
# file:			qt_screen_recorder.pro  
# created on:	2020 Jul 13 
# created by:	D. Kalantaryan (davit.kalantaryan@gmail.com)  
#  


message("!!! $${_PRO_FILE_}")
include($${PWD}/../../common/common_qt/flagsandsys_common_private.pri)


CONFIG += console
QT += widgets

FFMPEG_DEV_ROOT = $$(FFMPEG_DEV_ROOT)

SOURSES_DIR = $${screenRecorderRepositoryRoot}/src/tests/ffmpeg_test01

win32 {
    #LIBS += -lD3D11
    LIBS += -lOle32
}

INCLUDEPATH += $${FFMPEG_DEV_ROOT}/include
LIBS += -L$${FFMPEG_DEV_ROOT}/bin
LIBS += -lavformat
LIBS += -lavcodec
LIBS += -lavutil
LIBS += -lswscale

SOURCES += $$files($${SOURSES_DIR}/*.cpp,false)

HEADERS += $$files($${SOURSES_DIR}/*.hpp,true)
