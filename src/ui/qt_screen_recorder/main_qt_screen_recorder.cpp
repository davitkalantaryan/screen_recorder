//  
// file:		main_qt_screen_recorder.cpp  
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
// 


#include "qt_screen_recorder_application.hpp"
#include "qt_screen_recorder_maintab.hpp"


int main(int a_argc, char* a_argv[])
{
	qt_screen_recorder::Application aApp(a_argc,a_argv);
	qt_screen_recorder::MainTab  aMainGui;
	
	aMainGui.show();
	
	QCoreApplication::exec();
	return 0;
}
