import FWCore.ParameterSet.Config as cms

process = cms.Process("AlcarecoAnalysis")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
process.load("Configuration.StandardSequences.Services_cff")
#process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'START71_V8A::All'

process.source = cms.Source ("PoolSource",fileNames =  cms.untracked.vstring())
process.source.fileNames = [

       '/store/mc/Spring14dr/Upsilon1SToMuMu_13TeV_starlight/ALCARECO/TkAlUpsilonMuMu-PU_S14_POSTLS170_V6-v1/00000/127E37F9-4AF0-E311-9A84-1CC1DE1D03FC.root',
       '/store/mc/Spring14dr/Upsilon1SToMuMu_13TeV_starlight/ALCARECO/TkAlUpsilonMuMu-PU_S14_POSTLS170_V6-v1/00000/5CA7A0C6-4BF0-E311-9079-00266CFFBF4C.root' 


    ]
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.myanalysis = cms.EDAnalyzer("TrackAnalyzerNewTwoBodyHisto",
                                    TkTag = cms.string ('ALCARECOTkAlUpsilonMuMu'),
                                    maxMass = cms.double(8.9),
                                    minMass = cms.double(9.9)
                                    )
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('myUpsilon1SToMuMu_13TeV_V6-v1.root')
)
process.MessageLogger = cms.Service("MessageLogger",
         destinations = cms.untracked.vstring(
                        "cout"
                        ),
         cout = cms.untracked.PSet(
         threshold = cms.untracked.string('DEBUG'),
         INFO = cms.untracked.PSet(
              reportEvery = cms.untracked.int32(10000))
         ),

)
process.p1 = cms.Path(process.offlineBeamSpot*process.myanalysis)
