README

To create the error corrector:
    Either create a new ErrorCorrector object in a shell or edit errorcorrection.py's main with the parameters
    (CountMinW, CountMinH, ReadLength, KMerLength, Occurances, Accuracy, RQThresh, KQThresh, EThresh, glob)

    CountMinW = Width of the count min sketch
    CountMinH = Height of count min sketch
    ReadLength = Read length
    KMerLength = Kmer length
    Occurances = average frequency of each element in the count min structure,
        (If the average frequency is much higher than this, you may need to make width and height larger)
    Accuracy = Factor from 0-1 that indicates how many times a kmer must appear for it to be considered automatically valid and not correctworthy
        High accuracy means more corrections, most of the time incorrect ones
    RQThresh = RQThresh factor from 0-1 is the read quality threshold. Reads with a total quality score of less than
        the maximum possible quality (length * maximum quality(93)) will be pushed to the hold stack and not even considered for filling the structure
        generally lower
    KQThresh = KMER quality threshold from 0-1 is the same as read threshold but with respect to kmers.
        this is used to determine which kmers are added to the count min to initialize the error corrector,
        any kmers with a quality value over a certain level are also considered to be "good" and no attempt is made to correct them
            higher values means stricter selection, which means higher accuracy overall, but it also means that the hold list will be larger before the structure is filled
            too high a value and it may take forever

    EThresh = Number of changes a read is permitted to have before it is thrown out( not really used at the moment, set to 9999 for best performance)


    
To use the error corrector:
    You have several input choices;

    ErrorCorrector.inputFastq(filename)
        allows you to read in whole fastq files, or a list of fastq files (python list)
        takes the data as a filename, not a file object

    ErrorCorrector.correctRead(name, sequence, quality)
        takes in a single read and handles it, then prints it to output.changed or unchanged, depending on if it was changed  or not

    ErrorCorrector.correctKmer(kmer, qualityscore):
        takes in a kmer and a numeric quality value,
        returns a (new)kmer and a frequency of that (new) kmer
            Mostly useful for filling a table with non real data or customized kmers that for sure will fill the structure


Outputs:
    Several Log files, output.changed.fastq, output.unchanged.fastq
    
    

    
