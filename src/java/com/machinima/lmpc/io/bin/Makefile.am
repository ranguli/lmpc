# $Id$

FLAVORC=@FLAVORC@

FLAVORC_DEBUG=

CLASSPATH_ENV = CLASSPATH=$(JAVAROOT):$(srcdir)/$(JAVAROOT):@FLAVOR_HOME@/lib/JAVA:$$CLASSPATH

JAVAROOT=$(top_builddir)/src/java

dm2_q2_block.java: $(top_srcdir)/spec/dm2_q2.fl
	$(FLAVORC) $(FLAVORC_DEBUG) -gj -oj com.machinima.lmpc.io.bin $<

java_JAVA=dm2_q2_block.java

CLEANFILES=dm2_q2_block.java

