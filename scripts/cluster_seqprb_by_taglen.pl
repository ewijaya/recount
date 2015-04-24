#!/usr/bin/perl -w
#===============================================================================
#
#
#===============================================================================
use strict;
use Data::Dumper;
use Carp;
use File::Basename;

my $file = $ARGV[0] || "temp_seq-prb.txt";
my $base = basename($file,".txt");

my	$INFILE_file_name = $file;		# input file name

open ( INFILE, '<', $INFILE_file_name )
	or croak "$0 : failed to open input file $INFILE_file_name : $!\n";

my %hash;

while ( <INFILE> ) {
    chomp;
    my $line = $_;
    my ($tag,$quals) = split(/\s+/,$line);
    my $tlen = length($tag);

    push @{$hash{$tlen}}, $line;

}               

close ( INFILE );			# close input file


foreach my $taglen ( sort keys %hash  ) {
    
    my @mems = sort @{$hash{$taglen}};
    my $nn = $base."_len".$taglen.".txt";



    my	$OUTFILE_file_name = $nn;		# output file name

    print "$nn\n";
    open ( OUTFILE, '>', $OUTFILE_file_name )
        or croak "$0 : failed to open output file $OUTFILE_file_name : $!\n";

    print OUTFILE join("\n", @mems),"\n";

    close ( OUTFILE );			# close output file

    print "\n";


}               

