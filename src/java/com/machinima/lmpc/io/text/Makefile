JAVAC=javac
JFLEX=jflex
BYACCJ=byaccj -J

all: LsLex.class LsParse.class LsParseVal.class

clean:
	rm -f	LsLex.class LsLex.java \
		LsParse.class LsParse.java \
		LsParseVal.class LsParseVal.java

%.class: %.java
	$(JAVAC) $<

LsLex.java: LsLex.flex
	$(JFLEX) LsLex.flex

LsParse.java: LsParse.y
	$(BYACCJ) -v -Jclass=LsParse LsParse.y

LsParseVal.java: LsParse.java

LsLex.class: LsLex.java LsParse.java

LsParse.class: LsParse.java LsLex.java

