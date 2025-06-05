#   
# file:			qt_screen_recorder.pro  
# created on:	2020 Jul 13 
# created by:	D. Kalantaryan (davit.kalantaryan@gmail.com)  
#  


message("!!! $${_PRO_FILE_}")
include($${PWD}/../../common/common_qt/flagsandsys_common_private.pri)

QSR_APPLICATION_VERSION_VAR=1.1.8

DEFINES +=	QSR_APPLICATION_VERSION=\\\"$${QSR_APPLICATION_VERSION_VAR}\\\"

#win32: debug: CONFIG += console
QT += widgets

SOURCES			+=		\
	$${PWD}/../../../src/ui/qt_screen_recorder/main_qt_screen_recorder.cpp					\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_application.cpp			\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_worker.cpp				\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_earlyparams.cpp			\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_settings.cpp				\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_maintab.cpp				\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_centralwidget.cpp			\
	$${PWD}/../../../src/core/core_prepare_gif_file.cpp


HEADERS			+=		\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_application.hpp			\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_worker.hpp				\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_earlyparams.hpp			\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_settings.hpp				\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_maintab.hpp				\
	$${PWD}/../../../src/ui/qt_screen_recorder/qt_screen_recorder_centralwidget.hpp			\
	$${PWD}/../../../include/core_prepare_gif_file.h


RESOURCES		+=		\
	$${PWD}/../../../src/resources/qt_screen_recorder.qrc


OTHER_FILES		+=		\
	$${PWD}/../../../.gitignore																\
	$${PWD}/../../../README.md
