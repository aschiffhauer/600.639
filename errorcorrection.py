#Error correction strategies
#Search each kmer in the count min data structure
#(Below a certain "filled" factor, add to count min for frequency)
#Above this factor use the frequencies in the table to determine if a kmer is
#frequent or not
'''
Do we update after the filled point? what do we update with, only correct kmers
or "corrected" kmers as well? (this one most likely)


How do we correct? Quality score based base modulation and re-searching
What about 2 error reads. ie 1 correction brings freq closer to highest, but 2 brings to higher, how do we decide if we want to do this and upgrade to a 2/3/4 error correction?
Best way may be a shifting corrector that takes the highest frequency kmer of all 4 possibilities of each base and stores it in a previous variable. Use the previous highest corrections in further corrections down the chain,
with paired ends reads we may want to alternate first/last +- 1 
'''

from __future__ import print_function

import logging
import itertools as it
from countminsketch import CountMinSketch as cms
import sys




#Step 0: Iteration of the read, for each kmer in the read,
    
#Step 1: Fill data structure (Either one int or two, one: frequency, two:"correctness" value
#according to the Initial Quality Score of an input Kmer
#Otherwise push to a stack for correction later






#Step 2: After threshold check if kmer is frequent, if it is frequent is is considered valid
#if not it is considered invalid,


    #Step 2a: If invalid correct the kmer and research, takes a kmer and always handles the position the same way
            
#Step 3: Pass kmer back to read for continued correction

'''
Skeleton Code:

paired end read handler:
    Takes in two paired end reads ie 2x2 paired Strings
    Aligns Strings
    for each in each String:
        make two Strings with the higher q from each pair and the nucleotide from that pair
    return new Strings


read handling:
    Take in read
    iterate over kmers:
        send to kmer handling()
        get from kmer handling,
            while filling : if kmer is low, add read to set (unique stack) for re correction 
            after filling : if first kmer is low, disgard read
            if still low add 1 to low counter, if counter is too high disgard read
    return read (corrected/high quality?) 


kmer handling: Takes a kmer, returns kmer
    if data structure is not filled,
        if high enough quality, fill
        else pass kmer back
    else
        check for frequency in data structure:
            if high, already correct, update frequency and pass kmer
            if low, possibly not correct:
                correct()
                pass kmer



align:
    takes 2x2 Strings, returns 2x2 Strings
    while the first value of one String does not equal the nth value of the second
    n+1, iterate

    for each char in String1:
        if char[0] String2 matches, extend until end of either String and return if match quality is below n


May want to merge this into the above
error correction: Takes a kmer, returns kmer(or char?)
    change the right most value to each other* nucleotide
    get the highest value (frequency) can be the same nucleotide, and return it
    
'''
#


class ErrorCorrector:
    _filled=0
    _filledThresh=0
    #Initiates the error corrector with a threshold of the countmin's width * the average number of times each kmer in the structure should occur for the structure to consider it filled
    def __init__(self, CountMinW, CountMinH, KMerLength, Occurances, Accuracy, QThresh, EThresh):

        self._qualitythresh=KMerLength * (33 * QThresh)
        self._klength=KMerLength
        self._ethresh=EThresh
        self._filledThresh=CountMinW*Occurances
        self._othresh=Occurances*Accuracy
        self._countminh=CountMinH
        self._countminw=CountMinW
        self._filled=0
        self.countmin = cms(CountMinW, CountMinH)
        self.holdlist = list()
        
    #Takes in a quality string, and returns an int representing the sum of all the values within the string
    def sumQString(self, instring ):
        sumInt = 0;
        for char in instring:
            sumInt += (ord(char) - 33)
        return sumInt

    #Takes in a read, read quality and kmer length
    def correctRead(self, seq, qual):
        #print ("Correcting "+seq)
        ecount=0
        for i in range(len(seq)-self._klength):
            kmerS = seq[i:i+self._klength]
            kmerQ = qual[i:i+self._klength]
            kmerQ = self.sumQString(kmerQ)
            kmerNew, freq = self.correctKmer(kmerS, seq, kmerQ)
            if (freq < self._othresh):
                ecount+=1
            if freq > self.countmin[kmerS]:
                #print ("BEFORE CHANGE "+ seq)
                #print (seq[:i]+" "+kmerNew+" "+seq[i+self._klength:])
                seq = seq[:i] + kmerNew + seq[i+self._klength:]
                nqual = str(unichr((ord(qual[i+self._klength]) - 5)))
                qual = qual[:i+self._klength-1]+nqual+ qual[i+self._klength:]
                #print ("SEQ CHANGED "+ seq)
            if ecount >= self._ethresh:
                return "READ INVALID", "READ INVALID"
        return seq, qual


    def correctKmer(self, kmer, read, qualityscore):
        #print self._qualitythresh
        #we can store the quality score for higher accuracy
        if(self._filled < self._filledThresh):
            if qualityscore >= self._qualitythresh:
                self.countmin.add(kmer)
                self._filled += 1
                return kmer, -1
            else:
                self.holdlist.append(read)
                return "HOLD", -2 
        if(self._filled >= self._filledThresh):
            freq = self.countmin[kmer]
            if freq >= self._othresh:
                return kmer, freq
            if freq < self._othresh:
                results=list()
                for n in ['A','G','C','T']:
                    newKmer = kmer[:-1]+n
                    freq=self.countmin[newKmer]
                    results.append([freq,newKmer])
                result=max(results)
                return result[1], result[0]
                    


def parse_fastq(filelike):
    count=0
    line="NULL"
    seq=""
    qual=""
    name=""
    while ( True ):
        if line=="":
            break
        while count < 4:
            line=filelike.readline()
            if (count==0):
                name=line[1:].strip()
            elif (count==1):
                seq=line.strip()        
            elif (count==2):
                pass  
            elif (count==3):
                qual=line.strip()
            count+=1
        yield name, seq, qual
        count=0


def printFastq(name, seq, qual, filelike):
    print ('@'+name, file=filelike)
    print (seq, file=filelike)
    print ('+', file=filelike)
    print (qual, file=filelike)
    filelike.close()
    



    
def main():
    #makeLogger()
    
    e = ErrorCorrector(100, 10, 40,  10, 0.5, 0.5, 9999)
    open('output.fastq','w').close()

    for name, seq, qual in parse_fastq(open('yeast.fastq','r')):
        newseq, newqual=e.correctRead(seq, qual)
        printFastq('Corrected:'+name, newseq, newqual, open('output.fastq','a'))


def test():
    s1, s2 = e.correctKmer("TESTKMERA", "XXXXTESTKMERAXXXX", 1800)
    for x in range(0, 100):
        e.correctRead("AAAAAAAAAAAACACACAAATTAGTGA", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
    print ("TestKmer"+ e.correctKmer("ACACACAAG", "AAAAAAAAAAAACACACAAATTAGTGA", 1800))
    s1, s2 = e.correctKmer("TESTKMERA", "XXXXTESTKMERGXXXX", 500)
    print (s1+" "+s2+" "+e.countmin["TESTKMERG"]+" "+ e.countmin["TESTKMERA"])

    print (e.correctRead("AAAAAAAAAAAAGACACAGGTTAGTGA", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@"))
    
    print (e.holdlist)

    

main()


        
    
    
        
