import FWCore.ParameterSet.Config as cms 
maxEvents = cms.untracked.PSet( 
input = cms.untracked.int32(-1) 
) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend( [ 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/230/00000/8C364165-0367-E411-9CBB-02163E010E89.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/233/00000/38362558-0367-E411-93E5-02163E010C5A.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/240/00000/7C58BFE5-0167-E411-91AC-02163E010F1A.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/271/00000/4A9C5E49-0167-E411-A3D1-02163E010BDD.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/279/00000/88389972-FF66-E411-AA80-02163E010F6C.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/290/00000/544A2EB7-0167-E411-A9A6-02163E010DDA.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/351/00000/72F27649-0167-E411-9D76-02163E010BDD.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/355/00000/48D58AA1-FF66-E411-8F0B-02163E0100EB.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/381/00000/56823945-0167-E411-8F63-02163E010D42.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/398/00000/FC86D549-0167-E411-9794-02163E010F89.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/405/00000/A4B3624A-0367-E411-96E2-02163E0100E6.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/409/00000/E267B330-0267-E411-A799-02163E00FD98.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/415/00000/14B4A465-0367-E411-AC8A-02163E010F0A.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/427/00000/EEF1B6C1-FF66-E411-943C-02163E00FE65.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/428/00000/9C61987C-FF66-E411-9B9C-02163E010E1C.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/430/00000/06435EAF-8367-E411-A4FD-02163E010CFC.root', 
] ); 
