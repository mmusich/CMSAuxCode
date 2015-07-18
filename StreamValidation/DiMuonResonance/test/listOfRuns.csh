#!/bin/tcsh

set myAli=$1

if( $2 == 0 ) then
    set cwd=.
else
    set cwd=$2
endif
    
if( $3 == "a") then
     ## MinBias 2013A 
    set ARRAY=(211739 211740 211752 211754 211760 211765 211769 211770 211771 211772 211777 211778 211783 211784 211788 211792 211794 211797 211800 211804 211812 211821 211822 211823 211831 212029)

else 
    #PPMinBias 2013A
    set ARRAY=(211739 211740 211752 211760 211765 211792 211797 211812 211821 211822 211823 211831)
endif

foreach i (`seq $#ARRAY`)
    echo "------ creating > $ARRAY[$i] dat file "
    cat ${cwd}/InputSource_${myAli}_tpl | sed "s?<DATE>?$ARRAY[$i]?g" > ${cwd}/InputSource_${myAli}_$ARRAY[$i].dat 
end
mv ${cwd}/InputSource_${myAli}_tpl ../TPLS







