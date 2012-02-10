#! /bin/bash

myrand=$1
mass=$2
OUTDIR=DataCards
echo "Starting HiggsCombination with seed=$myrand at $( date +%c ) on $hostname."

startdir=$( pwd )

#set CMSSW environment
RELEASEDIR=/afs/cern.ch/user/b/bonato/scratch0/PhysAnalysis/devel/NewSpinPar/CMGTools/CMSSW_4_2_3/src/

algo="MarkovChainMC"
#algo="HybridNew"
#algo="ProfileLikelihood"
hint="ProfileLikelihood" # before the algo method, run the hint method for restricting integration field
label="Grav2l2q"
ntoys=5
#WORKDIR=${RELEASEDIR}/HiggsAna/HLLJJCommon/test/fits//${OUTDIR}/${mass}
WORKDIR=/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN2/HIP/bonato/XtoZZto2L2J/${OUTDIR}/${mass}
datacard="CMS_hzz2l2q_${mass}_6channels" #"counting-twochannel-2l2j.txt"  
OUTDIR="combine_${label}_${algo}_"${datacard}

cd $RELEASEDIR
export SCRAM_ARCH=slc5_amd64_gcc434
#cmsenv
eval `scramv1 runtime -sh`
cd $startdir

TMPDIR="/tmp/$(whoami)"
mkdir ${TMPDIR}/combine_${myrand}
cd $TMPDIR/combine_${myrand}
cp $WORKDIR/*input*root .
echo "I am in $( pwd ) (it should be: $TMPDIR/combine_${myrand} )"
echo


if [ ! -d ${WORKDIR}/$OUTDIR/ ]
    then
    mkdir ${WORKDIR}/$OUTDIR/
fi
echo "Datacard: $datacard"
# if algo=HybridNew
#combine -M $algo -n $label -m 400 -s $myrand -t $ntoys -U  -d $WORKDIR/$datacard --freq --singlePoint 1

#if algo="Asymptotic"  ###-t $ntoys
combine -M "Asymptotic" -n $label -m $mass  -s $myrand -d $WORKDIR/"${datacard}.txt" -U 


#if algo="ProfileLikelihood"
combine -M "ProfileLikelihood" -n $label -m $mass  -s $myrand  -d $WORKDIR/"${datacard}.txt" -U  -t $ntoys

#if algo="MarkovChainMC" 
#expected
echo
echo "*********************************************"
echo "Starting calculation of expected limit with Bayesian MCMC"
echo
echo
labelExp=${label}".exp"
combine -M $algo -n $labelExp -m $mass  -s $myrand -d $WORKDIR/"${datacard}.txt"  -H $hint  -t $ntoys -U
#observed
echo
echo "*********************************************"
echo "Starting calculation of observed limit with Bayesian MCMC"
echo
echo
labelObs=${label}".obs"
combine -M $algo -n $labelObs -m $mass  -s $myrand -d $WORKDIR/"${datacard}.txt"  -H $hint  -U

### if you have some library/class not in the cvs package, compile it and add somehting like this
# -L  /afs/cern.ch/user/s/sbologne/scratch0/CMSSW/CMSSW_4_2_4/src/HiggsAnalysis/CombinedLimit/test/rotatedEps/PDFs/RooCB_cc.so 



#if algo="ProfileLikelihood"
#combine -M $algo   -n $label -m $mass  -s $myrand  -d $WORKDIR/$datacard -U  -t $ntoys

echo "List of files in $( pwd ):"
ls -lh
echo "Moving files into ${WORKDIR}/$OUTDIR/"
mv $TMPDIR/combine_${myrand}/higgsCombine${label}*.root  ${WORKDIR}/$OUTDIR/
###mv $TMPDIR/combine_${myrand}/log_combine_${label}_${mass}.${myrand}.out  ${WORKDIR}/$OUTDIR/
