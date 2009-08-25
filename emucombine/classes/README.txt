**************
*instructions*
**************
README.txt some basic descriptions on the files and procedures involved in the statisitcal analysis 
	   it's not complete, but should help you getting started.

PaperPlot.txt instructions how to reproduce the pltos from the  TeVMu+HEEP meeting on the 25th Aug. 09

************
*core tools*
************
Model.cxx, model.h contains the basic defintions that model the signal and background shapes, as well
	    as some logic to nudge MINUIT towards the global likelihood maximum
DataSetHelper.cxx, DataSetHelper.h contains the pseudoexperiments for the e, mu and combined channels
	    as well as some routines to generate pseudo-experiments or read them from files
RooEBackground.cxx,RooEBackground.h,RooMuBackground.cxx,RooMuBackground.h definitions of the 
            background shapes for the e and mu channels
TreeLoader.cxx,TreeLoader.h this class is used to by the DataSetHelper to read pseudoexperiments
            from root-trees
ResultCollection.cxx,ResultCollection.h	some code to automatically book and fill histograms of the 
            various fit-results.							  

These all need to be compiled once, separately, i.e.
root
.L XXX.cxx+
.q

Don't worry about the linker errors. Once they have been compiled you can restart root and load 
them all at once with 
.x loadlib.C

***************
*other scripts*
***************
early paper combination (more details in PaperPlot.C):
papersig.C does some scanning of pseudoexperiments and calculated the Z-values 
opt.C      auxilliary script used to steer the above
e_0_3.txt, e_40_43.txt electron and muon pseudoexperiments (the first one is a properly random 
           pseudoexperiment using all available backgrounds for the electrons, the one for the muons
           is currently just phanatasy. Background is the same as for electrons, signal is choosen to 
           look somewhat wider than the e-signal)
plotnll.C produces all the relevant plots from the output of papersig.C

look-elsewhere effect:
hadd.C the standard root-example for merging root files. It's needed to merge the output of the
       many grid-jobs used in the look-elsewhere study
lookelsewhere.C produces background-only pseudoexperiments and records the Z-values
fwlitegridtest.sh,crab.cfg grid-submission scripts for the look-elsewhere effect study. 
       crab.cfg is currently configured to run directly on the Brussels node so needs modification 
       for people without direct access to the Belgian Tier 2 site.
correspond.C Will compare a given histogram to a one-sided Gaussian, i.e. it produces the 
       typical look-elsewhere plot from the distribution of Z-values
lookelse.C produces a pleasing plot from the output of the above


whatever's left:
HEEPStyle_noTitle.C Sam's old plot-style improvements
loadlib.C this script will load all the core libraries
deltam.C this script was used togehter with opt.C to generate the delta-M distribution
         shown at the TeVMu+HEEP meeting on the 25th Aug. 09
         it also provides delta-S

