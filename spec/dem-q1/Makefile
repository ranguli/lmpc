# $Id$

#   dem -- the unofficial DEM format description
#   Copyright (C) 1996-99,2004 Uwe Girlich
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#   Uwe Girlich (uwe@planetquake.com)

#****************************************************************************#
#  Makefile  -  makefile                                                     #
#****************************************************************************#


######################################################################
# general settings

# wpu parse start /^([^=]+)=(.+)\s*$/
PACKAGE=dem
TITLE=The unofficial DEM format description
VERSION=1.0.9
DATE=1/8/1999
# wpu parse end

DOCBOOK2HTML=/usr/bin/docbook2html
DOCBOOK2PDF=/usr/bin/docbook2pdf
SGMLNORM=/usr/bin/sgmlnorm
JADE=/usr/bin/jade
RM_F=/bin/rm -f
WPU=../../bin/wpu

all: html pdf

# This rule converts (once) the old LinuxDoc input file into a DocBook file.
# The conversion goes as follows:
# - via Stylesheet from LinuxDoc to DocBook 3.0
# - with Perl from DocBook 3.0 to DocBook 4.2
# - add some (in HTML unknown) entities
# - normalize the SGML file with SGMLNorm.
$(PACKAGE).sgml.in: $(PACKAGE).base
	$(JADE) \
		-t sgml \
		-c /usr/share/sgml/openjade/catalog \
		-c /usr/lib/sgml-tools/dtd/catalog \
		-d "/usr/share/sgml/docbook/dsssl-stylesheets/ld2db.dsl#db" \
		$< > temp1
	perl -p \
	-e "s,-//Davenport//DTD DocBook V3.0//EN,-//OASIS//DTD DocBook V4.2//EN,;" \
	-e "s,ArtHeader,ArticleInfo,g;" \
	-e 's,&#38;,&amp;,g;' \
	-e 's,&#60;,&lt;,g;' \
	-e 's,&#62;,&gt;,g;' \
	-e 's,&#65533;,,g;' \
	< temp1 > temp2
	echo '<!DOCTYPE ARTICLE PUBLIC "-//OASIS//DTD DocBook V4.2//EN" [' > $@
	echo '<!ENTITY lowbar "_">' >> $@
	echo '<!ENTITY lsqb "[">' >> $@
	echo '<!ENTITY rsqb "]">' >> $@
	echo '<!ENTITY num "#">' >> $@
	echo '<!ENTITY bsol "\">' >> $@
	echo ']>' >> $@
	$(SGMLNORM) temp2 >> $@

$(PACKAGE).sgml: $(PACKAGE).sgml.in
	$(WPU) --set ACTION=subst $< $@

html: $(PACKAGE).html

pdf: $(PACKAGE).pdf

$(PACKAGE).html: $(PACKAGE).sgml
	$(DOCBOOK2HTML) --nochunks $<

$(PACKAGE).pdf: $(PACKAGE).sgml
	$(DOCBOOK2PDF) $<

clean:
	$(RM_F) temp1 temp2
	$(RM_F) $(PACKAGE).sgml
	$(RM_F) $(PACKAGE).html
	$(RM_F) $(PACKAGE).pdf
	$(RM_F) *.tex *.aux *.log *.out

