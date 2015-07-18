#!/bin/tcsh

set COUNT=0

foreach inputfile (`ls .`)
    #echo $inputfile
    set namebase=`echo $inputfile |awk '{split($0,b,"_"); print b[3]}'`
    if ("$namebase" =~ *"18"*) then
	echo $namebase 
    @ COUNT+=1
    endif  
end


