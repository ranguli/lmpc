#!/usr/bin/perl -w

use strict;
use IO::Seekable;
use IO::File;

sub read_with_check($$);
sub LittleFloat($);


(my $release = q$Revision$) =~ s/^[^:]+:\s*(.*?)\s*$/$1/;
(my $date = q$Date$) =~ s/^[^:]+:\s*(.*?)\s*$/$1/;

if (@ARGV != 2) {
	print "Half-Life demo analyser\n";
	print "DEM-HL (c) Uwe Girlich, Release $release, $date\n";
	print "dem-hl.pl dembin demtxt\n";
	exit;
}

my ($name_in, $name_out) = @ARGV;

my $file_in = new IO::File "$name_in", "r";
die "could not open $name_in for reading: $!" unless $file_in;
binmode $file_in;

my $file_out = new IO::File "$name_out", "w";
die "could not open $name_out for writing: $!" unless $file_out;
binmode $file_out;

printf "%s (DEM bin HL) -> %s (DEM text HL)\n",
	$name_in, $name_out;

# file header #################################################################

$file_in->seek(0, SEEK_SET);
my $header_d = read_with_check($file_in, 544);
my ($magick, $demo_version, $network_version, $map_name, $game_dll, $dir_offset) = 
	unpack
	"Z8 V V Z260 Z264 V",
	$header_d;

printf $file_out "// source: binary Half-Life demo %s\n\n", $name_in;
printf $file_out "game \"Half-Life\";\n";
printf $file_out "header {\n";
printf $file_out " magick \"%s\";\n", $magick;
printf $file_out " demo_version %i;\n", $demo_version;
printf $file_out " network_version %i;\n", $network_version;
printf $file_out " map_name \"%s\";\n", $map_name;
printf $file_out " game_dll \"%s\";\n", $game_dll;
printf $file_out " dir_offset %i; // 0x%08x\n", $dir_offset, $dir_offset;
printf $file_out "}\n";

# end of file header ##########################################################


# segment directory ###########################################################


## number of segment directory entries ########################################

$file_in->seek($dir_offset,SEEK_SET);
my $dir_entries_d = read_with_check($file_in, 4);
my ($dir_entries) = unpack 
	"V",
	$dir_entries_d;

printf $file_out "directory {\n";
printf $file_out " entries %i;\n", $dir_entries;

## end of number of segment directory entries #################################

## segment directory entries ##################################################

my @directory;
for (my $i = 0 ; $i < $dir_entries ; $i++ ) {
	my $dir_entry_d = read_with_check($file_in, 92);
	my %segment;
	(
		$segment{"number"},
		$segment{"title"},
		$segment{"flags"},
		$segment{"play"},
		$segment{"time"},
		$segment{"frames"},
		$segment{"offset"},
		$segment{"length"}
	) = unpack
		"V Z64 V V f V V V",
		$dir_entry_d;
	$segment{"time"} = LittleFloat($segment{"time"});

	$segment{"end"} = $segment{"offset"} + $segment{"length"} - 1;

	push @directory, \%segment;

	printf $file_out " entry {\n";
	printf $file_out "  number %i;\n", $segment{"number"};
	printf $file_out "  title \"%s\";\n", $segment{"title"};
	printf $file_out "  flags {\n";
	printf $file_out "   value 0x%08x;\n", $segment{"flags"};
	printf $file_out "   bit_title; // 0x01\n"		if $segment{"flags"} & 0x01;
	printf $file_out "   bit_play; // 0x04\n"		if $segment{"flags"} & 0x04;
	printf $file_out "   bit_fade_in_slow; // 0x08\n"	if $segment{"flags"} & 0x08;
	printf $file_out "   bit_fade_in_fast; // 0x10\n"	if $segment{"flags"} & 0x10;
	printf $file_out "   bit_fade_out_slow; // 0x20\n"	if $segment{"flags"} & 0x20;
	printf $file_out "   bit_fade_out_fast; // 0x40\n"	if $segment{"flags"} & 0x40;
	printf $file_out "  }\n";
	printf $file_out "  play %i;\n", $segment{"play"};
	printf $file_out "  time %fs;\n", $segment{"time"};
	printf $file_out "  frames %i;\n", $segment{"frames"};
	printf $file_out "  offset %i; // 0x%08x\n", $segment{"offset"}, $segment{"offset"};
	printf $file_out "  length %i; // 0x%08x\n", $segment{"length"}, $segment{"length"};
	printf $file_out "  end %i; // 0x%08x\n", $segment{"end"}, $segment{"end"};
	printf $file_out " }\n";
}

