/* $Id$ */

package com.machinima.lmpc.io.node;
import com.machinima.lmpc.io.node.Node;

public class StringNode extends Node {
	String value;

	public StringNode(String value)
	{
		this.value = value;
	}

	public String toString()
	{
		StringBuffer buffer = new StringBuffer("");
		buffer.append('"');
		for (int i=0 ; i<value.length() ; i++) {
			char c = value.charAt(i);
			if (c >= '\040') {
				switch(c) {
					case '"':
						buffer.append("\\\"");
					break;
					case '\\':
						buffer.append("\\\\");
					break;
					default:
						buffer.append(c);
					break;
				}
			}
			else {
				switch(c) {
					case '\n':
						buffer.append("\\n");
					break;
					case '\r':
						buffer.append("\\r");
					break;
					default:
						buffer.append('\\');
						// Standard Java variant:
						// buffer.append(Integer.toOctalString(c));
						// is not working because it is not always 3 
						// digits long.
						// Why we don't have something like "%03o" ?
						buffer.append((char)(((c & 0700)>>6) + '0'));
						buffer.append((char)(((c & 0070)>>3) + '0'));
						buffer.append((char)(((c & 0007)   ) + '0'));
						Integer.toOctalString(c));
					break;
				}
			}
		}
		buffer.append('"');
		return buffer.toString();
	}
}

