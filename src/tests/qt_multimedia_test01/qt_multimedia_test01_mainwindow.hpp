//
// file:			qt_multimedia_test01_mainwindow.hpp
// path:			src/tests/qt_multimedia_test01/qt_multimedia_test01_mainwindow.hpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

// see: https://www.qt.io/blog/qt-multimedia-in-qt-6

#pragma once

#include <screen_recorder/export_symbols.h>
#include <qtutils/disable_utils_warnings.h>
#include <QWidget>

namespace screen_recorder{ namespace qt_multimedia_test01{

class CINTERNAL_DLL_PRIVATE MainWindow final : public QWidget
{
public:
    MainWindow();
};

}}  //  namespace screen_recorder{ namespace qt_multimedia_test01{
