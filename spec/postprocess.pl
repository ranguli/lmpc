#!/usr/bin/perl -w

# $Id$

# Postprocess a Docbook file created by a LinuxDoc to DocBook stylesheet.

use strict;

$/='';
my $text = '';
while (<>) { $text .= $_; }

$text =~ s,-//Davenport//DTD DocBook V3.0//EN,-//OASIS//DTD DocBook V4.2//EN,;
$text =~ s,ArtHeader,ArticleInfo,g;
$text =~ s,&#38;,&amp;,g;
$text =~ s,&#60;,&lt;,g;
$text =~ s,&#62;,&gt;,g;
$text =~ s,&#65533;,,g;
$text =~ s:<FirstName>\@MYNAME@, </FirstName>:<FIRSTNAME>\@MYFIRSTNAME@</FIRSTNAME><SURNAME>\@MYSURNAME@</SURNAME>:;
$text =~ s:</AuthorBlurb>:</AuthorBlurb><affiliation><address><email>\@MYEMAIL@</email></address></affiliation>:;
$text =~ s:<AuthorBlurb>.*</AuthorBlurb>::s;

print $text;

