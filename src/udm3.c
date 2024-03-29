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

/* $Id$ */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "tools.h"
#include "record.h"
#include "udm3.h"
#include "binblock.h"


token_t DM3_token[]={
	{ "dm3",		TOKEN_DM3,		1	},
	{ "block",		TOKEN_BLOCK,		1	},
	{ "endblock",		TOKEN_ENDBLOCK,		1	},
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
	{ "origin[0]",		TOKEN_ORIGIN_0,		0	},
	{ "origin[1]",		TOKEN_ORIGIN_1,		0	},
	{ "bobCycle",		TOKEN_BOBCYCLE,		0	},
	{ "velocity[0]",	TOKEN_VELOCITY_0,	0	},
	{ "velocity[1]",	TOKEN_VELOCITY_1,	0	},
	{ "viewangles[1]",	TOKEN_VIEWANGLES_1,	0	},
	{ "viewangles[0]",	TOKEN_VIEWANGLES_0,	0	},
	{ "weaponTime",		TOKEN_WEAPONTIME,	0	},
	{ "origin[2]",		TOKEN_ORIGIN_2,		0	},
	{ "velocity[2]",	TOKEN_VELOCITY_2,	0	},
	{ "legsTimer",		TOKEN_LEGSTIMER,	0	},
	{ "pm_time",		TOKEN_PM_TIME,		0	},
	{ "eventSequence",	TOKEN_EVENTSEQUENCE,	0	},
	{ "torsoAnim",		TOKEN_TORSOANIM,	0	},
	{ "movementDir",	TOKEN_MOVEMENTDIR,	0	},
	{ "events[0]",		TOKEN_EVENTS_0,		0	},
	{ "legsAnim",		TOKEN_LEGSANIM,		0	},
	{ "events[1]",		TOKEN_EVENTS_1,		0	},
	{ "pm_flags",		TOKEN_PM_FLAGS,		0	},
	{ "groundEntityNum",	TOKEN_GROUNDENTITYNUM,	0	},
	{ "weaponstate",	TOKEN_WEAPONSTATE,	0	},
	{ "eFlags",		TOKEN_EFLAGS,		0	},
	{ "externalEvent",	TOKEN_EXTERNALEVENT,	0	},
	{ "gravity",		TOKEN_GRAVITY,		0	},
	{ "speed",		TOKEN_SPEED,		0	},
	{ "delta_angles[1]",	TOKEN_DELTAANGLES_1,	0	},
	{ "externalEventParm",	TOKEN_EXTERNALEVENTPARM,0	},
	{ "viewheight",		TOKEN_VIEWHEIGHT,	0	},
	{ "damageEvent",	TOKEN_DAMAGEEVENT,	0	},
	{ "damageYaw",		TOKEN_DAMAGEYAW,	0	},
	{ "damagePitch",	TOKEN_DAMAGEPITCH,	0	},
	{ "damageCount",	TOKEN_DAMAGECOUNT,	0	},
	{ "generic1",		TOKEN_GENERIC1,		0	},
	{ "pm_type",		TOKEN_PM_TYPE,		0	},
	{ "delta_angles[0]",	TOKEN_DELTAANGLES_0,	0	},
	{ "delta_angles[2]",	TOKEN_DELTAANGLES_2,	0	},
	{ "torsoTimer",		TOKEN_TORSOTIMER,	0	},
	{ "eventParms[0]",	TOKEN_EVENTPARMS_0,	0	},
	{ "eventParms[1]",	TOKEN_EVENTPARMS_1,	0	},
	{ "clientNum",		TOKEN_CLIENTNUM,	0	},
	{ "weapon",		TOKEN_WEAPON,		0	},
	{ "viewangles[2]",	TOKEN_VIEWANGLES_2,	0	},
	{ "grapplePoint[0]",	TOKEN_GRAPPLEPOINT_0,	0	},
	{ "grapplePoint[1]",	TOKEN_GRAPPLEPOINT_1,	0	},
	{ "grapplePoint[2]",	TOKEN_GRAPPLEPOINT_2,	0	},
	{ "jumppad_ent",	TOKEN_JUMPPAD_ENT,	0	},
	{ "loopSound",		TOKEN_LOOPSOUND,	0	},
	{ "index",		TOKEN_INDEX,		0	},
	{ "value",		TOKEN_VALUE,		0	},
	{ "stat",		TOKEN_STAT,		0	},
	{ "stats",		TOKEN_STATS,		0	},
	{ "persistant",		TOKEN_PERSISTANT,	0	},
	{ "persistants",	TOKEN_PERSISTANTS,	0	},
	{ "ammo",		TOKEN_AMMO,		0	},
	{ "ammos",		TOKEN_AMMOS,		0	},
	{ "powerup",		TOKEN_POWERUP,		0	},
	{ "powerups",		TOKEN_POWERUPS,		0	},
	{ "entity",		TOKEN_ENTITY,		0	},
	{ "entities",		TOKEN_ENTITIES,		0	},
	{ "remove",		TOKEN_REMOVE,		0	},
	{ "pos.trTime",		TOKEN_POS_TRTIME,	0	},
	{ "pos.trBase[0]",	TOKEN_POS_TRBASE_0,	0	},
	{ "pos.trBase[1]",	TOKEN_POS_TRBASE_1,	0	},
	{ "pos.trDelta[0]",	TOKEN_POS_TRDELTA_0,	0	},
	{ "pos.trDelta[1]",	TOKEN_POS_TRDELTA_1,	0	},
	{ "pos.trBase[2]",	TOKEN_POS_TRBASE_2,	0	},
	{ "apos.trBase[1]",	TOKEN_APOS_TRBASE_1,	0	},
	{ "pos.trDelta[2]",	TOKEN_POS_TRDELTA_2,	0	},
	{ "apos.trBase[0]",	TOKEN_APOS_TRBASE_0,	0	},
	{ "event",		TOKEN_EVENT,		0	},
	{ "angles2[1]",		TOKEN_ANGLES2_1,	0	},
	{ "eType",		TOKEN_ETYPE,		0	},
	{ "torsoAnim",		TOKEN_TORSOANIM,	0	},
	{ "eventParm",		TOKEN_EVENTPARM,	0	},
	{ "legsAnim",		TOKEN_LEGSANIM,		0	},
	{ "groundEntityNum",	TOKEN_GROUNDENTITYNUM,	0	},
	{ "pos.trType",		TOKEN_POS_TRTYPE,	0	},
	{ "eFlags",		TOKEN_EFLAGS,		0	},
	{ "otherEntityNum",	TOKEN_OTHERENTITYNUM,	0	},
	{ "weapon",		TOKEN_WEAPON,		0	},
	{ "clientNum",		TOKEN_CLIENTNUM,	0	},
	{ "angles[1]",		TOKEN_ANGLES_1,		0	},
	{ "pos.trDuration",	TOKEN_POS_TRDURATION,	0	},
	{ "apos.trType",	TOKEN_APOS_TRTYPE,	0	},
	{ "solid",		TOKEN_SOLID,		0	},
	{ "modelindex",		TOKEN_MODELINDEX,	0	},
	{ "otherEntityNum2",	TOKEN_OTHERENTITYNUM2,	0	},
	{ "origin2[2]",		TOKEN_ORIGIN2_2,	0	},
	{ "origin2[0]",		TOKEN_ORIGIN2_0,	0	},
	{ "origin2[1]",		TOKEN_ORIGIN2_1,	0	},
	{ "modelindex2",	TOKEN_MODELINDEX2,	0	},
	{ "angles[0]",		TOKEN_ANGLES_0,		0	},
	{ "time",		TOKEN_TIME,		0	},
	{ "apos.trTime",	TOKEN_APOS_TRTIME,	0	},
	{ "apos.trDuration",	TOKEN_APOS_TRDURATION,	0	},
	{ "apos.trBase[2]",	TOKEN_APOS_TRBASE_2,	0	},
	{ "apos.trDelta[0]",	TOKEN_APOS_TRDELTA_0,	0	},
	{ "apos.trDelta[1]",	TOKEN_APOS_TRDELTA_1,	0	},
	{ "apos.trDelta[2]",	TOKEN_APOS_TRDELTA_2,	0	},
	{ "time2",		TOKEN_TIME2,		0	},
	{ "angles[2]",		TOKEN_ANGLES_2,		0	},
	{ "angles2[0]",		TOKEN_ANGLES2_0,	0	},
	{ "angles2[2]",		TOKEN_ANGLES2_2,	0	},
	{ "constantLight",	TOKEN_CONSTANTLIGHT,	0	},
	{ "frame",		TOKEN_FRAME,		0	},
	{ "serverCommandSequence",TOKEN_SERVERCOMMANDSEQUENCE,	0	},
	{ "clientNum",		TOKEN_CLIENTNUM,	0	},
	{ "checksumFeed",	TOKEN_CHECKSUMFEED,	0	},
	{ "seq",		TOKEN_SEQ,		0	},
	{ "downloadSize",	TOKEN_DOWNLOADSIZE,	0	},
	{ "transfer",		TOKEN_TRANSFER,		0	},
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

int entitytoken[] = {
	TOKEN_POS_TRTIME,
	TOKEN_POS_TRBASE_0,
	TOKEN_POS_TRBASE_1,
	TOKEN_POS_TRDELTA_0,
	TOKEN_POS_TRDELTA_1,
	TOKEN_POS_TRBASE_2,
	TOKEN_APOS_TRBASE_1,
	TOKEN_POS_TRDELTA_2,
	TOKEN_APOS_TRBASE_0,
	TOKEN_EVENT,
	TOKEN_ANGLES2_1,
	TOKEN_ETYPE,
	TOKEN_TORSOANIM,
	TOKEN_EVENTPARM,
	TOKEN_LEGSANIM,
	TOKEN_GROUNDENTITYNUM,
	TOKEN_POS_TRTYPE,
	TOKEN_EFLAGS,
	TOKEN_OTHERENTITYNUM,
	TOKEN_WEAPON,
	TOKEN_CLIENTNUM,
	TOKEN_ANGLES_1,
	TOKEN_POS_TRDURATION,
	TOKEN_APOS_TRTYPE,
	TOKEN_ORIGIN_0,
	TOKEN_ORIGIN_1,
	TOKEN_ORIGIN_2,
	TOKEN_SOLID,
	TOKEN_POWERUPS,
	TOKEN_MODELINDEX,
	TOKEN_OTHERENTITYNUM2,
	TOKEN_LOOPSOUND,
	TOKEN_GENERIC1,
	TOKEN_ORIGIN2_2,
	TOKEN_ORIGIN2_0,
	TOKEN_ORIGIN2_1,
	TOKEN_MODELINDEX2,
	TOKEN_ANGLES_0,
	TOKEN_TIME,
	TOKEN_APOS_TRTIME,
	TOKEN_APOS_TRDURATION,
	TOKEN_APOS_TRBASE_2,
	TOKEN_APOS_TRDELTA_0,
	TOKEN_APOS_TRDELTA_1,
	TOKEN_APOS_TRDELTA_2,
	TOKEN_TIME2,
	TOKEN_ANGLES_2,
	TOKEN_ANGLES2_0,
	TOKEN_ANGLES2_2,
	TOKEN_CONSTANTLIGHT,
	TOKEN_FRAME
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
	FILE *file;
	#define BUFFER_SIZE 1000
	char buffer[BUFFER_SIZE];
	long blocksize;
	long p;
	int state;

	if ((file=fopen(filename, "rb"))==NULL)
		return 0;
	rewind(file);
	if ((blocksize=fread(buffer,1,BUFFER_SIZE,file))==0)
		return 0;
	if(fclose(file))
		return 0;
	if (blocksize<20)
		return 0;
	state = 0;
	for (p=0;p<blocksize;p++) {
		switch (state) {
			/* not a comment */
			/* Search for the 'dm3' keyword. */
			case 0:
				if (strncmp (&(buffer[p]), "dm3", 3)==0)
					return 1;
				if (strncmp (&(buffer[p]), "//", 2)==0) {
					state=1;
					break;
				}
				if (buffer[p]!=' ' && buffer[p]!='\t' &&
					buffer[p]!='\r' && buffer[p]!='\n' &&
					buffer[p]!='\f')
					return 0;
			break;
			/* a comment */
			case 1:
				if (buffer[p]=='\n') state=0;
			break;
		}
	}
	/* If it comes to this line it may be a DM2 txt file but we don't
	beleave it. */
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

        /* Get the sequence number. */
	if (fread(&s,1,4,d->file)!=4) syserror(FIREAD,"sequence number of file '%s'", d->filename);
        m->serverMessageSequence = LittleLong( s );

        /* Init the message. */
        MSG_Init( &(m->buf), m->bufData, sizeof( m->bufData ) );

        /* Get the length. */
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
DM3_bin_to_node_entity(msg_t *msg)
{
	node	*n = NULL;
	node	*tn = NULL;
	int	index;
	int	lc;
	netField_t	*field;
	int	*tp;
	int	i;

	/* Get the index itself. */
	index = MSG_ReadBits( msg, GENTITYNUM_BITS );
	if (index == (MAX_GENTITIES-1)) {
		goto lastout;
	}
#if 0
	fprintf(stderr,"index=%d\n", index);
#endif
	tn = node_link(tn, node_command_init(TOKEN_INDEX, V_INT, H_ENTITY_INDEX, NODE_VALUE_INT_dup(index), 0));

	/* Check for a remove. */
	if ( MSG_ReadBits( msg, 1 ) == 1 ) {
		tn=node_link(tn, node_init(TOKEN_REMOVE,NULL,0));
		goto out;
	}

	/* Check for no delta. */
	if ( MSG_ReadBits( msg, 1 ) == 0 ) {
		goto out;
	}

	/* Get the last field number. */
	lc = MSG_ReadByte(msg);
	if (lc>entityStateFields_length) {
		syserror(DM3INTE, "last entity field is %d, max allowed is %d, index=%d", lc, entityStateFields_length, index);
	}

	/* Loop over all fields. */
	for ( i = 0, field = entityStateFields, tp = entitytoken ;
		i < lc ;
		i++, field++, tp++ ) {

		/* Check the bit, if we have a change. */
		if ( MSG_ReadBits( msg, 1 ) ) {
			union {
				float	value_f;
				int	value_i;
			} value;

			if (field->bits == 0) {
				/* Float. */

				/* Check for 0. */
				if (MSG_ReadBits( msg, 1 ) == 0 ) {
					value.value_f = 0.0f;
				} else {
					/* Check the float type. */
					if (MSG_ReadBits( msg, 1 ) == 0) {
						/* Get an integral float. */
						int	trunc;

						trunc = MSG_ReadBits( msg, FLOAT_INT_BITS );
						/* Bias to allow equal parts positive and negative. */
						trunc -= FLOAT_INT_BIAS;
						value.value_f = trunc;
					}
					else {
						/* Get a full floating point value. */
						value.value_i = MSG_ReadBits( msg, 32 );
						/* TODO: byte order? */
					}
				}
				tn = node_link(tn, node_command_init(*tp, V_FLOAT, H_FLOAT, NODE_VALUE_FLOAT_dup(value.value_f), 0));
			} else {
				/* Integer. */

				/* Check for 0. */
				if ( MSG_ReadBits( msg, 1 ) == 0 ) {
					value.value_i = 0;
				}
				else {
					value.value_i = MSG_ReadBits( msg, field->bits );
				}
				tn = node_link(tn, node_command_init(*tp, V_INT, H_LONG, NODE_VALUE_INT_dup(value.value_i), 0));
			}
		}


	} /* End loop over all fields. */

out:
	n = node_init(TOKEN_ENTITY, tn, 0);

lastout:
	return n;
}


node*
DM3_bin_to_node(DM3_binblock_t *m, int opt _U_)
{
	node	*n, *tn, *ttn;
	long	rel_ack;
	int	loop_end;

	tn = NULL;

	/* If there is nothing in it, it must be an endblock. */
	if (m->buf.cursize == -1) {
		return node_init_all(TOKEN_ENDBLOCK,H_DM3_ENDBLOCK,tn,0);
	}

	/* Start the block with the sequence number. */
	tn=node_link(tn, node_command_init(TOKEN_SEQUENCE, V_INT, H_LONG, NODE_VALUE_INT_dup(m->serverMessageSequence), 0));

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
		/* fprintf(stderr,"cmd=%d\n", cmd); */
		switch(cmd) {
			case svc_nop: {	/* Complete. */
				tn=node_link(tn, node_init(TOKEN_NOP, NULL, 0));
			}
			break; /* End svc_nop. */
			case svc_gamestate: { /* Complete. */
				int	gamestate_loop_end = 0;
				ttn = NULL;

				/* A gamestate always marks a server command sequence. */
				ttn = node_link(ttn, node_command_init(TOKEN_SERVERCOMMANDSEQUENCE, V_INT, H_LONG, NODE_VALUE_INT_dup(MSG_ReadLong( &(m->buf))), 0));

				while (1) { /* Loop over gamestate commands. */
					int	gcmd;

					/* Get the gamestate command. */
					gcmd = MSG_ReadByte( &(m->buf) );

					switch(gcmd) {
						case svc_EOF: /* Complete. */
#if 0
							/* Don't tell anyone. The gamestate commands always end with EOF. */
							ttn=node_link(ttn, node_init(TOKEN_EOF, NULL, 0));
#endif
							gamestate_loop_end = 1;
						break;
						case svc_configstring: { /* Complete. */
							node	*tttn;
							int	index = MSG_ReadShort( &(m->buf) );
							tttn = node_link(
								node_command_init(TOKEN_INDEX,V_INT,H_SHORT,NODE_VALUE_INT_dup(index),0),
									node_command_init(TOKEN_VALUE,V_STRING,H_STRING,NODE_VALUE_STRING_dup(MSG_ReadBigString( &(m->buf ))),0));

							ttn=node_link(ttn, node_init(TOKEN_CONFIGSTRING, tttn, 0));
						}
						break;
						case svc_baseline: { /* Complete. */
							node	*tttn;

							tttn = DM3_bin_to_node_entity(&(m->buf));
							if (tttn == NULL) {
								syserror(EINVAL, "empty baseline entity");
							}
							ttn=node_link(ttn, node_init(TOKEN_BASELINE, tttn, 0));
						}
						break;
						default:
							syserror(WDM3, "bad gamestate command byte %d", gcmd);
					} /* End switch gcmd. */

					if (gamestate_loop_end) {
						break;
					}
				} /* End loop over gamestate commands. */

				/* Get the client number. */
				ttn = node_link(ttn,node_command_init(TOKEN_CLIENTNUM, V_INT, H_LONG, NODE_VALUE_INT_dup(MSG_ReadLong(&(m->buf))), 0));

				/* Get the checksum feed. */
				ttn = node_link(ttn,node_command_init(TOKEN_CHECKSUMFEED, V_INT, H_LONG, NODE_VALUE_INT_dup(MSG_ReadLong(&(m->buf))), 0));
				
				tn=node_link(tn, node_init(TOKEN_GAMESTATE, ttn, 0));
				loop_end = 1;
			} /* End svc_gamestate. */
			break;
			case svc_serverCommand: { /* Complete. */
				ttn = NULL;

				ttn = node_link(ttn, node_command_init(TOKEN_SEQ, V_INT, H_LONG, NODE_VALUE_INT_dup(MSG_ReadLong( &(m->buf) )), 0));
				ttn = node_link(ttn, node_command_init(TOKEN_VALUE, V_STRING, H_STRING, NODE_VALUE_STRING_dup(MSG_ReadString( &(m->buf) )), 0));
				
				tn=node_link(tn, node_init(TOKEN_SERVERCOMMAND, ttn, 0));
			} /* End svc_serverCommand. */
			break;
			case svc_download: { /* Complete. */
				int block;
				int size;
				unsigned char data[MAX_MSGLEN];
				node *tttn;

				ttn = 0;
				block = MSG_ReadShort ( &(m->buf) );
				ttn = node_link(ttn, node_command_init(TOKEN_TRANSFER, V_INT, H_SHORT, NODE_VALUE_INT_dup(block), 0));
				if (block == 0) {
					/* Block zero is special, contains file size. */
					/* This is a redundancy. block==0 means downloadsize,
					why use then block at all, if downloadSize is there? */
					ttn = node_link(ttn, node_command_init(TOKEN_DOWNLOADSIZE, V_INT, H_LONG, NODE_VALUE_INT_dup(MSG_ReadLong( &(m->buf) )), 0));
				}

				/* Get the size but don't put it into the node tree as this is a
				senseless redundancy. */
				size = MSG_ReadShort ( &(m->buf) );

				/* Make sure we don't get too many bytes. */
				if (size > (int)sizeof(data)) {
					syserror(WDM3, "Download block size is too big. Got %d but %d is only allowed.", size, MAX_MSGLEN);
				}

				/* size == 0 is also allowed (file end). */
				tttn = NULL;
				if (size > 0) {
					int	i;

					/* Get the actual data. */
					MSG_ReadData( &(m->buf), data, size );
					for (i=0; i<size; i++) {
						tttn=node_link(tttn,node_init(V_BYTEHEX, NODE_VALUE_INT_dup(data[i]), 0));
					}
				}

				/* Add the data node, even if it is empty (tttn==NULL). */
				ttn=node_link(ttn, node_init(TOKEN_DATA, tttn, 0));

				/* Link the parts together to the download node. */
				tn=node_link(tn, node_init(TOKEN_DOWNLOAD, ttn, 0));
			} /* End svc_download. */
			break;
			case svc_snapshot: { /* Complete. */
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
#if 0
				fprintf(stderr,"serverTime=%d\n", serverTime);
#endif
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
#if 0
						fprintf(stderr,"get %s\n", node_token_string(*tp));
#endif
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
#if 0
					else {
						fprintf(stderr,"no %s\n", node_token_string(*tp));
					}
#endif

				} /* End loop over the fields. */

				/* Read the arrays. */
				if (MSG_ReadBits( &(m->buf), 1 ) ) {
					int	bits;
					int	value;
					node	*a;
					node	*e;

					/* Parse the stats. */
					if ( MSG_ReadBits( &(m->buf), 1 ) ) {
#if 0
						fprintf(stderr,"get stats\n");
#endif
						a = NULL;
						bits = MSG_ReadShort (&(m->buf));
						for (i=0 ; i<16 ; i++) {
							if (bits & (1<<i) ) {
#if 0
								fprintf(stderr,"get stat[%d]\n", i);
#endif
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
#if 0
						fprintf(stderr,"get persistants\n");
#endif
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

					/* Parse the ammos. */
					if ( MSG_ReadBits( &(m->buf), 1 ) ) {
#if 0
						fprintf(stderr,"get ammos\n");
#endif
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

					/* Parse the powerups. */
					if ( MSG_ReadBits( &(m->buf), 1 ) ) {
#if 0
						fprintf(stderr,"get powerups\n");
#endif
						a = NULL;
						bits = MSG_ReadShort (&(m->buf));
						for (i=0 ; i<16 ; i++) {
							if (bits & (1<<i) ) {
#if 0
								fprintf(stderr,"get powerup[%d]\n", i);
#endif
								value = MSG_ReadLong(&(m->buf));
								e = node_link(
									node_command_init(TOKEN_INDEX,V_INT,H_SHORT,NODE_VALUE_INT_dup(i),0),
									node_command_init(TOKEN_VALUE,V_INT,H_LONG,NODE_VALUE_INT_dup(value),0));
								a = node_link(a,node_init(TOKEN_POWERUP,e,0));
							}
						}
						tttn = node_link(tttn,node_init(TOKEN_POWERUPS,a,0));
					}

				} /* End read the arrays. */


				/* Connect player parts to existing node tree. */
				ttn=node_link(ttn, node_init(TOKEN_PLAYER, tttn, 0));
				tttn = NULL;
				/* Read packet entities. */
				while (1) {
					node *e;

					e = DM3_bin_to_node_entity(&(m->buf));
					/* Last in list. */
					if (e==NULL) {
						break;
					}

					tttn = node_link(tttn, e);
				} /* End read packet entities. */

				/* Connect entity parts to existing node tree. */
				ttn=node_link(ttn, node_init(TOKEN_ENTITIES, tttn, 0));

				/* Create a "snapshot" node. */
				tn=node_link(tn, node_init(TOKEN_SNAPSHOT, ttn, 0));
				/* loop_end = 1; */
			} /* End svc_snapshot. */
			break;
			case svc_EOF:	/* Complete. */
				/* The last command is always an EOF. */
				loop_end = 1;
			break; /* End svc_EOF. */
			default:	/* Complete. */
				syserror(DM3INTE,"unknown cmd %d", cmd);
		}

		/* If we know no further, stop this loop. */
		if (loop_end) break;
	} /* End main message loop. */

	/* Connect commands into a message block. */
	n=node_init_all(TOKEN_BLOCK, H_DM3_BLOCK, tn, 0);
	
	/* Return the nop-level block. */
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


void
MSG_WriteNodeValue(msg_t *m, node *n)
{
	if (m==NULL) {
		syserror(EINVAL,"message pointer is NULL");
	}
	if (n==NULL) {
		syserror(EINVAL,"node pointer is NULL");
	}
	switch (n->type) {
		case V_INT:
			switch(n->hint) {
				case H_LONG:
					MSG_WriteLong(m, *(long*)n->down);
				break;
				case H_SHORT:
					MSG_WriteShort(m, *(long*)n->down);
				break;
				case H_BYTE:
					MSG_WriteByte(m, *(long*)n->down);
				break;
				case H_ENTITY_INDEX:
					MSG_WriteBits(m, *(long*)n->down, GENTITYNUM_BITS);
				break;
				default:
					syserror(DINTE, "wrong int hint type %d at pos=%d, type=%d",
						n->hint, n->pos, n->type);
			}
		break;
		case V_BYTEHEX:
			MSG_WriteByte(m, *(long*)n->down);
		break;
		case V_STRING:
			MSG_WriteString(m, (char*)n->down);
		break;
		default:
			syserror(DINTE, "wrong argument type at pos=%d, type=%s (%d)", n->pos, node_token_string(n->type), n->type);
	} /* End switch n->type. */
}


void
MSG_WriteNodeValues(msg_t *m, node *n)
{
	for ( ; n!=NULL; NODE_NEXT(n)) {
		MSG_WriteNodeValue(m, n);
	}
}



void
MSG_WriteNodeEntity(msg_t *m, node *n)
{
	node	*tn;
	node	*ttn;
	int	lc;
	int	i;
	int	*tp;
	netField_t	*field;

	/* The parts of an entity. */
	tn = n->down;

	/* index. */
	tn->down->hint = H_ENTITY_INDEX;
	MSG_WriteNodeValue(m, tn->down); NODE_NEXT(tn);

	/* Maybe there is nothing more after the index. */
	if (tn == NULL) {
		/* no remove */
		MSG_WriteBits(m, 0, 1);
		/* no delta data */
		MSG_WriteBits(m, 0, 1);
		return;
	}

	/* remove */
	if (tn->type == TOKEN_REMOVE) {
		MSG_WriteBits(m, 1, 1);
		return;
	}
	/* no remove */
	MSG_WriteBits(m, 0, 1);
	/* with delta */
	MSG_WriteBits(m, 1, 1);

	/* Search for the highest field number. */
	for ( i = 0, tp = entitytoken, ttn = tn, lc = 0 ;
		i < entityStateFields_length && ttn != NULL;
		i++, tp++ ) {
		/* If we have the right token... */
		if (*tp == ttn->type) {
			/* Remember the index. */
			lc = i+1;
			/* Advance the nodes. */
			NODE_NEXT(ttn);
		}
	}

	/* Consistency check. */
	if (lc == 0) {
		syserror(EINVAL, "only unknown fields found in an entity.");
	}

	/* Write the number of the last changed field. */
	MSG_WriteByte(m, lc);

	/* Go again over all fields. */
	for ( i = 0, field = entityStateFields, tp = entitytoken, ttn = tn ;
		i < lc ;
		i++, field++, tp++ ) {
		/* If we have the right token... */
		if (ttn!=NULL && *tp == ttn->type) {
			int	*toF;
			int	trunc;
			float	fullFloat;

			/* The "to" field pointer. */
			toF = (int *)ttn->down->down;

			/* There is a change in this field. */
			MSG_WriteBits( m, 1, 1 );

			if ( field->bits == 0 ) {
				/* A float number. */
				fullFloat = *(float *)toF;
				trunc = (int)fullFloat;

				if (fullFloat == 0.0f) {
					MSG_WriteBits( m, 0, 1 );
				} else {
					MSG_WriteBits( m, 1, 1 );
					if ( trunc == fullFloat && trunc + FLOAT_INT_BIAS >= 0 &&
					trunc + FLOAT_INT_BIAS < ( 1 << FLOAT_INT_BITS ) ) {
						/* Send as small integer. */
						MSG_WriteBits( m, 0, 1 );
						MSG_WriteBits( m, trunc + FLOAT_INT_BIAS, FLOAT_INT_BITS );
					} else {
						/* Send as full floating point value. */
						MSG_WriteBits( m, 1, 1 );
						MSG_WriteBits( m, *toF, 32 );
					}
				}
			}
			else {
				if (*toF == 0) {
					MSG_WriteBits( m, 0, 1 );
				}
				else {
					MSG_WriteBits( m, 1, 1 );
					/* Integer */
					MSG_WriteBits( m, *toF, field->bits );
				}
			}
 
			/* Advance the nodes. */
			NODE_NEXT(ttn);

		}
		else {
			/* No change in this field. */
			MSG_WriteBits( m, 0, 1 );
		}
	} /* End loop over all fields. */
}


void
MSG_WriteNodePlayer(msg_t *m, node *n)
{
	node	*tn;
	node	*ttn;
	int	lc;
	int	i;
	int	*tp;
	netField_t	*field;

	/* The parts of a player. */
	tn = n->down;

	/* Search for the highest field number. */
	for ( i = 0, tp = playertoken, ttn = tn, lc = 0 ;
		i < playerStateFields_length && ttn!=NULL ;
		i++, tp++ ) {
		/* If we have the right token... */
		if (*tp == ttn->type) {
			/* Remember the index. */
			lc = i+1;
			/* Advance the nodes. */
			NODE_NEXT(ttn);
		}
	}

	/* Write the number of the last changed field. */
	MSG_WriteByte(m, lc);
#if 0
	fprintf(stderr, "lc=%d\n", lc);
#endif

	/* Go again over all fields. */
	for ( i = 0, field = playerStateFields, tp = playertoken, ttn = tn ;
		i < lc ;
		i++, field++, tp++ ) {
		/* If we have the right token... */
		if (ttn != NULL && *tp == ttn->type) {
			int	*toF;
			int	trunc;
			float	fullFloat;

			/* There is a change in this field. */
			MSG_WriteBits( m, 1, 1 );

			/* The "to" field pointer. */
			toF = (int *)ttn->down->down;
#if 0
			fprintf(stderr,"change in %s\n", node_token_string(*tp));
#endif

			if ( field->bits == 0 ) {
				/* A float number. */
				fullFloat = *(float *)toF;
				trunc = (int)fullFloat;

				if ( trunc == fullFloat && trunc + FLOAT_INT_BIAS >= 0 &&
				trunc + FLOAT_INT_BIAS < ( 1 << FLOAT_INT_BITS ) ) {
					/* Send as small integer. */
					MSG_WriteBits( m, 0, 1 );
					MSG_WriteBits( m, trunc + FLOAT_INT_BIAS, FLOAT_INT_BITS );
				} else {
					/* Send as full floating point value. */
					MSG_WriteBits( m, 1, 1 );
					MSG_WriteBits( m, *toF, 32 );
				}
			}
			else {
				/* Integer */
				MSG_WriteBits( m, *toF, field->bits );
			}
 
			/* Advance the nodes. */
			NODE_NEXT(ttn);

		}
		else {
			/* No change in this field. */
			MSG_WriteBits( m, 0, 1 );
#if 0
			fprintf(stderr,"no change in %s\n", node_token_string(*tp));
#endif
		}
	} /* End loop over all fields. */

#if 0
	ttn=0;
#endif

	tn = ttn;

	if (tn==NULL) {
		/* We have nothing left. */
		MSG_WriteBits( m, 0, 1 );
		return;
	}

	/* There is still something. */
	MSG_WriteBits( m, 1, 1 );

	/* stats */
	if (tn->type == TOKEN_STATS) {
		short	bitmask;

#if 0
		fprintf(stderr,"stats");
#endif

		/* We have it. */
		MSG_WriteBits( m, 1, 1 );

		/* Loop over the single entries. */
		for (bitmask=0,ttn=tn->down;ttn!=NULL;NODE_NEXT(ttn)) {
			bitmask|=1<<(*(int*)(ttn->down->down->down));
		}

		/* Write the bitmask. */
		MSG_WriteShort(m, bitmask);

		/* Loop over the single entries. */
		for (ttn=tn->down;ttn!=NULL;NODE_NEXT(ttn)) {
			ttn->down->next->down->hint = H_SHORT;
			MSG_WriteNodeValue(m, ttn->down->next->down);
		}

		/* Advance node. */
		NODE_NEXT(tn);
	}
	else {
#if 0
		fprintf(stderr,"no stats");
#endif
		MSG_WriteBits( m, 0, 1 );
	}

	/* persistants */
	if (tn!=NULL && tn->type == TOKEN_PERSISTANTS) {
		short	bitmask;

		/* We have it. */
		MSG_WriteBits( m, 1, 1 );

		/* Loop over the single entries. */
		for (bitmask=0,ttn=tn->down;ttn!=NULL;NODE_NEXT(ttn)) {
			bitmask|=1<<(*(int*)(ttn->down->down->down));
		}

		/* Write the bitmask. */
		MSG_WriteShort(m, bitmask);

		/* Loop over the single entries. */
		for (ttn=tn->down;ttn!=NULL;NODE_NEXT(ttn)) {
			ttn->down->next->down->hint = H_SHORT;
			MSG_WriteNodeValue(m, ttn->down->next->down);
		}

		/* Advance node. */
		NODE_NEXT(tn);
	}
	else {
		MSG_WriteBits( m, 0, 1 );
	}

	/* ammos */
	if (tn!=NULL && tn->type == TOKEN_AMMOS) {
		short	bitmask;

		/* We have it. */
		MSG_WriteBits( m, 1, 1 );

		/* Loop over the single entries. */
		for (bitmask=0,ttn=tn->down;ttn!=NULL;NODE_NEXT(ttn)) {
			bitmask|=1<<(*(int*)(ttn->down->down->down));
		}

		/* Write the bitmask. */
		MSG_WriteShort(m, bitmask);

		/* Loop over the single entries. */
		for (ttn=tn->down;ttn!=NULL;NODE_NEXT(ttn)) {
			ttn->down->next->down->hint = H_SHORT;
			MSG_WriteNodeValue(m, ttn->down->next->down);
		}

		/* Advance node. */
		NODE_NEXT(tn);
	}
	else {
		MSG_WriteBits( m, 0, 1 );
	}

	/* powerups */
	if (tn!=NULL && tn->type == TOKEN_POWERUPS) {
		short	bitmask;

		/* We have it. */
		MSG_WriteBits( m, 1, 1 );

		/* Loop over the single entries. */
		for (bitmask=0,ttn=tn->down;ttn!=NULL;NODE_NEXT(ttn)) {
			bitmask|=1<<(*(int*)(ttn->down->down->down));
		}

		/* Write the bitmask. */
		MSG_WriteShort(m, bitmask);

		/* Loop over the single entries. */
		for (ttn=tn->down;ttn!=NULL;NODE_NEXT(ttn)) {
			MSG_WriteNodeValue(m, ttn->down->next->down);
		}

		/* Advance node. */
		NODE_NEXT(tn);
	}
	else {
		MSG_WriteBits( m, 0, 1 );
	}

	/* Consistency check. */
	if (tn != NULL) {
		syserror(EINVAL, "unexpected node type %s (%d) in player",
			node_token_string(tn->type), tn->type);
	}
}


node*
DM3_block_write_bin(node* b)
{
	if (b==NULL) {
		syserror(DM3INTE, "Empty block to write");
	}
	switch (b->type) {
		default:
			/* Unknown top level block. */
			syserror(DM3INTE, "Unknown top level block: '%s' (%d)",
				node_token_string(b->type), b->type);
		break;
		case TOKEN_DM3:
			/* Do nothing. */
		break;
		case TOKEN_ENDBLOCK: {
			/* The last block contains twice a '-1'. */
			int	len = -1;
			if (fwrite(&len, 1, 4, output_file) != 4) {
				syserror(errno, output_filename);
			}
			if (fwrite(&len, 1, 4, output_file) != 4) {
				syserror(errno, output_filename);
			}
		}
		break;
		case TOKEN_BLOCK: {
			/* The real stuff. */
			DM3_binblock_t	m;
			node		*n;
			int		len;

			/* Set all the data bits to 0. This is not really
			needed but only so we can ensure, that different LMPCs
			will generate always the same binary data: with the
			unused bits in the last byte set to zero. */
			memset(m.bufData, 0, sizeof( m.bufData ));

			/* Init the message. */
			MSG_Init( &(m.buf), m.bufData, sizeof( m.bufData ) );

			/* Messages are bit streams. */
			MSG_Bitstream(&(m.buf));

			/* We look around the "down" data. */
			n = b->down;
			/* TODO Where should all these checking be done? */

			/* Get the sequence number. */
			if (n == NULL) { syserror(DM3INTE, "Empty down list"); }
			if (n->type != TOKEN_SEQUENCE) {
				syserror(DM3INTE,
					"Expected token %s (%d) but got %s (%d)",
					node_token_string(TOKEN_SEQUENCE), TOKEN_SEQUENCE,
					node_token_string(n->type), n->type);
			}
			if (n->down == NULL) { syserror(DM3INTE, "Empty argument"); }
			if (n->down->type != V_INT) {
				syserror(DM3INTE,
					"Expected V_INT type but got %d", n->down->type);
			}
				
			/* Actually get the value. */
			m.serverMessageSequence = *(long*)(n->down->down);

			/* Go forward to the next entry. */
			NODE_NEXT(n);

			/* Get the reliable acknowledge sequence number. */
			if (n == NULL) { syserror(DM3INTE, "Empty next entry."); }
			if (n->type != TOKEN_REL_ACK) {
				syserror(DM3INTE,
					"Expected token %s (%d) but got %s (%d)",
					node_token_string(TOKEN_REL_ACK), TOKEN_REL_ACK,
					node_token_string(n->type), n->type);
			}

			/* Get the value and write it. */
			MSG_WriteNodeValue(&(m.buf), n->down); NODE_NEXT(n);
			
			/* Loop over all following commands. */
			for ( ; n!=NULL ; NODE_NEXT(n)) {
				switch (n->type) {
					default:
						syserror(EINVAL, "unknown command %d", n->type);
					break;
					case TOKEN_NOP: { /* Complete. */
						/* The command byte itself. */
						MSG_WriteByte(&(m.buf), svc_nop);
					} /* End svc_nop. */
					break;
					case TOKEN_SERVERCOMMAND: { /* Complete */
						node	*arg;
						/* The command byte itself. */
						MSG_WriteByte(&(m.buf), svc_serverCommand);
						arg = n->down;
						/* seq */
						MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);
						/* value */
						MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);
					} /* End svc_serverCommand. */
					break;
					case TOKEN_GAMESTATE: { /* Complete. */
						node	*arg;
						int	loop_end;
						/* The command byte itself. */
						MSG_WriteByte( &(m.buf), svc_gamestate );
						arg = n->down;
						/* serverCommandSequence */
						MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);
						/* gamestate sub-commands: configstring, baseline. */
						for (loop_end=0;loop_end==0;) {
							node *sub;
							sub = arg->down;
							switch (arg->type) {
								case TOKEN_CONFIGSTRING: {
									/* The command byte itself. */
									MSG_WriteByte(&(m.buf), svc_configstring);
									
									/* index */
									sub->down->hint = H_SHORT;
									MSG_WriteNodeValue(&(m.buf), sub->down); NODE_NEXT(sub);
									/* value */
									MSG_WriteNodeValue(&(m.buf), sub->down); NODE_NEXT(sub);
									NODE_NEXT(arg);
								}
								break;
								case TOKEN_BASELINE: {
									/* The command byte itself. */
									MSG_WriteByte(&(m.buf), svc_baseline);
									/* entity */
									MSG_WriteNodeEntity(&(m.buf), sub);
									NODE_NEXT(arg);
								}
								break;
								default:
									loop_end = 1;
							} /* End switch. */
						} /* End gamestate sub-commands. */

						/* Mark the end of the gamestate sub-commands. */
						MSG_WriteByte( &(m.buf), svc_EOF );

						/* clientNum */
						MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);

						/* checksumFeed */
						MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);

					} /* End svc_gamestate. */
					break;
					case TOKEN_SNAPSHOT: { /* Complete */
						node	*arg;
						node	*entity;

						/* The command byte itself. */
						MSG_WriteByte(&(m.buf), svc_snapshot);
						arg = n->down;

						/* serverTime */
						MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);

						/* deltaNum */
						arg->down->hint = H_BYTE;
						MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);

						/* snapFlags */
						arg->down->hint = H_BYTE;
						MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);

						/* areamask length */
						MSG_WriteByte(&(m.buf), node_count_next(arg->down));

						/* areamask data */
						MSG_WriteNodeValues(&(m.buf), arg->down); NODE_NEXT(arg);

						/* player */
						MSG_WriteNodePlayer(&(m.buf), arg); NODE_NEXT(arg);

						/* entities */
						for (entity = arg->down;entity!=NULL;NODE_NEXT(entity)) {
							/* entity */
							MSG_WriteNodeEntity(&(m.buf), entity);
						}
						/* End of list of entities. */
						MSG_WriteBits( &(m.buf), (MAX_GENTITIES-1), GENTITYNUM_BITS );

					} /* End svc_snapshot. */
					break;
					case TOKEN_DOWNLOAD: { /* Complete */
						node	*arg;

						/* The command byte itself. */
						MSG_WriteByte(&(m.buf), svc_download);
						arg = n->down;

						/* transfer */
						arg->down->hint = H_SHORT;
						MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);

						/* downloadSize */
						if (arg->type == TOKEN_DOWNLOADSIZE) {
							MSG_WriteNodeValue(&(m.buf), arg->down); NODE_NEXT(arg);
						}

						/* data length */
						MSG_WriteShort(&(m.buf), node_count_next(arg->down));

						/* data bytes */
						MSG_WriteNodeValues(&(m.buf), arg->down); NODE_NEXT(arg);

					} /* End svc_download. */
					break;
				} /* End switch n->type. */
			}

			/* Append an empty eof message. */
			MSG_WriteByte( &(m.buf), svc_EOF );

			/* Write the sequence number. */
			len = LittleLong(m.serverMessageSequence);
			if (fwrite(&len, 1, 4, output_file) != 4) {
				syserror(errno, output_filename);
			}

			/* Write the message length. */
			len = LittleLong (m.buf.cursize);
			if (fwrite(&len, 1, 4, output_file) != 4) {
				syserror(errno, output_filename);
			}

			/* Write the message itself. */
			if (fwrite (m.buf.data, 1, m.buf.cursize, output_file) !=
				(size_t)m.buf.cursize) {
				syserror(errno, output_filename);
			}
		}
		break;
	} /* End switch. */
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
