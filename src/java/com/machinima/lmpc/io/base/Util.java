// $Id$
// class with utility functions


package com.machinima.lmpc.io.base;


public class Util {

	public static String toString(int digits, float value)
	{
		return toString(digits, (double)value);
	}

	public static String toString(int digits, double value)
	{
		StringBuffer text = new StringBuffer();

		if (value < 0.0) {
			text.append("-");
			digits++;
		}

		double positiveFloatValue = Math.abs(value);
		int positiveIntValue = (int)positiveFloatValue;
		text.append(positiveIntValue);
		if (value != 0.0 && positiveIntValue == 0) {
			digits++;
		}

		text.append('.');

		positiveFloatValue -= (int)positiveFloatValue;
		while (text.length()<digits) {
			positiveFloatValue *= 10.0;
			text.append((char)('0' +
					(int)(positiveFloatValue) % 10));
		}

		return new String(text);
	}

}

