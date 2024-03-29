#! /bin/bash

startdir=$( pwd )
NMAXJOBS=30
mass=$1
queue="dummyqueue"
OUTDIR="./"
LOGDIRNAME="logs"
mkdir -p ${OUTDIR}/${mass}/${LOGDIRNAME}

if [ $# -gt 1 ]
then
    queue=$2
else
    queue=1nd
fi

ijob=1
chmod 744 combine_exec.sh

while [ $ijob -le $NMAXJOBS ]
do
  myrand=$RANDOM #random number generator (short integer: [0-32767])
  JOBNAME="combine_${myrand}"
  LOGFILE="${LOGDIRNAME}/log_batch_combine_${myrand}.out"
  bsub -q $queue -J $JOBNAME -oo ${OUTDIR}/${mass}/$LOGFILE ${startdir}/combine_exec.sh $myrand $mass
  let ijob=ijob+1
done


