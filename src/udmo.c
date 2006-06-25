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
|  udmo.c - implementation, general DMO routines                             |
\****************************************************************************/


#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tools.h"
#include "udmo.h"


char *DMOActionName[] = {
/*  0 Jump */               "JU",
/*  1 Crouch */             "CR",
/*  2 Fire */               "FW",
/*  3 Aim_Up */             "AU",
/*  4 Aim_Down */           "AD",
/*  5 Run */                "RU",
/*  6 Look_Left */          "LL",
/*  7 Look_Right */         "LR", 
/*  8 Weapon */             "",
/*  9 Weapon */             "",
/* 10 Weapon */             "",
/* 11 Weapon */             "",
/* 12 Steroids */           "ST",
/* 13 Look_Up */            "LU",
/* 14 Look_Down */          "LD",
/* 15 NightVision */        "VI",
/* 16 MedKit */             "MK",
/* 17 AutoRun ?? */         "AR",
/* 18 Center_View */        "LC",
/* 19 Holster_Weapon */     "HW",
/* 20 Inventory_Left */     "IL",
/* 21 Pause */              "PA",
/* 22 Quick_Kick */         "QK",
/* 23 AimMode */            "AM",
/* 24 Holo_Duke */          "HD",
/* 25 Jetpack */            "JP",
/* 26 GameQuit */           "GQ",
/* 27 Inventory_Right */    "IR",
/* 28 TurnAround */         "TA",
/* 29 Open (Use Thing) */   "UT",
/* 30 Inventory */          "IN",
/* 31 Esc */                "EC"
                       };

/******************************************************************************/
/** DMO ***********************************************************************/
/******************************************************************************/

#define DMO_BUFFER 65536L

void
DMO_init(DMO_t *d, char *filename, char *mode)
{
	struct stat buf;

	memset(d, 0, sizeof(*d));

	if ((d->filename = strdup(filename))==NULL)
		syserror(errno,"strdup");
	if ((d->file=fopen(d->filename, mode))==NULL)
		syserror(errno,d->filename);
	if (SETVBUF(d->file,NULL,_IOFBF,BUFSIZ>DMO_BUFFER?BUFSIZ:DMO_BUFFER)!=0)
		syserror(errno,"setvbuf");
	if (stat(d->filename,&buf)==-1)
		syserror(errno,d->filename);
	d->filesize=buf.st_size;
}

