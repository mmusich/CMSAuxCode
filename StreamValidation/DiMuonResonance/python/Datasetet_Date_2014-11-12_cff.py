import FWCore.ParameterSet.Config as cms 
maxEvents = cms.untracked.PSet( 
input = cms.untracked.int32(-1) 
) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend( [ 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/748/00000/DE750313-526C-E411-822F-02163E010DBD.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/760/00000/4A7AC7AF-806C-E411-A1D4-02163E00FC32.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/761/00000/6686E286-846C-E411-9784-02163E010CC2.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/775/00000/9E98E413-C070-E411-BDED-02163E010C89.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/781/00000/B4E181BA-806C-E411-B5C0-02163E010CC8.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/783/00000/16D2AD31-BE70-E411-83D6-02163E010EFB.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/791/00000/16410C10-BD70-E411-89E0-02163E010E4B.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/821/00000/C2DE3382-836C-E411-AC16-02163E010E0D.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/824/00000/6239C412-816C-E411-A308-02163E010EE4.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/825/00000/7E74BA86-B670-E411-BC28-02163E010BDD.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/827/00000/DCC0F081-846C-E411-84C8-02163E010DBE.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/828/00000/08D639D7-816C-E411-B753-02163E010F05.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/829/00000/94D60FF6-826C-E411-93BE-02163E010F47.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/830/00000/847157A8-856C-E411-9E9C-02163E010DBE.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/831/00000/667CFF0C-876C-E411-A5B0-02163E010EBC.root', 
] ); 
