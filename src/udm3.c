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
|  udm3.c - implementation, general DM3 routines                             |
\****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "tools.h"
#include "record.h"
#include "udm3.h"
#include "binblock.h"


token_t DM3_token[]={
	{ "block",		TOKEN_BLOCK,		0	},
	{ "endblock",		TOKEN_ENDBLOCK,		0	},
	{ "sequence",		TOKEN_SEQUENCE,		0	},
	{ "size",		TOKEN_SIZE,		0	},
	{ "rel_ack",		TOKEN_REL_ACK,		0	},
	{ "data",		TOKEN_DATA,		0	},
	{ "rest",		TOKEN_REST,		0	},
	{ "bits",		TOKEN_BITS,		0	},
	{ "bytes",		TOKEN_BYTES,		0	},
	{ "bad",		TOKEN_BYTES,		0	},
	{ "nop",		TOKEN_BYTES,		0	},
	{ "gamestate",		TOKEN_GAMESTATE,	0	},
	{ "configstring",	TOKEN_CONFIGSTRING,	0	},
	{ "baseline",		TOKEN_BASELINE,		0	},
	{ "serverCommand",	TOKEN_SERVERCOMMAND,	0	},
	{ "download",		TOKEN_DOWNLOAD,		0	},
	{ "snapshot",		TOKEN_SNAPSHOT,		0	},
	{ "EOF",		TOKEN_EOF,		0	},
	{ "serverTime",		TOKEN_SERVERTIME,	0	},
	{ "deltaNum",		TOKEN_DELTANUM,		0	},
	{ "snapFlags",		TOKEN_SNAPFLAGS,	0	},
	{ "areamask",		TOKEN_AREAMASK,		0	},
	{ "unknown",		TOKEN_UNKNOWN,		0	},
	{ "incomplete",		TOKEN_INCOMPLETE,	0	},
	{ "player",		TOKEN_PLAYER,		0	},
	{ "commandTime",	TOKEN_COMMANDTIME,	0	},
	{ "origin[0]",		TOKEN_ORIGIN_0,		0,	},
	{ "origin[1]",		TOKEN_ORIGIN_1,		0,	},
	{ "bobCycle",		TOKEN_BOBCYCLE,		0,	},
	{ "velocity[0]",	TOKEN_VELOCITY_0,	0,	},
	{ "velocity[1]",	TOKEN_VELOCITY_1,	0,	},
	{ "viewangles[1]",	TOKEN_VIEWANGLES_1,	0,	},
	{ "viewangles[0]",	TOKEN_VIEWANGLES_0,	0,	},
	{ "weaponTime",		TOKEN_WEAPONTIME,	0,	},
	{ "origin[2]",		TOKEN_ORIGIN_2,		0,	},
	{ "velocity[2]",	TOKEN_VELOCITY_2,	0,	},
	{ "legsTimer",		TOKEN_LEGSTIMER,	0,	},
	{ "pm_time",		TOKEN_PM_TIME,		0,	},
	{ "eventSequence",	TOKEN_EVENTSEQUENCE,	0,	},
	{ "torsoAnim",		TOKEN_TORSOANIM,	0,	},
	{ "movementDir",	TOKEN_MOVEMENTDIR,	0,	},
	{ "events[0]",		TOKEN_EVENTS_0,		0,	},
	{ "legsAnim",		TOKEN_LEGSANIM,		0,	},
	{ "events[1]",		TOKEN_EVENTS_1,		0,	},
	{ "pm_flags",		TOKEN_PM_FLAGS,		0,	},
	{ "groundEntityNum",	TOKEN_GROUNDENTITYNUM,	0,	},
	{ "weaponstate",	TOKEN_WEAPONSTATE,	0,	},
	{ "eFlags",		TOKEN_EFLAGS,		0,	},
	{ "externalEvent",	TOKEN_EXTERNALEVENT,	0,	},
	{ "gravity",		TOKEN_GRAVITY,		0,	},
	{ "speed",		TOKEN_SPEED,		0,	},
	{ "delta_angles[1]",	TOKEN_DELTAANGLES_1,	0,	},
	{ "externalEventParm",	TOKEN_EXTERNALEVENTPARM,0,	},
	{ "viewheight",		TOKEN_VIEWHEIGHT,	0,	},
	{ "damageEvent",	TOKEN_DAMAGEEVENT,	0,	},
	{ "damageYaw",		TOKEN_DAMAGEYAW,	0,	},
	{ "damagePitch",	TOKEN_DAMAGEPITCH,	0,	},
	{ "damageCount",	TOKEN_DAMAGECOUNT,	0,	},
	{ "generic1",		TOKEN_GENERIC1,		0,	},
	{ "pm_type",		TOKEN_PM_TYPE,		0,	},
	{ "delta_angles[0]",	TOKEN_DELTAANGLES_0,	0,	},
	{ "delta_angles[2]",	TOKEN_DELTAANGLES_2,	0,	},
	{ "torsoTimer",		TOKEN_TORSOTIMER,	0,	},
	{ "eventParms[0]",	TOKEN_EVENTPARMS_0,	0,	},
	{ "eventParms[1]",	TOKEN_EVENTPARMS_1,	0,	},
	{ "clientNum",		TOKEN_CLIENTNUM,	0,	},
	{ "weapon",		TOKEN_WEAPON,		0,	},
	{ "viewangles[2]",	TOKEN_VIEWANGLES_2,	0,	},
	{ "grapplePoint[0]",	TOKEN_GRAPPLEPOINT_0,	0,	},
	{ "grapplePoint[1]",	TOKEN_GRAPPLEPOINT_1,	0,	},
	{ "grapplePoint[2]",	TOKEN_GRAPPLEPOINT_2,	0,	},
	{ "jumppad_ent",	TOKEN_JUMPPAD_ENT,	0,	},
	{ "loopSound",		TOKEN_LOOPSOUND,	0,	},
	{ "index",		TOKEN_INDEX,		0,	},
	{ "value",		TOKEN_VALUE,		0,	},
	{ "stat",		TOKEN_STAT,		0,	},
	{ "stats",		TOKEN_STATS,		0,	},
	{ "persistant",		TOKEN_PERSISTANT,	0,	},
	{ "persistants",	TOKEN_PERSISTANTS,	0,	},
	{ "ammo",		TOKEN_AMMO,		0,	},
	{ "ammos",		TOKEN_AMMOS,		0,	},
	{ "powerup",		TOKEN_POWERUP,		0,	},
	{ "powerups",		TOKEN_POWERUPS,		0,	},
	{ "",			GEN_NOTHING,		0 	}
};


