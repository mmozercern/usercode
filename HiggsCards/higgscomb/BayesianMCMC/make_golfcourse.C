#include <Riostream.h>
#include <string>
#include <vector>
#include <cstdlib>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLine.h"
#include <sstream>
#include "TLatex.h"
#include "TLegend.h"
void getSigmaBands(string fileName);
void make_golfcourse();
void smoothing(double* in_centr,double* in_err, int np, bool up=true);
const float intLumi=4.6;
const int nMass=71;//71;
vector<double> median;
vector<double> up68,down68,up95,down95;

void getSigmaBands(string fileName){
  cout<<"\n\nAnalyzing file "<<fileName.c_str()<<endl;
  TFile *f = new TFile(fileName.c_str());

  TTree* t = (TTree* ) f->Get("limit");

  double limit;
  int ntoys = t->GetEntries();
  t->SetBranchAddress("limit",&limit);

  TH1D *h = new TH1D("h","h",2000,0,15);

  for(int iEvt=0; iEvt<t->GetEntries(); iEvt++){

    t->GetEntry(iEvt);

    h->Fill(limit);

  }

  int sum=0;
  int iBin=0;
  while (sum<ntoys*.025 && iBin<2000){
    iBin++;
    sum+=h->GetBinContent(iBin);
    
  }
  double low95p= (double)iBin*15.0/2000.0;
  //  cout << "lower 95%: " <<low95p << endl;
  down95.push_back(low95p);


  sum=0;
  iBin=2001;
  while (sum<ntoys*.025 && iBin>0){
    iBin--;
    sum+=h->GetBinContent(iBin);

  }
  double up95p= (double)iBin*15.0/2000.0;
  // cout << "upper 95%: " <<up95p  << endl;
  up95.push_back(up95p);

  sum=0;
  iBin=0;
  while (sum<ntoys*.16 && iBin<2000){
    iBin++;
    sum+=h->GetBinContent(iBin);

  }
  double low68p=(double)iBin*15/2000;
  // cout << "lower 68%: " << low68p << endl;
  down68.push_back(low68p);

  sum=0;
  iBin=2001;
  while (sum<ntoys*.16 && iBin>0){
    iBin--;
    sum+=h->GetBinContent(iBin);

  }
  double up68p=(double)iBin*15/2000;
  // cout << "upper 68%: " << up68p << endl;
 up68.push_back(up68p);


 sum=0;
 iBin=2001;
 while (sum<ntoys*.5 && iBin>0){
   iBin--;
   sum+=h->GetBinContent(iBin);
   
 }
 double medianp=(double)iBin*15/2000;
 // cout << "median: " << medianp << endl;
 median.push_back(medianp);

 delete t;
 delete f;

}


