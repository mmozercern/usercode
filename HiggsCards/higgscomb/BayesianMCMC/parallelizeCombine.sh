#! /bin/bash

NMAXJOBS=40
mass=$1
queue="dummyqueue"
OUTDIR="./"

if [ $# -gt 1 ]
then
    queue=$2
else
    queue=8nh
fi

ijob=1
chmod 744 combine_exec.sh

while [ $ijob -le $NMAXJOBS ]
do
  myrand=$RANDOM #random number generator (short integer: [0-32767])
  JOBNAME="combine_${myrand}"
  LOGFILE="log_batch_combine_${myrand}.out"
  bsub -q $queue -J $JOBNAME -oo ${OUTDIR}/${mass}/$LOGFILE combine_exec.sh $myrand $mass
  let ijob=ijob+1
done


