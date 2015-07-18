import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing()

###################################################################
# Setup 'standard' options
###################################################################

options.register('OutFileName',
                 "testing_mp1568SAFEAPE.root", # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string, # string, int, or float
                 "name of the output file (test.root is default)")

options.register('myGT',
                 "GR_P_V49::All", # default value
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
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_0.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_1.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_2.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_3.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_4.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_5.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_6.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_7.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_8.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_9.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_10.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_11.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_12.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_13.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_14.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_15.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_16.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_17.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_18.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_19.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_20.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_21.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_22.root',
    '/store/caf/user/musich/test_out/test_2015_02_26_22_01_35_DATA_mp1568SAFEAPE/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPE_23.root'
    ]

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

###################################################################
# Alignment Track Selector
###################################################################
import Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi

process.MuSkimSelector = Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi.AlignmentTrackSelector.clone(
    applyBasicCuts = True,                                                                            
    filter = True,
    src = 'ALCARECOTkAlCosmicsCTF0T',
    ptMin = 17.,
    pMin = 17.,
    etaMin = -2.5,
    etaMax = 2.5,
    d0Min = -2.,
    d0Max = 2.,
    dzMin = -25.,
    dzMax = 25.,
    nHitMin = 6,
    nHitMin2D = 0,
    )

###################################################################
# The module
###################################################################
process.myanalysis = cms.EDAnalyzer("GeneralTrackAnalyzerHisto",
                                    #TkTag  = cms.string('ALCARECOTkAlCosmicsCTF0T'),
                                    TkTag = cms.string ('ctfWithMaterialTracksP5'),
                                    #TkTag = cms.string('cosmictrackfinderP5'),
                                    isCosmics = cms.bool(True)
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
process.p1 = cms.Path(process.offlineBeamSpot*process.myanalysis)
#process.p2 = cms.Path(process.offlineBeamSpot*process.MuSkim*process.myanalysis)
