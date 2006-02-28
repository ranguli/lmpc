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
|  udemy.y - yacc source for DEM file parsing                                |
\****************************************************************************/

/* $Id$ */

%{
  #ifdef HAVE_CONFIG_H
    #include <config.h>
  #endif

#define yyparse demotext_parse
#define yylex	demotext_lex
#define yyerror	demotext_error
#define yynerrs	demotext_nerrs
#define yylval	demotext_lval
#define yylloc	demotext_lloc
#define yychar	demotext_char
#define yydebug	demotext_debug

  #include <stdlib.h> /* malloc, free */
  #include <string.h> /* strcat */
  #ifdef HAVE_MEMORY_H
    #include <memory.h>
  #endif
  #include "tools.h"  /* error */
  #include "node.h"   /* nodes */

  #define YYSTYPE nodep

  #include "record.h" /* SERVERVERSION */
  #include "binblock.h"
  #include "udm3.h"

  extern void yyerror(char* s);
  extern int yylex(void);
%}

		/* values */
%token v_int
%token v_float
%token v_string

		/* token */
%token DEMOTEXT_TOPLEVEL
%token DEMOTEXT_LOWLEVEL

%%

		/* tokens & blocks */

input:			demo_input ;

demo_input:		demo_toplevel_blocks ;

demo_toplevel_blocks:	demo_toplevel_block | demo_toplevel_blocks demo_toplevel_block ;

demo_toplevel_block: demo_toplevel_simple | demo_toplevel_curly;

demo_toplevel_simple: DEMOTEXT_TOPLEVEL ';' {
		do_block($1);
		node_delete($1);
	};

demo_toplevel_curly: DEMOTEXT_TOPLEVEL '{' demo_lowlevel_nodes '}' {
		node_add_down($1,$3);
		do_block($1);
		node_delete($1);
	};

demo_lowlevel_nodes:	demo_lowlevel_node { $$=$1; } 
		|	demo_lowlevel_nodes demo_lowlevel_node { $$=node_add_next($1,$2); };

demo_lowlevel_node: demo_lowlevel_simple { $$=$1; }
		|	demo_lowlevel_args { $$=$1; }
		|	demo_lowlevel_curly { $$=$1; };

demo_lowlevel_simple: DEMOTEXT_LOWLEVEL ';' { $$=$1; };

demo_lowlevel_args: DEMOTEXT_LOWLEVEL arglist ';' {
		$$=node_add_down($1,$3);
	};

demo_lowlevel_curly: DEMOTEXT_LOWLEVEL '{' demo_lowlevel_nodes '}' {
		$$=node_add_down($1,$3);
	};
	
		/* values */

arglist: intlist { $$=$1; }
	| floatlist { $$=$1; }
	| stringlist { $$=$1; }

intlist: v_int {
			$$=$1;
		}
		| intlist v_int {
			$$=node_add_next($1,$2);
		};

floatlist: v_float {
			$$=$1;
		}
		| floatlist v_float {
			$$=node_add_next($1,$2);
		};

stringlist: v_string {
			$$=$1;
		}
		| stringlist v_string {
			$$=node_add_next($1,$2);
		};

%%

/*- file end demotexty.y -----------------------------------------------------*/
