/* $Id$ */

package com.machinima.lmpc.io.node;
import com.machinima.lmpc.io.node.Node;

class TimeNode extends Node {
	float value;

	public TimeNode(IntNode h, IntNode m, FloatNode s)
	{
		value = 0.0f;
		if (s != null) {
			value += s.value;
		}
		if (m != null) {
			value += m.value * 60.0;
		}
		if (h != null) {
			value += h.value * 3600.0;
		}
	}

	public TimeNode(IntNode m, FloatNode s)
	{
		this((IntNode)null, m, s);
	}

	public TimeNode(FloatNode s)
	{
		this((IntNode)null, (IntNode)null, s);
	}

	public String toString()
	{
		StringBuffer result = new StringBuffer("");

		int hour = (int)(value / 3600.0);
		int min = (int)((value - hour * 3600.0) / 60.0);
		double sec = value - hour * 3600.0 - min * 60.0;
		int secFull = (int)sec;
		int sec100 = (int)((sec-secFull)*100.0);

		if (hour != 0) {
			result.append(Integer.toString(hour));
			result.append(':');
			result.append(Integer.toString(min));
			result.append(':');
			result.append(Integer.toString(secFull));
			result.append('.');
			result.append(Integer.toString(sec100));
			result.append('h');
		}
		else {
			if (min != 0) {
				result.append(Integer.toString(min));
				result.append(':');
				result.append(Integer.toString(secFull));
				result.append('.');
				result.append(Integer.toString(sec100));
				result.append('m');
			}
			else {
				result.append(Integer.toString(secFull));
				result.append('.');
				result.append(Integer.toString(sec100));
				result.append('s');
			}
		}
		return result.toString();
	}
}

