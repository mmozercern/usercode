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

void getSigmaBands(string fileName);
void make_golfcourse2();
vector<TGraph*> make_golfcourseNoSignSyst();

const int nMass=8;
const  int nMass2 =8;
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
  cout << "lower 95%: " <<low95p << endl;
  down95.push_back(low95p);


  sum=0;
  iBin=2001;
  while (sum<ntoys*.025 && iBin>0){
    iBin--;
    sum+=h->GetBinContent(iBin);

  }
  double up95p= (double)iBin*15.0/2000.0;
  cout << "upper 95%: " <<up95p  << endl;
  up95.push_back(up95p);

  sum=0;
  iBin=0;
  while (sum<ntoys*.16 && iBin<2000){
    iBin++;
    sum+=h->GetBinContent(iBin);

  }
  double low68p=(double)iBin*15/2000;
  cout << "lower 68%: " << low68p << endl;
  down68.push_back(low68p);

  sum=0;
  iBin=2001;
  while (sum<ntoys*.16 && iBin>0){
    iBin--;
    sum+=h->GetBinContent(iBin);

  }
  double up68p=(double)iBin*15/2000;
  cout << "upper 68%: " << up68p << endl;
 up68.push_back(up68p);


 sum=0;
 iBin=2001;
 while (sum<ntoys*.5 && iBin>0){
   iBin--;
   sum+=h->GetBinContent(iBin);
   
 }
 double medianp=(double)iBin*15/2000;
 cout << "median: " << medianp << endl;
 median.push_back(medianp);

 delete t;
 delete f;

}


