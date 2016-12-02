#!/bin/bash

for ntracks in 1
do
for momcut in 10 15 20 30
do
for ycut in 0 3
do
for centcut in 90 95 100
do

root -b -q hodoeff_calc_sim.cxx\($ntracks,$momcut,$ycut,$centcut\);

done
done
done
done