# end of segment directory entries ############################################

my $dir_end = $file_in->tell - 1;
printf $file_out " dir_end %i; // %08x\n", $dir_end, $dir_end;
printf $file_out "}\n";

# end of segment directory ####################################################


# go over the segments described in the directory

printf $file_out "segments {\n";

for (@directory) {
	my %segment = %{$_};
	printf $file_out " segment {\n";
	printf $file_out "  number %i\n", $segment{"number"};
	$file_in->seek($segment{"offset"},SEEK_SET);
	for ( ;; ) {
		my %macro;
		$macro{"file_offset"} = $file_in->tell;
		my $macro_d = read_with_check($file_in, 9);
		(
			$macro{"type"},
			$macro{"time"},
			$macro{"frame"}
		) = unpack "C f V", $macro_d;
		$macro{"time"} = LittleFloat($macro{"time"});

		printf $file_out "  macro { // offset=0x%08x\n", $macro{"file_offset"};
		printf $file_out "   type %i;\n", $macro{"type"};
		printf $file_out "   time %fs;\n", $macro{"time"};
		printf $file_out "   frame %i;\n", $macro{"frame"};
		# macro block 1 ###############################################
		if ($macro{"type"} == 1) {
			printf $file_out "   ukdata // offset=0x%08x, len=%d\n",
				$file_in->tell(), 560;
			my $data_1h_d = read_with_check($file_in,560);
			# leave it for later

			my $data_1_len_d = read_with_check($file_in,4);
			(
				$macro{"length"}
			) = unpack "V", $data_1_len_d;
			printf $file_out "   length %i;\n", $macro{"length"};
			printf $file_out "   gamedata";
			my $data_1_d = read_with_check($file_in,$macro{"length"});
			my @data = unpack "C" . $macro{"length"} , $data_1_d;
			for (@data) { printf $file_out " %02x", $_; }
			printf $file_out ";\n";
		}
		# macro block 3 ###############################################
		if ($macro{"type"} == 3) {
			my $data_3_d = read_with_check($file_in,64);
			(
				$macro{"text"}
			) = unpack "Z64", $data_3_d;
			printf $file_out "   text \"%s\";\n", $macro{"text"};
		}
		# macro block 4 ###############################################
		if ($macro{"type"} == 4) {
			my $data_4_d = read_with_check($file_in,32);
			my @data = unpack "C32", $data_4_d;
			printf $file_out "   data";
			for (@data) { printf $file_out " %02x", $_; }
			printf $file_out ";\n";
		}
		# macro block 9 ###############################################
		if ($macro{"type"} == 9) {
			my $data_9_len_d = read_with_check($file_in,4);
			(
				$macro{"length"}
			) = unpack "V", $data_9_len_d;
			my $data_9_d = read_with_check($file_in,$macro{"length"});
			my @data = unpack "C" . $macro{"length"} , $data_9_d;
			printf $file_out "   length %i\n", $macro{"length"};
			printf $file_out "   data";
			for (@data) { printf $file_out " %02x", $_; }
			printf $file_out ";\n";
		}
		#
		printf $file_out "  }\n";
		last if (
			$macro{"type"} != 1 &&
			$macro{"type"} != 2 &&
			$macro{"type"} != 3 &&
			$macro{"type"} != 4 &&
			$macro{"type"} != 9);
	}
	printf $file_out " }\n";
}

printf $file_out "}\n";

$file_in->close;
$file_out->close;


sub read_with_check($$)
{
        my ($infd, $length) = @_;
        my $data;
        my $res = $infd->read($data, $length);
        if ($res != $length) {
                if ($res < 0) {
                        die sprintf
			"could not read 0x%x bytes at 0x%x: $!\n",
                        $length, tell($infd);
                }
                else {
                        die sprintf
			"tried to read 0x%x bytes, got only 0x%x, pos is now 0x%x\n",
			$length, $res, tell($infd);
                }
        }
        return $data;
}


sub LittleFloat($) {
	unpack("f",pack("I",unpack("V",pack("f",$_[0]))));
}
