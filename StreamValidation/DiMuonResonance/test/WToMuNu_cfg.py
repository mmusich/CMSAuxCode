import FWCore.ParameterSet.Config as cms

#process = cms.Process("Refitting")
process = cms.Process("AlcarecoAnalysis")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
process.load("Configuration.StandardSequences.Services_cff")
#process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'START71_V8A::All'

process.source = cms.Source ("PoolSource",fileNames =  cms.untracked.vstring())
process.source.fileNames = [

       '/store/mc/Spring14dr/WToMuNu_Tune4C_13TeV-pythia8/ALCARECO/TkAlMuonIsolated-PU_S14_POSTLS170_V6-v1/00000/3CE998B8-A3C8-E311-8307-002590DB040C.root',
       '/store/mc/Spring14dr/WToMuNu_Tune4C_13TeV-pythia8/ALCARECO/TkAlMuonIsolated-PU_S14_POSTLS170_V6-v1/00000/C29F51C3-A3C8-E311-B8A4-002481E0E912.root' 


    ]
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.myanalysis = cms.EDAnalyzer("TrackAnalyzerHisto",
                                    TkTag = cms.string ('ALCARECOTkAlMuonIsolated'),
                                    )
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('myWToMuNu_Tune4C_13TeV-pythia8_V6-v1.root')
)
process.p1 = cms.Path(process.offlineBeamSpot*process.myanalysis)