int playertoken[] = {
	TOKEN_COMMANDTIME,
	TOKEN_ORIGIN_0,
	TOKEN_ORIGIN_1,
	TOKEN_BOBCYCLE,
	TOKEN_VELOCITY_0,
	TOKEN_VELOCITY_1,
	TOKEN_VIEWANGLES_1,
	TOKEN_VIEWANGLES_0,
	TOKEN_WEAPONTIME,
	TOKEN_ORIGIN_2,
	TOKEN_VELOCITY_2,
	TOKEN_LEGSTIMER,
	TOKEN_PM_TIME,
	TOKEN_EVENTSEQUENCE,
	TOKEN_TORSOANIM,
	TOKEN_MOVEMENTDIR,
	TOKEN_EVENTS_0,
	TOKEN_LEGSANIM,
	TOKEN_EVENTS_1,
	TOKEN_PM_FLAGS,
	TOKEN_GROUNDENTITYNUM,
	TOKEN_WEAPONSTATE,
	TOKEN_EFLAGS,
	TOKEN_EXTERNALEVENT,
	TOKEN_GRAVITY,
	TOKEN_SPEED,
	TOKEN_DELTAANGLES_1,
	TOKEN_EXTERNALEVENTPARM,
	TOKEN_VIEWHEIGHT,
	TOKEN_DAMAGEEVENT,
	TOKEN_DAMAGEYAW,
	TOKEN_DAMAGEPITCH,
	TOKEN_DAMAGECOUNT,
	TOKEN_GENERIC1,
	TOKEN_PM_TYPE,
	TOKEN_DELTAANGLES_0,
	TOKEN_DELTAANGLES_2,
	TOKEN_TORSOTIMER,
	TOKEN_EVENTPARMS_0,
	TOKEN_EVENTPARMS_1,
	TOKEN_CLIENTNUM,
	TOKEN_WEAPON,
	TOKEN_VIEWANGLES_2,
	TOKEN_GRAPPLEPOINT_0,
	TOKEN_GRAPPLEPOINT_1,
	TOKEN_GRAPPLEPOINT_2,
	TOKEN_JUMPPAD_ENT,
	TOKEN_LOOPSOUND
};

void
udm3_init(void)
{
	/* Nothing to init yet. */
}

void
udm3_done(void)
{
	/* Nothing to clean up yet. */
}

/* check, if file is DM3 */
unsigned long
isDM3bin(char *filename)
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
int
isDM3txt(char* filename _U_)
{
	return 0;
}


