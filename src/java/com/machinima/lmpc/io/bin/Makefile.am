# $Id$

# this is specific to my own computer, it should be checked for in configure
FLAVORC=/home/uwe/mi/flavor/flavor-4.0/bin/flavorc

FLAVOR_DEBUG=

LNS=ln -s
RMF=rm -f

SUFFIXES=.fl .java

JAVAROOT=$(top_builddir)/src/java

.fl.java:
	$(FLAVORC) $(FLAVOR_DEBUG) -gj $<

dm2_q2.fl: $(top_srcdir)/spec/dm2_q2.fl
	$(RMF) $@
	$(LNS) $< $@

java_JAVA=dm2_q2_block.java

