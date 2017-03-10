#! /bin/bash

for tar in 1 2
do
for mag in 1 0
do
echo ./Merged -k 1 -target $tar -magnet $mag
./Merged -k 1 -target $tar -magnet $mag
done
done
