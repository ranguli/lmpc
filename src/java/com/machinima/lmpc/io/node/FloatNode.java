/* $Id$ */

package com.machinima.lmpc.io.node;
import com.machinima.lmpc.io.node.Node;
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
		StringBuffer text = new StringBuffer();
		int maxlength = 10;
		if (value < 0.0) {
			text.append("-");
			maxlength++;
		}
		
		float positiveFloatValue = Math.abs(value);
		int positiveIntValue = (int)positiveFloatValue;
		text.append(positiveIntValue);
		if (value != 0.0 && positiveIntValue == 0) {
			maxlength++;
		}

		text.append('.');

		positiveFloatValue -= (int)positiveFloatValue;
		while (text.length()<maxlength) {
			positiveFloatValue *= 10.0;
			text.append((char)('0' + (int)(positiveFloatValue) % 10));

		}

		return new String(text);
	}

}

