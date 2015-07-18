#!/usr/bin/env python

import datetime,time
import os,sys
import string, re
import subprocess
import ConfigParser, json
from subprocess import Popen, PIPE
from optparse import OptionParser

##############################################
def convert_time(val):
##############################################
    val = int(val)
    day   = val%100
    month = ((val - day)%10000)/100
    year  = val/10000
    print " val:",val," year:",year," month:",month," day:",day
    mytime = datetime.datetime(year,month,day)
    return mytime
    #return mytime.strftime('%Y-%m-%d')

##############################################
def main():
##############################################

    desc="""This is a description of %prog."""
    parser = OptionParser(description=desc,version='%prog version 0.1')
    parser.add_option('-d','--isDate', help='decide if date or run', dest='isDate', action='store_true',default=False)
    parser.add_option('-m','--merge' , help='decide if cff are merged into one', dest='isMerged', action='store_true',default=False)
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
        theMergedCffName = "Dataset_Run_"+opts.start+"_to_Run_"+opts.end+"_cff.py" 
        if (opts.isMerged) :
            print "==== Merging into one single cff file ===="
            fout = open(os.path.join(cff_dir,theMergedCffName),'w+b')

        for run in listOfRuns:
            if (run>=opts.start and run<=opts.end):
                print "processing run:",run
                cffName = "Dataset_Run_"+run+"_cff.py" 
                if not opts.isMerged:
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
                if ( (opts.isMerged and run==opts.start) or not opts.isMerged):  
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
                if ( (opts.isMerged and run==opts.end) or not opts.isMerged):          
                    fout.write("] ); \n")    
                    fout.close()
                
    else :
        # das_client.py --limit=0 --query 'run between [229369,230191] | grep run.run_number, run.start_time, run.end_time | sort run.run_number'
        cmd = 'das_client.py --limit=0 --query \'run date between ['+opts.start +','+opts.end+'] | grep run.run_number,run.start_time | sort run.run_number\''
        print cmd
        p = Popen(cmd , shell=True, stdout=PIPE, stderr=PIPE)
        out, err = p.communicate()
        allinfo=out.split('\n')
        allinfo.pop()
        #print allinfo
        dict=list()
        for element in allinfo:
            run = element.split(' ')[0]
            day = element.split(' ')[1]
            dict.append((run,day))
        
        #print dict

        #start_date = datetime.datetime(2014,11,01)
        #end_date   = datetime.datetime(2014,11,30)

        dt   = convert_time(opts.start)
        end  = convert_time(opts.end)
        step = datetime.timedelta(days=1)

        listOfDates = []

        while dt < end:
            listOfDates.append(dt.strftime('%Y-%m-%d'))
            dt += step     
    
        #listOfDates=out.split('\n')
        #print listOfDates

        #theLastDate=None
        #strippedListOfDates=list()
        
        # for theDate in listOfDates:
        #     if ':' in theDate:
        #         strippedtime = time.strptime(theDate,"%Y-%m-%d %H:%M:%S")
        #         myDay=str(strippedtime[0])+"-"+str(strippedtime[1])+"-"+str(strippedtime[2])
        #         #print myDay
        #         if(myDay != theLastDate):
        #             theLastDate=myDay
        #             strippedListOfDates.append(theLastDate)
         
        # print strippedListOfDates            
        #print "starts here the stripped list"

        theMergedCffName = "Dataset_Date_"+opts.start+"_to_Date_"+opts.end+"_cff.py" 
        if (opts.isMerged) :
            print "==== Merging into one single cff file ===="
            fout = open(os.path.join(cff_dir,theMergedCffName),'w+b')

        for theNewDate in listOfDates:
            print "processing day:",theNewDate
            cffName = "Datasetet_Date_"+theNewDate+"_cff.py" 
            if not opts.isMerged:
                fout = open(os.path.join(cff_dir,cffName),'w+b')
          
            lfn=list()

            for mytup in dict:
                if(theNewDate==mytup[1]):
                    theRun = mytup[0]
                    print "match!","date:",mytup[1]," run:",mytup[0]
                    mylist = None
                    cmd2 = ' das_client.py --limit=0 --query \'file run='+theRun+' dataset='+opts.data+'\''
                    q = Popen(cmd2 , shell=True, stdout=PIPE, stderr=PIPE)
                    out2, err2 = q.communicate()
                    mylist = out2.split('\n')
                    for file in mylist:
                        if 'store' in file:
                            print "run: ",theRun," file: ",file
                            lfn.append(file)
                
            lfn_with_quotes = map(lambda x: "\'"+x+"\'",lfn)
            #print lfn_with_quotes
            #print lfn
            if ( (opts.isMerged and theNewDate==opts.start) or not opts.isMerged):  
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
            if ( (opts.isMerged and theNewDate==opts.end) or not opts.isMerged):  
                fout.write("] ); \n")    
                fout.close()
                                
if __name__ == "__main__":        
    main()
