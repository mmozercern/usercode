#include <Riostream.h>
#include <string>
#include <vector>
//#include <cstdlib>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"
//void getSigmaBands(string fileName);
void plot_golfcourse();

const float intLumi=4.6;

void plot_golfcourse(){

  // TFile *fFREQ=new TFile("higgsCombineHZZ2L2Q_FREQ.20111127MMv2.root","READ");
 TFile *fFREQ=new TFile("higgsCombineHZZ2L2Q_FREQ.20111129MMv2.root","READ");

  TTree *t=(TTree*)fFREQ->Get("limit");

  double mh,limit;
  float quant;
  t->SetBranchAddress("mh",&mh);
  t->SetBranchAddress("limit",&limit);
  t->SetBranchAddress("quantileExpected",&quant);

  vector<double> v_mh, v_median,v_68l,v_68h,v_95l,v_95h, v_obs;

  for(int i=0;i<t->GetEntries();i++){
    t->GetEntry(i);
    //  cout<<"i "<<i<<flush<<"  m = "<<mh<<endl;
   
   
    // if(mh==600)cout<<"$$$$$$$$$ TREE 600 $$$$$$$$$$$$$$"<<endl;
    if(quant>-1.01&&quant<-0.99){
      v_obs.push_back(limit);
      v_mh.push_back(mh);
    }
    else if(quant>0.02&&quant<0.03)v_95l.push_back(limit);
    else if(quant>0.15&&quant<0.17)v_68l.push_back(limit);
    else if(quant>0.49&&quant<0.51)v_median.push_back(limit);
    else if(quant>0.83&&quant<0.85)v_68h.push_back(limit);
    else if(quant>0.965&&quant<0.98)v_95h.push_back(limit);
    else {cout<<"Error! Quantile =  "<<quant<<endl;}
  }
  cout<<"Out of the loop !"<<endl;

  //read in theoretical values from text files
  ifstream xsect_file("./xsect_higgs_173points_new.txt",ios::in);
  if (! xsect_file.is_open()){ cout<<"Failed to open file with xsections"<<endl;}
  float mHxs, CSgg, CSgg_p, CSgg_m, CSpdfgg_p,CSpdfgg_m,CSvbf, CSvbf_p, CSvbf_m,CSpdfvbf_p,CSpdfvbf_m, 
    Gamma, BRHZZ, BRZZ2l2q;

  vector<float> v_mhxs, v_ggxs,v_vbfxs, v_ggtoterrh,v_ggtoterrl,
                v_vbftoterrh,v_vbftoterrl, v_gamma, v_brhzz, v_brzz2l2q;
  while(xsect_file.good()){
    xsect_file >> mHxs>> CSgg>> CSgg_p >> CSgg_m >>  CSpdfgg_p>> CSpdfgg_m>> CSvbf >> CSvbf_p >> CSvbf_m >>  CSpdfvbf_p>>CSpdfvbf_m>>Gamma >> BRHZZ >> BRZZ2l2q;
    // if(mHxs==600)cout<<"~~~~~ 600 theor ~~~~~~~~~~~~~"<<endl;
    v_mhxs.push_back(mHxs);
    v_ggxs.push_back(CSgg*BRHZZ);//*BRZZ2l2q
    v_vbfxs.push_back(CSvbf);
    v_brhzz.push_back( BRHZZ);
    v_brzz2l2q.push_back(BRZZ2l2q);
    //sum up linearly theor errors    
    v_ggtoterrh.push_back(1.0+CSgg_p+CSpdfgg_p);
    v_ggtoterrl.push_back(1.0+CSgg_m+CSpdfgg_m);
  }
  cout<<"Size of theor "<<v_mhxs.size()<<flush;


  ///////////////

  const int nMass= v_mh.size();
  cout<<"SIZE SIZE SIZE "<<nMass<<endl;
  double mass[nMass],mass1[nMass],obs_lim_cls[nMass]; 
  double medianD[nMass];
  double up68err[nMass],down68err[nMass],up95err[nMass],down95err[nMass];
  double ggxs[nMass], ggxs_uperr[nMass], ggxs_downerr[nMass];

  int nMassEff=0,nMassEff1=0;
  int nM95=0;
  double mass95[nMass],median95[nMass];
  int nexcluded=0;
  bool excl; 
 for(int im=0;im<nMass;im++){
    // cout<<"Array "<<im<<flush<<"  m = "<<v_mh.at(im)<<flush;
    //protection against messed up jobs

   excl=false;
   if(v_68h.at(im)>=v_95h.at(im) || v_68l.at(im)<=v_95l.at(im) ){
     cout<<"Point at M = "<<v_mh.at(im) <<" excluded"<<endl;
     nexcluded++;
     //      continue;
     excl=true; 
   }

   if(im%2==1)excl=true;

    //search for right index in theor vectors
    bool found=false;
    int indtmp=0,ind=-1;
    while(!found){
      if(v_mhxs.at(indtmp)==v_mh.at(im)){found=true;ind=indtmp;}
      indtmp++;
      if(indtmp==v_mhxs.size()){
	cout<<"m="<<v_mh.at(im)<<" NOT found in theor matrix."<<endl;
	break;
      }
    }//end while    

    // if(v_mh.at(im)<226.0)found=false;

    if(!found){
      //  cout<<"(2) m="<<v_mh.at(im)<<" NOT found in theor matrix."<<endl;
      continue;
    }

    //cout<<"  Found mass, xs=  "<<v_ggxs.at(ind)<<endl;
    double xs=double(v_ggxs.at(ind));

    mass[nMassEff]=v_mh.at(im);
    //if( mass[nMassEff]==600.0)cout<<"=============> 600 !!!"<<endl;
    obs_lim_cls[nMassEff]=v_obs.at(im);
    nMassEff++;
    if(!excl){
    mass1[nMassEff1]=v_mh.at(im);
    medianD[nMassEff1]=v_median.at(im);
    up68err[nMassEff1]=(v_68h.at(im)-v_median.at(im));
    down68err[nMassEff1]=(v_median.at(im)-v_68l.at(im));
   
    ggxs[nMassEff1]=xs;
    ggxs_uperr[nMassEff1]=double( v_ggtoterrh.at(ind))*xs -xs ;
    ggxs_downerr[nMassEff1]=xs - double( v_ggtoterrl.at(ind))*xs;
    nMassEff1++;
    

    

    bool skip95= false;//
    skip95=v_mh.at(im)==204||v_mh.at(im)==208||v_mh.at(im)==212||v_mh.at(im)==214|| v_mh.at(im)==232 || v_mh.at(im)==240  || v_mh.at(im)==240 || v_mh.at(im)==244 || v_mh.at(im)==252 || v_mh.at(im)==264 || v_mh.at(im)==272 || v_mh.at(im)==288 ;
    //  skip95=false;
    
    if(skip95 )continue;
    mass95[nM95]=v_mh.at(im);
    median95[nM95]=v_median.at(im);
    up95err[nM95]=(v_95h.at(im)-v_median.at(im));
    down95err[nM95]=(v_median.at(im)-v_95l.at(im));
    nM95++;
    }

  }
  cout<<"Excluded "<<nexcluded<<" sick mass points."<<endl;

 


  //  cout<<"Working on TGraph"<<endl;
  TGraph *grobslim_cls=new TGraphAsymmErrors(nMassEff,mass,obs_lim_cls);
  grobslim_cls->SetName("LimitObservedCLs");
  TGraph *grmedian_cls=new TGraphAsymmErrors(nMassEff1,mass1,medianD);
  grmedian_cls->SetName("LimitExpectedCLs");
  TGraphAsymmErrors *gr68_cls=new TGraphAsymmErrors(nMassEff1,mass1,medianD,0,0,down68err,up68err);
  gr68_cls->SetName("Limit68CLs");
  TGraphAsymmErrors *gr95_cls=new TGraphAsymmErrors(nM95,mass95,median95,0,0,down95err,up95err);
  gr95_cls->SetName("Limit95CLs");

  TGraphAsymmErrors *grthSM=new TGraphAsymmErrors(nMassEff,mass,ggxs,0,0,ggxs_downerr,ggxs_uperr);//ggxs_downerr,ggxs_uperr);
  grthSM->SetName("SMXSection");

  // cout<<"Plotting"<<endl;
  TCanvas *cMCMC=new TCanvas("c_lim_MCMC","canvas with limits for MCMC",800,800);
  cMCMC->cd();
  cMCMC->SetGridx(1);
  cMCMC->SetGridy(1);
  // draw a frame to define the range
  TH1F *hr = cMCMC->DrawFrame(190.0,0.0,610.0,6.0,"frame1");
  hr->SetXTitle("M_{H} [GeV]");
  hr->SetYTitle("#sigma_{95%} / #sigma_{SM}");// #rightarrow 2l2q
  // cMCMC->GetFrame()->SetFillColor(21);
  //cMCMC->GetFrame()->SetBorderSize(12);
  
  gr95_cls->SetFillColor(kYellow);
  gr95_cls->SetFillStyle(1001);//solid
  gr95_cls->GetXaxis()->SetTitle("M_{H} [GeV]");
  gr95_cls->GetYaxis()->SetTitle("#sigma_{95%} / #sigma_{SM}");// #rightarrow 2l2q
  gr95_cls->GetXaxis()->SetRangeUser(200.0,600.0);
  
  gr95_cls->Draw("3");
  
  gr68_cls->SetFillColor(kGreen);
  gr68_cls->SetFillStyle(1001);//solid
  gr68_cls->Draw("3same");
  grmedian_cls->GetXaxis()->SetTitle("M_{H} [GeV]");
  grmedian_cls->GetYaxis()->SetTitle("#sigma_{95%} / #sigma_{SM}");// #rightarrow 2l2q
  grmedian_cls->SetMarkerStyle(24);//25=hollow squre
  grmedian_cls->SetMarkerColor(kBlack);
  grmedian_cls->SetLineStyle(2);
  grmedian_cls->SetLineWidth(2);
  grmedian_cls->SetMinimum(0.0);
  grmedian_cls->SetMaximum(8.0);
 
  grobslim_cls->SetMarkerColor(kBlack);
  grobslim_cls->SetMarkerStyle(21);//24=hollow circle
  //  grobslim_cls->SetMarkerSize(1.5);
  grobslim_cls->SetLineStyle(1);
  grobslim_cls->SetLineWidth(2);

  
  grthSM->SetLineColor(kRed);
  grthSM->SetLineWidth(2);
  grthSM->SetFillColor(kRed);
  grthSM->SetFillStyle(3344);
  //  grthSM->Draw("L3same");

  grmedian_cls->Draw("L");
  grobslim_cls->Draw("LP");

  TLine *l1=new TLine();
  l1->SetLineStyle(1);
  l1->SetLineWidth(2.0);
  l1->SetLineColor(kRed);
  l1->DrawLine(210.0,1.0,610.0,1.0);
  cMCMC->Update();
  // cMCMC->RedrawAxis("");
  gPad->RedrawAxis("");
  // hr->GetYaxis()->DrawClone();
  cMCMC->Update();
  


  //more graphics
   TLegend *leg = new TLegend(.35,.71,.90,.90);

   leg->SetFillColor(0);
   leg->SetShadowColor(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.03);
   //   leg->SetBorderMode(0);
   leg->AddEntry(grobslim_cls, "CL_{S} Observed", "LP");
   leg->AddEntry(grmedian_cls, "CL_{S} Expected", "L");
   leg->AddEntry(gr68_cls, "CL_{S} Expected #pm 1#sigma", "LF");
   leg->AddEntry(gr95_cls, "CL_{S} Expected #pm 2#sigma", "LF");
   leg->AddEntry(grthSM, "SM", "L");// #rightarrow 2l2q
    leg->Draw();
   

   TLatex * latex = new TLatex();
   latex->SetNDC();
   latex->SetTextSize(0.04);
   latex->SetTextAlign(31);
   latex->SetTextAlign(11); // align left 
   latex->DrawLatex(0.18, 0.96, "CMS preliminary 2011");
   latex->DrawLatex(0.60,0.96,Form("%.1f fb^{-1} at #sqrt{s} = 7 TeV",intLumi));
   

 /*
 leg->SetFillColor(0);
 leg->SetShadowColor(0);
 leg->SetTextFont(42);
 leg->SetTextSize(0.05);
 */
   
   TLine *l1b=new TLine();
   l1b->SetLineStyle(1);
   l1b->SetLineWidth(2.0);
   l1b->SetLineColor(kRed);
   //l1b->DrawLine(200.0,1.0,600.0,1.0);
   cMCMC->Update();
   
   
   // cMCMC->RedrawAxis("");
   gPad->RedrawAxis("");
   // hr->GetYaxis()->DrawClone();
   cMCMC->Update();
   // cMCMC->SaveAs("ClsLimit_1fb.eps");
   // cMCMC->SaveAs("ClsLimit_1fb.png");
   
}//end main
