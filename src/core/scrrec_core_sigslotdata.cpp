//
// file:			scrrec_core_sigslotdata.cpp
// path:			src/core/scrrec_core_sigslotdata.cpp
// created on:		2023 Jan 15
// Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//


#include <screen_recorder/sigslotdata.hpp>


SCRREC_DT_NSP_P1


CScreenshotData::CScreenshotData()
    :
      m_data(new SScreenshotData())
{
}


CScreenshotData::CScreenshotData(const CScreenshotData& a_cM)
    :
      m_data(a_cM.m_data)
{
}


CScreenshotData::CScreenshotData(CScreenshotData&& a_mM)
    :
      m_data(a_mM.m_data)
{
}


SScreenshotData* CScreenshotData::screenshotData()
{
    return m_data.get();
}


const SScreenshotData* CScreenshotData::screenshotData()const
{
    return m_data.get();
}


CScreenshotData& CScreenshotData::operator=(const CScreenshotData& a_cM)
{
    m_data = a_cM.m_data;
    return *this;
}


bool CScreenshotData::operator == (const CScreenshotData& a_cM) const
{
    const SScreenshotData* thisData = m_data.get();
    const SScreenshotData* cmData = a_cM.m_data.get();
    const size_t cunThisSize(thisData->screenshots.size()), cunCmSize(cmData->screenshots.size());

    if(cunThisSize==cunCmSize){
        const QPixmap*const thisPixmaps = thisData->screenshots.data();
        const QPixmap*const cpPixmaps = cmData->screenshots.data();
        for(size_t i(0); i<cunThisSize;++i){
            //if(thisPixmaps[i].cacheKey()!=cpPixmaps[i].cacheKey()){
            if(thisPixmaps[i].toImage()!=cpPixmaps[i].toImage()){
                return false;
            }
        }  //  for(size_t i(0); i<cunThisSize;++i){
        return true;
    }  //  if(cunThisSize==cunCmSize){

    return false;
}


SCRREC_DT_NSP_P2
