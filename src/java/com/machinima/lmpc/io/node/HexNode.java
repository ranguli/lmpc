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
		// We allow only byte values. Maybe we should check this here ?
		buffer.append((char)(((value & 0xf0)>>4) + '0'));
		buffer.append((char)(((value & 0x0f)   ) + '0'));
		return buffer.toString();
	}
}

