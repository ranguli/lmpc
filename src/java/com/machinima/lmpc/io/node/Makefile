# $Id$

JAVAC=javac

NODE_CLASSES= \
	Node.class \
	IntNode.class \
	FloatNode.class \
	StringNode.class \
	TimeNode.class \
	TokenNode.class

NODE_SOURCES= \
	Node.java \
	IntNode.java \
	FloatNode.java \
	StringNode.java \
	TimeNode.java \
	TokenNode.java

CLASSES=$(NODE_CLASSES)
SOURCES=$(NODE_SOURCES)

all: $(CLASSES)

clean:
	rm -f $(CLASSES)

%.class: %.java
	$(JAVAC) $<

