## Utility functions file

import logging




class LogFilter(logging.Filter):
    def filter(self, record):
        if record.levelno <= 50:
            return True
        return False


class OutErrorFilter(logging.Filter):
    def filter(self, record):
        if record.levelno == 40 or record.levelno == 50:
            return True
        return False




##File Logger
class GLOBALS:
    #Logger
    #KmerLogger
    #ReadLogger
    #OutputLogger

    
    def __init__(self):
        #Make Loggers
        self.Logger = logging.getLogger('log')
        self.Logger.setLevel(logging.DEBUG)
        self.ReadLogger = logging.getLogger('log.read')
        self.ReadLogger.setLevel(logging.DEBUG)
        self.KmerLogger = logging.getLogger('log.kmer')
        self.KmerLogger.setLevel(logging.DEBUG)
        self.PRINT = 60


        #Make Outputters
        self.Outputter = logging.getLogger('log.output')
        self.Outputter.setLevel(logging.DEBUG)
        self.ChangedOutputter = logging.getLogger('log.output.changed')
        self.ChangedOutputter.setLevel(logging.DEBUG)
        self.UnchangedOutputter = logging.getLogger('log.output.unchanged')
        self.UnchangedOutputter.setLevel(logging.DEBUG)
        

        
        
        #Make Logging Handlers
        ch = logging.StreamHandler()
        ch.setLevel(logging.WARNING)
        ch.addFilter(LogFilter())

        ffh = logging.FileHandler('full.log', 'w')
        ffh.setLevel(logging.DEBUG)
        ffh.addFilter(LogFilter())

        fih = logging.FileHandler('info.log', 'w')
        fih.setLevel(logging.INFO)
        fih.addFilter(LogFilter())

        fwh = logging.FileHandler('warning.log', 'w')
        fwh.setLevel(logging.WARNING)
        fwh.addFilter(LogFilter())
        
        feh = logging.FileHandler('error.log', 'w')
        feh.setLevel(logging.ERROR)
        feh.addFilter(LogFilter())
        
        fch = logging.FileHandler('critical.log', 'w')
        fch.setLevel(logging.CRITICAL)
        fch.addFilter(LogFilter())

        #Make Output (ERROR) Handlers
        outche = logging.FileHandler('output.changed.fastq','w')
        outche.setLevel(logging.ERROR)
        outche.addFilter(OutErrorFilter())

        
        outuhe = logging.FileHandler('output.unchanged.fastq','w')
        outuhe.setLevel(logging.ERROR)
        outuhe.addFilter(OutErrorFilter())

        #Make Output handlers
        outch = logging.FileHandler('output.changed.fastq','w')
        outch.setLevel(self.PRINT)
        outch.addFilter(logging.Filter('log.output.changed'))     
        
        outuh = logging.FileHandler('output.unchanged.fastq','w')
        outuh.setLevel(self.PRINT)
        outuh.addFilter(logging.Filter('log.output.unchanged'))

        #Format Handlers
        
        stdFormatter = logging.Formatter('%(asctime)s - %(name)s -%(levelname)s - %(message)s')
        outFormatter = logging.Formatter('%(message)s')

        
        ch.setFormatter(stdFormatter)
        ffh.setFormatter(stdFormatter)
        fih.setFormatter(stdFormatter)
        fwh.setFormatter(stdFormatter)
        feh.setFormatter(stdFormatter)
        fch.setFormatter(stdFormatter)
        
        outuh.setFormatter(outFormatter)
        outch.setFormatter(outFormatter)
        outche.setFormatter(stdFormatter)
        outuhe.setFormatter(stdFormatter)
        
        #Adding Handlers to logger
        self.Logger.addHandler(ch)
        self.Logger.addHandler(ffh)
        self.Logger.addHandler(fih)
        self.Logger.addHandler(fwh)
        self.Logger.addHandler(feh)
        self.Logger.addHandler(fch)
        
        self.Logger.addHandler(outuh)
        self.Logger.addHandler(outch)
        self.Logger.addHandler(outche)
        self.Logger.addHandler(outuhe)




            




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
            if line=="":
                break
            elif (count==0):
                name=line[1:].strip()
            elif (count==1):
                seq=line.strip()        
            elif (count==2):
                pass  
            elif (count==3):
                qual=line.strip()
            count+=1
        if line != "":
            yield name, seq, qual
        count=0


'''def printFastq(name, seq, qual):
    print ('@'+name, file=filelike)
    print (seq, file=filelike)
    print ('+', file=filelike)
    print (qual, file=filelike)'''
    
