/*
** FAAD - Freeware Advanced Audio Decoder
** Copyright (C) 2002 M. Bakker
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**
** $Id: faad.h,v 1.3 2002/01/20 17:00:48 menno Exp $
**/

#ifndef __AACDEC_H__
#define __AACDEC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef _WIN32
  #pragma pack(push, 8)
  #ifndef FAADAPI
    #define FAADAPI __stdcall
  #endif
#else
  #ifndef FAADAPI
    #define FAADAPI
  #endif
#endif


#define MAIN 0
#define LC   1
#define SSR  2
#define LTP  3

#define FAAD_FMT_16BIT 1
#define FAAD_FMT_24BIT 2
#define FAAD_FMT_32BIT 3
#define FAAD_FMT_FLOAT 4

/* A decode call can eat up to FAAD_MIN_STREAMSIZE octets per decoded channel,
   so at least so much octets per channel should be available in this stream */
#define FAAD_MIN_STREAMSIZE 768 /* 6144 bits/channel */


typedef void *faacDecHandle;
typedef void *faacProgConfig;

typedef struct faacDecConfiguration
{
    short defObjectType;
    unsigned long defSampleRate;
    unsigned long outputFormat;
} faacDecConfiguration, *faacDecConfigurationPtr;

typedef struct faacDecFrameInfo
{
    int bytesconsumed;
    int samples;
    int channels;
    int error;
} faacDecFrameInfo;

char* FAADAPI faacDecGetErrorMessage(int errcode);

faacDecHandle FAADAPI faacDecOpen();

faacDecConfigurationPtr FAADAPI faacDecGetCurrentConfiguration(faacDecHandle hDecoder);

int FAADAPI faacDecSetConfiguration(faacDecHandle hDecoder,
                                    faacDecConfigurationPtr config);

/* Init the library based on info from the AAC file (ADTS/ADIF) */
int FAADAPI faacDecInit(faacDecHandle hDecoder,
                        unsigned char *buffer,
                        unsigned long *samplerate,
                        unsigned long *channels);

/* Init the library using a DecoderSpecificInfo */
int FAADAPI faacDecInit2(faacDecHandle hDecoder, unsigned char *pBuffer,
                         unsigned long SizeOfDecoderSpecificInfo,
                         unsigned long *samplerate, unsigned long *channels);

void* FAADAPI faacDecDecode(faacDecHandle hDecoder,
                            faacDecFrameInfo *hInfo,
                            unsigned char *buffer);

void FAADAPI faacDecClose(faacDecHandle hDecoder);

int FAADAPI AudioSpecificConfig(unsigned char *pBuffer,
                                unsigned long *samplerate,
                                unsigned long *channels,
                                unsigned long *sf_index,
                                unsigned long *object_type);

#ifdef _WIN32
  #pragma pack(pop)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif