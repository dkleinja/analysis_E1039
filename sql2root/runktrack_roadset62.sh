#! /bin/bash

for((i = 0; i < 14; i++))
do

root -b -q getktrack.C\(\"cuts1489v1_merged_roadset62_R005_V001\",$i\)

done

