#!/bin/bash
if [[ "$1" == "" ]]; then echo "Usage: $0 mass"; exit 1; fi;
if test -d $1; then MASS=$1; else echo "Usage: $0 mass"; exit 1; fi; 
cd $MASS
WHAT=$2

COMB=""
if (( $MASS <= 140)); then test -L hgg_8cats.txt || cp -s ../common/hgg_8cats.txt . -v; fi

#if expr index $MASS .5 > /dev/null; then MASSD="$MASS"; else MASSD="$MASS.0"; fi
if echo $M | grep -F -q .5; then MASSD="$MASS"; else MASSD="$MASS.0"; fi

HWW="hww_0j_shape=hww_0j_shape.txt  hww_1j_shape=hww_1j_shape.txt hww_2j_cut=hww_2j_cut.txt"
HZZ4L="hzz4l_2e2mu=hzz4l_2e2muS.${MASSD}.txt hzz4l_4e=hzz4l_4eS.${MASSD}.txt hzz4l_4mu=hzz4l_4muS.${MASSD}.txt"
HZZ2Q="hzz2l2q_ee0b=hzz2l2q_ee0b.${MASS}.txt  hzz2l2q_ee1b=hzz2l2q_ee1b.${MASS}.txt  hzz2l2q_ee2b=hzz2l2q_ee2b.${MASS}.txt "
HZZ2Q="hzz2l2q_mm0b=hzz2l2q_mm0b.${MASS}.txt  hzz2l2q_mm1b=hzz2l2q_mm1b.${MASS}.txt  hzz2l2q_mm2b=hzz2l2q_mm2b.${MASS}.txt $HZZ2Q"
HZZ2N="hzz2l2nu_ee=hzz2l2nu_ee_${MASS}.txt  hzz2l2nu_mm=hzz2l2nu_mm_${MASS}.txt"
HTT="htt_em=htt_em_cmb.txt htt_et_0=htt_et_0.txt htt_et_1=htt_et_1.txt  htt_mt_0=htt_mt_0.txt  htt_mt_1=htt_mt_1.txt"

## Gamma Gamma
if (( $MASS <= 140 )); then 
    COMB="$COMB hgg=hgg_8cats.txt"; 
    test -L comb_hgg.txt || cp -s hgg_8cats.txt comb_hgg.txt; 
fi

## Tau Tau
if (( $MASS <= 140 )); then
    combineCards.py -S $HTT > comb_htt.txt
    COMB="$COMB $HTT"
fi

if (( $MASS >= 115)); then 
    ## W W
    COMB="$COMB $HWW"
    combineCards.py -S $HWW > comb_hww.txt
fi;

## ZZ 4L
COMB="$COMB $HZZ4L"
combineCards.py -S $HZZ4L > comb_hzz4l.txt

## ZZ 2Q
#if (( $MASS >= 100)); then 
COMB="$COMB $HZZ2Q";
combineCards.py -S $HZZ2Q > comb_hzz2l2q.txt
#fi;

## ZZ 2N
if (( $MASS >= 200)); then 
   COMB="$COMB $HZZ2N";
   combineCards.py -S $HZZ2N > comb_hzz2l2nu.txt
fi;

combineCards.py -S $COMB > comb.txt

echo "Done cards for mass $MASS"
