# $Id$

EXTRA_DIST=$(PACKAGE).base COMPILE COMPILE-go32

$(srcdir)/../README:
	$(top_srcdir)/bin/wpu --set ACTION=readme --set SRCDIR=$(top_srcdir) --set PACK=f $(top_srcdir)/README.base $(srcdir)/../README


TYPE=misc
UPLOAD_FILES_misc=$(UPLOAD_FILES_document) ../README
UPLOAD_DIR_misc=$(PACKAGE)-$(QUALITY)
DEFAULT_TARGET_misc=$(DEFAULT_TARGET_document)

include $(top_srcdir)/spec/Make.spec.rules

