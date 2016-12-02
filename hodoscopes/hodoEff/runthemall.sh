#!/bin/bash

for reco in 5 6 
do
for ntracks in 0 1 
do
for momcut in 10 15 20 30
do

root -b -q hodoeff_calc.cxx\($reco,$ntracks,$momcut,0,90\);

done
done
done
