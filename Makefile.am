 ## Process this file with automake to produce Makefile.in


AUTOMAKE_OPTIONS = foreign
EXTRA_DIST = wpurc README.base
SUBDIRS = bin doc src spec


######################################################################
# documentation rules

sgml:
	cd doc && $(MAKE) $@

html:
	cd doc && $(MAKE) $@

pdf:
	cd doc && $(MAKE) $@
	

#####################################################################
# WWW page rules

index.html indexpart.html:
	cd doc && $(MAKE) $@


######################################################################
# package compiling rules (binary packages, mostly cross-compile)
# All direcories are hard-coded in here. You _will_ have to change them.


# LINUX target
linux:
	$(MAKE) compile-bin TARGET=linux

TARGET_linux=i686-suse-linux
PREFIX_linux=/usr
CC_linux=gcc
CXX_linux=g++
RANLIB_linux=ranlib
STRIP_linux=strip
BINS_linux=dbs demcut lmpc
CHANGE_linux="\#"


# GO32 target
go32:
	$(MAKE) compile-bin TARGET=go32

TARGET_go32=i386-pc-msdosdjgpp
PREFIX_go32=/usr/self/go32
CC_go32=$(PREFIX_go32)/$(TARGET_go32)/bin/gcc
CXX_go32=$(PREFIX_go32)/$(TARGET_go32)/bin/g++
RANLIB_go32=$(PREFIX_go32)/$(TARGET_go32)/bin/ranlib
STRIP_go32=$(PREFIX_go32)/$(TARGET_go32)/bin/strip
BINS_go32=demcut.exe lmpc.exe
CHANGE_go32=s/.exe/.exe/


# CYGWIN target
cygwin:
	$(MAKE) compile-bin TARGET=cygwin

TARGET_cygwin=i386-pc-cygwin
PREFIX_cygwin=/usr/self/gcc3
CC_cygwin=$(PREFIX_cygwin)/$(TARGET_cygwin)/bin/gcc
CXX_cygwin=$(PREFIX_cygwin)/$(TARGET_cygwin)/bin/g++
RANLIB_cygwin=$(PREFIX_cygwin)/$(TARGET_cygwin)/bin/ranlib
STRIP_cygwin=$(PREFIX_cygwin)/$(TARGET_cygwin)/bin/strip
BINS_cygwin=dbs.exe demcut.exe lmpc.exe
CHANGE_cygwin=s/.exe/32.exe/


# MINGW target
mingw:
	$(MAKE) compile-bin TARGET=mingw

TARGET_mingw=i386-pc-mingw32
PREFIX_mingw=/usr/self/gcc3
CC_mingw=$(PREFIX_mingw)/$(TARGET_mingw)/bin/gcc
CXX_mingw=$(PREFIX_mingw)/$(TARGET_mingw)/bin/g++
RANLIB_mingw=$(PREFIX_mingw)/$(TARGET_mingw)/bin/ranlib
STRIP_mingw=$(PREFIX_mingw)/$(TARGET_mingw)/bin/strip
CFLAGS_mingw=-mno-cygwin
LDFLAGS_mingw=-mno-cygwin
BINS_mingw=demcut.exe lmpc.exe
CHANGE_mingw=s/.exe/m.exe/


# create generic target variables
TARGET_GEN=$(TARGET_$(TARGET))
PREFIX_GEN=$(PREFIX_$(TARGET))
CC_GEN=$(CC_$(TARGET))
CXX_GEN=$(CXX_$(TARGET))
RANLIB_GEN=$(RANLIB_$(TARGET))
STRIP_GEN=$(STRIP_$(TARGET))
CFLAGS_GEN=$(CFLAGS_$(TARGET))
LDFLAGS_GEN=$(LDFLAGS_$(TARGET))
BINS_GEN=$(BINS_$(TARGET))
CHANGE_GEN=$(CHANGE_$(TARGET))


# generic compile rules
compile-bin:
	mv config.status config.status.orig
	-mkdir -p systems/$(TARGET_GEN)
	cd systems/$(TARGET_GEN) && \
		CC=$(CC_GEN) \
		CXX=$(CXX_GEN) \
		RANLIB=$(RANLIB_GEN) \
		CFLAGS=$(CFLAGS_GEN) \
		LDFLAGS=$(LDFLAGS_GEN) \
		../../configure --host=$(TARGET_GEN)
	cd systems/$(TARGET_GEN)/src && $(MAKE) $(BINS_GEN)
	mv config.status.orig config.status
	for i in $(BINS_GEN) ; do \
		j=`echo $$i|sed -e $(CHANGE_GEN)` ; \
		cp -a systems/$(TARGET_GEN)/src/$$i $(top_srcdir)/bin/$$j ; \
		$(STRIP_GEN) $(top_srcdir)/bin/$$j ; \
	done


######################################################################
# distribution rules

dist-hook README:
	$(WPU) --set ACTION=readme --set SRCDIR=$(srcdir) --set PACK=s $(srcdir)/README.base README

.PHONY: pack s l d w m f

pack: dist l d w m f

l d w m f:
	$(WPU) --set ACTION=readme --set PACK=$@ README.base README
	$(WPU) --set ACTION=packaging --set PACK=$@

s: dist

l: linux html

d: go32 html

w: cygwin html

m: mingw html

f: linux go32 cygwin mingw html pdf


######################################################################
# publishing rules

.PHONY: upload

upload: pack html index.html indexpart.html
	$(WPU) --set ACTION=upload


######################################################################
# removing additionals

CLEANFILES=index.html indexpart.html