void
DM3_init(DM3_t* d, char *filename, char *mode)
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


void
DM3_prepare_write_bin(DM3_t* d)
{
	output_file = d->file;
	output_filename = d->filename;
} 


void
DM3_open(DM3_t *d, char *mode)
{
	if ((d->file=fopen(d->filename, mode))==NULL) syserror(errno,d->filename);
}

  
void
DM3_close(DM3_t *d)
{
	if (fclose(d->file)) syserror(errno,d->filename);
	d->file=NULL;
}


void
DM3_done(DM3_t* d)
{
	DM3_close(d);
	free(d->filename);
}


void
DM3_block_read_bin(DM3_t *d, DM3_binblock_t* m)
{
        int                     s;

	if (d->file == NULL) {
		syserror(EINVAL,"file '%s' not open for reading.", d->filename);
	}

        /* get the sequence number */
	if (fread(&s,1,4,d->file)!=4) syserror(FIREAD,"sequence number of file '%s'", d->filename);
        m->serverMessageSequence = LittleLong( s );

        /* init the message */
        MSG_Init( &(m->buf), m->bufData, sizeof( m->bufData ) );

        /* get the length */
        if (fread(&(m->buf.cursize),1,4,d->file) != 4)
		syserror(FIREAD,"data block length of file '%s'", d->filename);

        m->buf.cursize = LittleLong( m->buf.cursize );
        if ( m->buf.cursize != -1 ) {
		size_t result;

		if ( m->buf.cursize > m->buf.maxsize ) {
			syserror(EINVAL,"%s: demoMsglen (%d) > MAX (%d)", __func__,
			m->buf.cursize, m->buf.maxsize);
		}
		result = fread(m->buf.data,1,m->buf.cursize,d->file);
		if (result != (size_t)m->buf.cursize) {
			syserror(FIREAD,"reading %d bytes, got %d bytes in %s", m->buf.cursize, result, d->filename);
		}
		m->buf.readcount = 0;
	}
}


