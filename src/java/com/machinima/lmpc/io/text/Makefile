# $Id$

JAVAC=javac
JFLEX=jflex
BYACCJ=byaccj -J

NODE_CLASSES=Node.class IntNode.class FloatNode.class StringNode.class TokenNode.class
NODE_SOURCES=Node.java IntNode.java FloatNode.java StringNode.java TokenNode.java
CLASSES=$(NODE_CLASSES) LsLex.class LsParse.class
BUILT_SOURCES=LsLex.java LsParse.java
SOURCES=$(NODE_SOURCES) $(BUILT_SOURCES)

all: $(CLASSES)

clean:
	rm -f $(CLASSES) $(BUILT_SOURCES)

%.class: %.java
	$(JAVAC) $<

LsLex.java: LsLex.flex
	$(JFLEX) LsLex.flex

LsParse.java: LsParse.y
	$(BYACCJ) -v -Jclass=LsParse -Jsemantic=Node LsParse.y

LsLex.class: LsLex.java LsParse.java Node.java

LsParse.class: LsParse.java LsLex.java Node.java

