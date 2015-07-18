import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing()

###################################################################
# Setup 'standard' options
###################################################################

options.register('OutFileName',
                 "testing_mp1568PlusFPixSAFEAPEPixelOnly_NTUPLE.root", # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string, # string, int, or float
                 "name of the output file (test.root is default)")

options.register('myGT',
                 "GR_E_V42::All", # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string, # string, int, or float
                 "name of the input Global Tag (POSTLS170_V5 is default)")

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
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = options.myGT

###################################################################
# Source
###################################################################
process.source = cms.Source ("PoolSource",fileNames =  cms.untracked.vstring())
#process.source.fileNames = [options.InputFileName]
#process.load("StreamValidation.DiMuonResonance."+options.myDataset)
process.source = cms.Source ("PoolSource",fileNames =  cms.untracked.vstring())
process.source.fileNames = [
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_0.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_100.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_101.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_102.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_103.root',
    #'/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_104.root',
    #'/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_105.root',
    #'/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_108.root',
    #'/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_114.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_12.root',
    #'/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_13.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_16.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_17.root',
    #'/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_24.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_42.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_43.root',
    # '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_57.root',
    #'/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_62.root',
    #'/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_69.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_76.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_86.root',
    #'/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_92.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_96.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_97.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_98.root',
    '/store/user/musich/test_out/test_2015_03_06_22_05_58_DATA_ReReco_ReReco_mp1568_SAFEAPE_FPixOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_99.root'
    ]

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

###################################################################
# The module
###################################################################
process.myanalysis = cms.EDAnalyzer("GeneralTrackAnalyzerHisto_v2",
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
process.p1 = cms.Path(process.offlineBeamSpot*process.myanalysis*process.myntuple)
#process.p2 = cms.Path(process.offlineBeamSpot*process.MuSkim*process.myanalysis)
