//
// file:			qt_multimedia_test01_mainwindow.cpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_mainwindow.cpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//


#include "qt_multimedia_test01_mainwindow.hpp"
#include <qtutils/ui/global_functions.hpp>
#include "qt_multimedia_test01_application_p.hpp"


#define SCRREC_START_TEXT       "Start"
#define SCRREC_STOP_TEXT        "Stop"

namespace screen_recorder{ namespace qt_multimedia_test01{


MainWindow::MainWindow()
{
    ConnectSignals();

    ::qtutils::ui::SetProperParamsToImagedLbl(&m_screenShtLbl);
    m_startBtn.setText(SCRREC_START_TEXT);
    m_exitBtn.setText("Exit");

    m_mainLayout.addWidget(&m_screenShtLbl,0,0,3,3);
    m_mainLayout.addWidget(&m_startBtn,4,1,1,1);
    m_mainLayout.addWidget(&m_exitBtn,4,2,1,1);

    setLayout(&m_mainLayout);
}


void MainWindow::ConnectSignals()
{
    Application*const pThisApp = scrrecMmTst01App();

    connect(&m_exitBtn,&QPushButton::clicked,this,[](){
        Application*const pThisApp = scrrecMmTst01App();
        pThisApp->ExitApp();
    });

    connect(&m_startBtn,&QPushButton::clicked,this,[](){
        Application*const pThisApp = scrrecMmTst01App();
        pThisApp->StartRecording();
    });

    connect(pThisApp,&Application::ScreenshotDoneSignal,this,[this](SCRREC_DT_NSP CScreenshotData a_data){
        const SCRREC_DT_NSP SScreenshotData* pScrshtData = a_data.screenshotData();
        const int cnNumberOfScreens = static_cast<int>(pScrshtData->screenshots.size());
        if(cnNumberOfScreens>0){
            m_screenShtLbl.setPixmap(pScrshtData->screenshots[0]);
        }
    });
}


}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
