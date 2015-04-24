#! /usr/bin/bash
# Convert Fastq file into Fasta format
# Usage: ./fastq2fasta.sh [fastq file] 

sed '/^@/!d;s//>/;N' $1

