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
** $Id: ic_predict.c,v 1.1 2002/01/14 19:15:56 menno Exp $
**/

#ifdef __ICL
#include <mathf.h>
#else
#include <math.h>
#endif
#include "syntax.h"
#include "ic_predict.h"
#include "pns.h"

static void flt_round_inf(float *pf)
{
    int flg;
    unsigned long tmp;
    float *pt = (float *)&tmp;

    *pt = *pf;
    flg = tmp & (unsigned long)0x00008000;
    tmp &= (unsigned long)0xffff0000;
    *pf = *pt;

    /* round 1/2 lsb toward infinity */
    if (flg)
    {
        tmp &= (unsigned long)0xff800000; /* extract exponent and sign */
        tmp |= (unsigned long)0x00010000; /* insert 1 lsb */
        *pf += *pt;                       /* add 1 lsb and elided one */
        tmp &= (unsigned long)0xff800000; /* extract exponent and sign */
        *pf -= *pt;                       /* subtract elided one */
    }
}

static void ic_predict(pred_state *state, float input, float *output, int pred)
{
    float dr1, predictedvalue;
    float e0, e1;
    float k1, k2;

    float *r;
    float *KOR;
    float *VAR;

    r   = state->r;   /* delay elements */
    KOR = state->KOR; /* correlations */
    VAR = state->VAR; /* variances */

    k1 = KOR[0]/VAR[0]*B;

    if (pred)
    {
        /* only needed for the actual predicted value, k1 is always needed */
        k2 = KOR[1]/VAR[1]*B;

        predictedvalue = k1*r[0] + k2*r[1];
        flt_round_inf(&predictedvalue);

        *output = input + predictedvalue;
    } else {
        *output = input;
    }

    /* calculate new state data */
    e0 = *output;
    e1 = e0 - k1 * r[0];

    dr1 = k1 * e0;

    VAR[0] = ALPHA * VAR[0] + (0.5f)*(r[0]*r[0] + e0*e0);
    KOR[0] = ALPHA * KOR[0] + r[0]*e0;
    VAR[1] = ALPHA * VAR[1] + (0.5f)*(r[1]*r[1] + e1*e1);
    KOR[1] = ALPHA * KOR[1] + r[1]*e1;

    r[1] = A * (r[0]-dr1);
    r[0] = A * e0;
}

static void reset_pred_state(pred_state *state)
{
    state->r[0]   = 0.0f;
    state->r[1]   = 0.0f;
    state->KOR[0] = 0.0f;
    state->KOR[1] = 0.0f;
    state->VAR[0] = 1.0f;
    state->VAR[1] = 1.0f;
}

void pns_reset_pred_state(ic_stream *ics, pred_state *state)
{
    int sfb, g, b, i;
    int offs, size;

    /* prediction only for long blocks */
    if (ics->window_sequence == EIGHT_SHORT_SEQUENCE)
        return;

    for (g = 0; g < ics->num_window_groups; g++)
    {
        for (b = 0; b < ics->window_group_length[g]; b++)
        {
            for (sfb = 0; sfb < ics->max_sfb; sfb++)
            {
                if (is_noise(ics, g, sfb))
                {
                    offs = ics->swb_offset[sfb];
                    size = ics->swb_offset[sfb+1] - offs;

                    for (i = offs; i < size; i++)
                        reset_pred_state(&state[i]);
                }
            }
        }
    }
}

void reset_all_predictors(pred_state *state)
{
    int i;

    for (i = 0; i < 1024; i++)
        reset_pred_state(&state[i]);
}

/* intra channel prediction */
void ic_prediction(ic_stream *ics, float *spec, pred_state *state)
{
    int sfb, bin;

    if (ics->window_sequence == EIGHT_SHORT_SEQUENCE)
    {
        reset_all_predictors(state);
    } else {
        for (sfb = 0; sfb < ics->pred.limit; sfb++)
        {
            int low  = ics->swb_offset[sfb];
            int high = ics->swb_offset[sfb+1];

            for (bin = low; bin < high; bin++)
            {
                ic_predict(&state[bin], spec[bin], &spec[bin],
                    (ics->predictor_data_present &&
                    ics->pred.prediction_used[sfb]));
            }
        }

        if (ics->predictor_data_present)
        {
            if (ics->pred.predictor_reset)
            {
                for (bin = ics->pred.predictor_reset_group_number - 1;
                     bin < 1024; bin += 30)
                {
                    reset_pred_state(&state[bin]);
                }
            }
        }
    }
}