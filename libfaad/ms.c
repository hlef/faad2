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
** $Id: ms.c,v 1.1 2002/01/14 19:15:56 menno Exp $
**/

#include "syntax.h"
#include "ms.h"
#include "is.h"
#include "pns.h"

void ms_decode(ic_stream *ics, ic_stream *icsr, float *l_spec, float *r_spec)
{
    int g, b, sfb, i;
    int group = 0;

    int k;
    float tmp;

    if (ics->ms_mask_present >= 1)
    {
        for (g = 0; g < ics->num_window_groups; g++) 
        {
            for (b = 0; b < ics->window_group_length[g]; b++)
            {
                for(sfb = 0; sfb < ics->max_sfb; sfb++)
                {
                    /* If intensity stereo coding or noise substitution is on
                       for a particular scalefactor band, no M/S stereo decoding
                       is carried out.
                     */
                    if ((ics->ms_used[g][sfb] || ics->ms_mask_present == 2) &&
                        !is_intensity(icsr, g, sfb) && !is_noise(ics, g, sfb))
                    {
                        for (i = ics->swb_offset[sfb]; i < ics->swb_offset[sfb+1]; i++)
                        {
                            k = (group*128) + i;
                            tmp = l_spec[k] - r_spec[k];
                            l_spec[k] = l_spec[k] + r_spec[k];
                            r_spec[k] = tmp;
                        }
                    }
                }
                group++;
            }
        }
    }
}