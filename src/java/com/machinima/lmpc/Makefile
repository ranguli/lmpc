# $Id$

JAVAC=javac
JFLEX=jflex
BYACCJ=byaccj -J

MAIN_CLASSES= \
	lmpc.class

MAIN_SOURCES= \
	lmpc.java \

CLASSES=$(MAIN_CLASSES)
SOURCES=$(MAIN_SOURCES)

all: $(CLASSES)

clean:
	rm -f $(CLASSES) $(BUILT_SOURCES)

%.class: %.java
	$(JAVAC) $<

