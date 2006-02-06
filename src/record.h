/*  lmpc -- the Little Movie Processing Centre
    Copyright (C) 1994-99,2004 Uwe Girlich

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Uwe Girlich
    Germany
    E-mail: uwe@planetquake.com */

/****************************************************************************\
|  record.h - general header file for recordings                             |
\****************************************************************************/


#ifndef __RECORD_H
#define __RECORD_H


#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <stdio.h> /* for FILE */


#define H_RECORD_HINT (1<<17)


#define H_RECORD_STOPSOUND	( H_RECORD_HINT | 1 )


#include "udemy.h"


#ifndef VEC_DEFINED
typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];
#define VEC_DEFINED
#endif


extern FILE* output_file;
extern char* output_filename;
extern int global_serverversion;
#define SERVERVERSION global_serverversion


#endif /* __RECORD_H */


/*-- file end record.h ------------------------------------------------------*/
