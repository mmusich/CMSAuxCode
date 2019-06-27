CMSAuxCode: TkAlConditionsAnalysis
==========

Extra subsystem for CMSSW to analyze Tracker APE trends


Instructions to perform 2017 Ultra-Legacy analysis
-----------

```
cmsrel CMSSW_11_0_0_pre2
cd CMSSW_11_0_0_pre2/src
cmsenv
git clone -b TkAlConditionsAnalysis git@github.com:mmusich/CMSAuxCode.git .
scramv1 b -j 8
cd TkAlConditionsAnalysis/TkAlConditionsReader/test/
cmsenv

#### to analyze the v1 APE version
cmsRun ConfFile_cfg.py GT=106X_dataRun2_v15 rawFile=ULReReco_v1 records=TrackerAlignmentErrorExtendedRcd:TrackerAlignmentExtendedErrors_2017_ultralegacy_v1

#### to analyze the v2 APE version
cmsRun ConfFile_cfg.py GT=106X_dataRun2_v15 rawFile=ULReReco_v2 records=TrackerAlignmentErrorExtendedRcd:TrackerAlignmentExtendedErrors_2017_ultralegacy_v2

#### to get the comparison plots
cp -pr *.root ../macros/
cd ../macros/
root -b
[0].L CompareAPETrends.C
[1] CompareAPETrends("APETrend_ULReReco_v1.root","APETrend_ULReReco_v2.root")
[2].q
```