void
DMO_readheader(DMO_t *d)
{
  unsigned long newheadersize;

  rewind(d->file);
  d->headersize=HEADER_DUKE_old;
  if ((unsigned long)fread(d->header,1,d->headersize,d->file)!=d->headersize) 
    syserror(FIREAD,d->filename);
	/* fprintf(stderr, "got DUKE_old header\n"); */
  d->tics= ((unsigned long)d->header[0]      ) |
           ((unsigned long)d->header[1] <<  8) |
           ((unsigned long)d->header[2] << 16) |
           ((unsigned long)d->header[3] << 24); 
  /* game must be set before calling */
  switch (d->game) {
    case DUKE_old:
      d->game=DUKE_old;
      d->gs="Duke Nukem 3D";
      d->vs="1.0 or 1.1";
      d->episode=d->header[4]+1;
      d->map=d->header[5]+1;
      d->skill=d->header[6];
      d->playernum=((unsigned short)d->header[7]     ) |
                   ((unsigned short)d->header[8] << 8);
    break;
	case DUKE_new: {
		newheadersize = HEADER_DUKE_new;
		if (fread(d->header+d->headersize,1,newheadersize-d->headersize,d->file)!=newheadersize-d->headersize) syserror(FIREAD,d->filename);
		d->headersize = newheadersize;
		d->game=DUKE_new;
		d->gs="Duke Nukem 3D";
		d->vs="1.3D";
		d->episode=d->header[0x04] + 1;
		d->map=d->header[0x05] + 1;
		d->skill=d->header[0x06];
		d->m_coop=d->header[0x07];
		d->playernum=
			((unsigned short)d->header[0x08]     ) |
			((unsigned short)d->header[0x09] << 8);
		d->m_monsters_off=
			((unsigned short)d->header[0x0A]     ) |
			((unsigned short)d->header[0x0B] << 8);
		d->m_respawn_monsters=
			((unsigned long)d->header[0x0C]      ) |
			((unsigned long)d->header[0x0D] <<  8) |
			((unsigned long)d->header[0x0E] << 16) |
			((unsigned long)d->header[0x0F] << 24);
		d->m_respawn_items=
			((unsigned long)d->header[0x10]      ) |
			((unsigned long)d->header[0x11] <<  8) |
			((unsigned long)d->header[0x12] << 16) |
			((unsigned long)d->header[0x13] << 24);
		d->m_respawn_inventory=
			((unsigned long)d->header[0x14]      ) |
			((unsigned long)d->header[0x15] <<  8) |
			((unsigned long)d->header[0x16] << 16) |
			((unsigned long)d->header[0x17] << 24);
	}
	break;
	case GAME_DUKE_14PLUS: {
		unsigned long	ho;	/* Header offset. */
		unsigned short	i;	/* Loop counter. */
		size_t		read_rest;
		size_t		read_result;

		newheadersize = HEADER_DUKE_14PLUS;
		read_rest = newheadersize - d->headersize;
		/* fprintf(stderr,"reading DUKE_14PLUS header: ohs=0x%lx rest=%ld nhs=0x%lx\n", d->headersize, read_rest, newheadersize); */
		read_result = fread(d->header+d->headersize,1,read_rest,d->file);
		if (read_result == 0) {
			syserror(errno,"'%s'", d->filename);
		}
		else {
			if (read_result != read_rest) {
				syserror(FIREAD,"reading '%s': expected %ld bytes, got %ld\n",
					d->filename,
					newheadersize-d->headersize,
					read_result);
			}
		}
		/* fprintf(stderr,"got DUKE_14PLUS header\n"); */
		d->headersize = newheadersize;
		d->game=GAME_DUKE_14PLUS;
		d->gs="Duke Nukem 3D";
		d->max_players=16; /* should be fixed but we can never know */
		d->user_name_length = 32;
		d->vs="1.5";
		d->version = d->header[0x04];
		switch (d->version) {
			case 116:
				d->vs="1.4";
			break;
			case 117:
				d->vs="1.5";
			break;
			default:
				d->vs="unknown";
			break;
		}
		d->episode=d->header[0x05] + 1;
		d->map=d->header[0x06] + 1;
		d->skill=d->header[0x07];
		d->m_coop=d->header[0x08];
		d->m_ffire=d->header[0x09];
		d->playernum=
			((unsigned short)d->header[0x0A]     ) |
			((unsigned short)d->header[0x0B] << 8);
		if (d->playernum>d->max_players) {
			syserror(WDMO, "playernum out of range");
		}
		d->m_monsters_off=
			((unsigned short)d->header[0x0C]     ) |
			((unsigned short)d->header[0x0D] << 8);
		d->m_respawn_monsters=
			((unsigned long)d->header[0x0E]      ) |
			((unsigned long)d->header[0x0F] <<  8) |
			((unsigned long)d->header[0x10] << 16) |
			((unsigned long)d->header[0x11] << 24);
		d->m_respawn_items=
			((unsigned long)d->header[0x12]      ) |
			((unsigned long)d->header[0x13] <<  8) |
			((unsigned long)d->header[0x14] << 16) |
			((unsigned long)d->header[0x15] << 24);
		d->m_respawn_inventory=
			((unsigned long)d->header[0x16]      ) |
			((unsigned long)d->header[0x17] <<  8) |
			((unsigned long)d->header[0x18] << 16) |
			((unsigned long)d->header[0x19] << 24);
		d->playerai=
			((unsigned long)d->header[0x1A]      ) |
			((unsigned long)d->header[0x1B] <<  8) |
			((unsigned long)d->header[0x1C] << 16) |
			((unsigned long)d->header[0x1D] << 24);
		d->user_name=(char**) malloc((sizeof(char*))*d->max_players);
		if (d->user_name == NULL) {
			syserror(ENOMEM,"malloc user_name array");
		}
		ho=0x1E;
		for (i=0;i<d->max_players;i++,ho+=d->user_name_length) {
			d->user_name[i] = (char*)malloc(d->user_name_length);
			if (d->user_name[i] == NULL) {
				syserror(ENOMEM,"malloc user_name entry");
			}
			memcpy(d->user_name[i], &(d->header[ho]),
				d->user_name_length);
		}
		d->auto_run=
			((unsigned long)d->header[ho  ]      ) |
			((unsigned long)d->header[ho+1] <<  8) |
			((unsigned long)d->header[ho+2] << 16) |
			((unsigned long)d->header[ho+3] << 24);
		ho+=4;
		memcpy(&(d->boardfilename[0]),&(d->header[ho]),sizeof(d->boardfilename));
		ho+=sizeof(d->boardfilename);

		newheadersize = d->headersize + d->playernum;
		/* fprintf(stderr,"hs=0x%lx pn=%d\n", d->headersize, d->playernum); */
		if (fread(d->header+d->headersize,1,newheadersize-d->headersize,d->file)!=newheadersize-d->headersize) syserror(FIREAD,d->filename);
		d->headersize = newheadersize;

		d->aim_mode = (unsigned char*)malloc(d->playernum);
		for (i=0;i<d->playernum;i++,ho++) {
			d->aim_mode[i] = d->header[ho];
		}
		/* header size is 2a2 + multimode, max header size is 2b2 */
	}
	break;
	case REDNECK: {
      newheadersize = HEADER_REDNECK_BASE;
      if (fread(d->header+d->headersize,1,newheadersize-d->headersize,d->file)!=newheadersize-d->headersize) syserror(FIREAD,d->filename);
      d->headersize = newheadersize;
      d->game=REDNECK;
      d->gs="Redneck Rampage";
      d->vs="0.7 alpha";
      d->headersize=HEADER_REDNECK_BASE;
      d->episode=1; /* don't know any better yet */
      d->skill=d->header[0x07];
      d->m_coop=d->header[0x08]+1;
      d->map=d->header[0x09];
      d->playernum=
		((unsigned short)d->header[0x0A]     ) |
		((unsigned short)d->header[0x0B] << 8);
      d->m_monsters_off=
		((unsigned short)d->header[0x0C]     ) |
		((unsigned short)d->header[0x0D] << 8);
      d->m_respawn_monsters=
		((unsigned long)d->header[0x0E]      ) |
		((unsigned long)d->header[0x0F] <<  8) |
		((unsigned long)d->header[0x10] << 16) |
		((unsigned long)d->header[0x11] << 24);
      d->m_respawn_items=
		((unsigned long)d->header[0x12]      ) |
		((unsigned long)d->header[0x13] <<  8) |
		((unsigned long)d->header[0x14] << 16) |
		((unsigned long)d->header[0x15] << 24);
      d->m_respawn_inventory=
		((unsigned long)d->header[0x16]      ) |
		((unsigned long)d->header[0x17] <<  8) |
		((unsigned long)d->header[0x18] << 16) |
		((unsigned long)d->header[0x19] << 24);
      d->playerai =
		((unsigned long)d->header[0x1A]      ) |
		((unsigned long)d->header[0x1B] <<  8) |
		((unsigned long)d->header[0x1C] << 16) |
		((unsigned long)d->header[0x1D] << 24);
      d->name=(char*)d->header+0x1E;
      newheadersize = d->headersize + d->playernum;
      if (fread(d->header+d->headersize,1,newheadersize-d->headersize,d->file)!=newheadersize-d->headersize) syserror(FIREAD,d->filename);
      d->headersize = newheadersize;
	}
	break;
	default:
		syserror(WDMO,"game %d unknown", d->game);
	break;
  }

  d->incomplete = d->tics % d->playernum;
  d->tics =       d->tics / d->playernum;
  d->time = d->tics*DMO_TICTIME;
  d->datasize=d->filesize-d->headersize;

}

