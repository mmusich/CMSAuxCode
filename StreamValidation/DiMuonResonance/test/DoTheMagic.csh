#!/bin/tcsh

set ARRAY=(ppRun2013AMinBias2012Repro ppRun2013AMinBiasPromptReco ppRun2013APPMinBias2012Repro ppRun2013APPMinBiasPromptReco ppRun2013AMinBias2012ReproNoJSONPt5 ppRun2013AMinBiasPromptRecoNoJSONPt5 ppRun2013APPMinBias2012ReproNoJSONPt5 ppRun2013APPMinBiasPromptRecoNoJSONPt5)

foreach i (`seq $#ARRAY`)
    echo "---- I am preparing sample $ARRAY[$i]"

    if(! -d $ARRAY[$i]) then
	echo "$ARRAY[$i] does not exist yet: creating it"
	mkdir $ARRAY[$i]
    endif

    rm -fr ./$ARRAY[$i]/*   
    cp -pr ../TPLS/InputSource_$ARRAY[$i]_tpl ./$ARRAY[$i]

    if ($ARRAY[$i] =~ *PPMinBias*) then
	./listOfRuns.csh $ARRAY[$i] $ARRAY[$i] b
    else 
	./listOfRuns.csh $ARRAY[$i] $ARRAY[$i] a
    endif

end







