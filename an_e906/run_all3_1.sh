#! /bin/bash

echo -n "Enter the first number --> "
#read roadset
roadset=67;
echo -n "The roadset it ${roadset}"

#echo "root.exe asym2_2days.C\(0,1,${roadset},0,1\)";
#root.exe -b -q asym2_2days.C\(0,1,${roadset},0,1\)
#echo "root.exe asym2_2days.C\(0,5,${roadset},0,1\)";
#root.exe -b -q asym2_2days.C\(0,5,${roadset},0,1\)
for((i = 501; i < 1001; i++))
do
echo "$i"
echo "root.exe asym2_2days.C\(0,1,${roadset},$i\)";
root.exe -b -q asym2_2days.C\(0,1,${roadset},$i\)
echo "root.exe asym2_2days.C\(0,5,${roadset},$i\)";
root.exe -b -q asym2_2days.C\(0,5,${roadset},$i\)

done
