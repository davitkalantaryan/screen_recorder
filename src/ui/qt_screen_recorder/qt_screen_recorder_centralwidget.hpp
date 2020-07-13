//
// file:			qt_screen_recorder_centralwidget.hpp
// created on:		2020 Jul 13
// created by:		D. Kalantaryan (davit.kalantaryan@gmail.com)
//

#ifndef QT_SCREENRECORDER_CENTRALWIDGET_HPP
#define QT_SCREENRECORDER_CENTRALWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>


namespace qt_screen_recorder{


class MainWidget final : public ::QWidget
{
public:
	MainWidget();
		
private:
	QGridLayout		m_mainLayout;
	
};


class CentralWidget final : public ::QWidget
{
public:
	CentralWidget();
	
private:
	void mousePressEvent(QMouseEvent *event) override;
	
	//void StopBreakAction();

private:
	QVBoxLayout		m_mainLayout;
	QVBoxLayout		m_menuAndToolbarLayout;
	QHBoxLayout		m_menuLayout;
	QMenuBar		m_menuBarLeft ;
	QMenuBar		m_menuBarRight;
	QToolBar		m_mainToolBar;
	QMenu*			m_pFileMenu;
	QMenu*			m_pActionsMenu;
	MainWidget		m_mainWidget;
	QAction			m_actionExit;
	QAction			m_actionStartRecording;
	QAction			m_actionStopRecording;
};


}  // namespace qt_screen_recorder{


#endif  // #ifndef QT_SCREENRECORDER_CENTRALWIDGET_HPP
