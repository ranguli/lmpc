/* $Id$ */

package com.machinima.lmpc.io.node;
import com.machinima.lmpc.io.node.Node;

public class TokenNode extends Node {
	String value;

	public TokenNode(String value)
	{
		this.value = value;
	}

	public String toString()
	{
		return value;
	}
}

