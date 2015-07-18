import FWCore.ParameterSet.Config as cms 
maxEvents = cms.untracked.PSet( 
input = cms.untracked.int32(-1) 
) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend( [ 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/777/00000/CA00F5C3-2F66-E411-A338-02163E010CBC.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/861/00000/A61C184D-2C66-E411-8421-02163E010BEC.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/870/00000/5C5E385F-6161-E411-8B1D-02163E0105D1.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/871/00000/F407BA0B-3366-E411-B3A8-02163E010EBD.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/876/00000/A6AF967E-6864-E411-A139-02163E010E0C.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/877/00000/90FDB447-6061-E411-A5BE-02163E00FFD0.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/879/00000/265FE769-5F61-E411-9CB2-02163E00F035.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/880/00000/12045D49-5864-E411-97C4-02163E00F02E.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/888/00000/DC59BB8A-8C61-E411-9CC4-02163E008EDC.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/889/00000/E0D71101-E861-E411-992D-02163E008CD8.root', 
] ); 
