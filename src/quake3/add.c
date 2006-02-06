/*  lmpc -- the Little Movie Processing Centre
    Copyright (C) 2006 Uwe Girlich

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
|  add.c - functions needed to link against a part of the Q3A source.        |
\****************************************************************************/


#include "code/game/q_shared.h"
#include <stdio.h>
#include <stdarg.h>


cvar_t *cl_shownet;


void
Com_Printf(const char *fmt, ...)
{
	va_list	argptr;

	va_start (argptr, fmt);
	vprintf (fmt, argptr);
	va_end (argptr);
}


void
Com_Error(int code, const char *fmt, ...)
{
	va_list	argptr;

	printf("Com_Error(%d)\n", code);
	va_start (argptr, fmt);
	vprintf (fmt, argptr);
	va_end (argptr);
}


void
Com_Memcpy (void* dest, const void* src, const size_t count)
{
	memcpy(dest, src, count);
}


void
Com_Memset (void* dest, const int val, const size_t count)
{
	memset(dest, val, count);
}


void
Q_strncpyz(char *dest, const char *src, int destsize)
{
	strncpy( dest, src, destsize-1 );
	dest[destsize-1] = 0;
}


/*- file end add.c -----------------------------------------------------------*/
