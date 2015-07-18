import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing()

###################################################################
# Setup 'standard' options
###################################################################

options.register('OutFileName',
                 "testing_cosmics_FullCRUZET15_ReReco_mp1598_SAFEAPEPixelOnly.root", # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string, # string, int, or float
                 "name of the output file (test.root is default)")

options.register('myGT',
                 "GR_P_V50", # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string, # string, int, or float
                 "name of the input Global Tag (GR_P_V50 is default)")

options.register('myDataset',
                 #"Dataset_Cosmic70DR_0T-TkAlCosmics0T-Deco_Opti_COSM70_DEC_V2_cff", # default value
                 "myDataset_v1",
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string, # string, int, or float
                 "name of the input dataset")

options.parseArguments()

process = cms.Process("AlCaRECOAnalysis")

###################################################################
# Message logger service
###################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr = cms.untracked.PSet(placeholder = cms.untracked.bool(True))
process.MessageLogger.cout = cms.untracked.PSet(INFO = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(1000) # every 100th only
    #    limit = cms.untracked.int32(10)       # or limit to 10 printouts...
    ))

###################################################################
# Geometry producer and standard includes
###################################################################
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
#process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("CondCore.DBCommon.CondDBCommon_cfi")

###################################################################
# Option 1: just state the Global Tag 
###################################################################
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = options.myGT

###################################################################
# Source
###################################################################
process.source = cms.Source ("PoolSource",fileNames =  cms.untracked.vstring())
#process.source.fileNames = [options.InputFileName]
#process.load("StreamValidation.DiMuonResonance."+options.myDataset)
process.source = cms.Source ("PoolSource",fileNames =  cms.untracked.vstring())
process.source.fileNames = [
    '/store/user/musich/test_out/ReRecoFullCRUZET15_Superpointing/step0_ReRecoCRUZET15_Superpointing_mp1598_hierarchy_merged.root',
    '/store/user/musich/test_out/ReRecoFullCRUZET15_Superpointing/step1_ReRecoCRUZET15_Superpointing_mp1598_hierarchy_merged.root',
    '/store/user/musich/test_out/ReRecoFullCRUZET15_Superpointing/step2_ReRecoCRUZET15_Superpointing_mp1598_hierarchy_merged.root',
    '/store/user/musich/test_out/ReRecoFullCRUZET15_Superpointing/step3_ReRecoCRUZET15_Superpointing_mp1598_hierarchy_merged.root',
    '/store/user/musich/test_out/ReRecoFullCRUZET15_Superpointing/step4_ReRecoCRUZET15_Superpointing_mp1598_hierarchy_merged.root',    
    #'/store/user/musich/test_out/ReRecoFullCRUZET15_Superpointing/step5_ReRecoCRUZET15_Superpointing_mp1598_hierarchy_merged.root',    
    ]

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

###################################################################
# The AlCa Tracks filter
###################################################################
process.myalcarecofilter = cms.EDFilter("AlCaTrackFilter",
                                        src = cms.InputTag('ALCARECOTkAlCosmicsCTF0T'),
                                        selectBPix = cms.bool(False),
                                        selectFPix = cms.bool(False)
                                        )

###################################################################
# The module
###################################################################
process.myanalysis = cms.EDAnalyzer("GeneralTrackAnalyzerHisto_v3",
                                    TkTag  = cms.string('ALCARECOTkAlCosmicsCTF0T'),
                                    #TkTag = cms.string ('ctfWithMaterialTracksP5'),
                                    #TkTag = cms.string('cosmictrackfinderP5'),
                                    isCosmics = cms.bool(True)
                                    )

process.myntuple = cms.EDAnalyzer("TrackAnalyzer",
                                  TkTag  = cms.string('ALCARECOTkAlCosmicsCTF0T')
                                  )

###################################################################
# Output name
###################################################################
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.OutFileName)
                                   )

###################################################################
# Path
###################################################################
#process.p1 = cms.Path(process.offlineBeamSpot*process.myanalysis)
process.p1 = cms.Path(process.offlineBeamSpot*
                      #process.myalcarecofilter*
                      process.myanalysis
                      )
                      #*process.myntuple)
#process.p2 = cms.Path(process.offlineBeamSpot*process.MuSkim*process.myanalysis)
