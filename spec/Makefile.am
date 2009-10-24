# $Id$

# demo file specifications
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
# Uwe Girlich (uwe@half-empty.de)

#****************************************************************************#
# Makefile.am - general automake file for the whole demospecs site           #
#****************************************************************************#


SUBDIRS=dem-q1 dmo-dn dm2-q2 faq icons lmp-d1 main qwd-qw misc 

EXTRA_DIST=spec.dsl

include $(top_srcdir)/spec/Make.spec.rules

uploadlocalall:
	for i in $(SUBDIRS) ; do \
		$(MAKE) -C $$i uploadlocal ; \
	done

sitecopyrc: sitecopyrc.in
	$(WPU) --set ACTION=subst $< $@
	chmod 600 $@

upload: sitecopyrc
	$(WPU) --set ACTION=execsubst -- \
		mkdir -p -m 700 @STOREPATH@
	$(WPU) --set ACTION=execsubst -- \
		$(SITECOPY) -r sitecopyrc \
		-p @STOREPATH@ \
		--update half-empty

uploadcheck: sitecopyrc
	$(WPU) --set ACTION=execsubst -- \
		$(SITECOPY) -r sitecopyrc \
		-p @STOREPATH@ \
		half-empty

