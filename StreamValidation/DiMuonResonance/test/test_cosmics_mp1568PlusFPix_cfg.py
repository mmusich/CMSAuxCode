import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing()

###################################################################
# Setup 'standard' options
###################################################################

options.register('OutFileName',
                 "testing_mp1568FPlusFPix_NTUPLE.root", # default value
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
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_0.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_1.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_10.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_100.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_101.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_102.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_103.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_104.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_105.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_106.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_107.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_108.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_109.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_11.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_110.root',
    #'/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_111.root',
    #'/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_112.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_113.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_114.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_115.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_116.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_117.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_118.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_119.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_12.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_120.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_121.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_122.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_123.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_124.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_125.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_126.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_127.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_128.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_129.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_13.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_130.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_131.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_132.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_133.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_134.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_135.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_136.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_137.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_138.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_139.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_14.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_140.root',
    #'/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_141.root',
    #'/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_142.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_143.root',
    #'/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_144.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_145.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_147.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_148.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_149.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_15.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_150.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_151.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_152.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_153.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_16.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_17.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_18.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_19.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_2.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_20.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_21.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_22.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_23.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_24.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_25.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_26.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_27.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_28.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_29.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_3.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_30.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_31.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_32.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_33.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_34.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_35.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_36.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_37.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_38.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_39.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_4.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_40.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_41.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_42.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_43.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_44.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_45.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_46.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_47.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_48.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_49.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_5.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_50.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_51.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_52.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_53.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_54.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_55.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_56.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_57.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_58.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_59.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_6.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_60.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_61.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_62.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_63.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_64.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_65.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_66.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_67.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_68.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_69.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_7.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_70.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_71.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_72.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_73.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_74.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_75.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_76.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_77.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_78.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_79.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_8.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_80.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_81.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_82.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_83.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_84.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_85.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_86.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_87.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_88.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_89.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_9.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_90.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_91.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_92.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_93.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_94.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_95.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_96.root',
    #'/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_97.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_98.root',
    '/store/user/musich/test_out/test_2015_03_04_10_37_20_DATA_ReReco_testReReco_mp1568PlusFPix/myTest_ReRecoCRUZET15SuperPointing_mp1568plusFPix_99.root',
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
#process.p2 = cms.Path(process.offlineBeamSpot*process.MuSkim*process.myanalysis)
