/* $Id$ */

package com.machinima.lmpc.io.node;
import com.machinima.lmpc.io.node.Node;

class IntNode extends Node {
	int value;

	public IntNode(int value)
	{
		this.value = value;
	}

	public String toString()
	{
		return Integer.toString(value);
	}
}

