# $Id$

FLAVORC=@FLAVORC@

FLAVORC_DEBUG=

CLASSPATH_ENV = CLASSPATH=$(JAVAROOT):$(srcdir)/$(JAVAROOT):@FLAVOR_HOME@/lib/JAVA:$$CLASSPATH

JAVAROOT=$(top_builddir)/src/java

FLAVOR_JAVA_FILES=dm2_q2_block.java dm_67_q3a_block.java

dm2_q2_block.java: $(top_srcdir)/spec/dm2_q2.fl
	$(FLAVORC) $(FLAVORC_DEBUG) -gj -oj com.machinima.lmpc.io.bin $<

dm_67_q3a_block.java: $(top_srcdir)/spec/dm_67_q3a.fl
	$(FLAVORC) $(FLAVORC_DEBUG) -gj -oj com.machinima.lmpc.io.bin $<

javadir = $(datadir)/java
java_JAVA=$(FLAVOR_JAVA_FILES)

CLEANFILES=$(FLAVOR_JAVA_FILES)


