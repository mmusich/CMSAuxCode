import FWCore.ParameterSet.Config as cms 
maxEvents = cms.untracked.PSet( 
input = cms.untracked.int32(-1) 
) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend( [ 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/120/00000/18D1131D-2666-E411-8FF3-02163E010CAD.root', 
] ); 
