#!/usr/bin/perl -w
# $Id$
# Uwe Girlich (uwe@planetquake.com)
# DM2 Processor


# You need Perl from http://www.perl.org.


# modules init
use strict;
use Getopt::Long qw(:config no_ignore_case);
use IO::File;
use Pod::Usage;
use Pod::Text;


# prototypes
sub command_parse($);
sub logging;
sub warning;
sub syntaxerror($$$);
sub range_parse($);


# release information
sub release() { (my $release = q$Revision$) =~ s/^[^:]+:\s*(.*?)\s*$/$1/; $release; }
sub date() { (my $date = q$Date$) =~ s/^[^:]+:\s*(.*?)\s*$/$1/; $date; }
sub comment() { "alpha quality"; }


# default option values
my $opt_input = "input.dm2";	# default input file name
my $opt_output = "output.dm2";	# default output file name
my $opt_command = "command.proc";	# default processing command file
my $opt_lmpc = "lmpc";		# default lmpc command
my $opt_version = 0;		# version off
my $opt_help = 0;		# help off
my $opt_man = 0;		# manual off
my $opt_logging = 0;		# logging verbose level
my $opt_warning = 0;		# warning verbose level
# parse command line
GetOptions(
	'input|i=s'	=>	\$opt_input,
	'output|o=s'	=>	\$opt_output,
	'command|c=s'	=>	\$opt_command,
	'lmpc=s'	=>	\$opt_lmpc,
	'version|V'	=>	\$opt_version,
	'help|?'	=>	\$opt_help,
	'man|m'		=>	\$opt_man,
	'logging|l=i'	=>	\$opt_logging,
	'warning|w=i'	=>	\$opt_warning,
) or pod2usage(-msg=>"Syntax error", -verbose=>0);

pod2usage(-verbose=>0) if $opt_help;

pod2usage(-verbose=>2) if $opt_man;

if ($opt_version) {
	printf "DM2 Processor %s, %s (%s)\n", release(), date(), comment();
	exit(1);
}

my $command = command_parse($opt_command);
if (not ref $command) {
	warning 0, "Problem parsing command file $opt_command.\n";
	exit(1);
}

my $info = "$opt_lmpc --info $opt_input 2>&1";
my $info_fh = new IO::File "$info|";
if (!defined $info_fh) {
	warning 0, "Could not execute '$info': $!.\n";
	exit(1);
}
my $prepost = 1;
while (<$info_fh>) {
	if (/$opt_input.*DM2 txt/) {
		$prepost = 0;
		last;
	}
}

my $text_in;
my $text_out;
if ($prepost) {
	$text_in = "text_in_$$.txt";
	$text_out = "text_out_$$.txt";
}
else {
	$text_in = $opt_input;
	$text_out = $opt_output;
}

if ($prepost) {
	my $preproc = "$opt_lmpc --to-txt $opt_input $text_in";
	system $preproc;
}

logging 0, "$text_in (DM2 txt) -> $text_out (DM2 txt)\n";

logging 30, "Reading $text_in.\n";
my $in_fh = new IO::File "<$text_in";
if (!defined $in_fh) {
	warning 0, "Could not open $text_in for reading: $!\n";
	exit(1);
}

logging 30, "Writing $text_out.\n";
my $out_fh = new IO::File ">$text_out";
if (!defined $out_fh) {
	warning 0, "Could not open $text_out for writing: $!\n";
	exit(1);
}

while (<$in_fh>) {
	my $line = $_;
	# TODO: process the input line
	my $text = $line;
	print $out_fh $text;
}
$in_fh->close();
$out_fh->close();

if ($prepost) {
	my $postproc = "$opt_lmpc --to-bin $text_out $opt_output";
	system $postproc;
}


