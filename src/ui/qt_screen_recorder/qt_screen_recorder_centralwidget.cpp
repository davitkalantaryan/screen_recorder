//
// file:			qt_screen_recorder_centralwidget.cpp
// created on:		2020 Jul 13
// created by:		D. Kalantaryan (davit.kalantaryan@gmail.com)
//

#include "qt_screen_recorder_centralwidget.hpp"
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <QCoreApplication>
#include "qt_screen_recorder_application.hpp"



/*////////////////////////////////////////////////////////////////////////////////////*/

qt_screen_recorder::CentralWidget::CentralWidget()
	:
	  m_actionExit(QIcon(":/img/quit_pik.png"),"&Exit"),
	  m_actionStartRecording(QIcon(":/img/stop_break_button.png"),"Start Recording"),
	  m_actionStopRecording(QIcon(":/img/start_break_button.png"),"Stop Recording")
{	
	// make left side menu
	QMenu* pMenu;

	m_pFileMenu = m_menuBarLeft.addMenu("&File");
	m_pActionsMenu = m_menuBarLeft.addMenu("Actions");
		
	// connections
	connect(&m_actionExit,&QAction::triggered,[](){ ::QCoreApplication::quit(); });
	connect(&m_actionStartRecording,&QAction::triggered,this,[this](){
		thisApp()->StartRecording();
		m_actionStartRecording.setDisabled(true);
	});
	connect(&m_actionStopRecording,&QAction::triggered,this,[this](){
		thisApp()->StopRecording();
		m_actionStopRecording.setDisabled(true);
	});
	connect(thisApp(),&Application::RecordingStartedSignal,this,[this](){
		m_mainToolBar.removeAction(&m_actionStartRecording);
		m_mainToolBar.addAction(&m_actionStopRecording);
		m_actionStopRecording.setEnabled(true);
	});
	connect(thisApp(),&Application::RecordingFinishedSignal,this,[this](bool a_bOk){
		qDebug()<<__FUNCTION__<<"(ok:"<<a_bOk<<")";
		m_mainToolBar.removeAction(&m_actionStopRecording);
		m_mainToolBar.addAction(&m_actionStartRecording);
		m_actionStartRecording.setEnabled(true);
	});

	// now right side menu
	pMenu = m_menuBarRight.addMenu("&Help");
	pMenu->addAction("About",[this](){
		QMessageBox::about(this,"screen recorder application","To be done");
	});

	// toolbar
	m_mainToolBar.addAction(&m_actionExit);
	m_mainToolBar.addAction(&m_actionStartRecording);

	// layouts
	m_menuLayout.addWidget(&m_menuBarLeft);
	m_menuLayout.addWidget(&m_menuBarRight);
	m_menuLayout.setAlignment(&m_menuBarLeft, Qt::AlignLeft);
	m_menuLayout.setAlignment(&m_menuBarRight, Qt::AlignRight);
	
	m_menuAndToolbarLayout.addLayout(&m_menuLayout);
	m_menuAndToolbarLayout.addWidget(&m_mainToolBar);
	m_mainLayout.addLayout(&m_menuAndToolbarLayout);
	
	m_menuBarLeft.setMaximumHeight(m_menuBarLeft.sizeHint().height());
	m_menuBarRight.setMaximumHeight(m_menuBarRight.sizeHint().height());
	m_mainToolBar.setMaximumHeight(m_mainToolBar.sizeHint().height());

	m_mainLayout.addWidget(&m_mainWidget);

	setLayout(&m_mainLayout);
}


void qt_screen_recorder::CentralWidget::mousePressEvent(QMouseEvent *a_event)
{
	//switch(a_event->button()){
	//case Qt::RightButton:{
	//	QMenu aMenu;
    //    aMenu.addAction("Change base URL",[this,a_event](){
	//		BackendOptionsSetter dlg(mapToGlobal(a_event->pos()));
	//		dlg.MyExec();
    //    });
    //    aMenu.exec(mapToGlobal(a_event->pos()));
	//}break;
	//default:
	//	break;
	//}
	::QWidget::mousePressEvent(a_event);
}


/*////////////////////////////////////////////////////////////////////////////////////*/
qt_screen_recorder::MainWidget::MainWidget()
{
	setLayout(&m_mainLayout);
}
