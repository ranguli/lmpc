// $Id$

package com.machinima.lmpc;

import com.machinima.lmpc.io.text.*;
import com.machinima.lmpc.io.node.Node;
import com.machinima.lmpc.io.base.*;
// import java.io.*;
import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

class lmpc {

	public static final String release = "$Revision$";
	public static final String date = "$Date$";


	public static void Version()
	{
		System.err.println("LMPCj (c) U. Girlich, " +
			release + " " +
			date);
	}


	public static void Syntax()
	{
		System.err.println("Little Movie Processing Centre/Java");
		Version();
		System.err.println("Usage: java com.machinima.lmpc.lmpc [option ... ] filename [filename]");
		System.err.println("-h, --help		display this help and exit.");
		System.err.println("-V, --version	output version infromation and exit.");
		System.err.println("-s, --to-txt	text to text conversion.");
	}


	public static void main(String args[])
	{
		LongOpt[] longopts = new LongOpt[2];
		longopts[0] = new LongOpt("help", LongOpt.NO_ARGUMENT, null, 'h');
		longopts[1] = new LongOpt("version", LongOpt.NO_ARGUMENT, null, 'V');

		Getopt g = new Getopt("lmpcj", args, "hV", longopts);
		g.setOpterr(false);
		int c;
		while ((c = g.getopt()) != -1) {
			switch (c) {
				case 'h':
					Syntax();
					System.exit(0);
				break;
				case 'V':
					Version();
					System.exit(0);
				break;
				case '?':
					System.err.println("The option '" +
						(char)g.getOptopt() +
						"' is not valid");
					Syntax();
					System.exit(1);
				break;
				default:
					System.err.println("getopt() returned " + c);
					Syntax();
					System.exit(1);
				break;
			}
		}
		int LeftArgs = args.length - g.getOptind();

		// System.out.println("We have " + LeftArgs + " args left.");

		if (LeftArgs == 0) {
			Syntax();
			System.exit(1);
		}

		for (int i = g.getOptind(); i < args.length ; i++) {
			// System.out.println("Non option args element: " + args[i]);
		}

		String fromFilename = null;
		String toFilename = null;
		if (LeftArgs == 2) {
			fromFilename = args[g.getOptind()];
			toFilename = args[g.getOptind() + 1];
                }
                else {
                        Syntax();
			System.exit(1);
                }

		int result = 0;

		Action action = new Action();

		result = action.convert(args[0], args[1]);

		System.exit(result);
	}
}

