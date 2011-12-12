#include <Riostream.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TROOT.h"

void harvestAsymptotic(string fileName, string fileMassesName){
 
  cout<<"\n\nAnalyzing file "<<fileName.c_str()<<"  at MassFile = "<< fileMassesName.c_str()<<endl;
  TFile *f = new TFile(fileName.c_str());
  TTree* t = (TTree* ) f->Get("limit");
  double limit, mh; float quant;
  //  int ntoys = t->GetEntries();
  t->SetBranchAddress("mh",&mh);
  t->SetBranchAddress("limit",&limit);
  t->SetBranchAddress("quantileExpected",&quant);


  ifstream fMasses(fileMassesName.c_str(),ios::in);
  double M=0;

  double mhnew, quantnew,limitnew;
  const double quant95=0.025, quant68=0.16,quant50=0.5;
  const double quant95down=quant95,quant95up=1-quant95,quant68down=quant68,quant68up=1-quant68;
  TFile *fout=new TFile("higgsCombineHZZ2L2Q_Asymptotic.SM.root","RECREATE");
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
    TH1D *hOBS = new TH1D("hobs","hobs",nBins,0,maxBin);
    TH1D *h95down = new TH1D("h95down","h95down",nBins,0,maxBin);
    TH1D *h95up = new TH1D("h95up","h95up",nBins,0,maxBin);
    TH1D *h68down = new TH1D("h68down","h68down",nBins,0,maxBin);
    TH1D *h68up = new TH1D("h68up","h68up",nBins,0,maxBin);
    TH1D *hMedian = new TH1D("hmedian","hmedian",nBins,0,maxBin);

    for(int iEvt=0; iEvt<t->GetEntries(); iEvt++){
    
      t->GetEntry(iEvt);
      if(mh==M&&limit<maxBin&&limit>0.0){
	
	if(quant==-1.0)hOBS->Fill(limit);
	else if(double(quant)>=quant95down*0.99 && double(quant)<=quant95down*1.01     )h95down->Fill(limit);
	else if(double(quant)>=quant95up*0.99 && double(quant)<=quant95up*1.01  )h95up->Fill(limit);
	else if(quant>=quant68down*0.99 && double(quant)<=quant68down*1.01  )h68down->Fill(limit);
	else if(quant>=quant68up*0.99 && double(quant)<=quant68up*1.01  )h68up->Fill(limit);
	else if(quant>=quant50*0.99 && double(quant)<=quant50*1.01  )hMedian->Fill(limit);
	else{cout<<"Warning: m_H= "<<mh<<"  Quantile = "<<quant<<"  Limit = "<<limit<<endl;}
      }
 
   }//end loop on tree entries
    cout<<endl;
    double low95p= (double)h95down->GetMean();
    cout << "lower 95%: " <<low95p << endl;
    limitnew=low95p ;
    quantnew=quant95down;
    mhnew=M;
    tout->Fill();

  double up95p= (double)h95up->GetMean();
  cout << "upper 95%: " <<up95p  << endl;
  limitnew=up95p ;
  quantnew=quant95up;
  mhnew=M;
  tout->Fill();


  double low68p= (double)h68down->GetMean();
  cout << "lower 68%: " << low68p << endl;
  limitnew=low68p ;
  quantnew=quant68down;
  mhnew=M;
  tout->Fill();   

  double up68p=(double)h68up->GetMean();
  cout << "upper 68%: " << up68p << endl;
  limitnew=up68p ;
  quantnew=quant68up;
  mhnew=M;
  tout->Fill();

  double medianp=(double)hMedian->GetMean();
  cout << "median: " << medianp << endl;
  limitnew=medianp ;
  quantnew=quant50;
  mhnew=M;
  tout->Fill();

  double obsp=(double)hOBS->GetMean();
  cout << "observed: " << obsp << endl;
  limitnew=obsp ;
  quantnew=-1.0;
  mhnew=M;
  tout->Fill();

  delete hOBS; delete hMedian;
  delete h95down ; delete h95up; 
  delete h68down; delete h68up;

  }//end while loop on masses
 
 tout->Write();
 fout->Close();
 delete fout;
 delete t;
 delete f;



}
