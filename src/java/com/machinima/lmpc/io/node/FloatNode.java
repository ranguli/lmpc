/* $Id$ */

package com.machinima.lmpc.io.node;
import com.machinima.lmpc.io.node.Node;

public class FloatNode extends Node {
	float value;

	public FloatNode(float value)
	{
		this.value = value;
	}

	public String toString()
	{
		return Float.toString(value);
	}
}

