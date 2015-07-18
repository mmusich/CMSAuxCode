#!/usr/bin/env python

import datetime,time
import os,sys
import string, re
import subprocess
import ConfigParser, json
from subprocess import Popen, PIPE
from optparse import OptionParser

##############################################
def main():
##############################################

    desc="""This is a description of %prog."""
    parser = OptionParser(description=desc,version='%prog version 0.1')
    parser.add_option('-d','--isDate', help='decide if date or run', dest='isDate', action='store_true',default=False)
    parser.add_option('-s','--start',  help='starting point',        dest='start',  action='store'     ,default='')
    parser.add_option('-e','--end',    help='ending point',          dest='end',    action='store'     ,default='')
    parser.add_option('-D','--dataset',help='selected dataset',      dest='data',   action='store'     ,default='')
    (opts, args) = parser.parse_args()

    #cmd = 'python $DBSCMD_HOME/dbsCommandLine.py --query="find run where dataset='+opts.data+' and run.number>'+opts.start+' and run.number<'+opts.end+'" | sed \'/DBS/d\' | sed \'/run.number/d\' | sed \'/----/d\' | sed \'/run/d\''
    #cmd = 'das_client.py --limit=0 --query \'run dataset='+opts.data+' | grep run.run_number>='+opts.start+'| grep run.run_number<'+opts.end+'\''

    listOfRuns=None

    input_CMSSW_BASE = os.environ.get('CMSSW_BASE')
    cff_dir = os.path.join(input_CMSSW_BASE,"src/StreamValidation/DiMuonResonance/python")

    if(not opts.isDate):
        cmd = 'das_client.py --limit=0 --query \'run dataset='+opts.data+'\''
        p = Popen(cmd , shell=True, stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        listOfRuns=out.split('\n')

        for run in listOfRuns:
            if (run>=opts.start and run<opts.end):
                print "processing run:",run
                cffName = "Dataset_Run_"+run+"_cff.py" 
                fout = open(os.path.join(cff_dir,cffName),'w+b')
                mylist = None
                cmd2 = ' das_client.py --limit=0 --query \'file run='+run+' dataset='+opts.data+'\''
                q = Popen(cmd2 , shell=True, stdout=PIPE, stderr=PIPE)
                out2, err2 = q.communicate()
                mylist = out2.split('\n')
                lfn=list()
                for file in mylist:
                    if 'store' in file:
                        lfn.append(file)
                
                lfn_with_quotes = map(lambda x: "\'"+x+"\'",lfn)
                fout.write("import FWCore.ParameterSet.Config as cms \n")
                fout.write("maxEvents = cms.untracked.PSet( \n")
                fout.write("input = cms.untracked.int32(-1) \n")
                fout.write(") \n")
                fout.write("readFiles = cms.untracked.vstring() \n")
                fout.write("secFiles = cms.untracked.vstring() \n")
                fout.write("source = cms.Source (\"PoolSource\",fileNames = readFiles, secondaryFileNames = secFiles) \n")
                fout.write("readFiles.extend( [ \n")
                for f1 in lfn_with_quotes:
                    fout.write(f1+", \n")
                fout.write("] ); \n")    
                fout.close()
                
    else :
        cmd = 'python $DBSCMD_HOME/dbsCommandLine.py --query="find run.createdate where dataset='+opts.data+' and run.createdate>'+opts.start+' and run.createdate<'+opts.end+'" | sed \'/DBS/d\' | sed \'/run.number/d\' | sed \'/----/d\' | sed \'/run/d\''
        print cmd
        p = Popen(cmd , shell=True, stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        listOfDates=out.split('\n')
        #print listOfDates

        theLastDate=None
        strippedListOfDates=list()
        
        for theDate in listOfDates:
            if 'GMT' in theDate:
                strippedtime = time.strptime(theDate,"%a, %d %b %Y %H:%M:%S GMT")
                myDay=str(strippedtime[0])+"-"+str(strippedtime[1])+"-"+str(strippedtime[2])
                #print myDay
                if(myDay != theLastDate):
                    theLastDate=myDay
                    strippedListOfDates.append(theLastDate)
                    
        #print "starts here the stripped list"

        for theNewDate in strippedListOfDates:
            print "processing day:",theNewDate
            cffName = "Datasetet_Run_"+theNewDate+"_cff.py" 
            fout = open(os.path.join(cff_dir,cffName),'w+b')
            mylist = None
            cmd2 = 'python $DBSCMD_HOME/dbsCommandLine.py --query="find file where dataset='+opts.data+' and run.createdate='+theNewDate+'" | sed \'/DBS/d\' | sed \'/run.number/d\' | sed \'/----/d\' | sed \'/run/d\''
            #print cmd2
            q = Popen(cmd2 , shell=True, stdout=PIPE, stderr=PIPE)
            out2, err2 = q.communicate()
            #print "run: ",run,out2.split('\n')
            mylist = out2.split('\n')
            lfn=list()
            for file in mylist:
                if 'store' in file:
                    #print "run: ",run," file: ",file
                    lfn.append(file)
                
            lfn_with_quotes = map(lambda x: "\'"+x+"\'",lfn)
            #print lfn_with_quotes
            #print lfn
            fout.write("import FWCore.ParameterSet.Config as cms \n")
            fout.write("maxEvents = cms.untracked.PSet( \n")
            fout.write("input = cms.untracked.int32(-1) \n")
            fout.write(") \n")
            fout.write("readFiles = cms.untracked.vstring() \n")
            fout.write("secFiles = cms.untracked.vstring() \n")
            fout.write("source = cms.Source (\"PoolSource\",fileNames = readFiles, secondaryFileNames = secFiles) \n")
            fout.write("readFiles.extend( [ \n")
            for f1 in lfn_with_quotes:
                fout.write(f1+", \n")
            fout.write("] ); \n")    
            fout.close()
                                
if __name__ == "__main__":        
    main()
