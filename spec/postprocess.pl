#!/usr/bin/perl -w

# $Id$

# demo file specifications
# Copyright (C) 2004 Uwe Girlich
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

# Different postprocessing tasks on the way from a plain DocBook 3.0 file
# to a usable DocBook 4.2 file.

use strict;

my $step = shift;

$/='';
my $text = '';
while (<>) { $text .= $_; }


if ($step == "0") {
	$text =~ s,(<sect1>[^<]+)(<p>)(<label[^>]+>),$1$3$2,gi;
}

if ($step == "1") {
	$text =~ s,-//Davenport//DTD DocBook V3.0//EN,-//OASIS//DTD DocBook V4.2//EN,;
	$text =~ s,ArtHeader,ArticleInfo,g;
}

if ($step == "2") {

	$text =~ s,&#38;,&amp;,g;
	$text =~ s,&#60;,&lt;,g;
	$text =~ s,&#62;,&gt;,g;
	$text =~ s,&#65533;,,g;
	$text =~ s:<FirstName>\@MYNAME@, </FirstName>:<FirstName>\@MYFIRSTNAME@</FirstName><SurName>\@MYSURNAME@</SurName>:i;
	$text =~ s:(</AuthorBlurb>):$1<Affiliation><Address><Email>\@MYEMAIL@</Email></Address></Affiliation>:isg;
	$text =~ s:<AuthorBlurb>.*</AuthorBlurb>::si;

	if (0) {
		# make the article to a book
		$text =~ s:(</?)sect1:$1chapter:gi;
		$text =~ s:(</?)sect2:$1sect1:gi;
		$text =~ s:(</?)sect3:$1sect2:gi;
		$text =~ s:(</?)sect4:$1sect3:gi;
		$text =~ s:(</?)sect5:$1sect4:gi;
		$text =~ s:(</?)ArticleInfo:$1BookInfo:gi;
		$text =~ s:(</?)Article:$1Book:gi;
	}
	
	# We need everything as lowercase.
	# SGML does not care and XML needs it so.
	$text =~ s:(<[^> ]+>):lc($1):ge;

	print '<!ENTITY lowbar "_">
	<!ENTITY lsqb "[">
	<!ENTITY rsqb "]">
	<!ENTITY num "#">
	<!ENTITY bsol "\">
	]>';

}

if ($step == "3") {
	$text =~ s:(<ColSpec[^>/]+)>:$1/>:gi;
	$text =~ s:(<XRef[^>/]+)>:$1/>:gi;
}

print $text;

