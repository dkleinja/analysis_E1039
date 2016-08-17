#! /bin/bash

echo -n "Enter the first number --> "
#read roadse
roadset=${BASH_ARGV[1]}
seed=${BASH_ARGV[0]}
#echo -n "The roadset is ${roadset}"
#echo "${BASH_ARGV[0]}"
if(seed = 1)
then
echo "root.exe asym2_thirdday.C\(0,1,${roadset},0,1\)";
root.exe -b -q asym2_thirdday.C\(0,1,${roadset},0,1\)
echo "root.exe asym2_thirdday.C\(0,5,${roadset},0,1\)";
root.exe -b -q asym2_thirdday.C\(0,5,${roadset},0,1\)
fi
#for((i = 1; i < 501; i++))
for((i = seed; i < seed+501; i++))
do
echo "$i"
echo "root.exe asym2_thirdday.C\(0,1,${roadset},$i\)";
root.exe -b -q asym2_thirdday.C\(0,1,${roadset},$i\)
echo "root.exe asym2_thirdday.C\(0,5,${roadset},$i\)";
root.exe -b -q asym2_thirdday.C\(0,5,${roadset},$i\)

done