void
DMO_readblock_13(DMO_t *d, CHU_t *c)
{
  long count;

  if (fread(c->header,1,CHUNK_HEADER_SIZE,d->file)!=CHUNK_HEADER_SIZE) 
    syserror(FIREAD,d->filename);
  c->c_size = (((unsigned short) c->header[0]     ) | 
               ((unsigned short) c->header[1] << 8)  ) -4;
  c->u_size =  ((unsigned short) c->header[2]     ) | 
               ((unsigned short) c->header[3] << 8);
  c->tics = c->u_size/( DMO_TIC * d->playernum);
  c->time = c->tics * DMO_TICTIME;
  c->incomplete = c->u_size % (DMO_TIC * d->playernum);
  c->comp_number = ((unsigned short) c->header[4]     ) |
                   ((unsigned short) c->header[5] << 8);
  if (c->u_size > DMO_TIC*MAXTICS) syserror(WDMO,d->filename);

  fprintf(stderr, "c=0x%x, u=0x%x, l=0x0%x\n", c->c_size, c->u_size, 
                                               c->comp_number);
    
  if (fread(c->c_buffer,1,c->c_size,d->file)!=c->c_size) 
    syserror(FIREAD,d->filename);
  count = decompress(c->c_buffer,c->c_size,c->u_buffer,c->u_size,
                     c->comp_number,DMO_TIC*d->playernum);
  if (count!=c->u_size) {
    switch (count) {
      case -1: 
        fprintf(stderr,"malloc failed"); break;
      case -2:
        fprintf(stderr,"code size > 16 bits"); break;
      case -3:
        fprintf(stderr,"input buffer too small"); break;
      case -4:
        fprintf(stderr,"undefined code found"); break;
      case -5:
        fprintf(stderr,"output buffer too small"); break;
      case -6:
        fprintf(stderr,"uncompressed size is not a multiple of %i",DMO_TIC*d->playernum); break;
      default:
        fprintf(stderr,"decompression error code=%li",count); 
    }      
    fprintf(stderr,"\n");
    fflush(stderr);
    syserror(DECOMP,d->filename); 
  }
}


