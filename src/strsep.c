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
|  strsep.c  -  implementation, missing standard C routine                   |
\****************************************************************************/


/*
 *  Some cross-compilers don't have all standard C library functions.
 *  I copied the following code from the GNU C Library source.
 *  Look in COPYING.LIB for distribution hints of this code.
 */                      


#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif


#include <string.h>


char * strsep(char **pp, const char *delim)
{
  char *p, *q;
         
  if ((p = *pp) == NULL)
    return 0;
  if ((q = strpbrk (p, delim)) != NULL)
  {
    *pp = q + 1;
    *q = '\0';
  }
  else
    *pp = 0;
  return p;
}


/*-- file end strsep.c -----------------------------------------------------*/
