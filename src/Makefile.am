## Process this file with automake to produce Makefile.in
# $Id$

SUBDIRS = quake3 java

GENTOK=$(PERL) $(top_srcdir)/bin/gentok
EXTRACT=$(PERL) $(top_srcdir)/bin/extract


CALC_FROM_SPECS = udm2-te1.c udm2-te2.c udm2-te3.c


bin_PROGRAMS = @dbs_bin@ @demcut_bin@ @lmpc_bin@
EXTRA_PROGRAMS = dbs demcut lmpc
EXTRA_HEADERS = 
noinst_LIBRARIES = libutil.a
noinst_HEADERS = getopt.h binblock.h lmpc.h node.h tools.h udem.h \
	udm2.h $(UDM3_H) \
udmo.h ulmp.h uqwd.h record.h
EXTRA_DIST = udeml.l.in udemy.y.in tokenlist.in $(CALC_FROM_SPECS)
CLEANFILES = udemy.y udemy.c udemy.h udeml.l udeml.c tokenlist tokenlist.tmp

BUILT_SOURCES = udeml.l udemy.y 

dbs_SOURCES = dbs.c node.c udem.c
dbs_LDADD = $(NETLIBS) libutil.a
dbs_DEPENDENCIES = libutil.a udemy.h

demcut_SOURCES = demcut.c udem.c
demcut_LDADD = libutil.a
demcut_DEPENDENCIES = libutil.a udemy.h

lmpc_SOURCES = lmpc.c ulmp.c lzw.c unlzw.c udmo.c udem.c udemy.y udeml.l \
uqwd.c udm2.c demotextl.l
lmpc_LDADD = $(UDM3_O) libutil.a quake3/libquake3.a @LEXLIB@ @ALLOCA@
lmpc_DEPENDENCIES = libutil.a quake3/libquake3.a $(UDM3_O) @ALLOCA@

udm3.o: udm3.c udm3.h

libutil_a_SOURCES = tools.c node.c binblock.c record.c
libutil_a_LIBADD = @LIBOBJS@

TOKENBASEFILES = udem.c uqwd.c udm2.c udm2-te3.c $(UDM3_C)

tokenlist: tokenlist.in $(TOKENBASEFILES)
	srcdir=$(srcdir) $(GENTOK) $(TOKENBASEFILES) < $(srcdir)/tokenlist.in > tokenlist.tmp
	cmp -s tokenlist.tmp tokenlist || cp tokenlist.tmp tokenlist

udemy.y: udemy.y.in tokenlist
	srcdir=$(srcdir) $(GENTOK) $(TOKENBASEFILES) < $(srcdir)/udemy.y.in > udemy.y

udeml.l: udeml.l.in tokenlist
	srcdir=$(srcdir) $(GENTOK) $(TOKENBASEFILES) < $(srcdir)/udeml.l.in > udeml.l

# The general Automake generated rule is wrong, as it cannot cope with
# the source line %option prefix="demotext_". This option generates an
# unexpected file name.
demotextl.c: demotextl.l
	$(SHELL) $(YLWRAP) $< lex.demotext_.c $@ -- $(LEXCOMPILE)

record.o: udemy.h

udm2.o: $(CALC_FROM_SPECS)

$(CALC_FROM_SPECS):
	echo extract specs to $(CALC_FROM_SPECS)
	$(EXTRACT)

AM_YFLAGS = -d

dist-hook:
	cd $(distdir) && rm -f udemy.y udemy.c udemy.h udeml.l udeml.c
