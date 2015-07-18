import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing()

###################################################################
# Setup 'standard' options
###################################################################

options.register('OutFileName',
                 "testing_out_of_the_box_NTUPLE.root", # default value
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
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_0.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_1.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_10.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_100.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_101.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_102.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_103.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_104.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_105.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_106.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_107.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_108.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_109.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_11.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_110.root',
    #'/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_111.root',
    #'/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_112.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_113.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_114.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_115.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_116.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_117.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_118.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_119.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_12.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_120.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_121.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_122.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_123.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_124.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_125.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_126.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_127.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_128.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_129.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_13.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_130.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_131.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_132.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_133.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_134.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_135.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_136.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_137.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_138.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_139.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_14.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_140.root',
    # '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_141.root',
    # '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_142.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_143.root',
    # '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_144.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_145.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_146.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_147.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_148.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_149.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_15.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_150.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_151.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_152.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_153.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_16.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_17.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_18.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_19.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_2.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_20.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_21.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_22.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_23.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_24.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_25.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_26.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_27.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_28.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_29.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_3.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_30.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_31.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_32.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_33.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_34.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_35.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_36.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_37.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_38.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_39.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_4.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_40.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_41.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_42.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_43.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_44.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_45.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_46.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_47.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_48.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_49.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_5.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_50.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_51.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_52.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_53.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_54.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_55.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_56.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_57.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_58.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_59.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_6.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_60.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_61.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_62.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_63.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_64.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_65.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_66.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_67.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_68.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_69.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_7.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_70.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_71.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_72.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_73.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_74.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_75.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_76.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_77.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_78.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_79.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_8.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_80.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_81.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_82.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_83.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_84.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_85.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_86.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_87.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_88.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_89.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_9.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_90.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_91.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_92.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_93.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_94.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_95.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_96.root',
    # '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_97.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_98.root',
    '/store/caf/user/musich/test_out/test_2015_02_28_18_49_26_DATA_ReReco_StandardReco/myTest_ReRecoCRUZET15SuperPointing_99.root'
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
process.p1 = cms.Path(process.offlineBeamSpot*process.myanalysis*process.myntuple)
#process.p1 = cms.Path(process.offlineBeamSpot*process.myntuple)
#process.p2 = cms.Path(process.offlineBeamSpot*process.MuSkim*process.myanalysis)
