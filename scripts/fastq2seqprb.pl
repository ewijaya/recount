#!/usr/bin/perl -w
#===============================================================================
#
# Convert Fastq to Seq PRB Format where Qual is Phred Score
# Usage perl fastq2seqprb.pl fasqfile.fastq
#===============================================================================
use strict;
use Data::Dumper;
use Carp;


my $file = $ARGV[0] || "test.fastq";


my	$INFILE_file_name = $file;		# input file name

open ( INFILE, '<', $INFILE_file_name )
	or croak "$0 : failed to open input file $INFILE_file_name : $!\n";

my $count = 0;
my $tag = "";
my $qualoct = "";

while (<INFILE>) {
    chomp;

    next if ( !$count++ );

    if ( $count == 2 ) {
        $tag = $_;

    }
    elsif ( $count == 4 ) {
        $qualoct = $_;

    }

    if ( $count == 4 ) {
        $count = 0;
        my @pqs = oct2phred($qualoct);
        print "$tag\t", join( " ", @pqs ), "\n";
    }

}


close ( INFILE );			# close input file


#--------------------------------------------------
# Subroutines 
#-------------------------------------------------- 
sub oct2phred {

    my	$qoct	= shift;

    my @arrs;

    foreach my $q ( split(//,$qoct)  ) {
        my $Q = ord($q) - 33;
        push @arrs, $Q;

    }               

    return @arrs;
}

