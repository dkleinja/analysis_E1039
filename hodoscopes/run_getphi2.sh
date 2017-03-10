#! /bin/bash

for((i = 0; i < 148; i++))
do

root -b -q getphi_dpsim.C\(1,1,$i\)
#root -b -q getphi_dpsim.C\(1,0,$i\)

done