void make_golfcourse(){

  //gROOT->ProcessLine(".L ~/tdrstyle.C");
  //setTDRStyle();
  
  double massp[nMass]={200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,256,258,260,262,264,266,268,270,272,274,276,278,280,282,284,288,290,295,300,305,310,315,320,325,330,335,340,345,350,360,370,380,390,400,420,440,460,480,500,540,560,580,600};

  //double massp[nMass]={200,250,300,350,400,440,500,540,600};
  string mass_str[nMass];

   for(int imass=0; imass<nMass; imass++){
   stringstream convert;
   convert << massp[imass];
   //cout << convert.str() << endl;
   mass_str[imass]=convert.str();
    //convert.str();
  }

   //string random[nMass]={"25157","26136","8823","8881","15362","23832","14244","15129","22436","20088","27798","16776","27126","8291","23268","9753","15850","9839","28411","10577","31112","5316","14385","20300","24795","16757","27531","19670","3893","8634","22185","2948","29377","14249","32368","21939","20230","15468","30185","30653","22952","18780","13836","23073","29410","26421","25489","16110","11899","10381","7328","20757","29404","8079","18119","6973","32735","19608","23125","14984","11149","4816","31860","11406","26909","5720","28535","4274","683","25647","26663"};

  string filename;
  double limitTemp;
  double obs_lim_mcmc[nMass];


  for(int im=0; im<nMass; im++){
   
    filename = mass_str[im]+"/combine_2l2q_MarkovChainMC_comb_hzz2l2q/higgsCombine2l2q.obs.MarkovChainMC.mH"+mass_str[im]+".TOTAL.root";
    cout<<filename.c_str()<<endl;
    TFile* fTemp = new TFile(filename.c_str());
    cout<<"prova1"<<endl;
    TTree* tTemp = (TTree*) fTemp->Get("limit");
    tTemp->SetBranchAddress("limit",&limitTemp);
    //    cout<<"prova2"<<endl;
    double avg_obs=0.0, nobs=0.0;
    for(int i=0;i<tTemp->GetEntries();i++){
      tTemp->GetEntry(i);
      nobs++;
      avg_obs+=limitTemp;
    }
    obs_lim_mcmc[im]=avg_obs/nobs;
    cout << "Observed Limit at " << massp[im] << " is " << obs_lim_mcmc[im] << endl;
  }

  for(int im=0;im<nMass;im++){
    cout << mass_str[im] << endl;
    //filename=mass_str[im]+"/combine_2l2q_MarkovChainMC_CMS_hzz2l2q_"+mass_str[im]+"_6channels"+
    //  "/higgsCombine2l2q.MarkovChainMC.mH"+mass_str[im]+".TOTAL.root";
    filename = mass_str[im]+"/combine_2l2q_MarkovChainMC_comb_hzz2l2q/higgsCombine2l2q.exp.MarkovChainMC.mH"+mass_str[im]+".TOTAL.root";
    //   cout<<"expected for mass "<<mass_str[im].c_str()<<endl;
    cout<<filename.c_str()<<endl;
    getSigmaBands(filename);
    // cout<<"prova3"<<endl;
  }

  double medianD[nMass];
  double up68err[nMass],down68err[nMass],up95err[nMass],down95err[nMass];
  for(int im=0;im<nMass;im++){
   medianD[im]=median.at(im);
   up68err[im]=up68.at(im)-median.at(im);
   down68err[im]=median.at(im)-down68.at(im);
   up95err[im]=up95.at(im)-median.at(im);
   down95err[im]=median.at(im)-down95.at(im);
   cout<<"----- MASS #"<<im<<" = "<<massp[im] <<"---------"<<endl;
   cout<<"median "<<obs_lim_mcmc[im]<<endl;
   cout<<"median "<<medianD[im]<<endl;
   cout<<"up68 "<<up68err[im]<<endl;
   cout<<"down68 "<<down68err[im]<<endl;
   cout<<"up95 "<<up95err[im]<<endl;
   cout<<"down95 "<<down95err[im]<<endl;
 }

  smoothing(medianD,up95err,nMass,true);
  smoothing(medianD,down95err,nMass,false);
  smoothing(medianD,up68err,nMass,true);
  smoothing(medianD,down68err,nMass,false);
  //  cout<<"check smoothened: "<<massp[21]<<"  "<<medianD[21]<< "  " <<up95err[21]<<endl;

  const int nMassSel=11;
  double massSel[nMassSel]={200,220,250,300,350,370,400,440,500,540,600};
  double medianDSel[nMassSel];
  double up68errSel[nMassSel],down68errSel[nMassSel],up95errSel[nMassSel],down95errSel[nMassSel];
  int mInd=0;
  for(int im=0;im<nMass;im++){
    if(massp[im]==massSel[mInd]){
      medianDSel[mInd]=medianD[im];
      up68errSel[mInd]=up68err[im];
      down68errSel[mInd]=down68err[im];
      up95errSel[mInd]=up95err[im];
      down95errSel[mInd]=down95err[im];
      mInd++;
    }
  }
  


 TGraph *grobslim_pl=new TGraphAsymmErrors(nMass,massp,obs_lim_mcmc);
 grobslim_pl->SetName("LimitObserved");
 TGraph *grmedian_pl=new TGraphAsymmErrors(nMassSel,massSel,medianDSel);
 grmedian_pl->SetName("LimitMCMCmedian");
 TGraphAsymmErrors *gr68_pl=new TGraphAsymmErrors(nMassSel,massSel,medianDSel,0,0,down68errSel,up68errSel);
 gr68_pl->SetName("LimitMCMC68");
 TGraphAsymmErrors *gr95_pl=new TGraphAsymmErrors(nMassSel,massSel,medianDSel,0,0,down95errSel,up95errSel);
 gr95_pl->SetName("LimitMCMC95");
 TGraphAsymmErrors *grthSM=new TGraphAsymmErrors(nMass,massp,medianDSel,0,0,0,0);//ggxs_downerr,ggxs_uperr);
  grthSM->SetName("SMXSection");

 /* TGraph *grmedian_pl=new TGraphAsymmErrors(nMass,massp,medianD);
 grmedian_pl->SetName("LimitMCMCmedian");
 TGraphAsymmErrors *gr68_pl=new TGraphAsymmErrors(nMass,massp,medianD,0,0,down68err,up68err);
 gr68_pl->SetName("LimitMCMC68");
 TGraphAsymmErrors *gr95_pl=new TGraphAsymmErrors(nMass,massp,medianD,0,0,down95err,up95err);
 gr95_pl->SetName("LimitMCMC95");
 */

 TCanvas *cMCMC=new TCanvas("c_lim_MCMC","canvas with limits for MCMC",800,800);
 cMCMC->cd();
 cMCMC->SetGridx(1);
 cMCMC->SetGridy(1);
 // draw a frame to define the range
 TH1F *hr = cMCMC->DrawFrame(190.0,0.0,610.0,9.0,"frame1");
 hr->SetXTitle("M_{H} [GeV]");
 hr->SetYTitle("#sigma_{95%}/#sigma_{SM}");
 // cMCMC->GetFrame()->SetFillColor(21);
 //cMCMC->GetFrame()->SetBorderSize(12);

 gr95_pl->SetFillColor(kYellow);
 gr95_pl->SetFillStyle(1001);//solid
 gr95_pl->GetXaxis()->SetTitle("M_{H} [GeV]");
 gr95_pl->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
 gr95_pl->GetXaxis()->SetRangeUser(200.0,600.0);
 // gr95_pl->GetYaxis()->SetRangeUser(0.0,20.);

 gr95_pl->Draw("3");

 gr68_pl->SetFillColor(kGreen);
 gr68_pl->SetFillStyle(1001);//solid
 gr68_pl->Draw("3same");
 grmedian_pl->GetXaxis()->SetTitle("M_{H} [GeV]");
 grmedian_pl->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
 grmedian_pl->SetMarkerStyle(24);
 grmedian_pl->SetMarkerColor(kBlack);
 grmedian_pl->SetMinimum(0.0);
 grmedian_pl->SetMaximum(8.0);
 grmedian_pl->SetLineStyle(2);
 grmedian_pl->SetLineWidth(2);

 grobslim_pl->SetMarkerColor(kBlack);
 grobslim_pl->SetMarkerStyle(21);
 grobslim_pl->SetLineStyle(1);
 grobslim_pl->SetLineWidth(1);

  grthSM->SetLineColor(kRed);
  grthSM->SetLineWidth(2);
  grthSM->SetFillColor(kRed);
  grthSM->SetFillStyle(3344);
  //  grthSM->Draw("L3same");

 grmedian_pl->Draw("L");
 grobslim_pl->Draw("LP");

 TLine *l1=new TLine();
 l1->SetLineStyle(1);
 l1->SetLineWidth(2.0);
 l1->SetLineColor(kRed);
 l1->DrawLine(200.0,1.0,600.0,1.0);
 cMCMC->Update();
 // cMCMC->RedrawAxis("");
 gPad->RedrawAxis("");
 // hr->GetYaxis()->DrawClone();
 cMCMC->Update();

  TLegend *leg = new TLegend(.35,.71,.90,.90);

   leg->SetFillColor(0);
   leg->SetShadowColor(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.03);
   //   leg->SetBorderMode(0);
   leg->AddEntry(grobslim_pl, "CL_{S} Observed", "LP");
   leg->AddEntry(grmedian_pl, "CL_{S} Expected", "L");
   leg->AddEntry(gr68_pl, "CL_{S} Expected #pm 1#sigma", "LF");
   leg->AddEntry(gr95_pl, "CL_{S} Expected #pm 2#sigma", "LF");
   leg->AddEntry(grthSM, "SM", "L");// #rightarrow 2l2q
   leg->Draw();
   

   TLatex * latex = new TLatex();
   latex->SetNDC();
   latex->SetTextSize(0.04);
   latex->SetTextAlign(31);
   latex->SetTextAlign(11); // align left 
   latex->DrawLatex(0.18, 0.96, "CMS preliminary 2011");
   latex->DrawLatex(0.60,0.96,Form("%.1f fb^{-1} at #sqrt{s} = 7 TeV",intLumi));
 
 cMCMC->Update();
 // cMCMC->RedrawAxis("");
 gPad->RedrawAxis("");
 // hr->GetYaxis()->DrawClone();
 cMCMC->Update();

 // double clsmedian[4]={};


}

