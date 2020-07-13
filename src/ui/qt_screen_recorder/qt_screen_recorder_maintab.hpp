//  
// file:		qt_screen_recorder_maintab.hpp  
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
// 

#ifndef QT_SCREENRECORDER_MAINTAB_HPP
#define QT_SCREENRECORDER_MAINTAB_HPP

#include <QTabWidget>
#include "qt_screen_recorder_centralwidget.hpp"

namespace qt_screen_recorder{

class MainTab final : public ::QTabWidget
{
public:
	MainTab();
	
private:
	CentralWidget		m_centralWidget;
};

}  // namespace qt_screen_recorder{


#endif  // #ifndef QT_SCREENRECORDER_MAINTAB_HPP
