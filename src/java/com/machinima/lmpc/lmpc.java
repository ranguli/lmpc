// $Id$

package com.machinima.lmpc;

import com.machinima.lmpc.io.text.*;
import com.machinima.lmpc.io.node.Node;
import java.io.*;

class lmpc {


	public static void main(String args[]) throws IOException
	{
		System.out.println("Java LMPC");

		LsParse reader;

		if ( args.length > 0 ) {
			// parse a file
			reader = new LsParse(new FileReader(args[0]));
		}
		else {
			throw new IOException("error: no file");
		}

		TextOut writer = new TextOut(new FileWriter("out"));

		// this main loop should be the same for all tpes of files
		int top_level_blocks = 0;
		Node block = null;
		do {
			block = reader.ReadNextBlock();
			if (block != null) {
				top_level_blocks++;
				writer.WriteNextBlock(block);
			}
			else {
				break;
			}
		} while (block != null);

		System.out.println(top_level_blocks +
			" blocks");
	}


}

