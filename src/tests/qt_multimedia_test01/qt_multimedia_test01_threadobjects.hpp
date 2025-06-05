//
// file:			qt_multimedia_test01_threadobjects.hpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_threadobjects.hpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

// see: https://www.qt.io/blog/qt-multimedia-in-qt-6

#pragma once

#include <screen_recorder/export_symbols.h>
#include <qtutils/core/threadobject.hpp>
#include <screen_recorder/sigslotdata.hpp>
#include <qtutils/disable_utils_warnings.h>
#include <QTimer>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
typedef HWINEVENTHOOK   wineventhook_sr;
#else
#endif

namespace screen_recorder{ namespace qt_multimedia_test01{


class CPPUTILS_DLL_PRIVATE CRecorderObject final : public ::qtutils::ThreadObject
{
public:
    SCRREC_DT_NSP CScreenshotData   m_prevScreenShot;
    wineventhook_sr winHook;
};


class CPPUTILS_DLL_PRIVATE CTimersObject final : public ::qtutils::ThreadObject
{
public:
    QTimer     regularScreenshotTimer;
};


}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
