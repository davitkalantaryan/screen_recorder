//
// file:			qt_multimedia_test01_application_p.cpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_application_p.cpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//


#include "qt_multimedia_test01_application_p.hpp"


namespace screen_recorder{ namespace qt_multimedia_test01{


Application_p::~Application_p()
{
}


Application_p::Application_p(Application* a_thisApp)
    :
      m_pThisApp(a_thisApp)
{
}


}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
