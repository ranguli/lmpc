/* $Id$ */

package com.machinima.lmpc.io.node;
import com.machinima.lmpc.io.node.Node;

public class HexNode extends IntNode {

	public HexNode(int value)
	{
		super(value);
	}

	public String toString()
	{
		StringBuffer buffer = new StringBuffer("0x");
		int digit1 = ((value & 0xf0)>>>4);
		buffer.append( (char)((digit1<10 ? '0' : 'A'-10) + digit1) );
		int digit2 = ((value & 0x0f)    );
		buffer.append( (char)((digit2<10 ? '0' : 'A'-10) + digit2) );
		return buffer.toString();
	}
}

