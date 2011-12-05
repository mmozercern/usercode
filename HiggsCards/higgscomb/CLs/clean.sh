#! /bin/bash

for dir in $( ls -d * )
do
echo Moving in $dir
cd $dir
rm -f *.log
rm -f log*
rm -f CMS_hzz2l2q_${dir}*
rm -f batchScript*
rm -f model*
cd ..
done

