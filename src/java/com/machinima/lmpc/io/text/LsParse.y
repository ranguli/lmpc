%{
/* LS text parser */
/* $Id$ */

	package com.machinima.lmpc.io.text;
	import com.machinima.lmpc.io.node.*;
	import java.io.*;

%}
      
%token	v_int		/* integer value */
%token	v_float		/* float value */
%token	v_string	/* string value */
%token	TOKEN		/* keyword */

%%

input: /* empty string */ {
		parse_empty = true;
	}
	| top_blocks {
		$$ = $1;
	}
	;

top_blocks: 
	top_block {
		$$ = $1;
		// store the top block in yyval and stop
		end_of_top_block = true;
	}
	|
	top_blocks top_block {
		$$ = $1.add_next($2);
		// store the top block in yyval and stop
		end_of_top_block = true;
	}
	;

top_block: block

block: TOKEN ';' {
		$$ = $1;
		// System.out.println("block: TOKEN ';'");
	}
	| TOKEN arrayvalue ';' {
		$$ = $1.add_down($2);
		// System.out.println("block: TOKEN arrayvalue ';'");
	}
	| TOKEN '{' blocks '}' {
		$$ = $1.add_down($3);
		// System.out.println("block: TOKEN '{' blocks '}'");
	};

blocks: block {
		// System.out.println("blocks: block");
		$$ = $1;
	}
	| blocks block {
		// System.out.println("blocks: blocks block");
		$$ = $1.add_next($2);
	};

arrayvalue:	intarray { $$ = $1; }
	|	floatarray { $$ = $1; }
	|	stringarray { $$ = $1; }
	|	timearray { $$ = $1; }
	;

intarray:	v_int {
		$$ = $1;
	}
	|	intarray v_int {
		$$ = $1.add_next($2);
	}
	;

floatarray:	v_float {
		$$ = $1;
		// FloatNode t = (FloatNode)$$;
		// System.err.println(t.value);
	}
	|	floatarray v_float {
		$$ = $1.add_next($2);
	}
	;

stringarray:	v_string {
		$$ = $1;
	}
	|	stringarray v_string {
		$$ = $1.add_next($2);
	}
	;

timearray:	time {
		$$ = $1;
	}
	|	timearray time {
		$$ = $1.add_next($2);
	}
	;

time: v_float 's' {
			$$ = new TimeNode((FloatNode)$1);
	}
	| v_int ':' v_float 'm' {
			$$ = new TimeNode((IntNode)$1, (FloatNode)$3);
	}
	| v_int ':' v_int ':' v_float 'h' {
			$$ = new TimeNode((IntNode)$1, (IntNode)$3, (FloatNode)$5);
	}
	;

%%


	// lexer object
	private LsLex lexer;

	boolean end_of_top_block = false;

	// method to get the next token
	private int yylex ()
	{
		int yyl_return = -1;
		if (end_of_top_block) {
			end_of_top_block = false;
			if (yydebug) {
				System.out.print("self");
			}
		}
		else {
			try {
				yyl_return = lexer.yylex();
			}
			catch (IOException e) {
				System.err.println("IO error :" + e);
			}
		}
		if (yydebug) {
			System.out.print("yylex = " + yyl_return + ", ");
		}
		return yyl_return;
	}

	// print an error message
	public void yyerror (String error)
	{
		System.err.println ("Parse error at line " +
		lexer.curline() + " column " +
		lexer.curcolumn() + " : " + error);
	}

	// constructor, which intialises the lexer object too
	public LsParse(Reader r)
	{
		lexer = new LsLex(r, this);
	}

	private static boolean parse_empty = false;

	Node get_block() { return yyval; }

	boolean read_block()
	{
		boolean result;

		// yyparser.yydebug = true;
		yyparse();
		if (parse_empty) {
			parse_empty = false;
			result = false;
		}
		else {
			result = true;
		}

		return result;
	}


