//
// file:			qt_multimedia_test01_application_p.cpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_application_p.cpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//


#include "qt_multimedia_test01_application_p.hpp"


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


Application_p::Application_p(Application* a_thisApp)
    :
      m_pThisApp(a_thisApp),
      m_pRecorderObject(nullptr)
{
    ::qtutils::ThreadOrInitdeinit* pThread;

    InitBeforeThreads();

    // here we do not have any thread running

    pThread = new ::qtutils::ThreadOrInitdeinit(
    [this](void*){
        m_pRecorderObject = new CRecorderObject();
        //m_pAppMonObject->data =
    },
    [this](void*){
        delete m_pRecorderObject;
        m_pRecorderObject = nullptr;
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
}


}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
