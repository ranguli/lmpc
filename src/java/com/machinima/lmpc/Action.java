// $Id$


package com.machinima.lmpc;


import com.machinima.lmpc.io.text.*;
import com.machinima.lmpc.io.node.Node;
import com.machinima.lmpc.io.base.*;
import java.io.*;


class Action {


	public int convert(String fromFilename, String toFilename)
	{
		BlockReader reader = null;
		BlockWriter writer = null;

		try {
			reader = new TextBlockReader(new FileReader(fromFilename));
		}
		catch (java.io.FileNotFoundException e) {
			System.err.println("Could not open file for reading: " +
			e.getMessage());
			System.exit(1);
		}

		try {
			writer = new TextBlockWriter(new FileWriter(toFilename));
		}
		catch (java.io.IOException e) {
			System.err.println("Could not open file for writing: " +
			e.getMessage());
			System.exit(1);
		}

		System.err.println(fromFilename + " (txt) -> " +
					toFilename + " (txt)");

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

		/*
		System.out.println(top_level_blocks +
			" blocks converted");
		*/

		return 0;
	}


}
