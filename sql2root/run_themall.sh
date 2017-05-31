#! /bin/bash

for((i = 0; i < 41; i++))
do

root -b -q getktrack.C\(\"cuts1314_merged_roadset57_R004_V005\",$i\)

done

