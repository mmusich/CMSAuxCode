CMSAuxCode
==========

Extra subsystem for CMSSW

Recipe: 

scramv1 p CMSSW_7_1_0   
cd CMSSW_7_1_0/src/  
git clone -b forCMSSW_7_1_0 git@github.com:mmusich/CMSAuxCode.git .   
scramv1 b -j 8  
