#! /bin/bash

for((i = 0; i < 180; i++))
do

root -b -q getphi_dpsim.C\(0,1,$i\)
#root -b -q getphi_dpsim.C\(0,0,$i\)

done

for((i = 0; i < 132; i++))
do

#root -b -q getphi_dpsim.C\(1,1,$i\)
#root -b -q getphi_dpsim.C\(1,0,$i\)

done


