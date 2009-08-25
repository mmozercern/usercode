#!/bin/bash

pset="pset.py"
helpmacro="fwlitefunctions.C"
mainmacro="fwlitegridtest.C"

#echo "============================"
#cat $pset
#echo "============================"

# extract the number of events to run on
python << EOF > nrevents.tmp
execfile("$pset")
print process.maxEvents.input
EOF
nrevents=`cat nrevents.tmp | cut -d"(" -f2-2 | cut -d")" -f1-1`
# extract the number of events to skip
python << EOF > nrevents.tmp
execfile("$pset")
print process.source.skipEvents
EOF
skipevents=`cat nrevents.tmp | cut -d"(" -f2-2 | cut -d")" -f1-1`
# dump number of events in a tmp file
echo $nrevents > nrevents.tmp
echo $skipevents >> nrevents.tmp
echo "===== Number of events ====="
echo $nrevents
echo $skipevents
echo "============================"


# compile and run the macro(s)
root -b << EOF
  .L RooMuBackground.cxx+
  .L RooEBackground.cxx+
  .L TreeLoader.cxx+
  .L DataSetHelper.cxx+
  .L ResultCollection.cxx+
  .L Model.cxx+
  .L lookelsewhere.C
  test(1000)
EOF

rm -f nrevents.tmp
rm -f inputfiles.tmp
