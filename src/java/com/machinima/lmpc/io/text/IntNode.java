/* $Id$ */

class IntNode extends Node {
	int value;

	public IntNode(int value)
	{
		this.value = value;
	}

	public String toString()
	{
		return Integer.toString(value);
	}
}

