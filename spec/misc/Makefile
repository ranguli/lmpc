# $Id$

# misc -- Miscellaneous demo related stuff
# Copyright (C) 1996-99,2004 Uwe Girlich
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Uwe Girlich (uwe@planetquake.com)

#****************************************************************************#
# Makefile  -  makefile                                                      #
#****************************************************************************#

# wpu parse start /^([^=]+)=(.+)\s*$/
PACKAGE=misc
TITLE=Miscellaneous demo related stuff
VERSION=1.0.8
DATE=7/10/2004
TYPE=misc
INCLUDE=index.template
# wpu parse end


######################################################################
# main targets

all: cd.txt cd.html ers ers.html mulder mulder.html procdm2.html \
		procdm2.pl qfont.txt qfont.html \
		win32.html index.html indexpart.html


######################################################################
# simple file changes

cd.txt: cd.txt.in Makefile
	$(WPU) --set ACTION=subst $< $@

cd.html: cd.txt Makefile
	$(WPU) --set ACTION=t2h $< $@

ers: ers.in Makefile
	$(WPU) --set ACTION=subst $< $@
	chmod a+x $@

ers.html: ers Makefile
	$(WPU) --set ACTION=t2h $< $@

mulder: mulder.in Makefile
	$(WPU) --set ACTION=subst $< $@
	chmod a+x $@

mulder.html: mulder Makefile
	$(WPU) --set ACTION=t2h $< $@

procdm2.pl:
	rm -f procdm2.pl
	ln -fs ../../bin/procdm2.pl procdm2.pl

procdm2.html: procdm2.pl Makefile
	$(WPU) --set ACTION=t2h $< $@

qfont.txt: qfont.txt.in Makefile
	$(WPU) --set ACTION=subst $< $@

qfont.html: qfont.txt Makefile
	$(WPU) --set ACTION=t2h $< $@

win32.html: win32.html.in Makefile
	$(WPU) --set ACTION=subst $< $@


######################################################################
# clean rule

UPLOAD_FILES_dyn=index.html cd.txt cd.html ers ers.html mulder mulder.html \
		procdm2.html procdm2.pl qfont.txt qfont.html win32.html
UPLOAD_FILES_stat=cd.dem cygwin.dll cygwinb19.dll cygwin1.dll \
		qfont.dem qfont.pcx
UPLOAD_FILES_1=$(UPLOAD_FILES_dyn) $(UPLOAD_FILES_stat)

clean::
	$(RM_F) $(UPLOAD_FILES_dyn)


######################################################################
# include generic rules

include ../Make.spec.rules

