
package com.machinima.lmpc;
import com.machinima.lmpc.io.text.*;

import java.io.*;

class lmpc {


	public static void main(String args[]) throws IOException
	{
		System.out.println("Java LMPC");

		LsParse yyparser;

		if ( args.length > 0 ) {
			// parse a file
			yyparser = new 
				com.machinima.lmpc.io.text.LsParse(new FileReader(args[0]));
		}
		else {
			throw new IOException("error: no file");
		}

		TextOut textout = new TextOut(new FileWriter("out"));
		int top_level_blocks = 0;
		do {
			// yyparser.yydebug = true;
			boolean result = yyparser.block_read();
			if (result) {
				top_level_blocks++;
				textout.node_write_text(yyparser.get_yyval(),0);
			}
			else {
				break;
			}
		} while (!yyparser.lexer.lex_at_eof());

		System.out.println(top_level_blocks +
			" blocks");
	}


}

