//
// file:			export_symbols.h
// path:			include/directory_iterator/export_symbols.h
// created on:		2023 Jan 10
// created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#ifndef DIRECTORY_ITERATOR_INCLUDE_DIRECTORY_ITERATOR_EXPORT_SYMBOLS_H
#define DIRECTORY_ITERATOR_INCLUDE_DIRECTORY_ITERATOR_EXPORT_SYMBOLS_H

#include <cinternal/internal_header.h>


#ifndef DIRITER_EXPORT
#if defined(DIRITER_COMPILING_SHARED_LIB)
#define DIRITER_EXPORT CINTERNAL_DLL_PUBLIC
#elif defined(DIRITER_USING_STATIC_LIB_OR_OBJECTS)
#define CINTERNAL_EXPORT
#elif defined(DIRITER_LOAD_FROM_DLL)
#define DIRITER_EXPORT CINTERNAL_IMPORT_FROM_DLL
#else
#define DIRITER_EXPORT CINTERNAL_DLL_PRIVATE
#endif
#endif


#endif  // #ifndef DIRITER_INCLUDE_DIRITER_EXPORT_SYMBOLS_H
