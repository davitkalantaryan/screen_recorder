#
# file:			qt_multimedia_test01.pro
# path:			prj/tests/qt_multimedia_test01_qt/qt_multimedia_test01.pro
# created on:		2023 Jan 15
# Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
#
# see: https://www.qt.io/blog/qt-multimedia-in-qt-6
#

message (!!! $$_PRO_FILE_)

include ("$${PWD}/../../../prj/common/common_qt/sys_common.pri")
include ("$${PWD}/../../../prj/common/common_qt/flags_common.pri")

DESTDIR     = $${artifactRoot}/$${SYSTEM_PATH}/$$CONFIGURATION/test
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets
QT += multimedia

SCR_REC_INCLUDE = $${screenRecorderRepositoryRoot}/include
SOURCES_BASE_DIR = $${screenRecorderRepositoryRoot}/src
SOURCES_DIR = $${SOURCES_BASE_DIR}/tests/qt_multimedia_test01

INCLUDEPATH += $${SCR_REC_INCLUDE}

LIBS += -lUser32

SOURCES += $$files($${SOURCES_DIR}/*.cpp,false)

SOURCES +=	\
        $${SOURCES_BASE_DIR}/core/scrrec_core_sigslotdata.cpp

SOURCES += "$${cpputilsRepoRoot}/src/core/cpputils_inscopecleaner.cpp"
SOURCES += "$${cpputilsRepoRoot}/src/core/cpputils_mutex_ml.cpp"
SOURCES += "$${cpputilsRepoRoot}/src/core/cpputils_thread_local.cpp"
SOURCES += "$${cpputilsRepoRoot}/src/core/cpputils_unnamedsemaphore.cpp"

SOURCES += "$${qtutilsRepoRoot}/src/core/qtutils_core_logger.cpp"
SOURCES += "$${qtutilsRepoRoot}/src/core/qtutils_core_threadls.cpp"
SOURCES += "$${qtutilsRepoRoot}/src/core/qtutils_core_threadobject.cpp"
SOURCES += "$${qtutilsRepoRoot}/src/ui/qtutils_ui_global_functions.cpp"

HEADERS += $$files($${SCR_REC_INCLUDE}/*.h,true)
HEADERS += $$files($${SCR_REC_INCLUDE}/*.hpp,true)
HEADERS += $$files($${SOURCES_DIR}/*.hpp,true)
HEADERS += $$files($${SOURCES_DIR}/*.h,true)
