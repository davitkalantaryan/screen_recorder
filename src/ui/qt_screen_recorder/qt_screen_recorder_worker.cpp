//  
// file:		qt_screen_recorder_worker.cpp  
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
//

#include "qt_screen_recorder_worker.hpp"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QBuffer>
#include <QImage>
#include <core_prepare_gif_file.h>
#include "qt_screen_recorder_application.hpp"
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#define SleepMsIntr(_x)	SleepEx((_x),TRUE)
#else
#include <unistd.h>
#define SleepMsIntr(_x)	usleep(1000*(_x))
#endif


qt_screen_recorder::Worker::Worker(EarlyParams& a_eP)
	:
	  m_ep(a_eP)
{
	//
}


qt_screen_recorder::Worker::~Worker()
{
	//
}


void qt_screen_recorder::Worker::InitializeWorker()
{
	//
}


void qt_screen_recorder::Worker::CleanupWorker()
{
	//
}


struct RecordStruct{
	int width, height;
	int x,y,w,h;
	size_t unMemcpySize;
	QScreen* pScreen;
};

// todo: fix this
#define  fileName		"test.gif"

void qt_screen_recorder::Worker::ScreenRecording(uint32_t a_delayInMs)
{
	auto& bits = m_ep.m_settings.m_statuses.bits;
	uint32_t delay = a_delayInMs/10;
	GifWriter g;
	RecordStruct recStr{0,0,0,0,0,0,0,nullptr};
	RecordStruct* pRecStr = &recStr;
	QByteArray bArray;
	QByteArray* pbArray = &bArray;
	bool bGifStarted = false;
	bool bOk = true;
	bool* pbOk = &bOk;
	
	if( (!bits.shouldRun)||(!bits.recordScreen)){
		emit thisApp()->RecordingFinishedSignal(true);
		return;
	}
	
	//QMetaObject::invokeMethod(qApp,[this, pScreens, pRecStr, pbArray]() mutable {
	QMetaObject::invokeMethod(qApp,[this, pRecStr, pbArray, pbOk]() {
		auto screens = qApp->screens();
		QPixmap pxmScreen;
		QImage image1;
		int i, nLineOffset;
		uint32_t *pRGBOut;
		QRgb* pRgbIn;
		int nPixelsCount;
		
		if(screens.size()<1){
			auto& bits = m_ep.m_settings.m_statuses.bits;
			bits.recordScreen = 0;
			*pbOk = false;
			return;
		}
		
		// todo: make this better
		pRecStr->x = 0;
		pRecStr->y = 0;
		pRecStr->w = -1;
		pRecStr->h = -1;
		pRecStr->pScreen = screens.at(0);
		
		pxmScreen = pRecStr->pScreen->grabWindow(0,pRecStr->x,pRecStr->y,pRecStr->w,pRecStr->h);
		image1 = pxmScreen.toImage();
		
		pRecStr->width = static_cast< decltype (pRecStr->width) >(image1.width());
		pRecStr->height = static_cast< decltype (pRecStr->height) >(image1.height());
		pRecStr->unMemcpySize = static_cast<size_t>(pRecStr->width*4);
		nPixelsCount = static_cast<int>(pRecStr->width*pRecStr->height);
		
		pbArray->resize(nPixelsCount*4/*sizeof(uint32_t)*/);
		pRGBOut = reinterpret_cast<uint32_t*>(pbArray->data());
		
		for(i=0;i<pRecStr->height;++i){
			nLineOffset = i*pRecStr->width;
			//for(j=0;j<pRecStr->width;++j){
			//	pRGB[nLineOffset+j]=image1.pixel(i,j);
			//}
			pRgbIn = reinterpret_cast<QRgb*>(image1.scanLine(i));
			memcpy(&pRGBOut[nLineOffset],pRgbIn,pRecStr->unMemcpySize);
		}
		
		
		//QBuffer aBuffer(pbArray);
		//aBuffer.open(QIODevice::ReadWrite);
		//pxmScreen.save(&aBuffer,"PNG",100);
		//pRecStr->width = static_cast< decltype (pRecStr->width) >(pxmScreen.width());
		//pRecStr->height = static_cast< decltype (pRecStr->height) >(pxmScreen.width());
		//
		//nPixelsCount = static_cast<int>(pRecStr->width*pRecStr->height);
		//nArrayLen = pbArray->size();
		//if(nPixelsCount>nArrayLen){
		//	auto& bits = m_ep.m_settings.m_statuses.bits;
		//	bits.recordScreen = 0;
		//	delete pScreens;
		//	pScreens = nullptr;
		//	return;
		//}
		
	}, ::Qt::BlockingQueuedConnection);
	
	if( (!bits.shouldRun)||(!bits.recordScreen)){
		emit thisApp()->RecordingFinishedSignal(bOk);
		return;
	}
	
	GifBegin(&g, fileName,static_cast<uint32_t>(recStr.width), static_cast<uint32_t>(recStr.height), delay);
	GifWriteFrame(&g,reinterpret_cast<uint8_t*>(bArray.data()), static_cast<uint32_t>(recStr.width), static_cast<uint32_t>(recStr.height), delay);
	bGifStarted = true;
	
	emit thisApp()->RecordingStartedSignal();
	
	while(bits.shouldRun && bits.recordScreen){
		
		SleepMsIntr(a_delayInMs);
		
		QMetaObject::invokeMethod(qApp,[pRecStr,pbArray](){
			QPixmap pxmScreen;
			QImage image1;
			int i, nLineOffset;
			uint32_t *pRGBOut;
			QRgb* pRgbIn;
			int nPixelsCount;
						
			pxmScreen = pRecStr->pScreen->grabWindow(0,pRecStr->x,pRecStr->y,pRecStr->w,pRecStr->h);
			image1 = pxmScreen.toImage();
			
			pRecStr->width = static_cast< decltype (pRecStr->width) >(image1.width());
			pRecStr->height = static_cast< decltype (pRecStr->height) >(image1.height());
			pRecStr->unMemcpySize = static_cast<size_t>(pRecStr->width*4);
			nPixelsCount = static_cast<int>(pRecStr->width*pRecStr->height);
			
			pbArray->resize(nPixelsCount*4/*sizeof(uint32_t)*/);
			pRGBOut = reinterpret_cast<uint32_t*>(pbArray->data());
			
			for(i=0;i<pRecStr->height;++i){
				nLineOffset = i*pRecStr->width;
				//for(j=0;j<pRecStr->width;++j){
				//	pRGB[nLineOffset+j]=image1.pixel(i,j);
				//}
				pRgbIn = reinterpret_cast<QRgb*>(image1.scanLine(i));
				memcpy(&pRGBOut[nLineOffset],pRgbIn,pRecStr->unMemcpySize);
			}
		}, ::Qt::BlockingQueuedConnection);
		
		GifWriteFrame(&g,reinterpret_cast<uint8_t*>(bArray.data()), static_cast<uint32_t>(recStr.width), static_cast<uint32_t>(recStr.height), delay);
		
	}
	
	if(bGifStarted){
		GifEnd(&g);
	}
	
	emit thisApp()->RecordingFinishedSignal(bOk);
}
