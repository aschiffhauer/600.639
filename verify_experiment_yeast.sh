make clean 
rm -f yeast_kmers
rm -f yeast_corrected_kmers
make
./experiment_yeast
./kmerprinter yeast.fastq 8 > yeast_kmers
./kmerprinter yeast_corrected.fastq 8 > yeast_corrected_kmers
python unique_kmers.py yeast_kmers 1
python unique_kmers.py yeast_corrected_kmers 1