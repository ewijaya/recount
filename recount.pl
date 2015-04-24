#!/usr/bin/perl -w
#===============================================================================
# Wrapper for the Binaries
#
#===============================================================================
use strict;
use Data::Dumper;
use Carp;
use File::Basename;
use Cwd;

my $codepath = "./src";

my $file      = $ARGV[0] || "test5tags.txt";
my $mm        = $ARGV[1] || 1;

# This parameter only affect when mm = 2 
# This values means that RECOUNT will only consider those
# neighboring tags with error probability greater than 0.00262689
my $minBaseEr = 0.00262689; 


if (scalar @ARGV != 2) {
     die "perl recount.pl <preprocessed_input> <number_mismatch_to_neighbors>\n";
}

my $base      = basename( $file, ".txt" );
my $dir       = dirname($file)."/";
my $nb_file   = $dir . $base . ".nb";
my $prop_file = $dir . $base . ".prop";
my $nbq_file  = $dir . $base . ".nbq";
my $cwd       = getcwd;
#print STDERR "$dir $cwd\n";

my $fn    = $codepath ."/FindNeighboursWithQual";
my $gp    = $codepath ."/GenerateProportion";
my $em    = $codepath ."/EstimateTrueCount";

print STDERR "FindNeighboursWithQual";
system("$fn $file $mm $minBaseEr");

print STDERR "\nGenerateProportion";
system("$gp $file > $prop_file");

print STDERR "\nEstimateTrueCount\n";
system("$em $file");

print STDERR "\nDone\n";

unlink($nb_file);
unlink($prop_file);
unlink($nbq_file);
