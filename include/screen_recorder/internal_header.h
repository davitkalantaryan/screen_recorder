//
// file:			internal_header.h
// path:			include/screen_recorder/internal_header.h
// created on:		2023 Jan 15
// created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#ifndef SCREEN_RECORDER_INCLUDE_SCREEN_RECORDER_INTERNAL_HEADER_H
#define SCREEN_RECORDER_INCLUDE_SCREEN_RECORDER_INTERNAL_HEADER_H

#include <cinternal/internal_header.h>

#define SCRREC_DT_NSP_D     ::screen_recorder::
#define SCRREC_DT_NSP_P1_D  namespace screen_recorder{
#define SCRREC_DT_NSP_P2_D  }

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
#ifdef CPPUTILS_GCC_BUG_56480
#define SCRREC_DT_NSP       ::
#define SCRREC_DT_NSP_P1
#define SCRREC_DT_NSP_P2
#else
#define SCRREC_DT_NSP       SCRREC_DT_NSP_D
#define SCRREC_DT_NSP_P1    SCRREC_DT_NSP_P1_D
#define SCRREC_DT_NSP_P2    SCRREC_DT_NSP_P2_D
#endif


#endif  // #ifndef SCREEN_RECORDER_INCLUDE_SCREEN_RECORDER_INTERNAL_HEADER_H