node*
DM3_bin_to_node(DM3_binblock_t *m, int opt _U_)
{
	node	*n, *tn, *ttn;
#if 0
	int	i;
#endif
	int	c;
	long	rel_ack;
	int	loop_end;

	n = NULL;
	tn=node_command_init(TOKEN_SEQUENCE, V_INT, H_LONG, NODE_VALUE_INT_dup(m->serverMessageSequence), 0);
	if (m->buf.cursize == -1) {
		return node_init_all(TOKEN_ENDBLOCK,H_DM3_ENDBLOCK,tn,0);
	}
	tn = node_link(tn, node_command_init(TOKEN_SIZE, V_INT, H_LONG, NODE_VALUE_INT_dup(m->buf.cursize), 0));

#if 0
	for (ttn=NULL, i=0 ; i<m->buf.cursize ; i++) {
		ttn=node_link(ttn,node_init(V_BYTEHEX, NODE_VALUE_INT_dup(m->buf.data[i]), 0));
	}
	tn=node_link(tn, node_init(TOKEN_DATA,ttn,0));
#endif

	/* Messages are bit streams. */
	MSG_Bitstream(&(m->buf));

	/* Start with a long ack number. */
	rel_ack = MSG_ReadLong(&(m->buf));
	tn=node_link(tn, node_command_init(TOKEN_REL_ACK, V_INT, H_LONG, NODE_VALUE_INT_dup(rel_ack), 0));

	/* Parse the message. */
	loop_end = 0;
	while (1) {
		int	cmd;
		if ( m->buf.readcount > m->buf.cursize ) {
			syserror(DM3INTE,"read past end of server message");
		}
		/* Get the command. */
		cmd = MSG_ReadByte( &(m->buf) );
		switch(cmd) {
			case svc_nop:	/* Complete. */
				tn=node_link(tn, node_init(TOKEN_NOP, NULL, 0));
			break;
			case svc_gamestate:
				tn=node_link(tn, node_init(TOKEN_GAMESTATE, NULL, 0));
				tn=node_link(tn, node_init(TOKEN_UNKNOWN, NULL, 0));
				loop_end = 1;
			break;
			case svc_serverCommand:
				tn=node_link(tn, node_init(TOKEN_SERVERCOMMAND, NULL, 0));
				tn=node_link(tn, node_init(TOKEN_UNKNOWN, NULL, 0));
				loop_end = 1;
			break;
			case svc_download:
				tn=node_link(tn, node_init(TOKEN_DOWNLOAD, NULL, 0));
				tn=node_link(tn, node_init(TOKEN_UNKNOWN, NULL, 0));
				loop_end = 1;
			break;
			case svc_snapshot: {
				node	*tttn;
				int	serverTime;
				int	deltaNum;
				int	snapFlags;
				int	areamask_len;
				unsigned char	areamask_data[MAX_MAP_AREA_BYTES];
				int	i;
				int	lc;
				int	*tp;
				netField_t	*field;

				/* Start empty. */
				ttn = NULL;

				/* Get the serverTime. */
				serverTime = MSG_ReadLong( &(m->buf) );
				ttn = node_link(ttn, node_command_init(TOKEN_SERVERTIME, V_INT, H_LONG, NODE_VALUE_INT_dup(serverTime), 0));
				/* Get the deltaNum. */
				deltaNum = MSG_ReadByte( &(m->buf) );
				ttn = node_link(ttn, node_command_init(TOKEN_DELTANUM, V_INT, H_BYTE, NODE_VALUE_INT_dup(deltaNum), 0));

				/* Get the snapFlags. */
				snapFlags = MSG_ReadByte( &(m->buf) );
				ttn = node_link(ttn, node_command_init(TOKEN_SNAPFLAGS, V_INT, H_BYTE, NODE_VALUE_INT_dup(snapFlags), 0));

				/* Get the areamask length. */
				areamask_len = MSG_ReadByte( &(m->buf) );
				if (areamask_len>(int)sizeof(areamask_data)) {
					syserror(DM3INTE, "areamask has %d bytes, %lu is max.",
						areamask_len, sizeof(areamask_data));
				}

				/* Get the areamask data. */
				MSG_ReadData( &(m->buf), &areamask_data, areamask_len);
				for (tttn=NULL, i=0; i<areamask_len; i++) {
					tttn=node_link(tttn,node_init(V_BYTEHEX, NODE_VALUE_INT_dup(areamask_data[i]), 0));
				}
				ttn=node_link(ttn, node_init(TOKEN_AREAMASK, tttn, 0));

				/* Read player state. */
				tttn = NULL;

				/* Get the max field number. */
				lc = MSG_ReadByte(&(m->buf));
				if (lc>playerStateFields_length) {
					syserror(DM3INTE, "last player field is %d, max allowed is %d", lc, playerStateFields_length);
				}

				/* Loop over the fields. */
				for (i=0,field=playerStateFields,tp=playertoken;
					i<lc;
					i++,field++,tp++) {

					/* Check, if we have this field. */
					if ( MSG_ReadBits( &(m->buf), 1 ) ) {
						if (field->bits == 0) {
							/* Get a float value. */
							union {
								float	value_f;
								int	value_i;
							} value;
							int	hint;

							/* Check the float type. */
							if (MSG_ReadBits( &(m->buf), 1 ) == 0) {
								/* Get an integral float. */
								int	trunc;

								trunc = MSG_ReadBits( &(m->buf), FLOAT_INT_BITS );
								/* Bias to allow equal parts positive and negative. */
								trunc -= FLOAT_INT_BIAS;
								value.value_f = trunc;
								hint = H_FLOAT_INT;
							}
							else {
								/* Get a full floating point value. */
								value.value_i = MSG_ReadBits( &(m->buf), 32 );
								/* TODO: byte order? */
								hint = H_FLOAT;
							}

							tttn = node_link(tttn, node_command_init(*tp, V_FLOAT, hint, NODE_VALUE_FLOAT_dup(value.value_f), 0));
						}
						else {
							/* Get an integer value. */
							tttn = node_link(tttn, node_command_init(*tp, V_INT, H_LONG, NODE_VALUE_INT_dup(MSG_ReadBits( &(m->buf), field->bits )), 0)); 

						}
					}

				} /* End loop over the fields. */

				/* Read the arrays. */
				if (MSG_ReadBits( &(m->buf), 1 ) ) {
					int	bits;
					int	value;
					node	*a;
					node	*e;

					/* Parse the stats. */
					if ( MSG_ReadBits( &(m->buf), 1 ) ) {
						a = NULL;
						bits = MSG_ReadShort (&(m->buf));
						for (i=0 ; i<16 ; i++) {
							if (bits & (1<<i) ) {
								value = MSG_ReadShort(&(m->buf));
								e = node_link(
									node_command_init(TOKEN_INDEX,V_INT,H_SHORT,NODE_VALUE_INT_dup(i),0),
									node_command_init(TOKEN_VALUE,V_INT,H_SHORT,NODE_VALUE_INT_dup(value),0));
								a = node_link(a,node_init(TOKEN_STAT,e,0));
							}
						}
						tttn = node_link(tttn,node_init(TOKEN_STATS,a,0));
					}

					/* Parse the persistant stats. */
					if ( MSG_ReadBits( &(m->buf), 1 ) ) {
						a = NULL;
						bits = MSG_ReadShort (&(m->buf));
						for (i=0 ; i<16 ; i++) {
							if (bits & (1<<i) ) {
								value = MSG_ReadShort(&(m->buf));
								e = node_link(
									node_command_init(TOKEN_INDEX,V_INT,H_SHORT,NODE_VALUE_INT_dup(i),0),
									node_command_init(TOKEN_VALUE,V_INT,H_SHORT,NODE_VALUE_INT_dup(value),0));
								a = node_link(a,node_init(TOKEN_PERSISTANT,e,0));
							}
						}
						tttn = node_link(tttn,node_init(TOKEN_PERSISTANTS,a,0));
					}

					/* Parse the ammo. */
					if ( MSG_ReadBits( &(m->buf), 1 ) ) {
						a = NULL;
						bits = MSG_ReadShort (&(m->buf));
						for (i=0 ; i<16 ; i++) {
							if (bits & (1<<i) ) {
								value = MSG_ReadShort(&(m->buf));
								e = node_link(
									node_command_init(TOKEN_INDEX,V_INT,H_SHORT,NODE_VALUE_INT_dup(i),0),
									node_command_init(TOKEN_VALUE,V_INT,H_SHORT,NODE_VALUE_INT_dup(value),0));
								a = node_link(a,node_init(TOKEN_AMMO,e,0));
							}
						}
						tttn = node_link(tttn,node_init(TOKEN_AMMOS,a,0));
					}

					/* Parse the powerup. */
					if ( MSG_ReadBits( &(m->buf), 1 ) ) {
						a = NULL;
						bits = MSG_ReadShort (&(m->buf));
						for (i=0 ; i<16 ; i++) {
							if (bits & (1<<i) ) {
								value = MSG_ReadLong(&(m->buf));
								e = node_link(
									node_command_init(TOKEN_INDEX,V_INT,H_SHORT,NODE_VALUE_INT_dup(i),0),
									node_command_init(TOKEN_VALUE,V_INT,H_SHORT,NODE_VALUE_INT_dup(value),0));
								a = node_link(a,node_init(TOKEN_POWERUP,e,0));
							}
						}
						tttn = node_link(tttn,node_init(TOKEN_POWERUPS,a,0));
					}

				} /* End read the arrays. */


				/* Connect player parts to existing node tree. */
				ttn=node_link(ttn, node_init(TOKEN_PLAYER, tttn, 0));

				/* Read entity state. */
				/* TODO */

				/* This code is still incomplete. */
				ttn = node_link(ttn, node_init(TOKEN_INCOMPLETE,NULL,0));

				/* Create a "snapshot" node. */
				tn=node_link(tn, node_init(TOKEN_SNAPSHOT, ttn, 0));
				loop_end = 1;
			}
			break;
			case svc_EOF:	/* Complete. */
				tn=node_link(tn, node_init(TOKEN_EOF, NULL, 0));
				loop_end = 1;
			break;
			default:	/* Complete. */
				syserror(DM3INTE,"unknown cmd %d", cmd);
		}

		/* If we know no further, stop this loop. */
		if (loop_end) break;
	}

	for (ttn=NULL; (c = MSG_ReadByte(&(m->buf))) != -1 ; ) {
		ttn=node_link(ttn,node_init(V_BYTEHEX, NODE_VALUE_INT_dup(c), 0));
	}
	tn=node_link(tn, node_init(TOKEN_BYTES, ttn, 0));
	n=node_link(n,node_init_all(TOKEN_BLOCK, H_DM3_BLOCK, tn, 0));
	
	return n;
}


/*----------------------------------------------------------------------------*/
/* end of specific message decoding ------------------------------------------*/
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/* block output: text version *************************************************/
/******************************************************************************/

node*
DM3_block_write_text(node* b)
{
	/* b may be NULL (something was wrong or removed) */
	if (b == NULL) return b;
	node_write_text(b,0);
	return b;
}

/******************************************************************************/
/* block output: binary version ***********************************************/
/******************************************************************************/

node*
DM3_block_write_bin(node* b)
{
	return b;
}


/******************************************************************************/
/** DM3 block editing *********************************************************/
/******************************************************************************/

void
DM3_block_edit(node *n _U_)
{
	/* Do nothing. */
}


/*- file end udm3.c ---------------------------------------------------------*/