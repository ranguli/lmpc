# $Id$

JAVAC=javac
JFLEX=jflex
BYACCJ=byaccj -J

CLASSES=LsLex.class LsParse.class TextOut.class
BUILT_SOURCES=LsLex.java LsParse.java
SOURCES=$(BUILT_SOURCES)

all: $(CLASSES)

clean:
	rm -f $(CLASSES) $(BUILT_SOURCES)

%.class: %.java
	$(JAVAC) $<

LsLex.java: LsLex.flex
	$(JFLEX) LsLex.flex

LsParse.java: LsParse.y
	$(BYACCJ) -v -Jclass=LsParse -Jsemantic=com.machinima.lmpc.io.node.Node LsParse.y

LsLex.class: LsLex.java LsParse.java

LsParse.class: LsParse.java LsLex.java

