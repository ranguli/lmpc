#!/usr/bin/perl -w

# -------------
# DM2 Processor
# -------------

# With this Perl script you can manipulate frames and skins in ranges of
# blocks in DM2 files. It works on DM2 text files and (if LMPC is available),
# also on DM2 binary files.

# Get dm2proc (without HTML decorations) from
# http://www.planetquake.com/demospecs/misc/dm2proc.pl
# or from the CVS repository
# http://cvs.sourceforge.net/viewcvs.py/lmpc/lmpc/bin/procdm2.pl.

# $Id$
# Uwe Girlich (uwe@planetquake.com)


# modules init
use strict;
use Getopt::Long qw(:config no_ignore_case);
use IO::File;
use Pod::Usage;
use Pod::Text;
use IPC::Open3;
use File::Spec;


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

if ($opt_help) {
	pod2usage(-verbose=>0);
	warning 10, "podusage() for short help should not return.\n";
	exit(1);
}

if ($opt_man) {
	pod2usage(-verbose=>2);
	warning 10, "podusage() for long help should not return.\n";
	exit(1);
}

if ($opt_version) {
	printf "DM2 Processor %s, %s (%s)\n", release(), date(), comment();
	exit(1);
}

my $command = command_parse($opt_command);
if (not ref $command) {
	warning 0, "Problem parsing command file $opt_command.\n";
	exit(1);
}
logging 10, "parsing command file $opt_command completed.\n";

my $prepost = 0;
my $lmpc_ok = 0;
if (-f $opt_lmpc && -x $opt_lmpc) {
	logging 30, "$opt_lmpc is an executable.\n";
	# but maybe it is without directory and not in the PATH
	my ($volume,$directories,$file) = File::Spec->splitpath($opt_lmpc);
	if ($volume eq "" && $directories eq "") {
		# single filename, must be in PATH or not callable
		my @path = File::Spec->path();
		foreach (@path) {
			my $abs_path = File::Spec->catfile($_,$opt_lmpc);
			if (-f $abs_path && -x $abs_path) {
				logging 20, "$opt_lmpc was found in PATH env.\n";
				$lmpc_ok = 1;
			}
		}
		if ($lmpc_ok == 0) {
			logging 20, "$opt_lmpc was not found in PATH env.\n";

		}
	}
	else {
		if (File::Spec->file_name_is_absolute($opt_lmpc)) {
			logging 20, "$opt_lmpc is absolute path.\n";
			$lmpc_ok = 1;
		}
		else {
			logging 20, "$opt_lmpc is relative path.\n";
			$lmpc_ok = 1;
		}
	}
}
else {
	my ($volume,$directories,$file) = File::Spec->splitpath($opt_lmpc);
	if ($volume eq "" && $directories eq "") {
		my @path = File::Spec->path();
		foreach (@path) {
			my $abs_path = File::Spec->catfile($_,$opt_lmpc);
			if (-f $abs_path && -x $abs_path) {
				logging 20, "$opt_lmpc was found in PATH env.\n";
				$lmpc_ok = 1;
			}
		}
	}
	else {
		logging 20, "$opt_lmpc does not exist.\n";
		$lmpc_ok = 0;
	}
}


if ($lmpc_ok == 1) {
	logging 20, "The command $opt_lmpc is available.\n";
	my $info = "$opt_lmpc --info $opt_input";
	logging 30, "checking with '$info', if input is text or binary\n";
	my $info_fh;
	my $pid = open3("<&STDIN", $info_fh, $info_fh, $info);
	if ($pid==0) {
		warning 0, "Could not execute ´$info´: $!.\n";
		exit(1);
	}
	logging 35, "started child with pid $pid.\n";
	if (!defined $info_fh) {
		warning 0, "Could not read output from  '$info': $!.\n";
		waitpid $pid, 0;
		exit(1);
	}
	logging 40, "reading output from '$info'\n";
	while (<$info_fh>) {
		if (/$opt_input.*DM2 txt/) {
			$prepost = 0;
			logging 35, "$opt_input is really text\n";
			last;
		}
		if (/$opt_input.*DM2 bin/) {
			logging 35, "$opt_input is really binary\n";
			$prepost = 1;
			last;
		}
	}
	$info_fh->close();
	logging 40, "output ended\n";
	waitpid $pid, 0;
}
else {
	warning 20, "The command $opt_lmpc is not available.\n";
	$prepost = 0;
}

my $text_in;
my $text_out;
if ($prepost) {
	logging 35, "$opt_input is assumed to contained binary\n";
	$text_in = "text_in_$$.txt";
	$text_out = "text_out_$$.txt";
}
else {
	logging 35, "$opt_input is assumed to contain text\n";
	$text_in = $opt_input;
	$text_out = $opt_output;
}

my $error = 0;

if ($prepost) {
	my $preproc = "$opt_lmpc --to-txt $opt_input $text_in";
	logging 20, "Calling '$preproc' to generate text file.\n";
	system $preproc;
	if (!-f $text_in) {
		warning 0, "LMPC ($opt_lmpc) did not create $text_in.\n";
		$error = 1;
		goto out;
	}
}

logging 0, "$text_in (DM2 txt) -> $text_out (DM2 txt)\n";

logging 30, "Reading $text_in.\n";
my $in_fh = new IO::File "<$text_in";
if (!defined $in_fh) {
	warning 0, "Could not open $text_in for reading: $!.\n";
	if (!$prepost) {
		warning 0, "Please use '-i inputfile'.\n";
	}
	$error = 1;
	goto out;
}

