CMSAuxCode: SiStripConditionsAnalysis
==========

Extra subsystem for CMSSW to analyze Strip Gains trends

Instructions to perform analysis
-----------

```
cmsrel CMSSW_8_0_25
cd CMSSW_8_0_25/src
cmsenv
git clone -b SiStripConditionsAnalysis git@github.com:mmusich/CMSAuxCode.git .
scramv1 b -j 8
cd SiStripConditionAnalysis/SiStripConditionsReader/test/
cmsenv

#### to analyze the Prompt conditions
cmsRun ConfFile_cfg.py GT=80X_dataRun2_Prompt_v15 rawFile=Prompt

#### to analyze the ReReco conditions
cmsRun ConfFile_cfg.py GT=80X_dataRun2_2016LegacyRepro_Candidate_v2 rawFile=ReReco

#### to get the comparison plots
cp -pr *.root ../macros/
cd ../macros/
root -b 
[0].L CompareGainsTrends.C++g
[1] CompareGainsTrends("GainTrend_Prompt.root","GainTrend_ReReco.root")
[2].q
```