void make_golfcourse2(){

  //gROOT->ProcessLine(".L ~/tdrstyle.C");
  //setTDRStyle();
  vector<TGraph*> tgraphsNoSignSyst = make_golfcourseNoSignSyst();
  median.clear();
  up68.clear();
  down68.clear();
  up95.clear();
  down95.clear();

  //double massp[nMass]={200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,256,258,260,262,264,266,268,270,272,274,276,278,280,282,284,288,290,295,300,305,310,315,320,325,330,335,340,345,350,360,370,380,390,400,420,440,460,480,500,540,560,580,600};
  double massp[nMass]={250,300,350,400,440,500,540,600};
  string mass_str[nMass];

  for(int imass=0; imass<nMass; imass++){
   stringstream convert;
   convert << massp[imass];
   cout << convert.str() << endl;
   mass_str[imass]=convert.str();
   //convert.str();
  }

  //string random[nMass]={"25157","26136","8823","8881","15362","23832","14244","15129","22436","20088","27798","16776","27126","8291","23268","9753","15850","9839","28411","10577","31112","5316","14385","20300","24795","16757","27531","19670","3893","8634","22185","2948","29377","14249","32368","21939","20230","15468","30185","30653","22952","18780","13836","23073","29410","26421","25489","16110","11899","10381","7328","20757","29404","8079","18119","6973","32735","19608","23125","14984","11149","4816","31860","11406","26909","5720","28535","4274","683","25647","26663"};

  string filename;
  double limitTemp;
  double obs_lim_mcmc[nMass];
  for(int im=0; im<nMass; im++){
    //filename="/afs/cern.ch/user/s/sbologne/scratch0/CMSSW/CMSSW_4_2_4/src/HiggsAnalysis/CombinedLimit/test/rotatedEpsSignUncert/datacards_20110804/"+mass_str[im]+"/combine_2l2qObs_MarkovChainMC_CMS_hzz2l2q_"+mass_str[im]+"_6channels"+
    //  "/higgsCombine2l2qObs.MarkovChainMC.mH"+mass_str[im]+"."+random[im]+".root";
    filename="/afs/cern.ch/user/m/mmozer/scratch0/higgs/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/test/"+mass_str[im]+"/res/outputToy/result.root";
    cout<<filename<<endl;
    TFile* fTemp = new TFile(filename.c_str());
    cout<<"prova1"<<endl;
    TTree* tTemp = (TTree*) fTemp->Get("limit");
    tTemp->SetBranchAddress("limit",&limitTemp);
    cout<<"prova2"<<endl;
    tTemp->GetEntry(0);
    obs_lim_mcmc[im]=limitTemp;
    cout << "Observed Limit at " << massp[im] << " is " << obs_lim_mcmc[im] << endl;
  }

  for(int im=0;im<nMass;im++){
    cout << mass_str[im] << endl;
//     filename="/afs/cern.ch/user/s/sbologne/scratch0/CMSSW/CMSSW_4_2_4/src/HiggsAnalysis/CombinedLimit/test/rotatedEpsSignUncert/datacards_20110804/"+mass_str[im]+"/combine_2l2q_MarkovChainMC_CMS_hzz2l2q_"+mass_str[im]+"_6channels"+
//       "/higgsCombine2l2q.MarkovChainMC.mH"+mass_str[im]+".TOTAL.root";
    filename="/afs/cern.ch/user/m/mmozer/scratch0/higgs/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/test/"+mass_str[im]+"/res/outputToy/result.root";
    cout<<"prova2"<<endl;
    cout<<filename<<endl;
    getSigmaBands(filename);
    cout<<"prova3"<<endl;
  }

  double medianD[nMass];
  double up68err[nMass],down68err[nMass],up95err[nMass],down95err[nMass];
  for(int im=0;im<nMass;im++){
   medianD[im]=median.at(im);
   up68err[im]=up68.at(im)-median.at(im);
   down68err[im]=median.at(im)-down68.at(im);
   up95err[im]=up95.at(im)-median.at(im);
   down95err[im]=median.at(im)-down95.at(im);
   cout<<"----- MASS "<<massp[im]<<"---------"<<endl;
   cout<<"median "<<obs_lim_mcmc[im]<<endl;
   cout<<"median "<<medianD[im]<<endl;
   cout<<"up68 "<<up68err[im]<<endl;
   cout<<"down68 "<<down68err[im]<<endl;
   cout<<"up95 "<<up95err[im]<<endl;
   cout<<"down95 "<<down95err[im]<<endl;
 }

 TGraph *grobslim_pl=new TGraphAsymmErrors(nMass,massp,obs_lim_mcmc);
 grobslim_pl->SetName("LimitObserved");
 TGraph *grmedian_pl=new TGraphAsymmErrors(nMass,massp,medianD);
 grmedian_pl->SetName("LimitMCMCmedian");
 TGraphAsymmErrors *gr68_pl=new TGraphAsymmErrors(nMass,massp,medianD,0,0,down68err,up68err);
 gr68_pl->SetName("LimitMCMC68");
 TGraphAsymmErrors *gr95_pl=new TGraphAsymmErrors(nMass,massp,medianD,0,0,down95err,up95err);
 gr95_pl->SetName("LimitMCMC95");
 

 TCanvas *cMCMC=new TCanvas("c_lim_MCMC","canvas with limits for MCMC",1200,900);
 cMCMC->cd();
 cMCMC->SetGridx(1);
 cMCMC->SetGridy(1);
 // draw a frame to define the range
 TH1F *hr = cMCMC->DrawFrame(200.0,0.0,600.0,12.0,"frame1");
 hr->SetXTitle("M_{H} [GeV]");
 hr->SetYTitle("#sigma_{95%}/#sigma_{SM}");
 // cMCMC->GetFrame()->SetFillColor(21);
 //cMCMC->GetFrame()->SetBorderSize(12);

 gr95_pl->SetFillColor(kGreen);
 gr95_pl->SetFillStyle(1001);//solid
 gr95_pl->GetXaxis()->SetTitle("M_{H} [GeV]");
 gr95_pl->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
 gr95_pl->GetXaxis()->SetRangeUser(200.0,600.0);
 gr95_pl->GetYaxis()->SetRangeUser(0.0,20.);

 gr95_pl->Draw("3");
 tgraphsNoSignSyst[4]->SetMarkerColor(2);
 tgraphsNoSignSyst[4]->SetMarkerStyle(15);
 tgraphsNoSignSyst[4]->SetLineColor(2);
 tgraphsNoSignSyst[4]->Draw("LPsame");
 tgraphsNoSignSyst[5]->SetMarkerColor(2);
 tgraphsNoSignSyst[5]->SetMarkerStyle(15);
 tgraphsNoSignSyst[5]->SetLineColor(2);
 tgraphsNoSignSyst[5]->Draw("LPsame");

 gr68_pl->SetFillColor(kYellow);
 gr68_pl->SetFillStyle(1001);//solid
 gr68_pl->Draw("3same");
 tgraphsNoSignSyst[2]->SetMarkerColor(2);
 tgraphsNoSignSyst[2]->SetMarkerStyle(15);
 tgraphsNoSignSyst[2]->SetLineColor(2);
 tgraphsNoSignSyst[2]->Draw("LPsame");
 tgraphsNoSignSyst[3]->SetMarkerColor(2);
 tgraphsNoSignSyst[3]->SetMarkerStyle(15);
 tgraphsNoSignSyst[3]->SetLineColor(2);
 tgraphsNoSignSyst[3]->Draw("LPsame");


 grmedian_pl->GetXaxis()->SetTitle("M_{H} [GeV]");
 grmedian_pl->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
 grmedian_pl->SetMarkerStyle(21);
 grmedian_pl->SetMarkerColor(kGreen+3);
 grmedian_pl->SetMinimum(0.0);
 grmedian_pl->SetMaximum(8.0);
 grmedian_pl->Draw("LPsame");
 tgraphsNoSignSyst[1]->SetMarkerStyle(21);
 tgraphsNoSignSyst[1]->SetMarkerColor(2);
 tgraphsNoSignSyst[1]->SetLineColor(2);
 tgraphsNoSignSyst[1]->Draw("LPsame");
 

 grobslim_pl->SetMarkerColor(kBlue+3);
 grobslim_pl->SetMarkerStyle(24);
 grobslim_pl->SetLineStyle(2);
 //grobslim_pl->Draw("LPsame");
 tgraphsNoSignSyst[0]->SetMarkerColor(2);
 tgraphsNoSignSyst[0]->SetMarkerStyle(24);
 tgraphsNoSignSyst[0]->SetLineColor(2);
 tgraphsNoSignSyst[0]->SetLineStyle(2);
 //tgraphsNoSignSyst[0]->Draw("LPsame");

 TLine *l1=new TLine();
 l1->SetLineStyle(1);
 l1->SetLineWidth(2.0);
 l1->SetLineColor(kRed);
 l1->DrawLine(200.0,1.0,600.0,1.0);
 cMCMC->Update();
 // cMCMC->RedrawAxis("");
 //gPad->RedrawAxis("");
 // hr->GetYaxis()->DrawClone();
 cMCMC->Update();

}

