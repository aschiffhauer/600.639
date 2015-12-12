# Build and run

Requirements: C99 compliant compiler (e.g. GCC or LLVM)

```
$ make clean && make
$ ./tests                 # runs the test suite
$ ./experiment_synthetic  # runs an experiment on synthetic reads
$ ./experiment_yeast      # runs an experiment on Saccharomyces cerevisiae (brewer's yeast) reads
```

# Verification of results

To verify the results reported in the writeup:

```
$ chmod +x verify_experiment_yeast.sh     # make executable
$ ./verify_experiment_yeast.sh            # verify experiment_yeast
$ chmod +x verify_experiment_synthetic.sh # make executable
$ ./verify_experiment_yeast.sh            # verify the experiment_yeast
```

# Usage

```
$ ./correct (input) (output) (kmer_size) (minsketch_width) (minsketch_height) (frequency_cutoff)
```

# Code overview

## Drivers
- `tests` Runs all tests (see below)
- `synthetic` Corrects sequences from synthetic reads (synthetic.fastq)
- `yeast` Corrects sequences from Saccharomyces cerevisiae (yeast.fastq) reads into corrected_yeast.fasq
- `correct` Performs error correction on any fastq input file `(./correct (input) (output) (kmer_size) (minsketch_width) (minsketch_height) (frequency_cutoff))`

## Data structures
- `bloomfilter` Bloom filter implementation
- `bloomfiltertests` Bloom filter tests
- `minsketch` Count-min sketch implementation
- `minsketchtests` Count-min sketch tests
- `histogram` Histogram (e.g. abstraction over bloom filters and count-min sketches) implementation
- `histogramtests` Histogram tests

## File handling
- `fastq` Fastq reading (e.g. sequences and qualities) implementation
- `fastqtests` Fastq reading tests

## Algorithms
- `error` Error detection and correction implementation
- `errortests` Error detection and correction tests
- `hash` String hash implementation for bloom filters and count-min sketches

# Miscellaneous
- `unique_kmers` Prints the number of unique kmers and kmers of a certain frequency for a file (`python unique_kmers.py (input) (frequency)`) 
- `kmerprinter` Prints the kmers of a file to stdout (`./kmerprinter (input) (kmer_size)`)
- `verify_experiment_yeast` Calculates numbers for yeast experiment (`./verify_experiment_yeast.sh`)
- `verify_experiment_synthetic` Calculates numbers for synthetic experiment (`./verify_experiment_synthetic.sh`)