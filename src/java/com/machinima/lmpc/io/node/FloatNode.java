/* $Id$ */

package com.machinima.lmpc.io.node;
import com.machinima.lmpc.io.node.Node;
import com.machinima.lmpc.io.base.Util;
import java.lang.*;
import java.text.*;
import java.util.*;

public class FloatNode extends Node {
	float value;

	public FloatNode(float value)
	{
		this.value = value;
	}

	public String toString()
	{
		return Util.toString(10, value);
	}

}

