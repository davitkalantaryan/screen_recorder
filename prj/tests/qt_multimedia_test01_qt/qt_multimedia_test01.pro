#
# file:			dir_iter_test01.pro
# path:			prj/tests/dir_iter_test01_qt/dir_iter_test01.pro
# created on:		2023 Jan 10
# Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
#
# see: https://www.qt.io/blog/qt-multimedia-in-qt-6
#

message (!!! $$_PRO_FILE_)

include ("$${PWD}/../../../prj/common/common_qt/sys_common.pri")
include ("$${PWD}/../../../prj/common/common_qt/flags_common.pri")

DESTDIR     = $${artifactRoot}/$${SYSTEM_PATH}/$$CONFIGURATION/test
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets

SCR_REC_INCLUDE = $${screenRecorderRepositoryRoot}/include
SOURCES_BASE_DIR = $${screenRecorderRepositoryRoot}/src
SOURCES_DIR = $${SOURCES_BASE_DIR}/tests/qt_multimedia_test01

INCLUDEPATH += $${SCR_REC_INCLUDE}

SOURCES += $$files($${SOURCES_DIR}/*.cpp,false)

SOURCES += "$${cpputilsRepoRoot}/src/core/cpputils_inscopecleaner.cpp"
SOURCES += "$${cpputilsRepoRoot}/src/core/cpputils_unnamedsemaphore.cpp"

SOURCES += "$${qtutilsRepoRoot}/src/core/qtutils_core_threadls.cpp"
SOURCES += "$${qtutilsRepoRoot}/src/core/qtutils_core_threadobject.cpp"

HEADERS += $$files($${SCR_REC_INCLUDE}/*.h,true)
HEADERS += $$files($${SOURCES_DIR}/*.hpp,true)
