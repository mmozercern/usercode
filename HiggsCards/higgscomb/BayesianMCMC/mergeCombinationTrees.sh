#! /bin/bash

###the order:
#  ./mergeCombinationTrees.sh
#  ./mergeHarvestedCombinationTrees.sh
# compile and run harvestMCMC.C passing the name of the grand total output file

MASS=0

if [ $# -gt 0 ]
then
    MASS=$1
else
    echo "Required one input, the mass point"
    exit 1
fi

DIR="combine_Grav2l2q_MarkovChainMC_CMS_hzz2l2q_${MASS}_6channels"
LISTFILES_EXP=""
LISTFILES_OBS=""
LISTFILES_ASYMPT=""
STEMEXP="higgsCombineGrav2l2q.exp.MarkovChainMC.mH${MASS}."
STEMOBS="higgsCombineGrav2l2q.obs.MarkovChainMC.mH${MASS}."
STEMASYMPT="higgsCombineGrav2l2q.Asymptotic.mH${MASS}."

for file in $( /bin/ls "${MASS}/${DIR}/${STEMEXP}"[0-9]*root  )
  do
 # echo $file
  LISTFILES_EXP=${LISTFILES_EXP}" $file "
done

for file in $( /bin/ls "${MASS}/${DIR}/${STEMOBS}"[0-9]*root  )
  do
 # echo $file
  LISTFILES_OBS=${LISTFILES_OBS}" $file "
done

for file in $( /bin/ls "${MASS}/${DIR}/${STEMASYMPT}"[0-9]*root  )
  do
 # echo $file
  LISTFILES_ASYMPT=${LISTFILES_ASYMPT}" $file "
done

echo "Merging: $LISTFILES"
hadd ${MASS}/$DIR/${STEMEXP}"TOTAL.root" $LISTFILES_EXP
hadd ${MASS}/$DIR/${STEMOBS}"TOTAL.root" $LISTFILES_OBS
hadd ${MASS}/$DIR/${STEMASYMPT}"TOTAL.root" $LISTFILES_ASYMPT

mkdir harvestedTrees/
cp ${MASS}/$DIR/*"TOTAL.root" harvestedTrees/

