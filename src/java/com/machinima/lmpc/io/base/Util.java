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

		boolean needExponent = false;
		int exponent = 0;
		StringBuffer exponentText = new StringBuffer();
		if (value!=0.0 && value<0.0001) {
			needExponent = true;
			exponent = (int)Math.log(value)/(int)Math.log(10.0);
			exponentText.append('-');
			exponent= -exponent;
			value *= Math.pow(10,exponent);
			if (exponent < 10) {
				exponentText.append('0');
			}
			exponentText.append(exponent);
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

		if (needExponent) {
			text.append('e');
			text.append(exponentText);
		}
			
		return new String(text);
	}

}

