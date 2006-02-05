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
|  udm3.h - header file of udm3.c, general DM3 routines                      |
\****************************************************************************/


#ifndef __UDM3_H
#define __UDM3_H


#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif


#include "node.h"


#define QUAKE3 16384


extern void udm3_init(void);
extern void udm3_done(void);

extern unsigned long isDM3bin(char *filename);
extern int isDM3txt(char *filename);

typedef struct DM3_s {
	char	*filename;
	size_t	filesize;
	FILE	*file;
	int	frame;
} DM3_t;


#define DM3_BBB_MAX 65536

typedef struct DM3_binblock_s {
         long size;
         unsigned char buffer[DM3_BBB_MAX];
         unsigned char* p;
         unsigned char* end;
         long base;
         void* top;
         long hint;
} DM3_binblock_t;

extern token_t DM3_token[];

extern void DM3_init(DM3_t* d, char *filename, char *mode);
extern void DM3_prepare_write_bin(DM3_t* d);
extern void DM3_open(DM3_t *d, char *mode);
extern void DM3_close(DM3_t *d);
extern void DM3_done(DM3_t* d);
extern void DM3_block_read_bin(DM3_t *d, DM3_binblock_t* m);
extern node* DM3_bin_to_node(DM3_binblock_t *m, int opt);
extern node* DM3_block_write_text(node* b);
extern node* DM3_block_write_bin(node* b);

extern void DM3_block_edit(node *n);

      
#endif /* __UDM3_H */


/*-- file end udm3.h --------------------------------------------------------*/
