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

       '/store/mc/Spring14dr/QCD_Pt-80to120_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/ALCARECO/TkAlMuonIsolated-PU_S14_POSTLS170_V6-v1/00000/08B92CF8-C8ED-E311-84CE-00261894392F.root',
       '/store/mc/Spring14dr/QCD_Pt-80to120_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/ALCARECO/TkAlMuonIsolated-PU_S14_POSTLS170_V6-v1/00000/7C3EAADB-AEEE-E311-80C2-002590596490.root',
       '/store/mc/Spring14dr/QCD_Pt-80to120_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/ALCARECO/TkAlMuonIsolated-PU_S14_POSTLS170_V6-v1/00000/86C04A48-A2EE-E311-B104-00261894397A.root',
       '/store/mc/Spring14dr/QCD_Pt-80to120_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/ALCARECO/TkAlMuonIsolated-PU_S14_POSTLS170_V6-v1/00000/8C7AD3E9-34EF-E311-9802-003048679164.root',
       '/store/mc/Spring14dr/QCD_Pt-80to120_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/ALCARECO/TkAlMuonIsolated-PU_S14_POSTLS170_V6-v1/00000/CAE79F67-C8ED-E311-A23E-002618FDA216.root'


    ]
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.myanalysis = cms.EDAnalyzer("TrackAnalyzerHisto",
                                    TkTag = cms.string ('ALCARECOTkAlMuonIsolated'),
                                    )
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('myQCD_Pt-800to1000_MuEnrichedPt5_TuneZ2star_13TeV_pythia6_V6-v1.root')
)
process.p1 = cms.Path(process.offlineBeamSpot*process.myanalysis)
