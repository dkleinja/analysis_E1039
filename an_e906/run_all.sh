#! /bin/bash

for roadset in 57 59 61 62 67 70
do

echo "root.exe asym2.C\(0,1,${roadset}\)";
root.exe -b -q asym2.C\(0,1,${roadset}\)
echo "root.exe asym2.C\(0,5,${roadset}\)";
root.exe -b -q asym2.C\(0,5,${roadset}\)

done
