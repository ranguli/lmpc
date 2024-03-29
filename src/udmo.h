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
|  udmo.h - header file of udmo.c, general DMO routines                      |
\****************************************************************************/


#ifndef __UDMO_H
#define __UDMO_H


#include <sys/types.h>
#include <stdio.h>


#define HEADER_DUKE_old     0x009   /* 0x0000 .. 0x0008 */
#define HEADER_DUKE_new     0x018   /* 0x0000 .. 0x0017 */
#define HEADER_DUKE_14PLUS  0x2a2   /* 0x0000 .. 0x02a1 */
#define HEADER_REDNECK_BASE 0x21E   /* 0x0000 .. 0x021D */
#define CHUNK_HEADER_SIZE 6
#define DMO_TICTIME       1.0/26.0
#define DMO_TIC           10
#define MAXPLAYER_DMO     8
#define MAXTICS           2520

#define DUKE_old          128
#define DUKE_new          256
#define REDNECK           512
#define GAME_DUKE_14PLUS		32768

typedef union DMO_TIC_u {
	struct d1_s {
		signed short	go_x;	/* go x */
		signed short	go_y;	/* go y */
		signed short	turn;	/* angle (turn left or right) */
		unsigned long	use;	/* actions */
	} d1;
	struct d2_s {
		signed char	avel;	/* angle (turn left or right) */
		signed char	horz;	/* angle (aim up or down) */
		signed short	fvel;	/* go x */
		signed short	svel;	/* go y */
		unsigned long	bits;	/* actions */
	} d2;
} DMO_TIC_t;

            
typedef struct {
	char           *filename;
	FILE           *file;
	unsigned char  header[HEADER_DUKE_14PLUS+16];
	unsigned long  headersize;
	int            game;
	char           *gs;
	char           *vs;
	unsigned char	version;
	long  tics;
	unsigned long  incomplete;
	unsigned long  filesize;
	unsigned long  datasize;
	unsigned char  skill;
	unsigned char  episode;
	unsigned char  map;
	unsigned char	m_coop;
	unsigned char	m_ffire;
	double         time;
	unsigned short max_players;
	unsigned short user_name_length;
	unsigned short playernum;
	unsigned long	m_monsters_off;		/* Option /m. */
	unsigned long	m_respawn_monsters;	/* Option /t1. */
	unsigned long	m_respawn_items;	/* Option /t2. */
	unsigned long	m_respawn_inventory;	/* Option /t3. */
	unsigned long	playerai;	/* Option /a. */
	char		*name;
	char		**user_name;
	unsigned long	auto_run;
	unsigned char	boardfilename[128];
	unsigned char	*aim_mode;
} DMO_t;

typedef struct {
	     unsigned char  header[CHUNK_HEADER_SIZE];
	     unsigned short c_size;
	     unsigned short u_size;
	     unsigned long  tics;
         unsigned long  incomplete;
	     unsigned short comp_number;
	     double         time;  
	     unsigned char  c_buffer[DMO_TIC*MAXTICS*2];
	     unsigned char  u_buffer[DMO_TIC*MAXTICS];
	   } CHU_t;


extern char *DMOActionName[];

/* LZW */
int compress(unsigned char *o_buf, long o_buf_size,
             unsigned char *i_buf, long i_buf_size,
             int last_code, int tic_size, int *used_last_code);
int decompress(unsigned char *i_buf, long i_buf_size,
               unsigned char *o_buf, long o_buf_size,
               int last_code, int tic_size);

/* DMO */
void DMO_init(DMO_t *d, char *filename, char *mode);
void DMO_readheader(DMO_t *d);
void DMO_readblock(DMO_t *d, CHU_t *c);
void DMO_readmacroblock(DMO_t *d, CHU_t *m);
void DMO_done(DMO_t *d);

/* outside */
int isDMO(char *filename);
int checkDMOtail(FILE* f, off_t filesize, unsigned long tics);

#endif /* __UDMO_H */


/*-- file end udmo.h ---------------------------------------------------------*/
