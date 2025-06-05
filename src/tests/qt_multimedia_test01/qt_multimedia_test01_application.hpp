//
// file:			qt_multimedia_test01_application.hpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_application.hpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

// see: https://www.qt.io/blog/qt-multimedia-in-qt-6

#pragma once

#include <screen_recorder/export_symbols.h>
#include <screen_recorder/sigslotdata.hpp>
#include <qtutils/disable_utils_warnings.h>
#include <QApplication>

namespace screen_recorder{ namespace qt_multimedia_test01{

class CPPUTILS_DLL_PRIVATE Application_p;

class CPPUTILS_DLL_PRIVATE Application final : public QApplication
{
    Q_OBJECT

public:
    ~Application() override;
    Application(int& argc, char** argv);

    void ExitApp();
    void StartRecording();

private:
signals:
    void ScreenshotDoneSignal(SCRREC_DT_NSP CScreenshotData a_data);

private:
    Application_p*const  m_app_data_ptr;

    friend class Application_p;
};


#define scrrecMmTst01App()   static_cast< ::screen_recorder::qt_multimedia_test01::Application*>(qApp)

}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
