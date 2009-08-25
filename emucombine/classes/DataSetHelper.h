#ifndef __DataSetHelper_h__
#define __DataSetHelper_h__

#include "RooDataSet.h"
#include "RooAbsPdf.h"

#include "TRandom3.h"

class Model;
class TreeLoader;

class DataSetHelper{
 public:
  RooDataSet* datae,*databe,*datase;                 // e data
  RooDataSet* datamu,*databmu,*datasmu;              // mu data
  RooDataSet* combdata;                              // combined data

  float meane,meanmu,meanbe,meanbmu,meanse,meansmu;  // save poisson means of generation


  DataSetHelper();
  ~DataSetHelper();

  void clear();                                      // delete contained datasets

  void initfileread();                               // initialize file readers for 
                                                     // files that have been set
  void generatefrompdf(float,float,float,float,Model&,int);   // generate Data from the PDFs in the model
  void generatefrompdffix(int,int,int,int,Model&,int);   // generate Data from the PDFs in the model no poisson fluctuations
  void generatefrompdf(float,float,Model&,int);       // generate Data from the PDFs (using x-secs from the model)
  void readfromfile(float,float,float,float,Model&,int);      // read data from files (background/signal separate)
  void readfromfile(float,float,Model&,int);          // read data from files (background/sginal together)
  void readfromtxt(char*,char*,Model&,int);          // read data from files (background/sginal together)
    

  std::string fnameetot,fnamemutot;                  // names of input files
  std::string fnamebkge,fnamesige;                   // 
  std::string fnamebkgmu,fnamesigmu;                // 
  
  std::string branche,branchmu;                      // name of branches to be read
  std::string leafe,leafmu;                          // name of leaf nodes (i.e. mass)

  int nsige,nbkge,nsigmu,nbkgmu,ntote,ntotmu;        // numbers of actually generated events

 private:
  TreeLoader* filereadere,*filereadermu;             // helper class for reading from file
  TreeLoader* filereaderbe,*filereaderbmu;           // helper class for reading from file (bkg only)
  TreeLoader* filereaderse,*filereadersmu;           // helper class for reading from file (sig only)
  
  TRandom3 rand;
};

#endif
