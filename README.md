CMSAuxCode
==========

Extra subsystem for CMSSW

Recipe:

scramv1 p CMSSW_10_2_5_patch1
cd CMSSW_10_2_5_patch1/src/  
git clone -b forCMSSW_10_2_X git@github.com:mmusich/CMSAuxCode.git .  
scramv1 b -j 8   
