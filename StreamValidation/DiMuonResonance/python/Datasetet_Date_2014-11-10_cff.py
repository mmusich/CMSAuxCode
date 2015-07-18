import FWCore.ParameterSet.Config as cms 
maxEvents = cms.untracked.PSet( 
input = cms.untracked.int32(-1) 
) 
readFiles = cms.untracked.vstring() 
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles) 
readFiles.extend( [ 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/453/00000/10958190-CB69-E411-80F0-02163E010BD9.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/460/00000/CAB6ACC1-D469-E411-8D1B-02163E010BE2.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/492/00000/EA1C0480-CF69-E411-B352-02163E010CC7.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/514/00000/F414FFED-CD69-E411-B8F8-02163E010DF4.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/527/00000/3E090129-CB69-E411-82AB-02163E0100B4.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/528/00000/2CA60834-D869-E411-A512-02163E010D0F.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/530/00000/9C69C916-D969-E411-B6DA-02163E010EA2.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/548/00000/3CEDC937-DA69-E411-A694-02163E010ED3.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/550/00000/16425E33-DA69-E411-87B5-02163E010EB4.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/559/00000/C4598D94-D969-E411-871A-02163E010E21.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/569/00000/76524F69-D969-E411-9096-02163E010DC2.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/601/00000/AE63DAC5-DA69-E411-B426-02163E010DE8.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/621/00000/A201CA90-D969-E411-ACAB-02163E010E1C.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/632/00000/445E8C20-D969-E411-8964-02163E010ED5.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/653/00000/0A195DA6-D969-E411-96F9-02163E010CD2.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/665/00000/381229C0-DB69-E411-A00C-02163E010F2D.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/666/00000/82A0183F-DB69-E411-B811-02163E010CC2.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/667/00000/C6FBA737-DC69-E411-B24E-02163E00FFD0.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/669/00000/AEE1F20B-DB69-E411-8C5A-02163E010CBC.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/672/00000/96584780-DB69-E411-B55B-02163E010CC7.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/680/00000/420D78D6-DA69-E411-BC77-02163E010CD6.root', 
'/store/data/Commissioning2014/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v4/000/229/684/00000/C8BA3687-EC69-E411-92D9-02163E010D45.root', 
] ); 
