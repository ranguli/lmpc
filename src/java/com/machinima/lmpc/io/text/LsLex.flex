/* LS text lexer */
/* $Id$ */

%%

%{
%}



%class LsLex
%byaccj
%char
%state STRING
%full
%line
%column


%{
	/* the buffer to hold string literals */
	StringBuffer string = new StringBuffer();

	/* the parser object */
	private LsParse yyparser;

	/* constructor */
	public LsLex(java.io.Reader r, LsParse yyparser)
	{
		this(r);	/* call the standard constructor */
		this.yyparser = yyparser;	/* store the parser */
	}

	public int curline () { return yyline + 1; }
	public int curcolumn () { return yycolumn + 1; }

	public boolean lex_at_eof() { return yy_atEOF; }
%}

LineTerminator = \r|\n|\r\n
InputCharacter = [^\r\n]
WhiteSpace = {LineTerminator} | [ \t\f]
CommentContent = ( [^*] | \*+[^*/] )*
TraditionalComment = "/*" [^*] {CommentContent} \*+ "/"
EndOfLineComment = "//" {InputCharacter}* {LineTerminator}
Comment = {TraditionalComment} | {EndOfLineComment}
Identifier = [A-Za-z_][A-Za-z_0-9]*

/* integer numbers */
DecIntegerLiteral = 0 | [1-9][0-9]*

HexDigit = [0-9a-fA-F]
HexIntegerLiteral = 0 [xX] 0* {HexDigit} {1,8}

OctDigit = [0-7]

/* floating point numbers */
Exponent = [eE] [+\-]? [0-9]+
FLit1 = [0-9]+ \. [0-9]* {Exponent}?
FLit2 = \. [0-9]+ {Exponent}?
FLit3 = [0-9]+ {Exponent}
FLit4 = [0-9]+ {Exponent}?

FloatLiteral  = ({FLit1}|{FLit2}|{FLit3}|{FLit4})

/* string values */
StringCharacter = [^\r\n\"\\]


%% 

<YYINITIAL> {

	/* single char specials */
	":" |
	"s" |
	"m" |
	"h" |
	"{" |
	"}" |
	";" {
		return (int) yycharat(0);
	}

	/* string literal */
	\" {
		yybegin(STRING);
		string.setLength(0);
	}

	/* numeric literals */
	-?{DecIntegerLiteral} {
		yyparser.yylval = new IntNode(Integer.parseInt(yytext()));
		return yyparser.v_int;
	}

	{HexIntegerLiteral} {
		yyparser.yylval = new IntNode(Integer.parseInt(yytext().substring(2),16));
		return yyparser.v_int;
	}
 
	-?{FloatLiteral} {
		yyparser.yylval = new FloatNode(Float.parseFloat(yytext().substring(0,yylength()-1)));
		return yyparser.v_float;
	}

	/* comments */
	{Comment} {
		/* ignore */
	}
 
	/* whitespace */
	{WhiteSpace} {
		/* ignore */
	}
 
	/* keywords */
	{Identifier} {
		yyparser.yylval = new TokenNode(yytext()); // set lval
		return yyparser.TOKEN;
	}

} /* end of YYINITIAL */


<STRING> {
	\"	{
			yybegin(YYINITIAL);	// reset old state
			yyparser.yylval = new StringNode(string.toString()); // set lval
			return yyparser.v_string;	// return "we have a string"
		}

	{StringCharacter}+	{
			string.append( yytext() );
		}
	/* escape sequences */
	"\\b"	{	string.append( '\b' );	}
	"\\t"	{	string.append( '\t' );	}
	"\\n"	{	string.append( '\n' );	}
	"\\f"	{	string.append( '\f' );	}
	"\\r"	{	string.append( '\r' );	}
	"\\\""	{	string.append( '\"' );	}
	"\\'"	{	string.append( '\'' );	}
	"\\\\"	{	string.append( '\\' );	}
	\\[0-3]?{OctDigit}?{OctDigit}	{
			char val =
			(char) Integer.parseInt(yytext().substring(1),8);
			string.append( val );
		}  
	/* error cases */
	\\.	{	throw new RuntimeException("Illegal escape sequence \""
			+ yytext() + "\"");
		}
	{LineTerminator}	{
			throw new RuntimeException("Unterminated string at end of line");
		}
} /* end of STRING */


/* error fallback */
. {
	throw new RuntimeException(
		"Illegal character \""
		+ yytext()
		+ "\" at line " + (yyline+1) + ", column " + (yycolumn+1));
}


