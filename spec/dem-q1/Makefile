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
XSLTPROC=/usr/bin/xsltproc
XSLTDIR=/usr/share/xml/docbook/stylesheet/nwalsh/current

all: html pdf

# This rule converts (once) the old LinuxDoc input file into a DocBook file.
# The conversion goes as follows:
# - via Stylesheet from LinuxDoc to DocBook 3.0
# - with Perl from DocBook 3.0 to DocBook 4.2
# - add some (in HTML unknown) entities
# - normalize the SGML file with SGMLNorm.
$(PACKAGE).gen.in: $(PACKAGE).base
	$(JADE) \
		-t sgml \
		-c /usr/share/sgml/openjade/catalog \
		-c /usr/lib/sgml-tools/dtd/catalog \
		-d "/usr/share/sgml/docbook/dsssl-stylesheets/ld2db.dsl#db" \
		$< > temp1
	perl ../postprocess.pl 1 < temp1 > temp2
	$(SGMLNORM) temp2 >> temp3
	perl ../postprocess.pl 2 < temp3 > $@

$(PACKAGE).sgml.in: $(PACKAGE).gen.in
	echo '<!DOCTYPE ARTICLE PUBLIC "-//OASIS//DTD DocBook V4.2//EN" [' > $@
	cat $< >> $@

$(PACKAGE).xml.in: $(PACKAGE).gen.in
	echo '<?xml version="1.0" encoding="UTF-8"?>' > $@
	echo '<!DOCTYPE article PUBLIC "-//OASIS//DTD DocBook XML V4.2//EN"' >> $@
	echo '"http://www.oasis-open.org/docbook/xml/4.2/docbookx.dtd" [' >> $@
	cat $< >> $@

$(PACKAGE).sgml: $(PACKAGE).sgml.in
	$(WPU) --set ACTION=subst $< $@

$(PACKAGE).xml: $(PACKAGE).xml.in
	$(WPU) --set ACTION=subst $< $@

$(PACKAGE)xml.html: $(PACKAGE).xml
	$(XSLTPROC) --output $(PACKAGE)xml.html $(XSLTDIR)/html/docbook.xsl $<

$(PACKAGE)xml.pdf: $(PACKAGE).xml
	$(XSLTPROC) --output $(PACKAGE)xml.pdf $(XSLTDIR)/fo/docbook.xsl $<

html: $(PACKAGE).html

pdf: $(PACKAGE).pdf

$(PACKAGE).html: $(PACKAGE).sgml
	$(DOCBOOK2HTML) --nochunks $<

$(PACKAGE).pdf: $(PACKAGE).sgml
	$(DOCBOOK2PDF) $<

clean:
	$(RM_F) temp1 temp2 temp3
	$(RM_F) $(PACKAGE).sgml
	$(RM_F) $(PACKAGE).xml
	$(RM_F) $(PACKAGE).html
	$(RM_F) $(PACKAGE).pdf
	$(RM_F) *.tex *.aux *.log *.out

