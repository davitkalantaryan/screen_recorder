//  
// file:		core_prepare_gif_file.h
// created on:	2020 Jul 13  
// created by:	D. Kalantaryan (davit.kalantaryan@gmail.com) 
//

#ifndef CORE_PREPARE_GIF_FILE_H
#define CORE_PREPARE_GIF_FILE_H

#include <stdio.h>   // for FILE*
#include <string.h>  // for memcpy and bzero
#include <stdint.h>  // for integer typedefs

struct GifWriter;
struct GifWriter
{
    FILE* f;
    uint8_t* oldImage;
    bool firstFrame;
};


// Creates a gif file.
// The input GIFWriter is assumed to be uninitialized.
// The delay value is the time between frames in hundredths of a second - note that not all viewers pay much attention to this value.
bool GifBegin( GifWriter* writer, const char* filename, uint32_t width, uint32_t height, uint32_t delay, int32_t bitDepth = 8, bool dither = false );

// Writes out a new frame to a GIF in progress.
// The GIFWriter should have been created by GIFBegin.
// AFAIK, it is legal to use different bit depths for different frames of an image -
// this may be handy to save bits in animations that don't change much.
bool GifWriteFrame( GifWriter* writer, const uint8_t* image, uint32_t width, uint32_t height, uint32_t delay, int bitDepth = 8, bool dither = false );

// Writes the EOF code, closes the file handle, and frees temp memory used by a GIF.
// Many if not most viewers will still display a GIF properly if the EOF code is missing,
// but it's still a good idea to write it out.
bool GifEnd( GifWriter* writer );

#endif  // #ifndef CORE_PREPARE_GIF_FILE_H
