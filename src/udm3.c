/*  lmpc -- the Little Movie Processing Centre
    Copyright (C) 1994-2000,2004 Uwe Girlich

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
|  udm2.c - implementation, general DM3 routines                             |
\****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "tools.h"
#include "record.h"
#include "udm3.h"


token_t DM3_token[]={
	{ "",	GEN_NOTHING,	0 },
};

void udm3_init(void)
{
	/* Nothing to init yet. */
}

void udm3_done(void)
{
	/* Nothing to clean up yet. */
}

/* check, if file is DM3 */
unsigned long isDM3bin(char *filename)
{
  FILE *file;
  struct stat buf;
  size_t filesize;
  unsigned char buffer[10];
  unsigned long blocksize;
  unsigned long tics;

  if ((file=fopen(filename, "rb"))==NULL) return 0;
  if (stat(filename,&buf)==-1) return 0;
  filesize=buf.st_size;
  rewind(file);
  tics=0;
  while (ftell(file)<(long)filesize) {
    tics++;
    /* Get the sequence number. */
    if (fread(buffer, 1, 4, file)!=4) return 0;
    /* Get the block size. */
    if (fread(buffer, 1, 4, file)!=4) return 0;
    blocksize = ((buffer[0])    ) +
                ((buffer[1])<< 8) +
                ((buffer[2])<<16) +
                ((buffer[3])<<24);
    if (blocksize==0xFFFFFFFF) break;
    if (fseek(file, blocksize, SEEK_CUR)!=0) return 0;
  }
  if (ftell(file)!=(long)filesize) return 0;
  fclose(file);
  return tics;
}

/* check, if file is DM3 source */
int isDM3txt(char* filename)
{
  FILE *file;
  /* 
  struct stat buf;
  size_t filesize;
  */
  #define BUFFER_SIZE 1000
  char buffer[BUFFER_SIZE];
  long blocksize;
  long p;
  int state;

  if ((file=fopen(filename, "rb"))==NULL) return 0;
  /* 
  if (stat(filename,&buf)==-1) return 0;
  filesize=buf.st_size;
  */
  rewind(file);
  if ((blocksize=fread(buffer,1,BUFFER_SIZE,file))==0) return 0;
  if(fclose(file)) return 0;
  if (blocksize<20) return 0;
  state = 0;
  for (p=0;p<blocksize;p++) {
    switch (state) {
      /* not a comment */
      case 0: if (strncmp (&(buffer[p]), "block", 5)==0) return 1;
              if (strncmp (&(buffer[p]), "endblock", 8)==0) return 1;
              if (strncmp (&(buffer[p]), "//", 2)==0) { state=1; break; }
              if (buffer[p]!=' ' && buffer[p]!='\t' && 
                  buffer[p]!='\r' && buffer[p]!='\n' &&
                  buffer[p]!='\f') return 0;
              break;
      /* a comment */
      case 1: if (buffer[p]=='\n') state=0; 
              break;
    }
  } 
  /* If it comes to this line it may be a DM2 txt file but we don't 
     beleave it. */
  return 0;
}


void DM3_init(DM3_t* d, char *filename, char *mode)
{
  struct stat buf;
  
  if ((d->filename = strdup(filename))==NULL) syserror(errno,"strdup");
  if (strcmp(d->filename,"-")==0) {
    d->file=stdout;
  }
  else {
    DM3_open(d,mode);
    if (stat(d->filename,&buf)==-1) syserror(errno,d->filename);
    d->filesize=buf.st_size;
  }
  /* we need a rewind here because some fopen modes may seek at the end */
  rewind(d->file);
}

void DM3_prepare_write_bin(DM3_t* d)
{
  output_file = d->file;
  output_filename = d->filename;
} 

void DM3_open(DM3_t *d, char *mode)
{
  if ((d->file=fopen(d->filename, mode))==NULL) syserror(errno,d->filename);
}
  
void DM3_close(DM3_t *d)
{
  if (fclose(d->file)) syserror(errno,d->filename);
  d->file=NULL;
}

void DM3_done(DM3_t* d)
{
  DM3_close(d);
  free(d->filename);
}

void DM3_block_read_bin(DM3_t *d _U_, DM3_binblock_t* m _U_)
{
}


node* DM3_bin_to_node(DM3_binblock_t *m _U_, int opt _U_)
{
  return NULL;
}

/*----------------------------------------------------------------------------*/
/* end of specific message decoding ------------------------------------------*/
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/* block output: text version *************************************************/
/******************************************************************************/

node* DM3_block_write_text(node* b)
{
  /* b may be NULL (something was wrong or removed) */
  if (b == NULL) return b;
  /* type is TOKEN_BETWEENBLOCK, TOKEN_ENDBLOCK or 
     TOKEN_BLOCK with down as first message */
  if ( b->type == TOKEN_BETWEENBLOCK || b->type == TOKEN_ENDBLOCK ||
      (b->type == TOKEN_BLOCK && b->down!=NULL)) {
    node_write_text(b,0);
  }
  return b;
}

/******************************************************************************/
/* block output: binary version ***********************************************/
/******************************************************************************/

node* DM3_block_write_bin(node* b)
{
	return b;
}


/******************************************************************************/
/** DM3 block editing *********************************************************/
/******************************************************************************/

void DM3_block_edit(node *n _U_)
{
}


/*- file end udm3.c ---------------------------------------------------------*/
