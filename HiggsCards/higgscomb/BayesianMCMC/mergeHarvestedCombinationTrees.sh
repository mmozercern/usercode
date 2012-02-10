#! /bin/bash

###the order:
#  ./mergeCombinationTrees.sh
#  ./mergeHarvestedCombinationTrees.sh
# compile and run harvestMCMC.C passing the name of the grand total output file


LISTFILES_EXP=""
LISTFILES_OBS=""
LISTFILES_ASYMPT=""
STEMEXP="higgsCombineGrav2l2q.exp.MarkovChainMC."
STEMOBS="higgsCombineGrav2l2q.obs.MarkovChainMC."
STEMASYMPT="higgsCombineGrav2l2q.Asymptotic."

for file in $( /bin/ls "harvested_20120209//${STEMEXP}"*TOTAL.root  )
  do
 # echo $file
  LISTFILES_EXP=${LISTFILES_EXP}" $file "
done

for file in $( /bin/ls "harvested_20120209//${STEMOBS}"*TOTAL.root  )
  do
 # echo $file
  LISTFILES_OBS=${LISTFILES_OBS}" $file "
done

for file in $( /bin/ls "harvested_20120209//${STEMASYMPT}"*TOTAL.root  )
  do
 # echo $file
  LISTFILES_ASYMPT=${LISTFILES_ASYMPT}" $file "
done

echo "Merging: $LISTFILES"
hadd ./${STEMEXP}"_20120209.TOTAL.root" $LISTFILES_EXP
hadd ./${STEMOBS}"_20120209.TOTAL.root" $LISTFILES_OBS
hadd ./${STEMASYMPT}"_20120209.TOTAL.root" $LISTFILES_ASYMPT