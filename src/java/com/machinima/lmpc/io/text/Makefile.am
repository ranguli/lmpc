## Process this file with automake to produce Makefile.in
# $Id$

JFLEX=jflex
BYACCJ=byaccj -J
JAVAROOT=$(top_builddir)/src/java

BUILT_SOURCES=LsLex.java LsParse.java
java_JAVA=TextOut.java $(BUILT_SOURCES)

LsLex.java: LsLex.flex
	$(JFLEX) LsLex.flex

LsParse.java: LsParse.y
	$(BYACCJ) -v -Jclass=LsParse -Jsemantic=com.machinima.lmpc.io.node.Node LsParse.y

LsLex.class: LsLex.java LsParse.java

LsParse.class: LsParse.java LsLex.java

