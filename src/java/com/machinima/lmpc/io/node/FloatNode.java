/* $Id$ */

package com.machinima.lmpc.io.node;
import com.machinima.lmpc.io.node.Node;
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
		NumberFormat floatFormat =
			NumberFormat.getNumberInstance(Locale.US);
		floatFormat.setMinimumIntegerDigits(1);
		floatFormat.setMaximumIntegerDigits(8);
		floatFormat.setMinimumFractionDigits(1);
		floatFormat.setMaximumFractionDigits(8);
		floatFormat.setGroupingUsed(false);
		if (floatFormat instanceof DecimalFormat) {
			DecimalFormat df = (DecimalFormat)floatFormat;
			df.setDecimalSeparatorAlwaysShown(true);
			return df.format((double)value);
		}

		return floatFormat.format((double)value);
	}
}

