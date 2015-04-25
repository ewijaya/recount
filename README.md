# RECOUNT: Probabilistic Error Correction for Next Generation Sequencing Data

RECOUNT is a software for estimating the true count of Illumina reads
based on a probabilistic model. It uses the quality score provided by Illumina and the reads as its input. 
Typical application of this software is for transcriptome or 
metagenomic expression analysis.


## Requirement 
To handle around 20 million reads of length 34, using 1 Hamming distance 
neighbors you will need at least 10 gigabytes of disk space. To install
the software you need a C++ compiler.


## Installation 
Type `make` in the `/src` directory. It should make two groups of programs:

1. Components of RECOUNT: `FindNeighboursWithQual`, `GenerateProportion` and `EstimateTrueCount`.
2. Additional preprocessing programs: `AverageTagsQuals_27`, `AverageTagsQuals_36` and `PickBaseQual`


## Input format 
RECOUNT takes a pre-processed data as input. It looks like this:

```
700218	AAA     40	40	40	
25078	AAC     40	40	3	
25010	AAG     40	40	3	
25315	AAT     40	40	3	
25045	ACA     40	3	40
```

First column is the observed/actual count of a read, second colum is the read, 
and third column to the end is the *average* quality score of each bases 
in the corresponding read. 


## Usage 

You can run recount by executing the wrapper written in Perl.
The command is simply:


```
perl recount.pl [input] [no_of_neighbor_mismatch]
```

For example:

```
perl recount.pl test-data.txt 1
```

Maximum allowable number of mismatch is 2. Note that the running time
and space requirement of RECOUNT using mismatch 2 is quadratic compare
to using mismatch 1. For 2 mismatches option, you may reduce the 
space and running time requirement by setting the error error probability
bar under a variable in `recount.pl`.


## Additional data pre-processing softwares 
Illumina's pipeline generated two types of data: sequence (seq) and quality score
(prb). In this package we provide several types of  additional softwares. 

  a. `AverageTagsQuals_seq_prb` takes the input of the
following format:

```
AAA  40 -40 -40 -40     40 -40 -40 -40     40 -40 -40 -40
AAA  30 -40 -40 -40     10 -40 -40 -40     20 -40 -40 -40
AAA  20 -40 -40 -40     20 -40 -40 -40     40 -40 -40 -40
AAC  40 -40 -40 -40     40 -40 -40 -40     -40 -40 -40 40
```

then compute the average of the quality score.
Note that the above input must be sorted by the reads ascendingly.
And the output is:

```
3 AAA  30 -40 -40 -40     23.33 -40 -40 -40     33.33 -40 -40 -40
1 AAC  40 -40 -40 -40     40 -40 -40 -40     -40 -40 -40 40
```

The command is simply:
```
./AverageTagsQuals [sorted_seq_prb_file] 
```

  b. `PickBaseQual` takes the output of `AverageTagsQuals` above and  produce the following result:

```
3 AAA 30 23.33 33.33
1 AAC 40 40 -40
```
The command is simply:

```
./PickBaseQual [average_prb_file]
```

Sometimes the data does not come in PRB format, but in FASTQ
where the each base in the tag has only one corresponding quality score.
For that reason we also provide another version of `AverageTagsQuals_seq-prb`
which only average out based on single quality value of each base in the tag.

  c. `AverageTagsQuals` takes the input of the following format:

```
AAA  40 40 40 
AAA  30 10 20 
AAA  20 20 40 
AAC  40 40 40
```

We call this SEQ-QUAL format. And the output is:
```
3 AAA  30  23.33 33.33 
1 AAC  40  40 40
```

## Scripts 
We also provided two scripts for converting FASTQ to FASTA format and
FASTQ to SEQ-QUAL format.

```
  ./fastq2fasta.sh [fastq_file]
```

and

```
  perl fastq2seqprb.pl [fastq_file] 
```

## Clustering Reads By Length 

After the FASTAQ format has been converted to SEQ-PRB format
we need to separate the reads into several files according to its length.
This can be done with the given script.

```
perl cluster_seqprb_by_taglen.pl [seq-prb-file]
```

It will then create several files with name: [seq-prb-file-LEN] 

And next we need to create the average version of the reads

```
./AverageTagQuals_LEN [seq-prb-file-LEN]  > [seq-prb-file-LEN-avg]
```

Finally we can run Recount at the final output 

```
perl recount.pl [seq-prb-file-LEN-avg]
```

## Publication

_Edward Wijaya_, Martin C. Frith, Yutaka Suzuki, Paul Horton, *Recount: expectation maximization based error correction tool for next
generation sequencing data*, (2009) Genome Inform. 23(1):189-201.
[PMID:20180274](http://www.ncbi.nlm.nih.gov/pubmed/20180274).


## Credits 
RECOUNT is developed in C++ by Edward Wijaya in the 
Computational Biology Research Center (CBRC) - AIST.
The EM algorithm is based on (*Beissbarth. et.al 
Bioinformatics (20),  i31-39, 2007*). 


## Questions, Comments, Problems 
Email: ewijaya@gmail.com; ewijaya@ifrec.osaka-u.ac.jp or p-horton@aist.go.jp.
If reporting the problem, please describe exactly how to trigger
the problem.
