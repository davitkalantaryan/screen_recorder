//  
// file:		qt_screen_recorder_application.cpp  
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
// 


#include "qt_screen_recorder_application.hpp"
#include "qt_screen_recorder_worker.hpp"


qt_screen_recorder::Application::Application(int& a_argc, char** a_argv)
	:
	  ::QApplication(a_argc,a_argv)
{
	m_pWorker = nullptr;
	m_ep.m_settings.m_statuses.bits.shouldRun = 1;
	m_workerThread.start();
}


qt_screen_recorder::Application::~Application()
{
	m_ep.m_settings.m_statuses.bits.shouldRun = 0;
	::QCoreApplication::processEvents();
	m_workerThread.quit();
	m_workerThread.wait();
}


void qt_screen_recorder::Application::StartRecording()
{
	if(!m_pWorker){
		return;
	}
	
	m_ep.m_settings.m_statuses.bits.recordScreen = 1;
	QMetaObject::invokeMethod(m_pWorker,[this](){
		m_pWorker->ScreenRecording(100);
	});
}


void qt_screen_recorder::Application::StopRecording()
{
	m_ep.m_settings.m_statuses.bits.recordScreen = 0;
}


/*////////////////////////////////////////////////////////////////////////////////////*/

void qt_screen_recorder::WorkerThread::run()
{
	Worker* pWorker = thisApp()->m_pWorker = new Worker(thisApp()->m_ep);
	thisApp()->m_pWorker->InitializeWorker();
	QThread::exec();
	thisApp()->m_pWorker->CleanupWorker();
	thisApp()->m_pWorker = nullptr;
	delete pWorker;
}
