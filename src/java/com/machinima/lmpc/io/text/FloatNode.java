/* $Id$ */

class FloatNode extends Node {
	float value;

	public FloatNode(float value)
	{
		this.value = value;
	}

	public String toString()
	{
		return Float.toString(value);
	}
}

