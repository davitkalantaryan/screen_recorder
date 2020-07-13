//  
// file:		qt_screen_recorder_settings.hpp  
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
// 

#ifndef QT_SCREENRECORDER_SETTINGS_HPP
#define QT_SCREENRECORDER_SETTINGS_HPP

#include <stdint.h>
#include <QSettings>

namespace qt_screen_recorder{

class Application;
class Worker;

class Settings final 
{
	friend class Application;
	friend class Worker;
	
public:
	Settings();
	~Settings();
	
private:
	QSettings*	m_pSettings;
	
	union{
		struct{
			uint64_t	shouldRun : 1;
			uint64_t	recordScreen : 1;
		}bits;
		uint64_t	allBits;
	}m_statuses;
};

}  // namespace qt_screen_recorder{


#endif  // #ifndef QT_SCREENRECORDER_SETTINGS_HPP
