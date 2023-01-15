//
// file:			qt_multimedia_test01_application_p.hpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_application_p.hpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

// see: https://www.qt.io/blog/qt-multimedia-in-qt-6

#pragma once

#include <screen_recorder/export_symbols.h>
#include "qt_multimedia_test01_application.hpp"

namespace screen_recorder{ namespace qt_multimedia_test01{


class CINTERNAL_DLL_PRIVATE Application_p final
{
public:
    ~Application_p();
    Application_p(Application* a_thisApp);

private:
    Application*const                   m_pThisApp;
};

}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
