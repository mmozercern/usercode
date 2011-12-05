#!/bin/bash
if test -d $1; then MASS=$1; else echo "Usage: $0 mass [what ]"; exit 1; fi; 
cd $MASS
MATCH=$2;

function t2w {
    if [[ "$MATCH" == "" || "$MATCH" == "$1" ]]; then
        test -f $1 && test $1 -nt ${1/.txt/.root} && text2workspace.py -b $1
    fi;
}


t2w comb_hgg.txt
t2w comb_hww.txt
t2w comb_htt.txt
t2w comb_hzz4l.txt
t2w comb_hzz2l2nu.txt
t2w comb_hzz2l2q.txt
t2w comb.txt
echo "Done workspaces for $MASS"
