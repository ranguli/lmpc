// $Id$
// class for text block reading


package com.machinima.lmpc.io.text;


import com.machinima.lmpc.io.node.Node;
import com.machinima.lmpc.io.base.BlockReader;
import com.machinima.lmpc.io.text.LsParse;

 
public class TextBlockReader extends BlockReader {


	private LsParse parser;


	public TextBlockReader(java.io.Reader r) {
		super(r);
		parser = new LsParse(r);
	}

 
	public Node ReadBlock() {
		boolean read_result = parser.read_block();
		Node get_result;

		if (read_result) {
			get_result = parser.get_block();
		}
		else {
			get_result = null;
		}
		// !yyparser.lexer.lex_at_eof()
	
		return get_result;
	}


}

