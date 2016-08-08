#! /bin/bash

roadset=67
echo "root.exe asym2_bs.C\(0,1,${roadset},0,1\)";
root.exe -b -q asym2_bs.C\(0,1,${roadset},0,1\)
echo "root.exe asym2_bs.C\(0,5,${roadset},0,1\)";
root.exe -b -q asym2_bs.C\(0,5,${roadset},0,1\)
for((i = 1; i < 5001; i++))
do
echo "$i"
echo "root.exe asym2_bs.C\(0,1,${roadset},$i\)";
root.exe -b -q asym2_bs.C\(0,1,${roadset},$i\)
echo "root.exe asym2_bs.C\(0,5,${roadset},$i\)";
root.exe -b -q asym2_bs.C\(0,5,${roadset},$i\)

done
