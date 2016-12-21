import FWCore.ParameterSet.Config as cms

process = cms.Process("READ")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000                            # do not clog output with IO

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )      

####################################################################
# Empty source 
####################################################################

process.source = cms.Source("EmptySource",
                            firstRun = cms.untracked.uint32(273291),
                            numberEventsInRun = cms.untracked.uint32(1),           # a number of events in single run 
                            )

####################################################################
# Connect to conditions DB
####################################################################
process.load("Configuration.Geometry.GeometryRecoDB_cff")

# either from Global Tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,"80X_dataRun2_Prompt_v15")

# ...or specify database connection and tag:  
#from CondCore.CondDB.CondDB_cfi import *
#CondDBBeamSpotObjects = CondDB.clone(connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'))
#process.dbInput = cms.ESSource("PoolDBESSource",
#                               CondDBBeamSpotObjects,
#                               toGet = cms.VPSet(cms.PSet(record = cms.string('BeamSpotObjectsRcd'),
#                                                          tag = cms.string('BeamSpotObjects_PCL_byLumi_v0_prompt') #choose your own favourite
#                                                          )
#                                                 )
#                               )

####################################################################
# Load and configure analyzer
###################################################################
process.demo = cms.EDAnalyzer('SiStripConditionsReader')

####################################################################
# Output file
####################################################################

# Put module in path:
process.p = cms.Path(process.demo)
