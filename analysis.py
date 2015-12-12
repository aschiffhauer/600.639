
'''
analysis.py <file_name> prints out the number of unique lines in a file
if file_name is the output from kmerprinter, it will output the number of unique kmers
this is useful for comparing before-and-after sequences for error corrections
'''
import sys

def main():
	fn = sys.argv[1]
	unique_kmers = {}
	with open(fn) as fh:
		for line in fh:
			unique_kmers[line] = unique_kmers.get(line, 0) + 1
	print len(unique_kmers), "unique kmers in", fn

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print "invalid arguments"
	else:
		main()