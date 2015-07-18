import FWCore.ParameterSet.Config as cms 
maxEvents = cms.untracked.PSet( 
input = cms.untracked.int32(-1) 
) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend( [ 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/734/00000/18DE5C44-2B66-E411-B6B2-02163E010FAE.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/744/00000/168AF1D0-9260-E411-AD6E-02163E008EB6.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/745/00000/2EFA368F-9360-E411-8598-02163E008EC5.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/748/00000/BA313D25-9360-E411-8042-02163E00A1E7.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/750/00000/443F0A00-4466-E411-9F45-02163E010DED.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/761/00000/F4F9AD8E-6066-E411-B298-02163E010ECC.root', 
] ); 
