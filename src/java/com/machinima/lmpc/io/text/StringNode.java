/* $Id$ */

class StringNode extends Node {
	String value;

	public StringNode(String value)
	{
		this.value = value;
	}

	public String toString()
	{
		return value;
	}
}

