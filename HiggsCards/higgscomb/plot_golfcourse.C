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
#include "TStyle.h"
#include "TPaveText.h"
//void getSigmaBands(string fileName);
void plot_golfcourse();
void setFPStyle();

const float intLumi=4.6;

void plot_golfcourse(){

  const  bool useNewStyle=true;
  const bool plotMCMC=false;
  if(useNewStyle)  setFPStyle();

  // TFile *fFREQ=new TFile("higgsCombineHZZ2L2Q_FREQ.20111127MMv2.root","READ");
 TFile *fFREQ=new TFile("higgsCombineHZZ2L2Q_FREQ.exp2.root","READ");
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


  TFile *fMCMC=new TFile("higgsCombineHZZ2L2Q_MCMC.exp.SM.root","READ");
  TTree *tMCMC=0;
  vector<double> v_mh_mcmc, v_median_mcmc,v_68l_mcmc,v_68h_mcmc,v_95l_mcmc,v_95h_mcmc, v_obs_mcmc;
  if(plotMCMC){
    tMCMC=(TTree*)fMCMC->Get("limit");

  double mh_mcmc,limit_mcmc;
  double quant_mcmc;
  tMCMC->SetBranchAddress("mh",&mh_mcmc);
  tMCMC->SetBranchAddress("limit",&limit_mcmc);
  tMCMC->SetBranchAddress("quantileExpected",&quant_mcmc);


  for(int i=0;i<tMCMC->GetEntries();i++){
    tMCMC->GetEntry(i);
    //  cout<<"i "<<i<<flush<<"  m = "<<mh<<endl;
   
   
    // if(mh==600)cout<<"$$$$$$$$$ TREE 600 $$$$$$$$$$$$$$"<<endl;
    if(quant_mcmc>-1.01&&quant_mcmc<-0.99){
      v_obs_mcmc.push_back(limit_mcmc);
      v_mh_mcmc.push_back(mh_mcmc);
    }
    else if(quant_mcmc>0.02&&quant_mcmc<0.03)v_95l_mcmc.push_back(limit_mcmc);
    else if(quant_mcmc>0.15&&quant_mcmc<0.17)v_68l_mcmc.push_back(limit_mcmc);
    else if(quant_mcmc>0.49&&quant_mcmc<0.51)v_median_mcmc.push_back(limit_mcmc);
    else if(quant_mcmc>0.83&&quant_mcmc<0.85)v_68h_mcmc.push_back(limit_mcmc);
    else if(quant_mcmc>0.965&&quant_mcmc<0.98)v_95h_mcmc.push_back(limit_mcmc);
    else {cout<<"Error! Quantile =  "<<quant_mcmc<<endl;}
  }
  }//end if plotMCMC
  //read in theoretical values from text files
  ifstream xsect_file("./xsect_higgs_173points_new.txt",ios::in);
  if (! xsect_file.is_open()){ cout<<"Failed to open file with xsections"<<endl;}
  float mHxs, CSgg, CSgg_p, CSgg_m, CSpdfgg_p,CSpdfgg_m,CSvbf, CSvbf_p, CSvbf_m,CSpdfvbf_p,CSpdfvbf_m, 
    Gamma, BRHZZ, BRZZ2l2q;

  vector<float> v_mhxs, v_ggxs,v_vbfxs, v_ggtoterrh,v_ggtoterrl,
                v_vbftoterrh,v_vbftoterrl, v_gamma, v_brhzz, v_brzz2l2q;
  while(xsect_file.good()){
    xsect_file >> mHxs>> CSgg>> CSgg_p >> CSgg_m >>  CSpdfgg_p>> CSpdfgg_m>> CSvbf >> CSvbf_p >> CSvbf_m >>  CSpdfvbf_p>>CSpdfvbf_m>>Gamma >> BRHZZ >> BRZZ2l2q;
    if(mHxs==600)cout<<"~~~~~ 600 theor ~~~~~~~~~~~~~"<<endl;
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
  double mass_mcmc[nMass],obs_lim_mcmc[nMass],median_mcmc[nMass]; 
  double medianD[nMass];
  double up68err[nMass],down68err[nMass],up95err[nMass],down95err[nMass];
  double ggxs[nMass], ggxs_uperr[nMass], ggxs_downerr[nMass];

  int nMassEff=0,nMassEff1=0,nMassEff2=0;//counters for fillign graphs in different ways
  int nM95=0;
  double mass95[nMass],median95[nMass];
  int nexcluded=0;
  bool excl; 
 for(int im=0;im<nMass;im++){
   //    cout<<"Array "<<im<<flush<<"  m = "<<v_mh.at(im)<<endl;
    //protection against messed up jobs

   excl=false;
   if(v_68h.at(im)>=v_95h.at(im) || v_68l.at(im)<=v_95l.at(im) ){
     cout<<"Point at M = "<<v_mh.at(im) <<" excluded"<<endl;
     nexcluded++;
     //      continue;
     excl=true; 
   }

   //if(im%2==1)excl=true;

    //search for right index in theor vectors
    bool found=false;
    int indtmp=0,ind=-1;
    while(!found){
      if(v_mhxs.at(indtmp)==v_mh.at(im)){found=true;ind=indtmp;}
      indtmp++;
      if(indtmp==v_mhxs.size()){
	cout<<"m="<<v_mh.at(im)<<" NOT found in theor matrix of size "<<v_mhxs.size()<<endl;
	//	cout<<"M "<<v_mh.at(nMass-1)<<"  Theor "<<v_mhxs.at(v_mhxs.size()-1)<<endl;
	if(im==nMass-1){//patch for 600 GeV
	  found = true;
	}
	break;
      }
    }//end while    

    // if(v_mh.at(im)<226.0)found=false;

    if(!found){
        cout<<"(2) m="<<v_mh.at(im)<<" NOT found in theor matrix."<<endl;
      continue;
    }

    //cout<<"  Found mass, xs=  "<<v_ggxs.at(ind)<<endl;
    double xs=double(v_ggxs.at(ind));

    mass[nMassEff]=v_mh.at(im);
    //if( mass[nMassEff]==600.0)cout<<"=============> 600 !!!"<<endl;
    obs_lim_cls[nMassEff]=v_obs.at(im);
    nMassEff++;

    if(plotMCMC){
      bool skipMCMCobs=false;
      if(v_mh.at(im)==238)skipMCMCobs=true;
      if(!skipMCMCobs){
	mass_mcmc[nMassEff2]=v_mh.at(im);
	obs_lim_mcmc[nMassEff2]=v_obs_mcmc.at(im);
	median_mcmc[nMassEff2]=v_median_mcmc.at(im);
	nMassEff2++;
      }
    }

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
    //    skip95=v_mh.at(im)==204||v_mh.at(im)==208||v_mh.at(im)==212||v_mh.at(im)==214|| v_mh.at(im)==232 || v_mh.at(im)==240  || v_mh.at(im)==240 || v_mh.at(im)==244 || v_mh.at(im)==252 || v_mh.at(im)==256 || v_mh.at(im)==264 || v_mh.at(im)==272 || v_mh.at(im)==274  || v_mh.at(im)==278|| v_mh.at(im)==288 ;
    if(int(v_mh.at(im))%10==0&&v_mh.at(im)!=240)skip95=false;
    else skip95=true;
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
  TGraph *grobslim_mcmc=new TGraphAsymmErrors(nMassEff2,mass_mcmc,obs_lim_mcmc);
  grobslim_mcmc->SetName("LimitObservedMCMC");
  TGraph *grmedian_cls=new TGraphAsymmErrors(nMassEff1,mass1,medianD);
  grmedian_cls->SetName("LimitExpectedCLs");
  TGraph *grmedian_mcmc=new TGraphAsymmErrors(nMassEff2,mass_mcmc,median_mcmc);
  grmedian_mcmc->SetName("LimitExpectedMCMC");
  TGraphAsymmErrors *gr68_cls=new TGraphAsymmErrors(nMassEff1,mass1,medianD,0,0,down68err,up68err);
  gr68_cls->SetName("Limit68CLs");
  TGraphAsymmErrors *gr95_cls=new TGraphAsymmErrors(nM95,mass95,median95,0,0,down95err,up95err);
  gr95_cls->SetName("Limit95CLs");
  TGraphAsymmErrors *grthSM=new TGraphAsymmErrors(nMassEff,mass,ggxs,0,0,ggxs_downerr,ggxs_uperr);//ggxs_downerr,ggxs_uperr);
  grthSM->SetName("SMXSection");

  // cout<<"Plotting"<<endl;


  TCanvas *cMCMC=new TCanvas("c_lim_MCMC","canvas with limits for MCMC",630,600);
  cMCMC->cd();
  cMCMC->SetGridx(1);
  cMCMC->SetGridy(1);
  // draw a frame to define the range
  double fr_left=190.0, fr_down=0.09,fr_right=610.0,fr_up=8.0;
  TH1F *hr = cMCMC->DrawFrame(fr_left,fr_down,fr_right,fr_up,"frame1");
  hr->SetXTitle("m_{H} [GeV]");
  hr->SetYTitle("#sigma_{95%} / #sigma_{SM}");// #rightarrow 2l2q
  // cMCMC->GetFrame()->SetFillColor(21);
  //cMCMC->GetFrame()->SetBorderSize(12);
  
  gr95_cls->SetFillColor(kYellow);
  gr95_cls->SetFillStyle(1001);//solid
  gr95_cls->SetLineStyle(kDashed);
  gr95_cls->SetLineWidth(3);
  gr95_cls->GetXaxis()->SetTitle("m_{H} [GeV]");
  gr95_cls->GetYaxis()->SetTitle("#sigma_{95%} / #sigma_{SM}");// #rightarrow 2l2q
  gr95_cls->GetXaxis()->SetRangeUser(fr_left,fr_right);
  
  gr95_cls->Draw("3");
  
  gr68_cls->SetFillColor(kGreen);
  gr68_cls->SetFillStyle(1001);//solid
  gr68_cls->SetLineStyle(kDashed);
  gr68_cls->SetLineWidth(3);
  gr68_cls->Draw("3same");
  grmedian_cls->GetXaxis()->SetTitle("m_{H} [GeV]");
  grmedian_cls->GetYaxis()->SetTitle("#sigma_{95%} / #sigma_{SM}");// #rightarrow 2l2q
  grmedian_cls->SetMarkerStyle(24);//25=hollow squre
  grmedian_cls->SetMarkerColor(kBlack);
  grmedian_cls->SetLineStyle(2);
  grmedian_cls->SetLineWidth(3);
  grmedian_cls->SetMinimum(0.0);
  grmedian_cls->SetMaximum(8.0);
 
  grobslim_cls->SetMarkerColor(kBlack);
  grobslim_cls->SetMarkerStyle(21);//24=hollow circle
  grobslim_cls->SetMarkerSize(1.0);
  grobslim_cls->SetLineStyle(1);
  grobslim_cls->SetLineWidth(3);

  grobslim_mcmc->SetMarkerColor(kViolet+8);
  grobslim_mcmc->SetMarkerStyle(20);//24=hollow circle
  grobslim_mcmc->SetMarkerSize(0.85);
  grobslim_mcmc->SetLineStyle(1);
  grobslim_mcmc->SetLineWidth(1.5);
  grmedian_mcmc->GetXaxis()->SetTitle("m_{H} [GeV]");
  grmedian_mcmc->GetYaxis()->SetTitle("#sigma_{95%} / #sigma_{SM}");// #rightarrow 2l2q
  grmedian_mcmc->SetMarkerStyle(20);//25=hollow squre
  grmedian_mcmc->SetMarkerColor(kViolet+8);
  grmedian_mcmc->SetLineColor(kViolet+8);
  grmedian_mcmc->SetLineStyle(2);
  grmedian_mcmc->SetLineWidth(2.0);
  grmedian_mcmc->SetMinimum(0.0);
  grmedian_mcmc->SetMaximum(8.0);  


  grthSM->SetLineColor(kRed);
  grthSM->SetLineWidth(2);
  grthSM->SetFillColor(kRed);
  grthSM->SetFillStyle(3344);
  //  grthSM->Draw("L3same");

  if(plotMCMC){
    grobslim_mcmc->Draw("LP");
    grmedian_mcmc->Draw("L");
  }
  grmedian_cls->Draw("L");
  grobslim_cls->Draw("LP");

 //draw grid on top of limits
  gStyle->SetOptStat(0);
  TH1D* postGrid = new TH1D("postGrid","",1,fr_left,fr_right);
  postGrid->GetYaxis()->SetRangeUser(fr_down,fr_up);
  postGrid->Draw("AXIGSAME");

  TLine *l1=new TLine();
  l1->SetLineStyle(1);
  l1->SetLineWidth(2.0);
  l1->SetLineColor(kRed);
  l1->DrawLine(fr_left,1.0,fr_right,1.0);
  cMCMC->Update();
  // cMCMC->RedrawAxis("");
  gPad->RedrawAxis("");
  // hr->GetYaxis()->DrawClone();
  cMCMC->Update();
  


  //more graphics
   TLegend *leg = new TLegend(.25,.71,.60,.90);
   leg->SetFillColor(0);
   leg->SetShadowColor(0);
   leg->SetTextFont(42);
   leg->SetTextSize(0.03);
   //   leg->SetBorderMode(0);
   leg->AddEntry(grobslim_cls, "CL_{S} Observed", "LP");
   //   leg->AddEntry(grmedian_cls, "CL_{S} Expected", "L");
   leg->AddEntry(gr68_cls, "CL_{S} Expected #pm 1#sigma", "LF");
   leg->AddEntry(gr95_cls, "CL_{S} Expected #pm 2#sigma", "LF");
   leg->AddEntry(grobslim_mcmc, "Bayesian Observed", "LP");
   leg->AddEntry(grthSM, "SM", "L");// #rightarrow 2l2q
   leg->Draw();
   
   if(useNewStyle){

   TPaveText* cmslabel = new TPaveText( 0.145, 0.953, 0.6, 0.975, "brNDC");
   cmslabel->SetFillColor(kWhite);
   cmslabel->SetTextSize(0.038);
   cmslabel->SetTextAlign(11);
   cmslabel->SetTextFont(62);
   cmslabel->SetBorderSize(0);
    std::string leftText = "CMS Preliminary 2011";
    // std::string leftText = "CMS 2011";
   std::string units = "fb ^{-1}";
   char lumiText[300];
   sprintf( lumiText, "%.1f %s", intLumi, units.c_str());
   cmslabel->AddText(Form("%s, %s", leftText.c_str(), lumiText));
   //cmslabel->AddText(Form("%s,  #sqrt{s} = 7 TeV, %s", leftText.c_str(), lumiText));
   cmslabel->Draw();

   TPaveText* label_sqrt = new TPaveText(0.7,0.953,0.96,0.975, "brNDC");
   label_sqrt->SetFillColor(kWhite);
   label_sqrt->SetBorderSize(0);
   label_sqrt->SetTextSize(0.038);
   label_sqrt->SetTextFont(42);   
   label_sqrt->SetTextAlign(31); // align right
   label_sqrt->AddText("#sqrt{s} = 7 TeV");
      label_sqrt->Draw();

    /*
     TPaveText* cmslabel = new TPaveText( 0.152, 0.953, 0.8, 0.975, "brNDC" );
     cmslabel->SetFillColor(kWhite);
     cmslabel->SetTextSize(0.038);
     cmslabel->SetTextAlign(11);
     cmslabel->SetTextFont(62);
     cmslabel->SetBorderSize(0);

     //     cmslabel->AddText("CMS Preliminary 2011, ");
     cmslabel->AddText(Form("CMS Preliminary 2011, %.1f fb^{-1}",intLumi) );
     cmslabel->Draw();

     TPaveText* label_sqrt = new TPaveText(0.75,0.955,1.04,0.988, "brNDC");
     label_sqrt->SetFillColor(kWhite);
     label_sqrt->SetTextSize(0.038);
     label_sqrt->SetTextFont(42);
     label_sqrt->AddText("#sqrt{s} = 7 TeV");
     label_sqrt->SetBorderSize(0);
     label_sqrt->Draw();
    */
   }
   else{

   TLatex * latex = new TLatex();
   latex->SetNDC();
   latex->SetTextSize(0.04);
   latex->SetTextAlign(31);
   latex->SetTextAlign(11); // align left 
   latex->DrawLatex(0.18, 0.96, "CMS preliminary 2011");
   latex->DrawLatex(0.60,0.96,Form("%.1f fb^{-1} at #sqrt{s} = 7 TeV",intLumi));
   
   }
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

void setFPStyle(){


  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);

  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);//0.13);
  gStyle->SetPadLeftMargin(0.15);//0.16);
  gStyle->SetPadRightMargin(0.05);//0.02);



 // For the Pad:
  gStyle->SetPadBorderMode(0);
  // gStyle->SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  gStyle->SetPadTickY(1);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);


  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.05, "XYZ");
  // gStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // gStyle->SetTitleYSize(Float_t size = 0.02);
  gStyle->SetTitleXOffset(1.15);//0.9);
  gStyle->SetTitleYOffset(1.3); // => 1.15 if exponents
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.045, "XYZ");

  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);


  
}
