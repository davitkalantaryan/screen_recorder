//
// file:			qt_multimedia_test01_application_p.hpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_application_p.hpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

// see: https://www.qt.io/blog/qt-multimedia-in-qt-6

#pragma once

#include <screen_recorder/export_symbols.h>
#include "qt_multimedia_test01_application.hpp"
#include "qt_multimedia_test01_threadobjects.hpp"
#include <qtutils/core/threadorinitdeinit.hpp>
#include <list>

namespace screen_recorder{ namespace qt_multimedia_test01{

typedef ::std::list<::qtutils::ThreadOrInitdeinit*> TypeThreadsList;

class CPPUTILS_DLL_PRIVATE ApplicationEarlyData final
{
public:
    ApplicationEarlyData();
};

class CPPUTILS_DLL_PRIVATE Application_p final
{
public:
    ~Application_p();
    Application_p(Application* a_thisApp);

    void StartVideoCapture();

private:
    void InitBeforeThreads();
    void InitAfterThreads();
    void MakeOneScreenshotAndPutItToImage();
    void StartVideoCaptureRecThread();

private:
    Application*const       m_pThisApp;
    ApplicationEarlyData    m_earlyData;
    TypeThreadsList         m_threadsList;
    CRecorderObject*        m_pRecorderObject;
    CTimersObject*          m_pTimersObject;
};

}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
