#ifndef __ResulCollection_h__
#define __ResulCollection_h__
#include "DataSetHelper.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

class Model;
class DataSetHelper;

class ResultCollection{
 public:
  ResultCollection();
  ~ResultCollection();

  void clear();

  //set up histograms
  void setsighistos(int,int,float,float);              // significance histos
  void setpeakhistos(int,int,Model&);                  // histograms for peak position ranges from mass
  void setpeaknormhistos(int,int);                     // pull-plots for peak position
  void setnsighistos(int,int,float,float);             // n-signal
  void setnsignormhistos(int,int);                     // pull-plots for n-signal
  void setnbkghistos(int,int,float,float);             // n-bkg
  void setnbkgnormhistos(int,int);                     // pull-plots for n-bkg
  void seteffratio(int,Model&);                        // efficiency ratio and pull plot for effratio
  void setmassshift(int,Model&);                       // massshift and pull plot for massshift
  void settotalmass(int,int,Model&);                   // sum of all generated events
  void setsamplemass(int,int,Model&);                  // sample mass spectrum
  void setwidthhistos(int,int,Model&);
  void setwidthnormhistos(int,int,Model&);
  void set2d(int,int,int,float,Model&);


  // add likelihood scan

  //fill histograms with fit results
  void fillhistos(Model&,DataSetHelper&);           // fill all histos that have been set
  
  void savehistos(char*);                           // write histograms to a file
  void savehistos(TFile&,char*);                          // write histograms to a file

  TH1F* sige,*sigmu,*sigboth;
  TH1F* peake,*peakmu,*peakboth;
  TH1F* peakepull,*peakmupull,*peakbothpull;
  TH1F* nsige,*nsigmu,*nsigboth;
  TH1F* nsigepull,*nsigmupull,*nsigbothpull;
  TH1F* nbkge,*nbkgmu,*nbkgbothe,*nbkgbothmu;
  TH1F* nbkgepull,*nbkgmupull,*nbkgbothpulle,*nbkgbothpullmu;
  TH1F* effratioplot,*effratiopull;
  TH1F* massshiftplot,*massshiftpull;
  TH1F* totalmasse,*totalmassmu;
  TH1F* samplemasse,*samplemassmu;
  TH1F* widthe,*widthmu,*widthboth;
  TH1F* widthepull,*widthmupull,*widthbothpull;
  
  TH2F* sigvspeake,*sigvspeakmu,*sigvspeakboth;
  TH2F* sigvswide,*sigvswidmu, *sigvswidboth ;

};

#endif