logging 30, "Writing $text_out.\n";
my $out_fh = new IO::File ">$text_out";
if (!defined $out_fh) {
	warning 0, "Could not open $text_out for writing: $!.\n";
	$error = 1;
	goto out;
}

my $state = 0;
my $block = 0;
my $entity = 1;
my $frame = 0;
my $skin = 0;
my $new_frame = 0;
my $new_skin = 0;
my $subst_frame = 0;
my $subst_skin = 0;
my %entity = ();
while (<$in_fh>) {
	my $line = $_;
	# process the input line
	if ($line =~ /^  seq1 (\d+)/) {
		$block = $1;
		logging 30, "block number $block: ";
		if (!exists $command->{$block}) {
			logging 30, "ignore\n";
			$state = 0;
		}
		else {
			logging 30, "work\n";
			$state = 1;
			# Memorize all the entities where we have something
			# to do in this frame.
			%entity=();
			for (keys %{$command->{$block}}) {
				$entity{$_} = 1;
			}
		}
	}
	if ($state == 1) {
		if ($line =~ /packetentities {/) {
			logging 40, "block number $block packetentities start\n";
			$state = 2;
		}
	}
	if ($state == 2) {
		if ($line =~ /  packetentity {/) {
			logging 40, "block number $block packetentity start\n";
			$state = 3;
		}
		if ($line =~ /^ }/) {
			logging 40, "block number $block packetentites ended\n";
			for $entity (keys  %entity) {
				logging 40, "block $block, entity $entity missing\n";
				my $line_new = "  packetentity {\n   entity $entity;\n";
				if (exists $command->{$block}->{$entity}->{"f"}) {
					$frame = $command->{$block}->{$entity}->{"f"};
					logging 40, "adding frame $frame\n";
					$line_new .= "   frame $frame;\n";
				}
				if (exists $command->{$block}->{$entity}->{"s"}) {
					$skin = $command->{$block}->{$entity}->{"s"};
					logging 40, "adding skin $frame\n";
					$line_new .= "   skin $skin;\n";
				}
				$line = $line_new . "  }\n" . $line;
			}
			%entity=();
			$state = 0;
		}
	}
	if ($state == 3) {
		if ($line =~ /   entity (\d+);/) {
			$entity = $1;
			logging 30, "entity number $entity: ";
			if (!exists $command->{$block}->{$entity}) {
				logging 30, "ignore\n";
				$state = 2;
			}
			else {
				logging 30, "work\n";
				$state = 4;
				if (exists $command->{$block}->{$entity}->{"f"}) {
					$frame = $command->{$block}->{$entity}->{"f"};
					$new_frame = 1;
					$subst_frame = 0;
					logging 31, "frame -> $frame\n";
				}
				if (exists $command->{$block}->{$entity}->{"s"}) {
					$skin = $command->{$block}->{$entity}->{"s"};
					$new_skin = 1;
					$subst_skin = 0;
					logging 31, "skin -> $skin\n";
				}
			}
		}
	}
	if ($state == 4) {
		if ($new_frame &&
			$line =~ s/(   frame )\d+(;)/$1$frame$2/) {
			logging 32, "frame subst OK\n";
			$subst_frame = 1;
		}
		if ($new_skin &&
			$line =~ s/(   skin )\d+(;)/$1$skin$2/) {
			logging 32, "skin subst OK\n";
			$subst_skin = 1;
		}
		if ($line =~ /  }/) {
			if ($new_frame && !$subst_frame) {
					$line = "   frame $frame;\n" . $line;
					logging 32, "frame append OK\n";
			}
			if ($new_skin && !$subst_skin) {
					$line = "   skin $skin;\n" . $line;
					logging 32, "skin append OK\n";
			}
			logging 40, "packetentity ended\n";
			$state = 2;
		}
		delete $entity{$entity}; # This is done.
	}
	
	print $out_fh $line;
}
$in_fh->close();
$out_fh->close();

if ($prepost) {
	my $postproc = "$opt_lmpc --to-bin $text_out $opt_output";
	logging 20, "Calling '$postproc' to generate binary file.\n";
	system $postproc;
}

out:

if ($prepost) {
	logging 20, "Remove temporary text files $text_in and $text_out.\n";
	unlink $text_in, $text_out;
}


# End of program.
exit $error;


sub command_parse($)
{
	my $command_filename = shift;
	my %command = ();
	logging 0, "Reading command file %s.\n", $command_filename;
	my $command_fh=new IO::File("<$command_filename");
	if (!defined $command_fh) {
		warning 0, "Could not open %s for reading: $!.
Please use '-c commandfile'.\n", $command_filename;
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
	return \%command;
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

=head1 PROCESSING COMMANDS

The processing command text file consists of lines with commands.
# starts a comment. Empty lines will be ignored.

=over 8

=item B<e entity>

This defines the number of the entity, for which the following commands
are valid. Defaults to 1.

=item B<f blockrange framerange>

This commands defines, that in the blocks in blockrange the frames from
framerange must be used.

=item B<s blockrange skinrange>

This commands defines, that in the blocks in blockrange the skins from
skinrange must be used.

=back

=head1 RANGES

A range consists of a starting number and optional a dash and an ending
number. Whitespaces and backward ordering (end smaller than begin) are
allowed.

=head1 SEE ALSO

L<http://www.planetquake.com/demospecs/lmpc-stable>
L<http://www.planetquake.com/demospecs/misc/procdm2.html>
L<http://www.sourceforge.net/projects/lmpc>

=head1 AUTHOR

Uwe Girlich (uwe@planetquake.com)

=cut

