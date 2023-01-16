//
// file:			qt_multimedia_test01_application_p.cpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_application_p.cpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//


#include "qt_multimedia_test01_application_p.hpp"
#include <qtutils/core/settings.hpp>
#include <qtutils/core/logger.hpp>
#include <qtutils/disable_utils_warnings.h>
#include <QMetaType>
#include <QScreen>
#include <QMediaRecorder>
#include <QUrl>
#include <QString>
//#include <QMediaFormat>
//#include <QMediaCaptureSession>


namespace screen_recorder{ namespace qt_multimedia_test01{


Application_p::~Application_p()
{
    ::qtutils::ThreadOrInitdeinit* pThread;
    const TypeThreadsList::const_iterator endIter = m_threadsList.cend();
    TypeThreadsList::const_iterator iter = m_threadsList.cbegin();
    for(;iter!=endIter;++iter){
        pThread = *iter;
        delete pThread;
    }

    // here we do not have any thread running
}


static VOID CALLBACK DestopSwitchClbk(
    HWINEVENTHOOK a_hWinEventHook,
    DWORD         a_event,
    HWND          a_hWnd,
    LONG          a_idObject,
    LONG          a_idChild,
    DWORD         a_idEventThread,
    DWORD         a_dwmsEventTime) noexcept
{
    CPPUTILS_STATIC_CAST(void, a_hWinEventHook);
    CPPUTILS_STATIC_CAST(void, a_event);
    CPPUTILS_STATIC_CAST(void, a_hWnd);
    CPPUTILS_STATIC_CAST(void, a_idObject);
    CPPUTILS_STATIC_CAST(void, a_idChild);
    CPPUTILS_STATIC_CAST(void, a_dwmsEventTime);
    CPPUTILS_STATIC_CAST(void, a_idEventThread);

    QtUtilsDebug()<<" ";
}


#define VK_D    0x44

void ShowDesktop()
{
    //OutputString(L"Sending 'Win-D'\r\n");
    INPUT inputs[4] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LWIN;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_D;

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_D;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_LWIN;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        //OutputString(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}


Application_p::Application_p(Application* a_thisApp)
    :
      m_pThisApp(a_thisApp),
      m_pRecorderObject(nullptr),
      m_pTimersObject(nullptr)
{
    ::qtutils::ThreadOrInitdeinit* pThread;

    InitBeforeThreads();

    // here we do not have any thread running

    pThread = new ::qtutils::ThreadOrInitdeinit(
    [this](void*){
        m_pRecorderObject = new CRecorderObject();
        ShowDesktop();
        m_pRecorderObject->winHook = SetWinEventHook(EVENT_SYSTEM_DESKTOPSWITCH, EVENT_SYSTEM_DESKTOPSWITCH, CPPUTILS_NULL,
                                                     &DestopSwitchClbk, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNTHREAD);
    },
    [this](void*){
        if (m_pRecorderObject->winHook) { UnhookWinEvent(m_pRecorderObject->winHook); }
        delete m_pRecorderObject;
        m_pRecorderObject = nullptr;
    },nullptr);
    m_threadsList.push_back(pThread);

    pThread = new ::qtutils::ThreadOrInitdeinit(
    [this](void*){
        m_pTimersObject = new CTimersObject();
        m_pTimersObject->regularScreenshotTimer.start(100); // each 0.5s
    },
    [this](void*){
        m_pTimersObject->regularScreenshotTimer.stop();
        delete m_pTimersObject;
        m_pTimersObject = nullptr;
    },nullptr);
    m_threadsList.push_back(pThread);

    InitAfterThreads();
}


void Application_p::InitBeforeThreads()
{
    //::qtutils::Settings aSettings;
    // todo: Init setting
}


void Application_p::InitAfterThreads()
{
    // signal/slot connections
    // todo: Init all signals

    QObject::connect(&(m_pTimersObject->regularScreenshotTimer),&QTimer::timeout,m_pRecorderObject,[this](){
        MakeOneScreenshotAndPutItToImage();
    });
}


void Application_p::MakeOneScreenshotAndPutItToImage()
{
    const auto allScreens = qApp->screens();
    const int cnCount = allScreens.size();
    if(cnCount>0){
        QScreen* pScreen;
        SCRREC_DT_NSP CScreenshotData aScrshtData;
        SCRREC_DT_NSP SScreenshotData* pScrshtData = aScrshtData.screenshotData();

        pScrshtData->screenshots.reserve(static_cast<size_t>(cnCount));

        for(int i(0); i<cnCount;++i){
            pScreen = allScreens.at(i);
            if(pScreen){
                pScrshtData->screenshots.push_back(pScreen->grabWindow(0));
            }
            else{
                QtUtilsWarning("Screen %i is not available", i);
            }
        }

        if(aScrshtData==m_pRecorderObject->m_prevScreenShot){
            QtUtilsDebug()<<"Desktop is not changed";
            return;
        }

        pScrshtData->scrinshootDateTimeUtc = QDateTime::currentDateTimeUtc();

        if(pScrshtData->screenshots.size()>0){
            emit m_pThisApp->ScreenshotDoneSignal(aScrshtData);
        }

        m_pRecorderObject->m_prevScreenShot = aScrshtData;
    }
    else{
        QtUtilsWarning()<<"No sreen is attached!";
    }
}


void Application_p::StartVideoCapture()
{
    QMetaObject::invokeMethod(m_pRecorderObject,[this](){
        StartVideoCaptureRecThread();
    });
}


void Application_p::StartVideoCaptureRecThread()
{
#if 0
    QMediaCaptureSession session;
    QMediaRecorder recorder;
    QMediaFormat format;
    format.setVideoCodec(QMediaFormat::VideoCodec::H265);
    recorder.setMediaFormat(format);
    recorder.setOutputLocation(QUrl(QString("mycapture.mp4")));
    session.setRecorder(&recorder);
    recorder.record();
#endif
}


/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

ApplicationEarlyData::ApplicationEarlyData()
{
    QCoreApplication::setOrganizationDomain("smartdav");
    QCoreApplication::setOrganizationName("smartdav");  // todo: set correct organization name
    QCoreApplication::setApplicationName("qt_multimedia_test01");
    QCoreApplication::setApplicationVersion("1.0.0");
    //QApplication::setWindowIcon( QIcon(":/focust/focust_logo.png") );
    ::qtutils::Settings::setDefaultFormat(QSettings::IniFormat);  // todo: place where we can play with settings type

    qRegisterMetaType< SCRREC_DT_NSP CScreenshotData >( "screen_recorder_CScreenshotData" );
}


}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
