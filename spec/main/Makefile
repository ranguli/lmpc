# $Id$

# demospecs -- the demo specification web site
# Copyright (C) 1996-2000,2004 Uwe Girlich
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
#  Makefile  -  makefile                                                     #
#****************************************************************************#

# wpu parse start /^([^=]+)=(.+)\s*$/
PACKAGE=main
TITLE=Main files for the Web page
VERSION=1.1.2
DATE=8/4/2000
# wpu parse end

OLDPARTS=lmpc-3.1.7 lmpc-3.1.9 lmpc-3.3.0
NEWPARTS=dem-q1 dm2-q2 dmo-dn faq lmp-d1 misc qwd-qw

######################################################################
# main targets

all: mainparts


######################################################################
# simple target rules


MAINFIXED=$(shell cat mainfixed.html)

mainbase:
	MAINFIXED=`cat mainfixed.html` $(WPU) --set ACTION=mainbase index.html


mainparts: mainbase
	for subdir in $(NEWPARTS); do \
	  ( cd ../$$subdir && make indexpart.html) ; \
	  $(WPU) --set ACTION=mainpart ../$$subdir/indexpart.html index.html ; \
	done


######################################################################
# clean rule

UPLOAD_FILES_dyn=index.html
UPLOAD_FILES_stat=FreePower2.1.gif anybrowser3.gif
UPLOAD_FILES_1=$(UPLOAD_FILES_dyn)
UPLOAD_DIR_1=.
UPLOAD_FILES_2=$(UPLOAD_FILES_stat)
UPLOAD_DIR_2=icons

clean::
	$(RM_F) $(UPLOAD_FILES_dyn)


include ../Make.spec.rules