sub command_parse($)
{
	my $command_filename = shift;
	my %command = ();
	logging 30, "Reading %s.\n", $command_filename;
	my $command_fh=new IO::File("<$command_filename");
	if (!defined $command_fh) {
		warning 0, "Could not open %s for reading: $!.\n", $command_filename;
		return 0;
	}
	my $e=0;
	while (<$command_fh>) {
		my $line = $_;
		$line =~ s/#.*//;		# remove comment
		$line =~ s/^\s*//;		# remove leading whitespaces
		$line =~ s/\s*$//;		# remove trailing whitespaces
		$line =~ s/\s*-/-/g;		# remove whitespace before -
		$line =~ s/-\s*/-/g;		# remove whitespace after -
		next if $line =~ /^$/;		# ignore empty line
		my $letter;
		my $args;
		if ($line =~ /^([a-zA-Z])\s*(.*)$/) {
			$letter = lc $1;
			$args = $2;
		}
		else {
			syntaxerror $command_filename, $., "no command";
			return 0;
		}
		if ($letter eq "e") {
			if ($args =~ /^(\d+)$/) {
				$e = $1;
			}
			else {
				syntaxerror $command_filename, $., "$args is not a number";
				return 0;
			}
			logging 40, "entity $e\n";
		}
		elsif ($letter eq "f" or $letter eq "s") {
			my $blocks;
			my $frames;
			if ($args =~ /^([^\s]+)\s+([^\s]+)$/) {
				$blocks = $1;
				$frames = $2;
			}
			else {
				syntaxerror $command_filename, $., "command $letter: $args is not blockrange argsrange";
				return 0;
			}
			my ($bs,$be,$bi) = range_parse($blocks);
			my ($fs,$fe,$fi) = range_parse($frames);
			logging 50, "blocks $bs ... $be (inc $bi): $letter $fs ... $fe (inc $fi)\n";
			my $f = $fs;
			for (my $b=$bs;$bi>0?$b<=$be:$b>=$be;$b+=$bi) {
				$command{$b}{$e}{$letter}=$f;
				logging 60, "b $b e $e $letter $f\n";
				$f += $fi;
				if ($fi>0?$f>$fe:$f<$fe) {
					$f = $fs;
				}
			}
		}
		else {
			syntaxerror $command_filename, $., "unknown command";
			return 0;
		}
	}
	$command_fh->close();
	return \$command;
}


# trace
sub logging
{
		my $level = shift;
		if ($level<=$opt_logging) {
			printf STDERR @_;
		}
}


sub warning
{
	my $level = shift;
	if ($level<=$opt_warning) {
		printf STDERR @_;
	}
}


sub syntaxerror($$$)
{
	my ($command, $line, $message) = @_;
	warning 0, "Command file $command: syntax error in line $line.\n$message.\n";
}


sub range_parse($)
{
	my $text = shift;
	if ($text =~ /^([^-]+)-([^-]+)$/) {
		return $1, $2, $1<$2? 1 : -1;
	}
	else {
		return $text, $text, 1;
	}
}


__END__

=head1 NAME

procdm2 - DM2 Processor

=head1 SYNOPSIS

procdm2.pl [options]

 Options:
  -i|--input file       input file name (default: input.dm2).
  -o|--output file      output file name (default: output.dm2).
  -c|--command file     command file name (default: command.proc).
  --lmpc file		LMPC executable (default: lmpc)
  -V|--version          print version.
  -h|-?|--help          brief help message.
  -m|--man              full documentation.
  -l|--logging level    logging verbose level (default: 0, off).
  -w|--warning level    warning verbose level (default: 0, off).

=head1 OPTIONS

=over 8

=item B<--input file>

Defines the input file name. Default: input.dm2. The input file can be
a DM2 text file or a DM2 binary file. In case of a binary file, it will
be converted into a text file by calling LMPC --to-txt.

=item B<--output file>

Defines the output file name. Default: output.dm2. The output file has the
same type (binary or text) as the input file.

=item B<--command file>

Defines the processing command file name. Default: command.proc.

=item B<--lmpc file>

Defines the name of the LMPC command. Default: lmpc.

=item B<--version>

Prints the program version and exits.

=item B<--help>

Print a brief help message and exits.

=item B<--man>

Prints the manual page and exits.

=item B<--logging level>

Set the verbose level for logging trace output. Default: 0, off.

=item B<--warning level>

Set the verbose level for warning trace output. Default: 0, off.

=back

=head1 DESCRIPTION

This program will read the given input file, perform the actions defined in
the command file and write the output file.

=head1 SEE ALSO

L<http://www.planetquake.com/demospecs/lmpc-stable>
L<http://www.sourceforge.net/projects/lmpc>

=head1 AUTHOR

Uwe Girlich (uwe@planetquake.com)

=cut

