// $Id$

package com.machinima.lmpc;

import com.machinima.lmpc.io.text.*;
import com.machinima.lmpc.io.node.Node;
import com.machinima.lmpc.io.base.*;
import java.io.*;

class lmpc {


	public static void main(String args[]) throws IOException
	{
		System.out.println("Java LMPC");

		BlockReader reader;
		BlockWriter writer;

		if ( args.length > 0 ) {
			// parse a file
			reader = new TextBlockReader(new FileReader(args[0]));
		}
		else {
			throw new IOException("error: no file");
		}

		writer = new TextBlockWriter(new FileWriter("out"));

		// this main loop should be the same for all tpes of files
		int top_level_blocks = 0;
		Node block = null;
		do {
			block = reader.ReadBlock();
			if (block != null) {
				top_level_blocks++;
				writer.WriteBlock(block);
			}
			else {
				break;
			}
		} while (block != null);

		System.out.println(top_level_blocks +
			" blocks");
	}


}

