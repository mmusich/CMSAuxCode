import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("READ")

options = VarParsing.VarParsing()
options.register('GT',
                 'auto:run2_data',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "the input Global Tag")

options.register('rawFile',
                 'ReReco',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "the output files name")

options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 10000                           # do not clog output with IO

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )      

####################################################################
# Empty source 
####################################################################

process.source = cms.Source("EmptySource",
                            firstRun = cms.untracked.uint32(1),
                            numberEventsInRun = cms.untracked.uint32(1),           # a number of events in single run 
                            )

####################################################################
# Connect to conditions DB
####################################################################
process.load("Configuration.Geometry.GeometryRecoDB_cff")

# either from Global Tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,"80X_mcRun2_design_Queue")

#...or specify database connection and tag:  
from CondCore.CondDB.CondDB_cfi import *
myCondDB = CondDB.clone(connect = cms.string('sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/PayLoads/2015_APEfor25ns_38Tand0T/ape_DESRUN2_74_V4_barrel10-disks20.db'))
process.dbInput = cms.ESSource("PoolDBESSource",
                               myCondDB,
                               toGet = cms.VPSet(cms.PSet(record = cms.string('TrackerAlignmentErrorExtendedRcd'),
                                                          tag = cms.string('testTagAPE') #choose your own favourite
                                                          )
                                                 )
                               )

process.es_prefer_dbInput = cms.ESPrefer("PoolDBESSource","dbInput")

###################################################################
# Output name
###################################################################
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("APETrend_"+options.rawFile+".root")
                                   )

####################################################################
# Load and configure analyzer
###################################################################
process.TrackerAPEReader = cms.EDAnalyzer('TkAlConditionsReader',
                                          rawFileName = cms.untracked.string("log_"+options.rawFile+".txt")
                                          )

####################################################################
# Output file
####################################################################

# Put module in path:
process.p = cms.Path(process.TrackerAPEReader)
