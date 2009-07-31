#ifndef __TreeLoader_h__
#define __TreeLoader_h__
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "TTree.h"
#include "TFile.h"

class TreeLoader{
 public:
  
  TreeLoader();
  ~TreeLoader();
  void loadfile(const char*,const char*,const char*);
  RooDataSet* getevents(int,RooRealVar&);
  
 private:
  TTree* tree;
  TFile* file;
  int nevents;
  int neventsused;
  Float_t mass;
  TBranch* massbranch;
};
#endif
