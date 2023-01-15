//
// file:			main_qt_multimedia_test01.cpp
// path:			src/tests/qt_multimedia_test01/main_qt_multimedia_test01.cpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#include "qt_multimedia_test01_application.hpp"
#include "qt_multimedia_test01_mainwindow.hpp"

int main(int a_argc, char* a_argv[])
{
    ::screen_recorder::qt_multimedia_test01::Application aApp(a_argc,a_argv);
    ::screen_recorder::qt_multimedia_test01::MainWindow mainWnd;

    mainWnd.show();

    aApp.exec();

    return 0;
}