/******************************************************************************\
* Copied from the published source code of Duke Nukem 3D 1.5 *******************
\******************************************************************************/


/* Apperently, the compression routines changed between 1.3D and 1.5. */


static __inline unsigned short _swap16(unsigned short D)
{
    return((D<<8)|(D>>8));
}


static __inline unsigned int _swap32(unsigned int D)
{
    return((D<<24)|((D<<8)&0x00FF0000)|((D>>8)&0x0000FF00)|(D>>24));
}


#include <endian.h>


#if __BYTE_ORDER == __LITTLE_ENDIAN
#define PLATFORM_LITTLEENDIAN 1
#define BUILDSWAP_INTEL16(x) (x)
#define BUILDSWAP_INTEL32(x) (x)
#else
#define PLATFORM_BIGENDIAN 1
#define BUILDSWAP_INTEL16(x) _swap16(x)
#define BUILDSWAP_INTEL32(x) _swap32(x)
#endif


void
copybuf(void *source, void *dest, int size) {
	int i;
	for (i = 0; i < size; i++) ((long*)dest)[i] = ((long*)source)[i];
}


#define FP_OFF(x) ((long) (x))


static char	*lzwbuf1 = NULL;
static short	*lzwbuf2 = NULL;
static short	*lzwbuf3 = NULL;
static int	lzwbuf_initialized = 0;


#define LZWSIZE 16384


void
lzwbuf_init(void)
{
	if (lzwbuf_initialized == 0) {
		lzwbuf1 = malloc( LZWSIZE+(LZWSIZE>>4)   );
		lzwbuf2 = malloc((LZWSIZE+(LZWSIZE>>4))*2);
		lzwbuf3 = malloc((LZWSIZE+(LZWSIZE>>4))*2);
		lzwbuf_initialized = 1;
	}
}


void
lzwbuf_done(void)
{
	CFREE(lzwbuf1);
	CFREE(lzwbuf2);
	CFREE(lzwbuf3);
	lzwbuf_initialized = 0;
}


long
uncompress(char *lzwinbuf, long compleng, char *lzwoutbuf)
{
        long strtot, currstr, numbits, oneupnumbits;
        long i, dat, leng, bitcnt, outbytecnt, *longptr;
        short *shortptr;

	lzwbuf_init();

        shortptr = (short *)lzwinbuf;
        strtot = (long) BUILDSWAP_INTEL16(shortptr[1]);
        if (strtot == 0)
        {
                copybuf((void *)(FP_OFF(lzwinbuf)+4),(void *)(FP_OFF(lzwoutbuf)),((compleng-4)+3)>>2);
                return((long) BUILDSWAP_INTEL16(shortptr[0])); /* uncompleng */
        }
        for(i=255;i>=0;i--) { lzwbuf2[i] = (short) i; lzwbuf3[i] = (short) i; }
        currstr = 256; bitcnt = (4<<3); outbytecnt = 0;
        numbits = 8; oneupnumbits = (1<<8);
        do
        {
                longptr = (long *)&lzwinbuf[bitcnt>>3];
                dat = ((BUILDSWAP_INTEL32(longptr[0])>>(bitcnt&7)) & (oneupnumbits-1));
                bitcnt += numbits;
                if ((dat&((oneupnumbits>>1)-1)) > ((currstr-1)&((oneupnumbits>>1)-1)))
                        { dat &= ((oneupnumbits>>1)-1); bitcnt--; }

                lzwbuf3[currstr] = (short) dat;

                for(leng=0;dat>=256;leng++,dat=lzwbuf3[dat])
                        lzwbuf1[leng] = (char) lzwbuf2[dat];

                lzwoutbuf[outbytecnt++] = (char) dat;
                for(i=leng-1;i>=0;i--) lzwoutbuf[outbytecnt++] = lzwbuf1[i];

                lzwbuf2[currstr-1] = (short) dat; lzwbuf2[currstr] = (short) dat;
                currstr++;
                if (currstr > oneupnumbits) { numbits++; oneupnumbits <<= 1; }
        } while (currstr < strtot);
        return((long) BUILDSWAP_INTEL16(shortptr[0])); /* uncompleng */
}


