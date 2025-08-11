#   
# file:			qt_screen_recorder.pro  
# created on:	2020 Jul 13 
# created by:	D. Kalantaryan (davit.kalantaryan@gmail.com)  
#  


message("!!! $${_PRO_FILE_}")
include($${PWD}/../../common/common_qt/flagsandsys_common_private.pri)


CONFIG += console
QT += widgets


SOURSES_DIR = $${screenRecorderRepositoryRoot}/src/tests/os_desktop_change_clbk_test

win32 {
    LIBS += -lD3D11
}

SOURCES += $$files($${SOURSES_DIR}/*.cpp,true)

HEADERS += $$files($${SOURSES_DIR}/*.hpp,true)
