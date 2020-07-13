//  
// file:		qt_screen_recorder_settings.hpp  
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
// 

#include "qt_screen_recorder_settings.hpp"
#include <QCoreApplication>


qt_screen_recorder::Settings::Settings()
{
	QCoreApplication::setOrganizationName("alantar"); 
    QCoreApplication::setApplicationName("qt_screen_recorder");
	QCoreApplication::setApplicationVersion(QSR_APPLICATION_VERSION);
	
	QSettings::setDefaultFormat(QSettings::IniFormat);
	
	m_pSettings = new QSettings();
}


qt_screen_recorder::Settings::~Settings()
{
	delete m_pSettings;
}
