// $Id$
// class with utility functions


package com.machinima.lmpc.io.base;


public class Util {

	public static String toString(int digits, char format, float value)
	{
		return toString(digits, format, (double)value);
	}

	public static String toString(int digits, char format, double value)
	{
		StringBuffer text = new StringBuffer();

		if (value < 0.0) {
			text.append("-");
			value = -value;
		}

		int intPart = (int)value;
		double fracPart = value - intPart;

		String intPartString = new Integer(intPart).toString();
		int intPartDigits = intPartString.length();
		int valuableIntDigits = 0;
		if (format=='g' && intPart != 0) {
			valuableIntDigits = intPartDigits;
		}

		text.append(intPartString);

		text.append('.');

		int addDigits=0;

		for (int i=valuableIntDigits ;
			i<digits + addDigits; i++) {
			fracPart *= 10.0;
			if (format=='g' && intPart==0 && fracPart < 1.0 &&
				addDigits<4) {
				addDigits++;
			}
		}

		long fracPartLong = Math.round(fracPart);
		if (fracPartLong != 0) {
			digits += addDigits;
		}
		else {
			if (intPart == 0) {
				valuableIntDigits = 1;
			}
		}

		String fracPartString = new Long(fracPartLong).toString();
		int fracPartDigits = fracPartString.length();

		for (	int i=valuableIntDigits ;
			i < digits - fracPartDigits;
			i++) {
			text.append('0');
		}

		text.append(fracPartString);
			
		return new String(text);
	}

}

