#!/bin/tcsh

set CMSSW_DIR=${CMSSW_BASE}/src/StreamValidation/DiMuonResonance/test
cd $CMSSW_DIR

set Scenarios = (DECO_OPT_0T \
		 DECO_OPT_38T \
                 DECO_PESS_0T \
                 DECO_PESS_38T \
	         PEAK_OPT_0T \
	         PEAK_OPT_38T \
	         PEAK_PESS_0T \
	         PEAK_PESS_38T )

set GTs = (COSM70_DEC_V2::All \
	   COSM70_DEC_V2::All \
	   COSM70_DEC_V1::All \
	   COSM70_DEC_V1::All \
	   COSM70_PEA_V2::All \
	   COSM70_PEA_V2::All \
	   COSM70_PEA_V1::All \
	   COSM70_PEA_V1::All )

set InputFiles = (file:/afs/cern.ch/work/l/lbeck/public/Cosmics_7_0_X_DR/DECO_OPT_0T/TkAlCosmics0T.root \
		  file:/afs/cern.ch/work/l/lbeck/public/Cosmics_7_0_X_DR/DECO_OPT_38T/TkAlCosmics0T.root \
		  file:/afs/cern.ch/work/l/lbeck/public/Cosmics_7_0_X_DR/DECO_PESS_0T/TkAlCosmics0T.root \
		  file:/afs/cern.ch/work/l/lbeck/public/Cosmics_7_0_X_DR/DECO_PESS_38T/TkAlCosmics0T.root \
		  file:/afs/cern.ch/work/l/lbeck/public/Cosmics_7_0_X_DR/PEAK_OPT_0T/TkAlCosmics0T.root \
		  file:/afs/cern.ch/work/l/lbeck/public/Cosmics_7_0_X_DR/PEAK_OPT_38T/TkAlCosmics0T.root \
		  file:/afs/cern.ch/work/l/lbeck/public/Cosmics_7_0_X_DR/PEAK_PESS_0T/TkAlCosmics0T.root \
		  file:/afs/cern.ch/work/l/lbeck/public/Cosmics_7_0_X_DR/PEAK_PESS_38T/TkAlCosmics0T.root )

set OutputFiles = (test_DECO_OPT_0T.root \
		   test_DECO_OPT_38T.root \
                   test_DECO_PESS_0T.root \
		   test_DECO_PESS_38T.root \
		   test_PEAK_OPT_0T.root \
                   test_PEAK_OPT_38T.root \
		   test_PEAK_PESS_0T.root  \
		   test_PEAK_PESS_38T.root)

foreach i (`seq $#Scenarios`) 

    echo "---------> submitting job $i with:"
    echo "   -Scenario   : $Scenarios[$i]"
    echo "   -GT         : $GTs[$i]"
    echo "   -Inputfile  : $InputFiles[$i]"
    echo "   -Outputfile : $OutputFiles[$i]"
  
    bsub -o tmp.tmp -q cmscaf1nd test_cosmics_TEMPL.lsf $Scenarios[$i] $GTs[$i] $InputFiles[$i] $OutputFiles[$i] 
end

