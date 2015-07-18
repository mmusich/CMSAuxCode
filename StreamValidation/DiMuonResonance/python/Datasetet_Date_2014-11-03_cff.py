import FWCore.ParameterSet.Config as cms 
maxEvents = cms.untracked.PSet( 
input = cms.untracked.int32(-1) 
) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend( [ 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/929/00000/1645F03F-6164-E411-BCA0-02163E00EF81.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/936/00000/F6476C89-B463-E411-A939-02163E008CD2.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/938/00000/60DCBFE3-1466-E411-864A-02163E010C64.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/944/00000/08577185-1C66-E411-99D8-02163E010CCF.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/952/00000/1879F1AD-8464-E411-A7C9-02163E008D0D.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/958/00000/762DBC31-1366-E411-84DD-02163E010231.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/967/00000/5C5AE65E-1C66-E411-82C3-02163E010DBB.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/968/00000/36FF9020-8864-E411-B881-02163E00EF84.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/969/00000/7659DA66-8664-E411-8CCA-02163E00ECD2.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/978/00000/10A15BBA-8464-E411-8769-02163E00D363.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/979/00000/A848B647-8564-E411-87A9-02163E00D1AC.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/980/00000/FE48C73C-1366-E411-BFFE-02163E010E7F.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/228/981/00000/3E9F95BE-8864-E411-A9BC-02163E009E94.root', 
] ); 
