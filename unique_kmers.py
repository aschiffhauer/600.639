
'''
unique_kmers.py <file_name> prints out the number of unique lines in a file
if file_name is the output from kmerprinter, it will output the number of unique kmers
this is useful for comparing before-and-after sequences for error corrections
'''
import sys

def main():
	fn = sys.argv[1]
	frequency = int(sys.argv[2])
	unique_kmers = {}
	with open(fn) as fh:
		for line in fh:
			unique_kmers[line] = unique_kmers.get(line, 0) + 1
	print len(unique_kmers), "unique kmers in", fn
	distribution = {}
	for kmer, count in unique_kmers.items():
		distribution[count] = distribution.get(count, 0) + 1
	count = 0 if frequency not in distribution else distribution[frequency]
	print "# of kmers that appear", frequency, "time(s) in", fn, ":", count

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print "invalid arguments"
	else:
		main()