/******************************************************************************\
* End of the copy from the published source code of Duke Nukem 3D 1.5 **********
\******************************************************************************/
 

void
DMO_readblock_14PLUS(DMO_t *d, CHU_t *c)
{
	size_t	read_result;

	/* Read the compressed length. */
	read_result = fread(c->header,1,2,d->file);
	if (read_result == 0) {
		int	error;

		error = errno;
		fprintf(stderr,"tell(%s)=%ld\n", d->filename, ftell(d->file));
		syserror(error,"reading 2 bytes compression header of '%s'", d->filename);
	}
	else {
		if (read_result != 2) {
			syserror(FIREAD,
			"reading 2 bytes compression header of '%s', got %ld bytes",
			d->filename, read_result);
		}
	}
	c->c_size =
		((unsigned short) c->header[0]     )|
		((unsigned short) c->header[1] << 8);
	/* Read the compressed data in. */
	if (fread(c->c_buffer,1,c->c_size,d->file)!=c->c_size) 
		syserror(FIREAD,"reading %l compressed data from '%s'", c->c_size, d->filename);
	/* Uncompress it. */
	c->u_size = uncompress((char*)(c->c_buffer),c->c_size,(char*)(c->u_buffer));

	/* Store the number of tics just read. */
	c->tics = c->u_size/( DMO_TIC * d->playernum);

	/* Derive the time from the number. */
	c->time = c->tics * DMO_TICTIME;
}
	

void
DMO_readmacroblock(DMO_t *d, CHU_t *m)
{
  CHU_t c;
  unsigned long i, j, tic_size;

	/* fprintf(stderr,"%s start\n", __func__); */

  m->tics = 0;
  do {
	switch (d->game) {
		case DUKE_old:
		case DUKE_new:
			DMO_readblock_13(d, &c);
		break;
		case REDNECK:
		case GAME_DUKE_14PLUS:
			DMO_readblock_14PLUS(d, &c);
		break;
		default:
			syserror(WDMO,"%s: wrong game %d", __func__, d->game);
		break;
	}
    memmove(&(m->c_buffer[m->tics*d->playernum*DMO_TIC]), c.u_buffer, 
            c.tics*d->playernum*DMO_TIC);
    m->tics+=c.tics;
    if (m->tics*d->playernum>MAXTICS) syserror(WDMO,d->filename);
  } while (((unsigned long)ftell(d->file)<d->filesize) && m->tics*d->playernum<MAXTICS);
  tic_size=d->playernum*DMO_TIC;
  for (j=0;j<tic_size;j++) {
    m->u_buffer[j]=m->c_buffer[j]; 
  }
  for (i=1;i<m->tics;i++) {
    for (j=0;j<tic_size;j++) {
      m->u_buffer[i*tic_size+j] = 
        m->u_buffer[(i-1)*tic_size+j] + m->c_buffer[i*tic_size+j]; 
    }
  } 
  m->time = m->tics * DMO_TICTIME;

	/* fprintf(stderr,"%s end\n", __func__); */
}


void
DMO_done(DMO_t *d)
{
	int	i;

	CFCLOSE(d->file);
	if (d->filename != NULL) {
		free(d->filename);
		d->filename = NULL;
	}
	CFREE(d->aim_mode);
	for(i=0;i<d->max_players;i++) {
		CFREE(d->user_name[i]);
	}
	CFREE(d->user_name);

	lzwbuf_done();
}


#define creturn(x) { CFCLOSE(file); return x; }

