make clean 
rm -f yeast_kmers
rm -f yeast_corrected_kmers
make
./experiment_synthetic
./kmerprinter synthetic.fastq 8 > synthetic_kmers
./kmerprinter synthetic_corrected.fastq 8 > synthetic_corrected_kmers
python unique_kmers.py synthetic_kmers 1
python unique_kmers.py synthetic_corrected_kmers 1