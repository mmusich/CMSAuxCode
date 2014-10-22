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

#process.GlobalTag.globaltag = 'DESIGN72_V1::All'
process.GlobalTag.globaltag = 'START71_V8A::All'

process.source = cms.Source ("PoolSource",fileNames =  cms.untracked.vstring())
process.source.fileNames = [
    '/store/mc/Spring14dr/WToMuNu_Tune4C_13TeV-pythia8/ALCARECO/TkAlMuonIsolated-PU20bx25_POSTLS170_V5-v1/00000/2A6E789C-EFC8-E311-90EE-002481E107A8.root',
    '/store/mc/Spring14dr/WToMuNu_Tune4C_13TeV-pythia8/ALCARECO/TkAlMuonIsolated-PU20bx25_POSTLS170_V5-v1/00000/90E53999-EFC8-E311-80D4-002481E0DAB0.root' 
    #'/store/mc/Spring14dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/ALCARECO/TkAlMuonIsolated-castor_PU20bx25_POSTLS170_V5-v1/00000/02A475B4-FFEF-E311-B15C-002590A36FB2.root'
    ]

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

import Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi

process.MuSkimSelector = Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi.AlignmentTrackSelector.clone(
    applyBasicCuts = True,                                                                            
    filter = True,
    src = 'ALCARECOTkAlMuonIsolated',
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

process.myanalysis = cms.EDAnalyzer("TrackAnalyzerHisto",
                                    TkTag = cms.string ('ALCARECOTkAlMuonIsolated'),
                                    maxMass = cms.double(115),
                                    minMass = cms.double(65)
                                    )
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('myDYToMuMu_M-50_Tune4C_13TeV-pythia8_V6-v1.root')
                                   )

process.MessageLogger = cms.Service("MessageLogger",
         destinations = cms.untracked.vstring(
                        "cout"
                        ),
         cout = cms.untracked.PSet(
         threshold = cms.untracked.string('DEBUG'),
         INFO = cms.untracked.PSet(
              reportEvery = cms.untracked.int32(100))
         ),

)

process.p1 = cms.Path(process.offlineBeamSpot*process.myanalysis)
#process.p2 = cms.Path(process.offlineBeamSpot*process.MuSkim*process.myanalysis)