/* outside */
/* Checks, if a file exists and is a proper DMO file, return the game code */
int
isDMO(char *filename)
{
	struct stat buf;
	FILE *file;
	off_t filesize;
	unsigned long tics;
	unsigned char buffer[HEADER_DUKE_14PLUS+16];
	unsigned long headersize;
	unsigned short playernum;
	unsigned char version;

	/* fprintf(stderr,"checking %s\n",filename); */
	if ((file=fopen(filename, "rb"))==NULL) { return 0; }
	/* fprintf(stderr,"file open ok\n"); */
	if (stat(filename,&buf)==-1) creturn(0); 
	filesize=buf.st_size;
	/* fprintf(stderr,"filesize=%li\n", filesize); */

	headersize=HEADER_DUKE_old;
	if (fread(buffer,1,headersize,file)!=headersize) creturn(0);
	tics=
		((unsigned long)buffer[0]      ) |
		((unsigned long)buffer[1] <<  8) |
		((unsigned long)buffer[2] << 16) |
		((unsigned long)buffer[3] << 24); 
	/* fprintf(stderr,"tics=%li\n",tics); */

	/* try DUKE_old */
	/* fprintf(stderr,"try Duke 3D old\n"); */
	if (checkDMOtail(file, filesize, tics)) creturn(DUKE_old);

	/* try DUKE_new */  
	/* fprintf(stderr,"try Duke 3D new\n"); */
	headersize = HEADER_DUKE_new;
	if (fseek(file, headersize, SEEK_SET)==0) {
		if (checkDMOtail(file, filesize, tics)) creturn(DUKE_new);
	}

	/* try GAME_DUKE_14PLUS */
	/* fprintf(stderr,"try Duke 3D 1.4 plus\n"); */
	rewind(file);
	headersize = HEADER_DUKE_14PLUS;
	if (fread(buffer,1,headersize,file) == headersize) {
		version = buffer[0x04];
		/* fprintf(stderr,"version = %d\n", version); */
		if (version == 116 || version == 117) {
			playernum=
				((unsigned short)buffer[0x0A]     ) |
				((unsigned short)buffer[0x0B] << 8);
			/* fprintf(stderr,"player = %d\n", playernum); */
			if (playernum>0 && playernum<=16) {
				headersize += playernum;
				if (fseek(file, headersize, SEEK_SET)==0) {
					/* fprintf(stderr,"seeked to 0x%lx\n", headersize); */
					if (checkDMOtail(file, filesize, tics)) {
						/* fprintf(stderr,"found 1.4PLUS\n"); */
						creturn(GAME_DUKE_14PLUS);
					}
				}
			}
		}
	}
	/* fprintf(stderr,"not found 1.4PLUS\n"); */

  /* try Redneck Rampage */
  /* fprintf(stderr,"try Redneck Rampage\n"); */
  rewind(file);
  headersize = HEADER_REDNECK_BASE;
  if (fread(buffer,1,headersize,file) != headersize) creturn(0);
  playernum=((unsigned short)buffer[0x0A]     ) |
            ((unsigned short)buffer[0x0B] << 8);
  if (playernum==0 || playernum>8) creturn(0);
  headersize += playernum;
  if (fseek(file, headersize, SEEK_SET)!=0) return 0;
  if (checkDMOtail(file, filesize, tics)) creturn(REDNECK);

  /* nothing at all */
  creturn(0);
}

int checkDMOtail(FILE* f, off_t filesize, unsigned long tics)
{
  char unsigned buffer[4];
  unsigned short chunk_size_compressed, chunk_size_uncompressed;
  
  while (tics>0) {
    /* fprintf(stderr,"pos is %li\n", ftell(f)); */
    if (fread(buffer,1,4,f)!=4) return 0;
    chunk_size_compressed=((unsigned short)buffer[0]     ) |
                          ((unsigned short)buffer[1] << 8);
    chunk_size_uncompressed=((unsigned short)buffer[2]     ) |
                            ((unsigned short)buffer[3] << 8);
    /*
    fprintf(stderr,"c=%i, u=%i\n",chunk_size_compressed, chunk_size_uncompressed);
    */
    if (chunk_size_compressed<CHUNK_HEADER_SIZE) return 0;
    if (fseek(f, chunk_size_compressed-2, SEEK_CUR)!=0) return 0;
    if (chunk_size_uncompressed%DMO_TIC) return 0;
    tics-=chunk_size_uncompressed/DMO_TIC;
  }
  return (ftell(f)==filesize);
}

/*- file end udmo.c ----------------------------------------------------------*/

