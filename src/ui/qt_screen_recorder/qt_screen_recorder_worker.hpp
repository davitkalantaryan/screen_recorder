//  
// file:		qt_screen_recorder_worker.hpp  
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
// 

#ifndef QT_SCREENRECORDER_WORKER_HPP
#define QT_SCREENRECORDER_WORKER_HPP

#include <QObject>
#include <stdint.h>
#include "qt_screen_recorder_earlyparams.hpp"

namespace qt_screen_recorder{

class Application;
class WorkerThread;

class Worker final : public ::QObject
{
	friend class Application;
	friend class WorkerThread;
	
private:
	Worker(EarlyParams& a_eP);
	~Worker() override;
	
	void InitializeWorker();
	void CleanupWorker();
	
	void ScreenRecording(uint32_t delayInMs);
	
private:
	EarlyParams&		m_ep;
};

}  // namespace qt_screen_recorder{


#endif  // #ifndef QT_SCREENRECORDER_WORKER_HPP
