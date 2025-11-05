# RECOUNT: Probabilistic Error Correction for Next Generation Sequencing Data

RECOUNT is a software for estimating the true count of Illumina reads
based on a probabilistic model. It uses the quality score provided by Illumina and the reads as its input. 
Typical application of this software is for transcriptome or 
metagenomic expression analysis.


## Requirements
- **C++ compiler** (for compiling the core binaries)
- **Python 3.6+** (for the enhanced wrapper - optional but recommended)
- **Perl** (for legacy wrapper - optional)

To handle around 20 million reads of length 34, using 1 Hamming distance
neighbors you will need at least 10 gigabytes of disk space.


## Installation

1. Compile the C++ binaries:
   ```bash
   cd src/
   make
   ```

This will create two groups of programs:
- **Core RECOUNT components:** `FindNeighboursWithQual`, `GenerateProportion`, and `EstimateTrueCount`
- **Preprocessing tools:** `AverageTagsQuals_*`, `PickBaseQual`, etc.

2. The Python and Perl wrappers are ready to use immediately (no compilation needed)


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

### Python Wrapper (Recommended)

The enhanced Python wrapper provides better error handling, logging, and flexibility:

```bash
# Basic usage
python3 recount.py <input_file> <num_mismatches>

# Examples
python3 recount.py test-data.txt 1
python3 recount.py test-data.txt 2 --verbose
python3 recount.py data.txt 1 --keep-temp --min-base-error 0.001
python3 recount.py data.txt 2 --output-dir ./results
```

**Available options:**
- `--verbose, -v`: Enable detailed logging for debugging
- `--keep-temp`: Keep temporary files (.nb, .prop, .nbq) after completion
- `--min-base-error FLOAT`: Minimum base error probability (default: 0.00262689)
- `--src-path PATH`: Custom path to binaries directory (default: ./src)
- `--output-dir PATH`: Directory for output files (default: same as input)
- `--dry-run`: Show what would be executed without running
- `--help`: Show complete help message

### Perl Wrapper (Backwards Compatible)

The original Perl interface is maintained for backwards compatibility:

```bash
perl recount.pl <input_file> <num_mismatches>

# Example
perl recount.pl test-data.txt 1
```

**Note:** The Perl wrapper now calls the Python implementation internally,
so you get all the improvements while maintaining backwards compatibility.

### Important Notes

- Maximum allowable number of mismatches is **2**
- Running time and space requirement for mismatch=2 is **quadratic** compared to mismatch=1
- For mismatch=2, you can adjust the `--min-base-error` parameter to reduce space/time requirements
- The default `min-base-error` of 0.00262689 means RECOUNT only considers neighboring tags
  with error probability greater than this threshold


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

## Improvements in Version 2.0

The new Python wrapper (`recount.py`) provides significant improvements over the original Perl implementation:

### Enhanced Features
- ✅ **Comprehensive error handling** - Clear error messages with specific exit codes
- ✅ **Input validation** - Automatic checks for file existence, format, and binary availability
- ✅ **Better logging** - Timestamped logs with multiple verbosity levels
- ✅ **Flexible configuration** - Command-line options for all parameters
- ✅ **Dry-run mode** - Test your pipeline without executing
- ✅ **Progress tracking** - Real-time feedback on pipeline execution
- ✅ **Time reporting** - Automatic execution time measurement
- ✅ **Backwards compatibility** - Works with existing Perl scripts

### Files in This Repository
- `recount.py` - Modern Python wrapper (recommended)
- `recount.pl` - Backwards-compatible Perl wrapper
- `recount_legacy.pl` - Original Perl implementation (backup)
- `src/` - C++ source code and compiled binaries
- `test-data.txt` / `test-data-small.txt` - Example input files

## Publication

Edward Wijaya, Martin C. Frith, Yutaka Suzuki, Paul Horton, *Recount: expectation maximization based error correction tool for next
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
