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
** $Id: decoder.h,v 1.2 2002/01/19 09:39:41 menno Exp $
**/

#ifndef __DECODER_H__
#define __DECODER_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
  #pragma pack(push, 8)
  #ifndef FAADAPI
    #define FAADAPI __stdcall
  #endif
#else
  #ifndef FAADAPI
    #define FAADAPI
  #endif
#endif

#include "bits.h"
#include "syntax.h"
#include "specrec.h"
#include "filtbank.h"
#include "ic_predict.h"


#define FAAD_FMT_16BIT 1
#define FAAD_FMT_24BIT 2
#define FAAD_FMT_32BIT 3
#define FAAD_FMT_FLOAT 4

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

typedef struct
{
    int adts_header_present;
    int adif_header_present;
    int sf_index;
    int object_type;

    int frame;

    void *sample_buffer;

    int window_shape_prev[MAX_CHANNELS];
    fb_info fb;
    drc_info drc;

    float *time_state[MAX_CHANNELS];
    float *time_out[MAX_CHANNELS];

    pred_state *pred_stat[MAX_CHANNELS];
    float *lt_pred_stat[MAX_CHANNELS];

    float exp_table[256];
    float mnt_table[128];

#if IQ_TABLE_SIZE
    float iq_table[IQ_TABLE_SIZE];
#endif
#if POW_TABLE_SIZE
    float pow2_table[POW_TABLE_SIZE];
#endif

    /* Configuration data */
    faacDecConfiguration config;
} faacDecStruct, *faacDecHandle;


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

void FAADAPI faacDecClose(faacDecHandle hDecoder);

void* FAADAPI faacDecDecode(faacDecHandle hDecoder,
                            faacDecFrameInfo *hInfo,
                            unsigned char *buffer);

#ifdef WIN32
  #pragma pack(pop)
#endif

#ifdef __cplusplus
}
#endif
#endif