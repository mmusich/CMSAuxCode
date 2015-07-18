import FWCore.ParameterSet.Config as cms
maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
'/store/user/musich/test_out/test_2015_03_26_18_29_09_DATA_ReReco_week5only//myTest_ReRecoCRUZET15_Superpointing_mp1598_week5only_91.root',
'/store/user/musich/test_out/test_2015_03_26_18_29_09_DATA_ReReco_week5only//myTest_ReRecoCRUZET15_Superpointing_mp1598_week5only_92.root',
'/store/user/musich/test_out/test_2015_03_26_18_29_09_DATA_ReReco_week5only//myTest_ReRecoCRUZET15_Superpointing_mp1598_week5only_93.root',
'/store/user/musich/test_out/test_2015_03_26_18_29_09_DATA_ReReco_week5only//myTest_ReRecoCRUZET15_Superpointing_mp1598_week5only_94.root',
'/store/user/musich/test_out/test_2015_03_26_18_29_09_DATA_ReReco_week5only//myTest_ReRecoCRUZET15_Superpointing_mp1598_week5only_95.root',
'/store/user/musich/test_out/test_2015_03_26_18_29_09_DATA_ReReco_week5only//myTest_ReRecoCRUZET15_Superpointing_mp1598_week5only_96.root',
'/store/user/musich/test_out/test_2015_03_26_18_29_09_DATA_ReReco_week5only//myTest_ReRecoCRUZET15_Superpointing_mp1598_week5only_97.root',
'/store/user/musich/test_out/test_2015_03_26_18_29_09_DATA_ReReco_week5only//myTest_ReRecoCRUZET15_Superpointing_mp1598_week5only_98.root',
'/store/user/musich/test_out/test_2015_03_26_18_29_09_DATA_ReReco_week5only//myTest_ReRecoCRUZET15_Superpointing_mp1598_week5only_99.root',
]);
secFiles.extend( [
])