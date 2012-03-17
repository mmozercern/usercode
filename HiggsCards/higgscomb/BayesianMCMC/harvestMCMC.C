#include <Riostream.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TROOT.h"

//#1 ./mergeCombinationTrees.sh
//#2 ./mergeHarvestedCombinationTrees.sh
//#3 harvestMCMC

//fileName: TOTAL file with TOTAL merged output of combination
//fileMassesName: text file with list of mass points
//fileNameObs: TOTAL file with merged output of combination (for Observed limit)

void harvestMCMC(string fileName, string fileMassesName,string fileNameObs="dummy"){
 
  cout<<"\n\nAnalyzing file "<<fileName.c_str()<<"  at MassFile = "<< fileMassesName.c_str()<<endl;
  TFile *f = new TFile(fileName.c_str());
  TTree* t = (TTree* ) f->Get("limit");
  double limit, mh; float quant;
  //  int ntoys = t->GetEntries();
  t->SetBranchAddress("mh",&mh);
  t->SetBranchAddress("limit",&limit);
  t->SetBranchAddress("quantileExpected",&quant);

  TFile *fobs=0;TTree *tobs=0;
  double limitobs, mhobs; float quantobs;
  if(fileNameObs!="dummy"){
    cout<<"Reading in also observed MCMC limits from "<<fileNameObs.c_str()<<endl;
    fobs=new TFile(fileNameObs.c_str(),"READ");
    tobs = (TTree* ) fobs->Get("limit");
    //  int ntoys = t->GetEntries();
    tobs->SetBranchAddress("mh",&mhobs);
    tobs->SetBranchAddress("limit",&limitobs);
    tobs->SetBranchAddress("quantileExpected",&quantobs);
  }
  
  
  ifstream fMasses(fileMassesName.c_str(),ios::in);
  double M=0;

  double mhnew, quantnew,limitnew;
  const double quant95=0.025, quant68=0.16,quant50=0.5;
  const double quant95down=quant95,quant95up=1-quant95,quant68down=quant68,quant68up=1-quant68;
  TFile *fout=new TFile("higgsCombineGrav2L2Q_MCMC.RSGrav05.root","RECREATE");
  fout->cd();
  TTree *tout=new TTree("limit","Harvested limits");
  tout->Branch("limit",&limitnew,"limit/D");
  tout->Branch("mh",&mhnew,"mh/D");
  tout->Branch("quantileExpected",&quantnew,"quantileExpected/D");

 while (fMasses.good()){
    int ntoys=0;
    fMasses>>M;
    cout<<"\n\n----------\nMASS = "<<M<<endl;
    int nBins=30000;
    double maxBin=30.0;
    TH1D *h = new TH1D("h","h",nBins,0,maxBin);
    
    for(int iEvt=0; iEvt<t->GetEntries(); iEvt++){
    
      t->GetEntry(iEvt);
      if(mh==M){
	ntoys++;
	h->Fill(limit);
	if(M>=550.0){ 
	  cout<<"M=600: limit list"<<endl;
	  cout<<"   "<<limit<<flush;
	}
      }
    }//end loop on tree entries
    cout<<endl;
  int sum=0;
  int iBin=0;

  while (sum<ntoys*quant95 && iBin<nBins){
    iBin++;
    sum+=h->GetBinContent(iBin);
  }

  double low95p= (double)iBin*maxBin/nBins;
  cout << "lower 95%: " <<low95p << endl;
  limitnew=low95p ;
  quantnew=quant95down;
  mhnew=M;
  tout->Fill();



  sum=0;
  iBin=nBins+1;
  while (sum<ntoys*quant95 && iBin>0){
    iBin--;
    sum+=h->GetBinContent(iBin);
  }
  double up95p= (double)iBin*maxBin/nBins;
  cout << "upper 95%: " <<up95p  << endl;
  limitnew=up95p ;
  quantnew=quant95up;
  mhnew=M;
  tout->Fill();


  sum=0;
  iBin=0;
  while (sum<ntoys*quant68 && iBin<nBins){
    iBin++;
    sum+=h->GetBinContent(iBin);

  }
  double low68p=(double)iBin*maxBin/nBins;
  cout << "lower 68%: " << low68p << endl;
  limitnew=low68p ;
  quantnew=quant68down;
  mhnew=M;
  tout->Fill();   

  sum=0;
  iBin=nBins+1;
  while (sum<ntoys*quant68 && iBin>0){
    iBin--;
    sum+=h->GetBinContent(iBin);

  }
  double up68p=(double)iBin*maxBin/nBins;//*15/2000;
  cout << "upper 68%: " << up68p << endl;
  limitnew=up68p ;
  quantnew=quant68up;
  mhnew=M;
  tout->Fill();


  sum=0;
  iBin=nBins+1;
  while (sum<ntoys*quant50 && iBin>0){
   iBin--;
   sum+=h->GetBinContent(iBin);
  }
  double medianp=(double)iBin*maxBin/nBins;
  cout << "median: " << medianp << endl;
  limitnew=medianp ;
  quantnew=quant50;
  mhnew=M;
  tout->Fill();

  delete h;

  double obsp=-1.0;
  if(tobs!=0){
    TH1D *hobs = new TH1D("hobs","hobs",nBins,0,maxBin);
    for(int iEvt=0; iEvt<tobs->GetEntries(); iEvt++){
      
      tobs->GetEntry(iEvt);
      if(mhobs==M&&limitobs<maxBin){       
	hobs->Fill(limitobs);
      }
    }//end loop on observed tree entries    
    if(hobs->GetEntries()<1)cout<<" NO ENTRIES FOR OBS HISTO at M="<<mhobs<<endl;
    obsp=(double)hobs->GetMean();
    delete hobs;
  }
  limitnew=obsp;
  quantnew=-1.0;
  mhnew=M;
  tout->Fill();
  cout << "observed: " << obsp << endl;

  }//end while loop on masses
 
 tout->Write();
 fout->Close();
 delete fout;
 delete t;
 delete f;



}
