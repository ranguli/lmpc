# $Id$

# LMPC
# Copyright (C) 1996-2009 Uwe Girlich
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
# Makefile.am - SourceForge upload organising automake file                  #
#****************************************************************************#


include $(top_srcdir)/spec/Make.spec.rules


TYPE=misc
SHORTVERSION=$(subst .,,@VERSION@)
UPLOAD_FILES_misc = \
	../@PACKAGE@-@VERSION@.tar.gz \
	../../@PACKAGE@-@VERSION@-l32.tar.gz \
	../../@PACKAGE@-@VERSION@-l.tar.gz \
	../../@PACKAGE@-@VERSION@-f.tar.gz \
	../../@PACKAGE@$(SHORTVERSION)d.zip \
	../../@PACKAGE@$(SHORTVERSION)m.zip \
	../../@PACKAGE@$(SHORTVERSION)w.zip
UPLOAD_DIR_misc=.
UPLOAD_STAGE_misc=@FPSTAGE@

upload:
	mkdir -p @PACKAGE@-@VERSION@
	cp -a $(UPLOAD_FILES_misc) @PACKAGE@-@VERSION@
	$(WPU) --set ACTION=execsubst -- \
	scp -r @PACKAGE@-@VERSION@ @SFUPSCP@/
	rm -rf @PACKAGE@-@VERSION@

