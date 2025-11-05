#!/usr/bin/perl -w
#===============================================================================
# Backwards-compatible wrapper for RECOUNT
#
# This wrapper maintains the original Perl interface but calls the new
# Python implementation which provides better error handling and features.
#
# For the original legacy version, see: recount_legacy.pl
#===============================================================================
use strict;
use warnings;
use File::Basename;
use FindBin qw($RealBin);

# Check arguments
if (scalar @ARGV != 2) {
    die "Usage: perl recount.pl <preprocessed_input> <number_mismatch_to_neighbors>\n";
}

my $input_file = $ARGV[0];
my $num_mismatches = $ARGV[1];

# Path to Python wrapper
my $python_wrapper = "$RealBin/recount.py";

# Check if Python wrapper exists
unless (-f $python_wrapper) {
    die "ERROR: Python wrapper not found at: $python_wrapper\n";
}

# Call the Python wrapper with the same arguments
# This maintains backwards compatibility while using the new implementation
my $cmd = "python3 $python_wrapper $input_file $num_mismatches";

print STDERR "Running RECOUNT pipeline via Python wrapper...\n";
print STDERR "Command: $cmd\n\n";

# Execute and preserve exit code
system($cmd);
my $exit_code = $? >> 8;

exit($exit_code);
