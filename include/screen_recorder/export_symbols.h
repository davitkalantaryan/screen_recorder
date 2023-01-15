//
// file:			export_symbols.h
// path:			include/screen_recorder/export_symbols.h
// created on:		2023 Jan 15
// created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#ifndef SCREEN_RECORDER_INCLUDE_SCREEN_RECORDER_EXPORT_SYMBOLS_H
#define SCREEN_RECORDER_INCLUDE_SCREEN_RECORDER_EXPORT_SYMBOLS_H

#include <cinternal/internal_header.h>


#ifndef SCRREC_EXPORT
#if defined(SCRREC_COMPILING_SHARED_LIB)
#define SCRREC_EXPORT CINTERNAL_DLL_PUBLIC
#elif defined(SCRREC_USING_STATIC_LIB_OR_OBJECTS)
#define CINTERNAL_EXPORT
#elif defined(SCRREC_LOAD_FROM_DLL)
#define SCRREC_EXPORT CINTERNAL_IMPORT_FROM_DLL
#else
#define SCRREC_EXPORT CINTERNAL_DLL_PRIVATE
#endif
#endif


#endif  // #ifndef SCREEN_RECORDER_INCLUDE_SCREEN_RECORDER_EXPORT_SYMBOLS_H
