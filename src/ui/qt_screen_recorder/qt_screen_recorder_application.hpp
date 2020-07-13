//  
// file:		qt_screen_recorder_application.hpp  
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
// 

#ifndef QT_SCREENRECORDER_APPLICATION_HPP
#define QT_SCREENRECORDER_APPLICATION_HPP


#include <QApplication>
#include <QThread>
#include "qt_screen_recorder_earlyparams.hpp"

#define thisApp()  static_cast< ::qt_screen_recorder::Application* >( ::QCoreApplication::instance() )

namespace qt_screen_recorder{

class Worker;

class WorkerThread final : public QThread
{
	void run() override;
};

class Application final : public ::QApplication
{
	friend class WorkerThread;
	friend class Worker;
	
	Q_OBJECT
	
public:
	Application(int& argc, char** argv);
	~Application() override ;
	
	void StartRecording();
	void StopRecording();
	
private:
signals:
	void RecordingStartedSignal();
	void RecordingFinishedSignal(bool ok);
	
private:
	EarlyParams		m_ep;
	WorkerThread	m_workerThread;
	Worker*			m_pWorker;
};


}  // namespace qt_screen_recorder{



#endif  // #ifndef QT_SCREENRECORDER_APPLICATION_HPP