vector<TGraph*> make_golfcourseNoSignSyst(){

  //gROOT->ProcessLine(".L ~/tdrstyle.C");
  //setTDRStyle();
  //double massp[nMass2]={200,202,204,206,208,210,212,214,216,218,220,224,226,228,230,232,234,236,238,240,242,244,246,250,252,254,256,258,260,262,264,266,268,270,272,274,276,278,280,282,284,286,288,290,295,300,305,310,315,320,325,330,335,340,345,350,360,370,380,390,400,420,440,460,500,520,540,560,580,600};
  double massp[nMass2]={250,300,350,400,440,500,540,600};
  string mass_str[nMass2];

  for(int imass=0; imass<nMass2; imass++){
    stringstream convert;
    convert << massp[imass];
    cout << convert.str() << endl;
    mass_str[imass]=convert.str();
    //convert.str();
  }

   //string random[nMass2]={"5102","16661","24426","9329","20919","11272","26891","15533","26966","18800","2781","5267","29256","11632","15974","26814","14604","19243","28203","13535","21721","23467","23053","19872","2834","21020","13110","4995","21126","18742","9546","12647","24876","32042","15994","27962","28286","3197","12633","6073","32176","27166","16370","32385","10161","24157","15921","17063","15167","3490","10652","29755","11069","14200","5857","14003","19739","13010","5571","9253","4051","21232","11392","18147","12900","25756","23586","8045","7668","7842"};

  string filename;
  double limitTemp;
  double obs_lim_mcmc[nMass2];
  for(int im=0; im<nMass2; im++){
//     filename="/afs/cern.ch/user/s/sbologne/scratch0/CMSSW/CMSSW_4_2_4/src/HiggsAnalysis/CombinedLimit/test/rotatedEps/datacards_20110803_epsrotated/"+mass_str[im]+"/combine_2l2qObs_MarkovChainMC_CMS_hzz2l2q_"+mass_str[im]+"_6channels"+
//       "/higgsCombine2l2qObs.MarkovChainMC.mH"+mass_str[im]+"."+random[im]+".root";
    filename="/afs/cern.ch/user/m/mmozer/scratch0/higgs/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/test/MM/test/datacards_20110929_CLs/crab/"+mass_str[im]+"/res/outputToy/result.root";
    cout<<filename<<endl;
    TFile* fTemp = new TFile(filename.c_str());
    cout<<"prova1"<<endl;
    TTree* tTemp = (TTree*) fTemp->Get("limit");
    tTemp->SetBranchAddress("limit",&limitTemp);
    cout<<"prova2"<<endl;
    tTemp->GetEntry(0);
    obs_lim_mcmc[im]=limitTemp;
    cout << "Observed Limit at " << massp[im] << " is " << obs_lim_mcmc[im] << endl;
  }

  for(int im=0;im<nMass2;im++){
    cout << mass_str[im] << endl;
//     filename="/afs/cern.ch/user/s/sbologne/scratch0/CMSSW/CMSSW_4_2_4/src/HiggsAnalysis/CombinedLimit/test/rotatedEps/datacards_20110803_epsrotated/"+mass_str[im]+"/combine_2l2q_MarkovChainMC_CMS_hzz2l2q_"+mass_str[im]+"_6channels"+
//       "/higgsCombine2l2q.MarkovChainMC.mH"+mass_str[im]+".TOTAL.root";
    filename="/afs/cern.ch/user/m/mmozer/scratch0/higgs/CMSSW_4_2_8/src/HiggsAnalysis/CombinedLimit/test/MM/test/datacards_20110929_CLs/crab/"+mass_str[im]+"/res/outputToy/result.root";
    cout<<"prova2"<<endl;
    cout<<filename<<endl;
    getSigmaBands(filename);
    cout<<"prova3"<<endl;
  }

  double medianD[nMass2];
  double up68err[nMass2],down68err[nMass2],up95err[nMass2],down95err[nMass2];
  for(int im=0;im<nMass2;im++){
   medianD[im]=median.at(im);
   up68err[im]=up68.at(im);
   down68err[im]=down68.at(im);
   up95err[im]=up95.at(im);
   down95err[im]=down95.at(im);
   cout<<"----- MASS "<<massp[im]<<"---------"<<endl;
   cout<<"median "<<obs_lim_mcmc[im]<<endl;
   cout<<"median "<<medianD[im]<<endl;
   cout<<"up68 "<<up68err[im]<<endl;
   cout<<"down68 "<<down68err[im]<<endl;
   cout<<"up95 "<<up95err[im]<<endl;
   cout<<"down95 "<<down95err[im]<<endl;
 }

 TGraph *grobslim_pl=new TGraph(nMass2,massp,obs_lim_mcmc);
 grobslim_pl->SetName("LimitObserved2");
 TGraph *grmedian_pl=new TGraph(nMass2,massp,medianD);
 grmedian_pl->SetName("LimitMCMCmedian2");
 TGraph *gr68Up_pl=new TGraph(nMass2,massp,up68err);
 gr68Up_pl->SetName("LimitUpMCMC68");
 TGraph *gr68Down_pl=new TGraph(nMass2,massp,down68err);
 gr68Down_pl->SetName("LimitDownMCMC68");
 TGraph *gr95Up_pl=new TGraph(nMass2,massp,up95err);
 gr95Up_pl->SetName("LimitUpMCMC95");
 TGraph *gr95Down_pl=new TGraph(nMass2,massp,down95err);
 gr95Down_pl->SetName("LimitDownMCMC95");

 vector<TGraph*> tgraphsNoSignSyst;
 tgraphsNoSignSyst.push_back(grobslim_pl);
 tgraphsNoSignSyst.push_back(grmedian_pl);
 tgraphsNoSignSyst.push_back(gr68Up_pl);
 tgraphsNoSignSyst.push_back(gr68Down_pl);
 tgraphsNoSignSyst.push_back(gr95Up_pl);
 tgraphsNoSignSyst.push_back(gr95Down_pl);

 return tgraphsNoSignSyst;
}
