/* $Id$ */

class TokenNode extends Node {
	String value;

	public TokenNode(String value)
	{
		this.value = value;
	}

	public String toString()
	{
		return value;
	}
}

