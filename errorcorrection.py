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

import argparse
import logging
import itertools as it
from countminsketch import CountMinSketch as cms
import sys

import jutils




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
    def __init__(self, CountMinW, CountMinH, ReadLength, KMerLength, Occurances, Accuracy, RQThresh, KQThresh, EThresh):


        self._occurances=Occurances
        self._kqualitythresh=KMerLength * (93 * KQThresh)
        self._rqualitythresh=ReadLength * (93 * RQThresh)
        self._klength=KMerLength
        self._rlength=ReadLength
        self._ethresh=EThresh
        self._filledThresh=CountMinW*Occurances
        self._othresh=Occurances*Accuracy
        self._countminh=CountMinH
        self._countminw=CountMinW
        self._filled=0
        self.countmin = cms(CountMinW, CountMinH)
        self.holdlist = list()
        self.GLOBALS=jutils.GLOBALS()
        self.log=self.GLOBALS.Logger
        self._holdCleared=False
        self.PRINT=60
        self._corrected=0
        self._uncorrected=0
        self._processedReads=0

        '''
        self.parser=argparse.ArgumentParser(description='Initialize Error Corrector')
        self.parser.add_argument('-w', '--width', type=int, action='store', dest=self._countminw,
                                 default=100, help="Set the width of the Count Min Sketch Default=100")
        self.parser.add_argument('-i', '--height', type=int, action='store', dest=self._countminh,
                                 default=10, help="Set the width of the Count Min Sketch Default=10")
        self.parser.add_argument('-k', '--kmer', type=int, action='store', dest=self._countminh,
                                 default=16, help="Set Kmer Length Default=16")
        self.parser.add_argument('-r', '--read', type=int, action='store', dest=self._countminh,
                                 default=16, help="Set Kmer Length Default=16")
        self.parser.add_argument('-e', '--error', type=int, action='store', dest=self._countminh,
                                 default=16, help="Set Kmer Length Default=16")
        self.parser.add_argument('-o', '--occurance', type=int, action='store', dest=self._countminh,
                                 default=16, help="Set Kmer Length Default=16")
        self.parser.add_argument('-q', '--kmerquality', type=int, action='store', dest=self._countminh,
                                 default=16, help="Set Kmer Length Default=16")
        self.parser.add_argument('-u', '--readquality', type=int, action='store', dest=self._countminh,
                                 default=16, help="Set Kmer Length Default=16")
        self.parser.add_argument('-a', '--accuracy', type=int, action='store', dest=self._countminh,
                                 default=16, help="Set Kmer Length Default=16")'''

        

        self.log.info("Creating new Error Corrector with parameters: ")
        self.log.info("Count Min Length(Columns): "+str(self._countminw)+ " Count Min Height(Hashfunctions): "+str(self._countminh))
        self.log.info("Read Length: "+str(ReadLength)+ " KMER Length: "+str(KMerLength))
        self.log.info("Avg Kmer Occurance to fill: "+str(Occurances)+ " Occurance Threshold: "+str(self._othresh))
        self.log.info("Avg Read Quality Threshold: "+str(93*RQThresh)+ " Avg Kmer Quality Threshold: "+str(93*KQThresh))
        self.log.info("Error Threshold: "+str(EThresh))
        
    #Takes in a quality string, and returns an int representing the sum of all the values within the string
    def sumQString(self, instring ):
        self.log.debug("Summing a quality file")
        self.log.debug(instring)
        sumInt = 0;
        for char in instring:
            sumInt += (ord(char) - 33)

        self.log.debug("Quality="+str(sumInt))
        return sumInt

    #Takes in a read, read quality and kmer length
    def correctRead(self, name, seq, qual):
        oldseq=seq
        self.log.debug("Correcting a read "+name)
        self.log.debug(seq)
        self.log.debug(qual)
        ecount=0
        changed=0
        qualTotal=self.sumQString(qual)
        #if qscore is too low, and not filled, hold the read
        if self._filled < self._filledThresh and qualTotal < self._rqualitythresh:
            self.log.info("Read Quality too low, holding...")
            self.log.info("Read Quality: "+str(qualTotal)+" Read Quality Threshold: "+str(self._rqualitythresh))
            self.holdlist.append([name, seq, qual])      
            return "HOLD", "HOLD", "HOLD", 0
        for i in range(len(seq)-self._klength):
            kmerS = seq[i:i+self._klength]
            oldFreq = self.countmin[kmerS]
            kmerQ = qual[i:i+self._klength]
            kmerQ = self.sumQString(kmerQ)
            kmerNew, freq = self.correctKmer(kmerS, seq, kmerQ)
            #if kmer of the read is not good enough, hold the whole read
            if kmerNew == "HOLD":
                self.holdlist.append([name, seq, qual])
                return "HOLD", "HOLD", "HOLD", 0
            if (freq < self._othresh and freq != -1):
                self.log.info("Low frequency KMER detected, more and the read will be discarded...")
                self.log.info(str(freq)+"   "+str(self._othresh))
                ecount+=1

            if kmerS != kmerNew and freq > oldFreq:
                changed += 1
                self.log.debug("KMER changed, Changes Overall: "+str(changed))
                self.log.debug("Old KMER: "+kmerS)
                self.log.debug("New KMER: "+kmerNew)
                #print ("BEFORE CHANGE "+ seq)
                #print (seq[:i]+" "+kmerNew+" "+seq[i+self._klength:])
                seq = seq[:i] + kmerNew + seq[i+self._klength:]
                nqual = str(unichr((ord(qual[i+self._klength]) + 5)))
                qual = qual[:i+self._klength-1]+nqual+ qual[i+self._klength:]
                #print ("SEQ CHANGED "+ seq)
            if ecount >= self._ethresh:
                return "READ INVALID", "READ INVALID", "READ INVALID", -1

            
        if self._filled >= self._filledThresh and self._holdCleared ==False:
            self.log.critical("Clearing Hold Stack......... any new reads will be corrected")
            self._holdCleared=True
            self.log.critical(str(len(self.holdlist)))
            self.clearHold()



            
        self.log.debug("Returning a possibly corrected read")
        self.log.debug(name)
        self.log.debug(seq)
        self.log.debug(qual)
        self.log.debug("Changed: "+str(changed))
        if seq != oldseq:
            self._corrected+=1
        else:
            self._uncorrected+=1
        return name, seq, qual, changed


                

    def clearHold(self):
        self.log.critical("Clearing Hold Stack.........")

        while len(self.holdlist) > 0:
            held = self.holdlist.pop()
            self.log.debug("Held read: "+held[0])
            self.log.debug(held[1])
            self.log.debug(held[2])
            name, newseq, newqual, changed = self.correctRead(held[0],held[1],held[2])
            self.printFastq(name, newseq, newqual, changed)

        self.holdlist=[]
        self.log.critical(str(len(self.holdlist)))
        
        

    def correctKmer(self, kmer, qualityscore):
        self.log.debug("Handling Kmer with qscore["+str(qualityscore)+"] " + kmer)
        #we can store the quality score for higher accuracy
        if(self._filled < self._filledThresh):
            self.log.debug("Countmin not populated")
            if qualityscore >= self._kqualitythresh:

                if (self._filled % 100 == 0):
                    self.log.critical("Populating Count Min...")
                    self.log.critical("FILLED/THRESH = "+str(self._filled)+'/'+str(self._filledThresh))
                    #self.log.critical("Quality of kmer: "+str(qualityscore)+" KMER Quality Threshold: "+str(self._kqualitythresh))
                else:
                    self.log.info("Populating Count Min...")
                    self.log.info("FILLED/THRESH = "+str(self._filled)+'/'+str(self._filledThresh))
                    self.log.info("Quality of kmer: "+str(qualityscore)+" KMER Quality Threshold: "+str(self._kqualitythresh))
                self.log.info(kmer)
                self.countmin.add(kmer)
                self._filled += 1
                return kmer, -1
            else:
                self.log.info("KMER quality too low, adding read to hold...")
                self.log.info("Quality: "+str(qualityscore)+" Threshold: "+str(self._kqualitythresh))
                return "HOLD", -2 
        if(self._filled >= self._filledThresh):
            self.log.debug("Populated already handle kmer...")
            freq = self.countmin[kmer]
            if freq >= self._othresh:
                self.log.info("Good KMER with frequency: "+str(freq)+ " Threshold: " +str(self._othresh))
                self.log.info(kmer)
                return kmer, freq
            if freq < self._othresh:
                self.log.info("BAD KMER with frequency: "+str(freq)+ " Threshold: " +str(self._othresh))
                self.log.info("OLD: "+kmer)
                results=list()
                for n in ['A','G','C','T']:
                    newKmer = kmer[:-1]+n
                    freq=self.countmin[newKmer]
                    results.append([freq,newKmer])
                result=max(results)
                self.log.info("KMER Corrected... new freq: "+str(result[0]))
                self.log.info("NEW: "+result[1])
                return result[1], result[0]

    #Takes a filename fastq and handles it
    def inputFastq(self, filenames):
        if type(filenames) == str:
            tStr=filenames
            filenames = list()
            filenames.append(tStr)
            for filename in filenames:
                for name, seq, qual in jutils.parse_fastq(open(filename,'r')):
                    name, newseq, newqual, changed=self.correctRead(name, seq, qual)
                    if name != "HOLD":
                        self.printFastq(name, newseq, newqual, changed)

    #Takes a name, seq, qual, and changed, and prints it appropriatly
    def printFastq(self, name, seq, qual, changed):
        name = name[1:]
        percent = float(changed/100.0)
        if changed > 0:
            self.GLOBALS.ChangedOutputter.log(self.PRINT, '@Corrected: ['+str(changed)+']['+str(percent)+'%] '+name)
            self.GLOBALS.ChangedOutputter.log(self.PRINT, seq)
            self.GLOBALS.ChangedOutputter.log(self.PRINT, '+')
            self.GLOBALS.ChangedOutputter.log(self.PRINT, qual)
            self.GLOBALS.ChangedOutputter.log(self.PRINT, 'CORRECTED: '+ str(self._corrected))
            self.GLOBALS.ChangedOutputter.log(self.PRINT, 'TOTAL READS: '+ str(self._corrected+self._uncorrected))
        if changed == 0:
            self.GLOBALS.UnchangedOutputter.log(self.PRINT, '@'+name)
            self.GLOBALS.UnchangedOutputter.log(self.PRINT, seq)
            self.GLOBALS.UnchangedOutputter.log(self.PRINT, '+')
            self.GLOBALS.UnchangedOutputter.log(self.PRINT, qual)
            self.GLOBALS.UnchangedOutputter.log(self.PRINT,'UNCORRECTED: '+str(self._uncorrected))
            self.GLOBALS.UnchangedOutputter.log(self.PRINT,'TOTAL READS: '+str(self._corrected+self._uncorrected))
        self._processedReads+=1
        if( self._processedReads % 100 == 0):
            self.GLOBALS.Logger.critical("Reads Processed so Far: "+str(self._processedReads)+" Reads in Holding: "+str(len(self.holdlist))+" Filled/Threshold: "+str(self._filled)+'/'+str(self._filledThresh))
                    
    
def main():
    #CountMinW, CountMinH, ReadLength, KMerLength, Occurances, Accuracy, RQThresh, KQThresh, EThresh, glob)
    e = ErrorCorrector(100, 10, 32, 16, 10, 0.5, 0.2, 0.4, 9999)
    e.inputFastq('yeast.fastq')


    


def test():
    #s1, s2 = e.correctKmer("TESTKMERA", "XXXXTESTKMERAXXXX", 1800)
    for x in range(0, 100):
        e.correctRead("AAAAAAAAAAAACACACAAATTAGTGA", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
    #print ("TestKmer"+ e.correctKmer("ACACACAAG", "AAAAAAAAAAAACACACAAATTAGTGA", 1800))
    s1, s2 = e.correctKmer("TESTKMERA", "XXXXTESTKMERGXXXX", 500)
    print (s1+" "+s2+" "+e.countmin["TESTKMERG"]+" "+ e.countmin["TESTKMERA"])

    print (e.correctRead("AAAAAAAAAAAAGACACAGGTTAGTGA", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@"))
    
    print (e.holdlist)

    

main()


        
    
    
        
