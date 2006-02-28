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
|  demotextl.l - lex source for demo text file parsing                       |
\****************************************************************************/

/* %option noyywrap never-interactive */
%option prefix="demotext_"

%{
#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif
#include "node.h"          /* nodes */
#define YYSTYPE nodep
#include <errno.h>         /* for ENOMEM */
#include <stdio.h>         /* for fileno in the created flex code */
#include "udem.h"
#include "tools.h"         /* for syserror */
#if 0
#include "udemy.h"         /* yyparse */
#endif
#include "demotexty.h"
#include <math.h>          /* for atof */
#define yyerror demotext_error
void yyerror(char *s);
/* the header file udemy.h created by yacc lacks this: */
#if 0
extern YYSTYPE yylval;
#endif

static int linenumber = 1;

static int first_lex = 1;

%}

DIGIT [0-9]
XDIGIT [0-9a-fA-F]
NL \r?\n

		/* ias stands for: is a string, n for: normal. */
		/* Every single rule has a start condition, so we can */
		/* use the more traditional s (inclusive) start conditions */
		/* The AT&T lex can't handle x (exclusive) start conditions */
		/* at all! */
%s ias norm

%%

%{
	char string_buf[MAX_STRING_SIZE];
	char* string_buf_ptr=NULL;

	/* look for lexing start */
	if (first_lex) {
		/* start with <norm> start condition */
		BEGIN(norm);
		/* now switch it off */
		first_lex = 0;
	}

%}

<norm>\/\/.*{NL} {		/* eat up C++ like comments */
			linenumber++;
		}

<norm>\"			string_buf_ptr = string_buf; BEGIN(ias);

<ias>\" { /* saw closing quote - all done */
			BEGIN(norm);
			*string_buf_ptr = '\0';

			yylval = node_init_all(V_STRING, H_STRING, NODE_VALUE_STRING_dup(string_buf), linenumber);
			return v_string;
		}

<ias>{NL} { /* unterminated string constant */
			linenumber++;
			fprintf(stderr,"line number %i\n", linenumber);
			syserror(DP_SC,"");
		}

<ias>\\[0-7]{1,3} { /* octal escape sequence */
			unsigned int result;

			(void) sscanf( yytext + 1, "%o", &result );
			if ( result > 0xff ) {
			  /* error, constant is out-of-bounds */
			  fprintf(stderr,"line number %i\n", linenumber);
			  syserror(DP_OOB,"");
			}

			*string_buf_ptr++ = result;
		}

<ias>\\n {
			*string_buf_ptr++ = '\n';
		}

<ias>\\r {
			*string_buf_ptr++ = '\r';
		}

<ias>\\(.|\n) {
			*string_buf_ptr++ = yytext[1];
		}

<ias>[^\\\r\n\"]+ {
			char *yptr = yytext;

			while ( *yptr )
			  *string_buf_ptr++ = *yptr++;
		}

<norm>\-?{DIGIT}+ { 
			long value;

			value = atoi(yytext);
			yylval = node_init(V_INT,NODE_VALUE_INT_dup(value),linenumber);
			
			return v_int;
		}

<norm>0[xX]{XDIGIT}+ {
			unsigned long value;

			sscanf(yytext+2, "%lx", &value);
			yylval = node_init(V_INT,NODE_VALUE_INT_dup(value),linenumber);

			return v_int;
		}

<norm>\-?{DIGIT}+"."{DIGIT}*([eE][-+]?{DIGIT}+)? {
			float value;

			value = atof(yytext);
			yylval = node_init(V_FLOAT,NODE_VALUE_FLOAT_dup(value),linenumber);

			return v_float;
		} 

<norm>: {
			return ':';
		}

<norm>s {	
			return 's';
        }

<norm>m {
			return 'm';
        }

<norm>h {
			return 'h';
        }

<norm>"{" {
			return '{';
        }

<norm>"}" {
			return '}';
		}

<norm>";" {
			return ';';
		}

<norm>[ \t]+		/* eat up whitespaces */

<norm>({NL})+ {		/* eat up new lines */
			linenumber++;
		}

<norm>[a-zA-Z_][a-zA-Z0-9_]+ {	/* These are keywords. */
			token_t	*token;

			/* Search the keyword in the token list. */
			token = node_token_get_by_string(yytext);
			if (token == NULL) {
				yyerror("unparsed text");
				syserror(DP_UTOK, yytext);
			}
			yylval = node_init(token->type,NULL,linenumber);
			if (token->id == 1) {
				return (DEMOTEXT_TOPLEVEL);
			}
			else {
				return (DEMOTEXT_LOWLEVEL);
			}
		}

%%

void
yyerror(char* s)
{
	fprintf(stderr, "line %d: %s near `%s'\n", linenumber, s, yytext);
}

int
yywrap(void)
{
	return 1;
}

