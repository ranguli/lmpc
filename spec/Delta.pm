package Delta;

use strict;
use Carp;

sub release() {
	(my $release = q$Revision$) =~ s/^[^:]+:\s*(.*?)\s*$/$1/;
	return $release;
}

sub date() {
	(my $date = q$Date$) =~ s/^[^:]+:\s*(.*?)\s*$/$1/;        
	return $date;
}

sub syntax($) {my($text)=@_; croak "syntax error in line $.: $text\n";}
sub I($) {"\t" x $_[0]; }

sub new {
	my $that = shift;
	my $class = ref($that) || $that;
	my $self = {};
	bless $self, $class;
	return $self;
}

sub readfile {
	my $self = shift;
	my ($deltafilename) = @_;
	
	open DELTA, "$deltafilename" or die "can't read $deltafilename: $!\n";

	my $state = 0;

	my @types=();
	my $typeref = undef;
	my @deltas = ();

	while (<DELTA>) {
		my $line = $_;

		$line =~ s|//.*||;	# remove comments
		$line =~ s|^\s*||;	# remove leading spaces
		$line =~ s|\s*$||;	# remove trailing spaces
		next if $line =~ /^$/;

#		print "$state $line\n";

		if ($state == 0) {
			my ($typename, $dllname, $functionname) = split ' ', $line;
			$typeref = undef;
			$typeref->{"typename"} = $typename;
			$typeref->{"typename"} = $typename;
			$typeref->{"dllname"} = $dllname;
			if ($dllname ne "none") {
				$typeref->{"functionname"} = $functionname;
			}
			@deltas=();
			$state = 1;
			next;
		} # end state == 0

		if ($state == 1) {
			if ($line ne "{") {
				syntax("{ expected");
			}
			$state = 2;
			next;
		} # end state == 1

		if ($state == 2) {

			if ($line eq "}") {
				# end this block
				my @newdeltas = @deltas;
				my $newdeltasref = \@newdeltas;
				$typeref->{"deltasref"} = $newdeltasref;
#print STDERR "deltas = $newdeltasref\n";
				push @types, $typeref;
				$state = 0;
				next;
			}

			if ($line =~ /^DEFINE_DELTA\s*\(/) {
				if ($line !~ /^DEFINE_DELTA\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\),?$/) {
					syntax "problem in DEFINE_DELTA expression";
				}
				my ($name, $flags, $bits, $pre) = ($1,$2,$3,$4);

				# split flags
				$flags =~ s/\s*//g;
				my @flags = split '\|', $flags;
				my $flagsref = \@flags;

				my %delta = ();
				$delta{"name"} = $name;
				$delta{"flagsref"} = $flagsref;
				$delta{"bits"} = $bits;
				$delta{"pre"} = $pre;

				my $deltaref = {
					"name" => $name,
					"flagsref" => $flagsref,
					"bits" => $bits,
					"pre" => $pre,
				};

				push @deltas, $deltaref;
#				print STDERR "Y";

				next;
			}

			if ($line =~ /^DEFINE_DELTA_POST\s*\(/) {
				if ($line !~ /^DEFINE_DELTA_POST\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^,]+?)\s*\),?$/) {
					syntax "problem in DEFINE_DELTA_POST expression";
				}
				my($name,$flags,$bits,$pre,$post)=($1,$2,$3,$4,$5);
				# split flags
				$flags =~ s/\s*//g;
				my @flags = split '\|', $flags;
				my $flagsref = \@flags;

				my %delta = ();
				$delta{"name"} = $name;
				$delta{"flagsref"} = $flagsref;
				$delta{"bits"} = $bits;
				$delta{"pre"} = $pre;
				$delta{"post"} = $post;

				my $deltaref = \%delta;
				push @deltas, $deltaref;

				next;
			}

			syntax "unknown command";

		} # end state == 2
	} # end while

	my $typesref = \@types;

	$self->{"typesref"} = $typesref;

	close DELTA;
}


sub writefilexml {
	my $self = shift;

	my ($xmlfilename) = @_;

	open XML, ">$xmlfilename" || die "can't write $xmlfilename: $!\n";
	print XML I(0) . "<?xml version=\"1.0\"?>\n";
	print XML I(0) . "<delta_description>\n";

	my $typesref = $self->{"typesref"};

	my @types = @$typesref;

	for my $typeref (@types) {
		my %type = %$typeref;

		my $typename = $type{"typename"};
		my $dllname = $type{"dllname"};
		my $functionname;
		if ($dllname ne "none") {
			$functionname = $type{"functionname"};
		}
		
		print XML I(1) . "<type>\n";
		print XML I(2) . "<name>$typename</name>\n";
		if ($dllname ne "none") {
			print XML I(2) . "<callback>\n";
			print XML I(3) . "<dll>$dllname</dll>\n";
			print XML I(3) . "<function>$functionname</function>\n";
			print XML I(2) . "</callback>\n";
		}

		my $deltasref = $type{"deltasref"};

#		print STDERR "\ndeltasref=$deltasref\n";

		my @deltas = @$deltasref;
#		print STDERR "lengh=", scalar @deltas, "\n";

		foreach my $deltaref (@deltas) {
#			print STDERR "ref=", ref $deltasref, " value=$deltasref\n";
			my %delta = %$deltaref;

			my $name = $delta{"name"};
			my $flagsref = $delta{"flagsref"};
			my $bits = $delta{"bits"};
			my $pre = $delta{"pre"};
			my $post = undef;
			if (exists $delta{"post"}) {
				$post = $delta{"post"};
			}
			my @flags = @$flagsref;
		
			# output XML
			print XML I(2) . "<define_delta>\n";
			print XML I(3) . "<name>$name</name>\n";
			print XML I(3) . "<flags>\n";
			for (@flags) {
				print XML I(4) . "<flag>$_</flag>\n";
			}
			print XML I(3) . "</flags>\n";
			print XML I(3) . "<bits>$bits</bits>\n";
			print XML I(3) . "<pre-multiply>$pre</pre-multiply>\n";
			if ($post) {
				print XML I(3) . "<post-multiply>$post</post-multiply>\n";
			}
			print XML I(2) . "</define_delta>\n";
		}

		print XML I(1) . "</type>\n";
	}

	print XML I(0) . "</delta_description>\n";
	close XML;
}

1;

