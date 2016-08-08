#! /bin/bash

for roadset in 57 59 61 62 67 68 69 70
do

echo "root.exe getphi.C\(1,1,${roadset}\)";
root.exe -b -q getphi.C\(1,0,${roadset}\)
root.exe -b -q getphi.C\(1,1,${roadset}\)
root.exe -b -q getphi.C\(1,2,${roadset}\)

done
