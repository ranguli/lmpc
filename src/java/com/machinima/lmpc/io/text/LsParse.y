/* LS text parser */

%{
  import java.io.*;
%}
      
%token	v_int		/* integer value */
%token	v_float		/* float value */
%token	v_string	/* string value */
%token	KEYWORD		/* keyword */

%type <ival> v_int
%type <dval> v_float
%type <sval> v_string
%type <obj> KEYWORD
%type <obj> block
%type <dval> time


%%

input: /* empty string */ {
		parse_empty = true;
	}
	| top_blocks
	;

top_blocks: 
	top_block {
		// do something with $1
		// and than stop
		end_of_top_block = true;
	}
	|
	top_blocks top_block {
		// do something with $2
		// and than stop
		end_of_top_block = true;
	}
	;

top_block: block

block: KEYWORD ';' {
		// System.out.println("block: KEYWORD ';'");
	}
	| KEYWORD arrayvalue ';' {
		// System.out.println("block: KEYWORD arrayvalue ';'");
	}
	| KEYWORD '{' blocks '}' {
		// System.out.println("block: KEYWORD '{' blocks '}'");
	};

blocks: block {
		// System.out.println("blocks: block");
	}
	| blocks block {
		// System.out.println("blocks: blocks block");
	};

arrayvalue:	intarray
	|	floatarray
	|	stringarray
	|	timearray
	;

intarray:	v_int
	|	intarray v_int
	;

floatarray:	v_float
	|	floatarray v_float
	;

stringarray:	v_string
	|	stringarray v_string
	;

timearray:	time
	|	timearray time
	;

time: v_float 's' {
			$$ = $1;
	}
	| v_int ':' v_float 'm' {
			$$ = $3;
			$$ += 60.0 * $1;
	}
	| v_int ':' v_int ':' v_float 'h' {
			$$ = $5;
			$$ += 60.0 * $3;
			$$ += 60.0 * $5;
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

	static boolean interactive;

	static boolean parse_empty = false;

	public static void main(String args[]) throws IOException
	{
		System.out.println("BYACC/Java & JFlex LS text input");

		LsParse yyparser;
		if ( args.length > 0 ) {
			// parse a file
			yyparser = new LsParse(new FileReader(args[0]));
		}
		else {
			throw new IOException("error: no file");
		}

		int top_level_blocks = 0;
		do {
			// yyparser.yydebug = true;
			yyparser.yyparse();
			if (yyparser.parse_empty) {
				yyparser.parse_empty = false;
				break;
			}
			else {
				top_level_blocks++;
			}
		} while (!yyparser.lexer.lex_at_eof());

		System.out.println(top_level_blocks +
			" top level blocks");

	}

