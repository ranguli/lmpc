## Process this file with automake to produce Makefile.in
# $Id$

JAVAROOT=$(top_builddir)/src/java

BUILT_SOURCES=LsLex.java LsParse.java
java_JAVA=TextBlockReader.java TextBlockWriter.java $(BUILT_SOURCES)
CLEANFILES=$(BUILT_SOURCES)

LsLex.java: LsLex.flex
	$(JFLEX) $(srcdir)/LsLex.flex

LsParse.java: LsParse.y
	$(BYACC) -J -v -Jclass=LsParse -Jsemantic=com.machinima.lmpc.io.node.Node $(srcdir)/LsParse.y

LsLex.class: LsLex.java LsParse.java

LsParse.class: LsParse.java LsLex.java

