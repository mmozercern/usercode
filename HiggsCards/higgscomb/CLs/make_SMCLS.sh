#!/bin/bash
if test -d $1; then MASS=$1; else echo "Usage: $0 mass [what ]"; exit 1; fi; 
cd $MASS
MATCH=$2;

OPTIONS="-M HybridNew --freq --optimizeSim=1 --singlePoint 1"

function run {
    WHAT=$1; shift
    NAM=$(echo $1 | sed -e s/comb_// -e s/.root//   | tr [a-z] [A-Z])
    BSP=${1/.root};
    if [[ "$MATCH" == "" || "$MATCH" == "$1" || "$MATCH" == "${NAM}" ]]; then
        if test -f $1; then
             if ls crab_0_${BSP}_SM*/res/*root > /dev/null 2>&1; then
                 echo "Runnining CLs for  limits for $NAM at $MASS. ";
                 ../hadd2 -f grid-$BSP.root crab_0_${BSP}_SM*/res/*root > /dev/null 2>&1;
                 GRID="--readHybridResult --toysFile=grid-$BSP.root"
                 combine $* -n ${NAM}_${WHAT} -m $MASS $GRID $OPTIONS > ${1/.root/.log.$WHAT} 2>&1 
                 for E in 50 16 025 84 975; do
                    combine $* -n ${NAM}_${WHAT} -m $MASS $GRID --expectedFromGrid 0.$E $OPTIONS  > ${1/.root/.log.$WHAT}_E0$E 2>&1
                 done;
                 grep '^CLs =' ${1/.root/.log.$WHAT}* | sed 's/:/\t/';
             else
                 echo "No grid ready for $NAM at $MASS";
             fi;
        fi;
    fi;
}
run SMCLS comb_hgg.root
run SMCLS comb_hww.root
run SMCLS comb_htt.root
run SMCLS comb_hzz4l.root
run SMCLS comb_hzz2l2nu.root
run SMCLS comb_hzz2l2q.root
run SMCLS comb.root
