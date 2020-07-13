//  
// file:		qt_screen_recorder_earlyparams.hpp  
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
// 

#ifndef QT_SCREENRECORDER_EARLYPARAMS_HPP
#define QT_SCREENRECORDER_EARLYPARAMS_HPP

#include "qt_screen_recorder_settings.hpp"

namespace qt_screen_recorder{

class Application;
class Worker;

class EarlyParams final 
{
	friend class Application;
	friend class Worker;
	
public:
	EarlyParams();
	
private:
	Settings	m_settings;
};

}  // namespace qt_screen_recorder{


#endif  // #ifndef QT_SCREENRECORDER_EARLYPARAMS_HPP
