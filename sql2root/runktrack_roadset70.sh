#! /bin/bash

for((i = 0; i < 11; i++))
do

root -b -q getktrack.C\(\"cuts1489v1_merged_roadset57_R005_V001\",$i\)

done

