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
** $Id: error.c,v 1.1 2002/01/14 19:15:55 menno Exp $
**/

#include "error.h"

extern char *err_msg[] = {
    "No error",
    "Gain control not yet implemented",
    "Pulse coding not allowed in short blocks",
    "Invalid huffman codebook",
    "Negative scalefactor found, should be impossible",
    "Unable to find ADTS syncword",
    "Channel coupling not yet implemented",
};