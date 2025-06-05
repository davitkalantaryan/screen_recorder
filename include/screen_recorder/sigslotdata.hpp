//
// file:			sigslotdata.hpp
// path:			include/screen_recorder/sigslotdata.hpp
// created on:		2023 Jan 15
// created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#pragma once

#include <screen_recorder/export_symbols.h>
#include <vector>
#include <qtutils/disable_utils_warnings.h>
#include <QtGlobal>
#include <QMetaType>
#include <QPixmap>
#include <QDateTime>


SCRREC_DT_NSP_P1

struct SCRREC_EXPORT SScreenshotData{
    ::std::vector<QPixmap>  screenshots;
    QDateTime               scrinshootDateTimeUtc;
};


class SCRREC_EXPORT CScreenshotData{
public:
    CScreenshotData();
    CScreenshotData(const CScreenshotData& cM);
    CScreenshotData(CScreenshotData&& mM);

    SScreenshotData* screenshotData();
    const SScreenshotData* screenshotData()const;
    CScreenshotData& operator=(const CScreenshotData& cM);
    bool operator == (const CScreenshotData& a_cM) const;

private:
    ::std::shared_ptr<SScreenshotData>  m_data;
};
Q_DECLARE_METATYPE(SCRREC_DT_NSP CScreenshotData)


SCRREC_DT_NSP_P2
