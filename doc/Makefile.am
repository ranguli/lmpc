## Process this file with automake to produce Makefile.in

PERL=@PERL@
SED=@SED@

EXTRA_DIST=$(PACKAGE).base COMPILE COMPILE-go32

TYPE=document

.PHONY: docu sgml txt doc html dvi ps ps.gz ps.zip

doc: pdf html

#CLEANFILES=$(PACKAGE).sgml \
#	$(PACKAGE).txt $(PACKAGE).doc \
#	$(PACKAGE).html $(PACKAGE)-?.html $(PACKAGE)-??.html \
#	$(PACKAGE).dvi $(PACKAGE).ps \
#	index.html indexpart.html

include $(top_srcdir)/spec/Make.spec.rules

