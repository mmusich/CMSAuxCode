#!/bin/tcsh

set CMSSW_DIR=${CMSSW_BASE}/src/StreamValidation/DiMuonResonance/test
cd $CMSSW_DIR

set theScenario = RealData
set GT = GR_P_V49::All

set datasets = (Dataset_Run_228734_cff \
		Dataset_Run_228744_cff \
		Dataset_Run_228745_cff \
		Dataset_Run_228748_cff \
		Dataset_Run_228750_cff \
		Dataset_Run_228761_cff \
		Dataset_Run_228777_cff \
		Dataset_Run_228861_cff \
		Dataset_Run_228870_cff \
		Dataset_Run_228871_cff \
		Dataset_Run_228876_cff \
		Dataset_Run_228877_cff \
		Dataset_Run_228879_cff \
		Dataset_Run_228880_cff \
		Dataset_Run_228888_cff \
		Dataset_Run_228889_cff \
                Dataset_Run_228905_cff \
		Dataset_Run_228906_cff \
		Dataset_Run_228907_cff \
		Dataset_Run_228909_cff \
		Dataset_Run_228910_cff \
		Dataset_Run_228911_cff \
		Dataset_Run_228912_cff \
		Dataset_Run_228915_cff \
		Dataset_Run_228928_cff \
		Dataset_Run_228929_cff \
		Dataset_Run_228936_cff \
		Dataset_Run_228938_cff \
		Dataset_Run_228944_cff \
		Dataset_Run_228952_cff \
		Dataset_Run_228958_cff \
		Dataset_Run_228967_cff \
		Dataset_Run_228968_cff \
		Dataset_Run_228969_cff \
		Dataset_Run_228978_cff \
		Dataset_Run_228979_cff \
		Dataset_Run_228980_cff \
		Dataset_Run_228981_cff \
		Dataset_Run_228982_cff \
		Dataset_Run_229007_cff \
		Dataset_Run_229059_cff \
		Dataset_Run_229063_cff \
		Dataset_Run_229065_cff \
		Dataset_Run_229068_cff \
		Dataset_Run_229069_cff \
		Dataset_Run_229070_cff \
		Dataset_Run_229073_cff \
		Dataset_Run_229074_cff \
		Dataset_Run_229076_cff \
		Dataset_Run_229084_cff \
		Dataset_Run_229094_cff \
		Dataset_Run_229102_cff \
		Dataset_Run_229103_cff \
		Dataset_Run_229106_cff \
		Dataset_Run_229109_cff \
		Dataset_Run_229111_cff \
		Dataset_Run_229112_cff \
		Dataset_Run_229114_cff \
		Dataset_Run_229115_cff \
		Dataset_Run_229117_cff \
		Dataset_Run_229120_cff \
		Dataset_Run_229122_cff \
		Dataset_Run_229140_cff \
		Dataset_Run_229142_cff \
		Dataset_Run_229151_cff \
		Dataset_Run_229152_cff \
		Dataset_Run_229162_cff \
		Dataset_Run_229164_cff \
		Dataset_Run_229167_cff \
		Dataset_Run_229183_cff \
		Dataset_Run_229207_cff \
		Dataset_Run_229211_cff \
		Dataset_Run_229218_cff \
		Dataset_Run_229220_cff \
		Dataset_Run_229221_cff \
		Dataset_Run_229230_cff \
		Dataset_Run_229233_cff \
		Dataset_Run_229240_cff \
		Dataset_Run_229271_cff \
		Dataset_Run_229279_cff \
		Dataset_Run_229290_cff \
		Dataset_Run_229351_cff \
		Dataset_Run_229355_cff \
		Dataset_Run_229381_cff \
		Dataset_Run_229398_cff \
		Dataset_Run_229405_cff \
		Dataset_Run_229409_cff \
		Dataset_Run_229415_cff \
		Dataset_Run_229427_cff \
		Dataset_Run_229428_cff \
		Dataset_Run_229430_cff \
		Dataset_Run_229435_cff \
		Dataset_Run_229441_cff \
		Dataset_Run_229452_cff \
		Dataset_Run_229453_cff \
		Dataset_Run_229460_cff \
		Dataset_Run_229492_cff \
		Dataset_Run_229514_cff \
		Dataset_Run_229527_cff \
		Dataset_Run_229528_cff \
		Dataset_Run_229530_cff \
		Dataset_Run_229548_cff \
		Dataset_Run_229550_cff \
		Dataset_Run_229559_cff \
		Dataset_Run_229569_cff \
		Dataset_Run_229601_cff \
		Dataset_Run_229621_cff \
		Dataset_Run_229632_cff \
		Dataset_Run_229653_cff \
		Dataset_Run_229665_cff \
		Dataset_Run_229666_cff \
		Dataset_Run_229667_cff \
		Dataset_Run_229669_cff \
		Dataset_Run_229672_cff \
		Dataset_Run_229680_cff \
		Dataset_Run_229684_cff \
		Dataset_Run_229685_cff \
		Dataset_Run_229695_cff \
		Dataset_Run_229699_cff \
		Dataset_Run_229702_cff \
		Dataset_Run_229708_cff \
		Dataset_Run_229709_cff \
		Dataset_Run_229712_cff )

foreach i (`seq $#datasets`) 

    set OutputFile = `echo  $datasets[$i]| sed "s/_cff/.root/g" |sed "s/Dataset_/test_cosmics_/g"`
    set Scenario   = $theScenario"_"`echo  $datasets[$i]| sed "s/_cff//g"`
    echo "---------> submitting job $i with:"
    echo "   -Scenario   : $Scenario"
    echo "   -GT         : $GT"
    echo "   -Dataset    : $datasets[$i]"
    echo "   -Outputfile : $OutputFile"
  
    bsub -o tmp.tmp -q cmscaf1nd test_cosmics_TEMPL.lsf $Scenario $GT $datasets[$i] $OutputFile 
end