void smoothing(double* in_centr,double* in_err, int np, bool up){
//void smoothing(double* in_array, int np){

  double mysign=up? +1.0 : -1.0;
  for(int i=0;i<np;i++){
    if(i==0||i==np-1)continue;

    double v0=in_centr[i-1]+mysign*in_err[i-1];
    double v1=in_centr[i]+mysign*in_err[i];
    double v2=in_centr[i+1]+mysign*in_err[i+1];

    bool isMonotonic=false;
    double diff1=v1-v0;
    double diff2=v2-v1;
    if(fabs(diff1)<0.05*v1)diff1=0.0;//against small fluctuations
    if(fabs(diff2)<0.05*v1)diff2=0.0;

    if( (diff1>=0.0 && diff2>=0.0) || (diff1<=0.0 && diff2<=0.0) ){//monotonically changing trend
      isMonotonic=true;
    }

    double avg=(v0+v2)/2.0;
    double reldiff=(v1-avg)/avg;

    if(!isMonotonic && fabs(reldiff)>0.05){
      //      cout<<"Mass #"<<i<<"  "<<in_centr[i] <<" ("<<v0 <<", "<<v1 <<", "<<v2 <<") smoothened from "<< v1<<" to "<<avg<<endl;
      v1=avg;
    }
   in_err[i]=mysign*(v1-in_centr[i]);
   // else  in_err[i]=in_centr[i];
  }

}
