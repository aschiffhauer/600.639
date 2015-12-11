# Build and run

```
$ make clean && make
$ ./tests                 # runs the test suite
$ ./experiment_synthetic  # runs an experiment on synthetic reads
$ ./experiment_yeast      # runs an experiment on Saccharomyces cerevisiae (brewer's yeast) reads
$ ./correct yeast.fastq yeast_corrected.fastq 8 8388608 1 1 # equivalent to ./experiment_yeast
$ ./kmerprinter yeast_corrected.fastq 8 # outputs all kmers to stdout
```

Requirements: C99 compliant compiler (e.g. GCC or LLVM)

Note: Untested on Windows

# Code overview

## Drivers
- `tests` Runs all tests (see below)
- `synthetic` Corrects sequences from synthetic reads (synthetic.fastq)
- `yeast` Corrects sequences from Saccharomyces cerevisiae (yeast.fastq) reads into corrected_yeast.fasq
- `correct` ./correct (input) (output) (kmer_size) (minsketch_width) (minsketch_height) (frequency_cutoff)
- `kmerprinter` ./kmerprinter (input) (kmer_size) 

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