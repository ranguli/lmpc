package BitField;

use strict;
use Carp;

my @bitmask_q3 = (0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80);
my @bitmask_hl_old = (0x10, 0x20, 0x40, 0x80, 0x01, 0x02, 0x04, 0x08);
my @bitmask_hl_new = @bitmask_q3;

my %fields = (
	bit => 0,
	buffer => "",
	bitmask => \@bitmask_hl_new,
);

sub new {
	my $that = shift;
	my $class = ref($that) || $that;
	my $self = {
		%fields,
	};
	bless $self, $class;
	return $self;
}                                                                               

sub set_buffer ($) {
	my $self = shift;
	my ($buffer) = @_;

	$self->{"bit"} = 0;
	$self->{"buffer"} = $buffer;
}

sub get_buffer () {
	my $self = shift;
	return $self->{"buffer"};
}

sub set_bitmask ($) {
	my $self = shift;
	my (@bitmask) = @_;

	$self->{"bitmask"} = \@bitmask;
}

sub read_bit () {
	my $self = shift;
	my $bit = $self->{"bit"};
	my $firstbyte = 0;
	my $buffer = $self->{"buffer"};
	my $bitmask = $self->{"bitmask"};

	# print STDERR ">>>>bit=$bit<<<<<\n";

	if ($bit == 8) {
		$bit = 0;
	}

	if ($bit == 0) {
		($firstbyte, $buffer) = unpack "C a*", $buffer;
		$self->{"firstbyte"} = $firstbyte;
		$self->{"buffer"} = $buffer;
	}
	else {
		$firstbyte = $self->{"firstbyte"};
	}

	my $value = ($bitmask->[$bit] & $firstbyte);

	$bit++;
	$self->{"bit"} = $bit;

	return $value;
}

sub read_bits($) {
	my $self = shift;
	my ($bits) = @_;
	my $value = 0;
	my $mask = 1;
	for (my $bit = 0; $bit<$bits ; $bit++ ) {
		if ($self->read_bit()) {
			$value |= $mask;
		}
		$mask <<=1 ;
	}
	return $value;
}

sub read_bits_unsignedo ($) {
	my $self = shift;
	return $self->read_bits(@_);
}

sub read_bits_signed ($) {
	my $self = shift;
	my ($bits) = @_;
	my $result = $self->read_bits($bits);

	if ($result && 1<<($bits-1)) {
		# sign extension necessary
		$result |= 0xffffffff << $bits;
	}
	return $result;
}

1;

