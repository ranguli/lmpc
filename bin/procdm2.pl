#!/usr/bin/perl -w
# $Id$
# Uwe Girlich (uwe@planetquake.com)
# DM2 Processor


# You need Perl from http://www.perl.org.


# modules init
use strict;
use Getopt::Long qw(:config no_ignore_case);
use Pod::Usage;
use Pod::Text;


# prototypes
sub logging;
sub warning;


# release information
sub release() { (my $release = q$Revision$) =~ s/^[^:]+:\s*(.*?)\s*$/$1/; $release; }
sub date() { (my $date = q$Date$) =~ s/^[^:]+:\s*(.*?)\s*$/$1/; $date; }
sub comment() { "alpha quality"; }


# default option values
my $opt_input = "input.dm2";	# default input file name
my $opt_output = "output.dm2";	# default output file name
my $opt_command = "command.proc";	# default processing command file
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


__END__

=head1 NAME

procdm2 - DM2 Processor

=head1 SYNOPSIS

procdm2.pl [options]

 Options:
  -i|--input file       input file name (default: input.dm2).
  -o|--output file      output file name (default: output.dm2).
  -c|--command file     command file name (default: command.proc).
  -V|--version          print version.
  -h|-?|--help          brief help message.
  -m|--man              full documentation.
  -l|--logging level    logging verbose level (default: 0, off).
  -w|--warning level    warning verbose level (default: 0, off).

=head1 OPTIONS

=over 8

=item B<--input file>

Defines the input file name. Default: input.dm2.

=item B<--output file>

Defines the output file name. Default: output.dm2.

=item B<--command file>

Defines the processing command file name. Default: command.proc.

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

