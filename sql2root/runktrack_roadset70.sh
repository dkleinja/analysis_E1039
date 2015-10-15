#! /bin/bash

for((i = 0; i < 13; i++))
do

root -b -q getktrack3.C\(\"cuts1489v1_merged_roadset70_R005_V001\",$i\)

done

