#!/usr/bin/perl -w

# $Id$

# Postprocess a Docbook file created by a LinuxDoc to DocBook stylesheet.

use strict;

my $step = shift;

$/='';
my $text = '';
while (<>) { $text .= $_; }

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
	$text =~ s:(</AuthorBlurb>):$1<Affiliation><Address><Email>\@MYEMAIL@</Email></Address></Affiliation>:i;
	$text =~ s:<AuthorBlurb>.*</AuthorBlurb>::si;
	$text =~ s:(<ColSpec[^>/]+)>:$1/>:gi;
	$text =~ s:(<XRef[^>/]+)>:$1/>:gi;
	# We need everything as lowercase.
	# SGML does not care and XML needs it so.
	$text =~ s:(<[^>]+>):lc($1):ge;

	print '<!ENTITY lowbar "_">
	<!ENTITY lsqb "[">
	<!ENTITY rsqb "]">
	<!ENTITY num "#">
	<!ENTITY bsol "\">
	]>';

}

print $text;


