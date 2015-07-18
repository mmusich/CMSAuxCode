#!/usr/bin/env python
import os,sys
import ROOT
import math

count=0
iEx=0
f = open('das2.out','rwb')
f1 = open('Dataset_CRAFT15_express_cff.py','w')

f1.write('import FWCore.ParameterSet.Config as cms\n')
f1.write('maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )\n')
f1.write('readFiles = cms.untracked.vstring()\n')
f1.write('secFiles = cms.untracked.vstring()\n')
f1.write('source = cms.Source (\"PoolSource\",fileNames = readFiles, secondaryFileNames = secFiles)\n')
f1.write('readFiles.extend( [\n')

for line in f:
    # os.system('edmEventSize -v -a `cmsPfn '+line.rstrip('\n')+'` |grep Events')
    f1.write('\''+line.rstrip('\n')+'\',\n')
    count+=1
    if(count%256==0):
        f1.write(']);')
        f1.write('\n')
        f1.write('readFiles.extend( [')
        iEx+=1
        print iEx,line

print count

f1.write(']);\n')
f1.write('secFiles.extend( [\n')
f1.write('])')
