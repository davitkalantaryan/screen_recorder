//
// file:			qt_multimedia_test01_application.cpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_application.cpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//


#include "qt_multimedia_test01_application_p.hpp"


namespace screen_recorder{ namespace qt_multimedia_test01{


Application::~Application()
{
    delete m_app_data_ptr;
}


Application::Application(int& a_argc, char** a_argv)
    :
      QApplication(a_argc,a_argv),
      m_app_data_ptr(new Application_p(this))
{
}


}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
