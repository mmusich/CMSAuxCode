import FWCore.ParameterSet.Config as cms 
maxEvents = cms.untracked.PSet( 
input = cms.untracked.int32(-1) 
) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend( [ 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/073/00000/6885E87F-1766-E411-9DAB-02163E0100B6.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/074/00000/00039D2E-2666-E411-A687-02163E010EDF.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/076/00000/8A406F44-2166-E411-89F7-02163E00FD8E.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/084/00000/A8FE1ED7-2566-E411-BFCF-02163E01068C.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/094/00000/CA37935C-2666-E411-BE92-02163E00FEE7.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/102/00000/A6DE113D-2666-E411-9273-02163E010EAE.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/103/00000/C8C1FEE1-2566-E411-B23C-02163E0100B4.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/106/00000/90B618A7-2566-E411-95DF-02163E00FC55.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/109/00000/768044EE-2066-E411-B8CC-02163E00FD8E.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/111/00000/E8790BB2-2566-E411-B2E4-02163E010EB9.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/112/00000/7C68960D-2166-E411-93C6-02163E01004F.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/114/00000/CCB8F7B3-1E66-E411-B157-02163E010E3A.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/115/00000/FCD764A5-2066-E411-978A-02163E00FAC1.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/117/00000/BAB266B5-2566-E411-8EF0-02163E010BD8.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/120/00000/18D1131D-2666-E411-8FF3-02163E010CAD.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/122/00000/E633D638-2666-E411-B521-02163E010CAD.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/140/00000/9E77F2C1-1666-E411-9F8D-02163E010CB8.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/142/00000/6C8A88D1-1766-E411-838A-02163E010BE0.root', 
] ); 
