import FWCore.ParameterSet.Config as cms

process = cms.Process("AlcarecoAnalysis")
###################################################################
def customiseAlignmentAndAPE(process):
###################################################################
    if not hasattr(process.GlobalTag,'toGet'):
        process.GlobalTag.toGet=cms.VPSet()
    process.GlobalTag.toGet.extend( cms.VPSet(cms.PSet(record = cms.string("TrackerAlignmentRcd"),
                                                       tag = cms.string("Alignments"),
                                                       connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction/um0001/jobData/jobm/um0001.db")
                                                       ),
                                              cms.PSet(record = cms.string("TrackerAlignmentErrorExtendedRcd"),
                                                       tag = cms.string("TrackerAlignmentExtendedErrors_v9_offline_IOVs"),
                                                       connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
                                                       ),
                                              cms.PSet(record = cms.string('SiPixelTemplateDBObjectRcd'), 
                                                       tag = cms.string('SiPixelTemplateDBObject_38T_v15_offline'), 
                                                       connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
                                                       )                                                
                                              )
                                    )
    return process

###################################################################
def customiseKinksAndBows(process):
###################################################################
     if not hasattr(process.GlobalTag,'toGet'):
          process.GlobalTag.toGet=cms.VPSet()
     process.GlobalTag.toGet.extend(cms.VPSet(cms.PSet(record = cms.string("TrackerSurfaceDeformationRcd"),
                                                       tag = cms.string("Deformations"),
                                                       connect = cms.string("sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction/um0001/jobData/jobm/um0001.db")
                                                       ),        
                                              )
                                    )
     return process



process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("CondCore.CondDB.CondDB_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_dataRun2_ReReco_EOY17_v2', '')
process.GlobalTag.DumpStat = cms.untracked.bool(True)

process=customiseAlignmentAndAPE(process)
process=customiseKinksAndBows(process)

process.source = cms.Source ("PoolSource",fileNames =  cms.untracked.vstring())
process.source.fileNames = [
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/B09182A3-D7D6-E711-B2E3-C4346BC7EE18.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/A6CA7019-61D8-E711-AB4C-A0369F7FC070.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/2820D29D-D7D6-E711-BBCA-FA163E201524.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/6A8A44F1-D8D6-E711-92B3-FA163EB1BCF9.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/9AA2258D-D7D6-E711-AF0F-FA163E44548A.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/0670231E-D8D6-E711-BABB-02163E011F96.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/3CCA784E-D6D6-E711-9EA8-02163E01A6D4.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/E806D0C1-D7D6-E711-A87A-008CFAFBEA7E.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/92E7F820-1DD4-E711-A8B1-24BE05CECBE1.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/784C4168-A1D4-E711-B1B0-00266CFEFCE8.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/AA019E18-DBD3-E711-8892-008CFAC93F3C.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/4E664729-1DD4-E711-B3C2-D067E5F914D3.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/9C45E462-78D4-E711-9CD8-002590A80DFA.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/54119BC4-BDD4-E711-857C-001E67792768.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/E8FFEF4B-78D4-E711-B930-A4BF0112F7D0.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/74D92200-77D4-E711-AD62-0CC47AF9B2CA.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/30D621E2-78D4-E711-AB2E-0025905C53F0.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/4E01DE87-E2D8-E711-A22B-001E67396E28.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/D0B24E61-BED4-E711-8E42-02163E01A792.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/A83F6E00-78D4-E711-BD53-02163E01A62F.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/F08CB9C6-9ED4-E711-8AE8-008CFAF74A32.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/70B7840F-77D4-E711-AD7C-008CFAFBE0DC.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/0CCB8694-1DD4-E711-9AA6-1866DA7F9265.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/18B0A08E-1DD4-E711-9A06-141877448B91.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/3666E1A6-1DD4-E711-A002-00259048A8F4.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/5CC44F55-1DD4-E711-8D6C-0CC47AA989C2.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/06E502A1-10D6-E711-8C02-001E6779264E.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/8CE65624-1DD4-E711-9B22-0025905B8562.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/B6380C7F-1DD4-E711-A05B-0025905B859E.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/30000/26A6FFD5-E7D7-E711-879E-0025905C3DD0.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/72C4A228-1DD4-E711-B602-782BCB20D86B.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/86F24FA3-9ED4-E711-9D64-FA163E71A241.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/EAA2607D-9FD4-E711-923C-FA163E612A48.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/F6D55952-2DD5-E711-A118-02163E01659A.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/24A0C5C1-BED4-E711-86A5-FA163E498FC0.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/FC609152-2DD5-E711-9118-02163E01309D.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/50000/9A67A6C5-F1D4-E711-8B6D-3417EBE480D1.root',
    '/store/data/Run2017B/DoubleMuon/ALCARECO/TkAlZMuMu-17Nov2017-v1/40000/80649910-1DD4-E711-9CFF-001E67DDC0FB.root'
    ]

###################################################################
# The TrackRefitter
###################################################################
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
import RecoTracker.TrackProducer.TrackRefitters_cff
process.TrackRefitter1 = process.TrackRefitterP5.clone(
    src =  'ALCARECOTkAlZMuMu', #'AliMomConstraint1',
    TrajectoryInEvent = True,
    TTRHBuilder = "WithAngleAndTemplate",
    NavigationSchool = "",
    #constraint = 'momentum', ### SPECIFIC FOR CRUZET
    #srcConstr='AliMomConstraint1' ### SPECIFIC FOR CRUZET$works only with tag V02-10-02 TrackingTools/PatternTools / or CMSSW >=31X
    )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100000))
process.myanalysis = cms.EDAnalyzer("TrackAnalyzerNewTwoBodyHisto",
                                    #TkTag = cms.string ('ALCARECOTkAlZMuMu'),
                                    TkTag = cms.string ('TrackRefitter1'),
                                    maxMass = cms.double(80),
                                    minMass = cms.double(120),
                                    verbose_fit = cms.untracked.bool(False)
                                    )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('myZMuMu_new.root')
                                   )

# process.MessageLogger = cms.Service("MessageLogger",
#                                     destinations = cms.untracked.vstring("cout"),
#                                     cout = cms.untracked.PSet(threshold = cms.untracked.string('DEBUG'),
#                                                               INFO = cms.untracked.PSet(reportEvery = cms.untracked.int32(10000))),
                                    
#                                     )

process.p1 = cms.Path(process.offlineBeamSpot*
                      process.TrackRefitter1*
                      process.myanalysis)
