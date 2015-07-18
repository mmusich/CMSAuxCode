import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing()

###################################################################
# Setup 'standard' options
###################################################################

options.register('OutFileName',
                 "testing_mp1568SAFEAPEPixelOnly_NTUPLE.root", # default value
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
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_0.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_1.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_2.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_3.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_4.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_5.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_6.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_7.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_8.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_9.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_10.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_11.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_12.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_13.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_14.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_15.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_16.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_17.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_18.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_19.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_20.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_21.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_22.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_23.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_24.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_25.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_26.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_27.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_28.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_29.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_30.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_31.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_32.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_33.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_34.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_35.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_36.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_37.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_38.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_39.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_40.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_41.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_42.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_43.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_44.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_45.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_46.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_47.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_48.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_49.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_50.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_51.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_52.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_53.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_54.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_55.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_56.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_57.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_58.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_59.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_60.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_61.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_62.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_63.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_64.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_65.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_66.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_67.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_68.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_69.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_70.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_71.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_72.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_73.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_74.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_75.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_76.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_77.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_78.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_79.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_80.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_81.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_82.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_83.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_84.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_85.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_86.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_87.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_88.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_89.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_90.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_91.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_92.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_93.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_94.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_95.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_96.root',
    #'/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_97.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_98.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_99.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_100.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_101.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_102.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_103.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_104.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_105.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_106.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_107.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_108.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_109.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_110.root',
    #'/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_111.root',
    #'/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_112.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_113.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_114.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_115.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_116.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_117.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_118.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_119.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_120.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_121.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_122.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_123.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_124.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_125.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_126.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_127.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_128.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_129.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_130.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_131.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_132.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_133.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_134.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_135.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_136.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_137.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_138.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_139.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_140.root',
    #'/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_141.root',
    #'/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_142.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_143.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_144.root',
    #'/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_145.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_146.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_147.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_148.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_149.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_150.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_151.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_152.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_58_DATA_ReReco_mp1568SAFEAPEPixelOnly/myTest_ReRecoCRUZET15SuperPointing_mp1568SAFEAPEPixelOnly_153.root'
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
