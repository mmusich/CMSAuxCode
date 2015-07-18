import FWCore.ParameterSet.Config as cms 
maxEvents = cms.untracked.PSet( 
input = cms.untracked.int32(-1) 
) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend( [ 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/982/00000/084419A1-2066-E411-BC80-02163E00FC87.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/007/00000/783AF5FF-1066-E411-9DA0-02163E010C0C.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/059/00000/E6372487-1C66-E411-8D4A-02163E0104A3.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/063/00000/BE64C016-1E66-E411-9166-02163E010ECC.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/065/00000/9C2DC1AD-1166-E411-A00C-02163E010F10.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/068/00000/F84EC369-1C66-E411-8FE9-02163E00FEA5.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/069/00000/7E938865-1366-E411-9863-02163E010BE9.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/070/00000/9EE0ABFA-2466-E411-9E5E-02163E010CEF.root', 
] ); 
