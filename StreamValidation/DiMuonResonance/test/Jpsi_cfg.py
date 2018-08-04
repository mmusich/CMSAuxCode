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

       '/store/mc/Spring14dr/JpsiToMuMu_JPsiPt20WithFSR_13TeV-pythia6-evtgen/ALCARECO/TkAlJpsiMuMu-PU_S14_POSTLS170_V6-v2/00000/0A5F30BA-5AF8-E311-9419-002590200A18.root',
       '/store/mc/Spring14dr/JpsiToMuMu_JPsiPt20WithFSR_13TeV-pythia6-evtgen/ALCARECO/TkAlJpsiMuMu-PU_S14_POSTLS170_V6-v2/00000/0C4D2630-31F8-E311-8E80-0025B3E063AC.root',
       '/store/mc/Spring14dr/JpsiToMuMu_JPsiPt20WithFSR_13TeV-pythia6-evtgen/ALCARECO/TkAlJpsiMuMu-PU_S14_POSTLS170_V6-v2/00000/32F4C95E-D7F8-E311-BDCF-001E67397396.root',
       '/store/mc/Spring14dr/JpsiToMuMu_JPsiPt20WithFSR_13TeV-pythia6-evtgen/ALCARECO/TkAlJpsiMuMu-PU_S14_POSTLS170_V6-v2/00000/568FD2FD-25F8-E311-B974-002590A81F32.root',
       '/store/mc/Spring14dr/JpsiToMuMu_JPsiPt20WithFSR_13TeV-pythia6-evtgen/ALCARECO/TkAlJpsiMuMu-PU_S14_POSTLS170_V6-v2/00000/86D306E2-25F8-E311-9E25-002481E14F5C.root' 

    ]
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.myanalysis = cms.EDAnalyzer("TrackAnalyzerNewTwoBodyHisto",
                                    TkTag = cms.string ('ALCARECOTkAlJpsiMuMu'),
                                    maxMass = cms.double(2.7),
                                    minMass = cms.double(3.4)
                                    )
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('myJpsiMuMu_JPsiPt20WithFSR_13TeV-pythia6-evtgen_V6-v2.root')
                                   )

process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring("cout"),
                                    cout = cms.untracked.PSet(threshold = cms.untracked.string('DEBUG'),
                                                              INFO = cms.untracked.PSet(reportEvery = cms.untracked.int32(10000))),
                                    
                                    )
process.p1 = cms.Path(process.offlineBeamSpot*process.myanalysis)
