#include "TH1F.h"
#include "TCanvas.h"
#include <Riostream.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


#include "RooRealVar.h"
#include "RooAddPdf.h"
#include "RooGenericPdf.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooProdPdf.h"
#include "RooFFTConvPdf.h"
#include "RooPolynomial.h"
#include "RooWorkspace.h"
#include "RooCBShape.h"
#include "RooExponential.h"

#include "PDFs/RooRodenbach.h"
#include "PDFs/RooFermi.h"
#include "PDFs/RooDoubleCB.h"
#include "PDFs/RooCB.h"
#include "PDFs/RooRelBW.h"
#include "PDFs/Triangle.h"

#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TROOT.h"

using namespace std;
//using namespace ROOT::Math;
using namespace RooFit;

void make_roofitfiles(int btag, int chan, double massH, double sigmaH, double &obs_yield, double &exp_yield, vector<double> cb_pars);
double readVal(int btag, int chan, double massH,std::string varname);


//inputs: btag category, observed bkg yield (-> expected one for MC limit calc)
//        mass of Higgs, sigma of Higgs, 4 parameters of CB (depend on mass)

void make_roofitfiles(int btag, int chan, double massH, double sigmaH, double &obs_yield, double &exp_yield, vector<double> cb_pars){

  gSystem->Load("libRooFit");
  cout<<"Trying to load custom PDFS ..."<<flush<<endl;
  gSystem->Load("libFFTW");

  string str_btag="dummyb";
  if(btag==0)str_btag="0b";
  else if(btag==1)str_btag="1b";
  else if(btag==2)str_btag="2b";
  else cout<<"Unrecognized number of btags: "<<btag<<endl;

  string str_chan="dummychan";
  if(chan==0)str_chan="ee";
  else if(chan==1)str_chan="mm";
  else cout<<"Unrecognized number of channels: "<<chan<<endl;


  //integration window
  double effWidth=sqrt(sigmaH*sigmaH+100);  // effective width used for defining window
  double fitRangeLow=180.;
  double fitRangeHigh=800.0; 

  cout<<"----- FIT RANGE : "<<fitRangeLow<<" - "<< fitRangeHigh<<endl;

  ////////////////////////////////////////////////////////////////

  std::ostringstream ossm;
  ossm<<massH; 
  string str_massH=ossm.str() ;

  // --------------------- initial values -----------------
  RooRealVar CMS_hzz2l2q_mZZ("CMS_hzz2l2q_mZZ", "zz inv mass",fitRangeLow,fitRangeHigh);

  // ==================== defining bkg PDF ==========================

  //CB for EPS/////////////////////////////////////////////////////////////

  // ------------------------ fermi ------------------------------            
  vector<double> BKGparam;
  //eps ones
  if(btag==0){
     BKGparam.push_back(186.41); //cutoff
     BKGparam.push_back(5.257); //beta
     BKGparam.push_back(222.72); //mean
     BKGparam.push_back(-.116428); //width
     BKGparam.push_back(13.39); //n
     BKGparam.push_back(54.704); //alpha
     BKGparam.push_back(1.589); //theta
     }else if(btag==1){
     BKGparam.push_back(184.04);
     BKGparam.push_back(3.225);
     BKGparam.push_back(166.6);
     BKGparam.push_back(87.467);
     BKGparam.push_back(21.499);
     BKGparam.push_back(.24946);
     BKGparam.push_back(0.0162); //theta
     }else if(btag==2){
     BKGparam.push_back(182.32);
     BKGparam.push_back(2.84922);
     BKGparam.push_back(226.23);
     BKGparam.push_back(72.124);
     BKGparam.push_back(2.75833);
     BKGparam.push_back(-.45718);
     BKGparam.push_back(0.0166); //theta
  }
  
  // -------------------- fermi ---------------------------
  RooRealVar cutOff_BKG("cutOff_BKG","position of fermi",BKGparam.at(0),0,1000);
  cutOff_BKG.setConstant(kTRUE);
  RooRealVar beta_BKG("beta_BKG","width of fermi",BKGparam.at(1),0,50);
  beta_BKG.setConstant(kTRUE);
	     		       
  RooFermi fermi_BKG("fermi_BKG","fermi function",CMS_hzz2l2q_mZZ,cutOff_BKG,beta_BKG);
  // -------------------- double gauss ---------------------------
  //par0 will be the overall bkgd normalization, used in the datacard
  string bkgp1name="CMS_hzz2l2q_bkg"+str_btag+"p1"; //m
  string bkgp2name="CMS_hzz2l2q_bkg"+str_btag+"p2"; //width
  string bkgp3name="CMS_hzz2l2q_bkg"+str_btag+"p3"; //n
  string bkgp4name="CMS_hzz2l2q_bkg"+str_btag+"p4"; //alpha
  string bkgp5name="CMS_hzz2l2q_bkg"+str_btag+"p5"; //theta (rotation)

  RooRealVar m(bkgp1name.c_str(),bkgp1name.c_str(),BKGparam.at(2),100.,1000.);
  m.setConstant(kTRUE);
  RooRealVar wdth(bkgp2name.c_str(),bkgp2name.c_str(),BKGparam.at(3),0,1000);
  wdth.setConstant(kTRUE);
  RooRealVar n(bkgp3name.c_str(),bkgp3name.c_str(),BKGparam.at(4),0.,1001.);//2.75833,0,1000);
  n.setConstant(kTRUE);
  RooRealVar alpha(bkgp4name.c_str(),bkgp4name.c_str(),BKGparam.at(5),-100,100);  //0,100);  //,-100,0);
  alpha.setConstant(kTRUE);
  RooRealVar theta(bkgp5name.c_str(),bkgp5name.c_str(),BKGparam.at(6),-3.1415,3.1415); 
  theta.setConstant(kTRUE);

  RooCB CB_BKG("CB_BKG","Crystal ball",CMS_hzz2l2q_mZZ,m,wdth,alpha,n, theta);
  RooProdPdf background("background","background",RooArgSet(fermi_BKG,CB_BKG));
  
  ///////////////////////////////////////////////////////////////////////////////////

  ////Fill dataset with REAL DATA 

  RooRealVar nBTags("nBTags","nBTags",-1.,3.);
  RooRealVar eventWeight("eventWeight","eventWeight",0,100.);
  RooRealVar mZjj("mZjj","mZjj",0,150.);
  RooRealVar leptType("leptType","lepton type",-1,2);

  string btag_sel="dummy";
  if(btag==0)btag_sel="nBTags==0.0";
  else if(btag==1)btag_sel="nBTags==1.0";
  else if(btag==2)btag_sel="nBTags==2.0";
  else btag_sel="DUMMYnBTags==99.0";
  string lept_sel= chan==0 ? "leptType==1.0" :"leptType==0.0" ;//opposite convention btw Francesco and me
  string tree_sel= btag_sel+" && mZjj>75.0 && mZjj<105.0 && "+lept_sel;
  stringstream ossmzz1;
  ossmzz1 << float(fitRangeLow);
  string mzzcut="CMS_hzz2l2q_mZZ>"+ossmzz1.str(); 
  stringstream ossmzz2;
  ossmzz2 << float(fitRangeHigh);
  mzzcut+="&&CMS_hzz2l2q_mZZ<"+ossmzz2.str();
  cout<<"$$$$$$ TEMP SEL:  "<<mzzcut.c_str()<<"  $$$$$$$$$$$$$$$$$$$$$$ "<<fitRangeLow<<" - "<< fitRangeHigh<<endl;
  tree_sel+=" && "+mzzcut;
 

  TFile* file = new TFile("./convertedTree_LP_20110811.root");
  RooFormulaVar cut1("mycut1",tree_sel.c_str(),RooArgList(CMS_hzz2l2q_mZZ,nBTags,mZjj,leptType));
  RooDataSet *dataset_obs_orig=new RooDataSet("dataset_obs_orig","dataset_obs_orig",(TTree*)file->Get("tree_passedEvents"),
					      RooArgSet(CMS_hzz2l2q_mZZ,nBTags,mZjj,leptType),
					      cut1,"eventWeight");

  obs_yield=double(dataset_obs_orig->numEntries());

  RooArgSet *newMZZargset= new RooArgSet(CMS_hzz2l2q_mZZ);
  RooDataSet *dataset_obs=(RooDataSet*) dataset_obs_orig->reduce(*newMZZargset);
  dataset_obs->SetName("dataset_obs");
  cout<<"Dataset entries: ORIG "<< dataset_obs_orig->sumEntries()<< "   NEW "<<dataset_obs->sumEntries()<<endl;
  // ----------------------------------------------

  // ====================== defining signal PDF =========================

  // ------------------- Crystal Ball (matched) -------------------------------
  string sigp1name="CMS_hzz2l2q_sig"+str_btag+"p1"; //m
  string sigp2name="CMS_hzz2l2q_sig"+str_btag+"p2"; //width
  RooRealVar CB_mean(sigp1name.c_str(),sigp1name.c_str(), readVal(btag,chan,massH,"matched_MassCBmean"));
  RooRealVar CB_sigma(sigp2name.c_str(),sigp2name.c_str(),readVal(btag,chan,massH,"matched_MassCBsigma"));
  RooRealVar CB_alpha1("CB_alpha1","param 3 of CB",readVal(btag,chan,massH,"matched_MassCBalpha1"));
  RooRealVar CB_n1("CB_n1","param 4 of CB",readVal(btag,chan,massH,"matched_MassCBn1"));
  RooRealVar CB_alpha2("CB_alpha2","param 3 of CB",readVal(btag,chan,massH,"matched_MassCBalpha2"));
  RooRealVar CB_n2("CB_n2","param 5 of CB",readVal(btag,chan,massH,"matched_MassCBn2"));

  RooDoubleCB CB_SIG("CB_SIG","Crystal Ball",CMS_hzz2l2q_mZZ,CB_mean,CB_sigma,CB_alpha1,CB_n1,CB_alpha2,CB_n2);


  // ------------------- SmearedTriangle (un-matched) -------------------------------
  RooRealVar CB_UMmean( "CB_UMmean"," CB_UMmean", readVal(btag,chan,massH,"unmatched_MassCBmean"));
  RooRealVar CB_UMsigma("CB_UMsigma","CB_UMsigma",readVal(btag,chan,massH,"unmatched_MassCBsigma"));
  RooRealVar CB_UMalpha("CB_UMalpha","CB_UMalpha",readVal(btag,chan,massH,"unmatched_MassCBalpha"));
  RooRealVar CB_UMn("CB_UMn","CB_UMn",readVal(btag,chan,massH,"unmatched_MassCBn"));
  RooCBShape CB_UM("CB_UM","Crystal Ball unmacthed",CMS_hzz2l2q_mZZ,CB_UMmean,CB_UMsigma ,CB_UMalpha,CB_UMn);

  RooRealVar TRI_start("TRI_start","TRI_start", readVal(btag,chan,massH,"unmatched_Mass_start"));
  RooRealVar TRI_turn("TRI_turn","TRI_turn", readVal(btag,chan,massH,"unmatched_Mass_turn"));
  RooRealVar TRI_stop("TRI_stop","TRI_stop", readVal(btag,chan,massH,"unmatched_Mass_stop"));
  //RooGenericPdf TRI("TRI","TRI","(@0>(@1+@2))*(@0<@2)*(-@0/@1 +1 + @2/@1 ) + (@0>=@2)*(@0<(@2+@3))*(-@0/@3 +1 + @2/@3 )",RooArgList(CMS_hzz2l2q_mZZ,TRI_start,TRI_turn,TRI_stop));
  Triangle TRI("TRI","TRI",CMS_hzz2l2q_mZZ,TRI_start,TRI_turn,TRI_stop);

  //------------------------ convolution -------------------------
  
  CMS_hzz2l2q_mZZ.setBins(10000,"fft");

  RooFFTConvPdf TRI_SMEAR("TRI_SMEAR","triangle (X) CB",CMS_hzz2l2q_mZZ,TRI,CB_UM);
  TRI_SMEAR.setBufferFraction(1.0);
  

  //------------------------ add matched and unmatched -------------------------
  RooRealVar MATCH("MATCH","MATCH", readVal(btag,chan,massH,"N_matched"));
  RooAddPdf signal("signal","signal",CB_SIG,TRI_SMEAR,MATCH);
  
  //   RooProdPdf signal("signal","signal",RooArgSet(sig,fermi_SIG,fermi2_SIG));

  //RooProdPdf signal_ggH(signal, "ggH_prodPDF");
  //RooProdPdf signal_VBF(signal, "qqH_prodPDF");
 

  //--- write everything into the workspace -------  

  RooWorkspace* w = new RooWorkspace("w","w");
  w->addClassDeclImportDir("/afs/cern.ch/cms/slc5_amd64_gcc434/lcg/roofit/5.28.00a-cms12/include/");
  // w->addClassDeclImportDir("/afs/cern.ch/user/w/whitbeck/scratch0/HiggsStats/newHiggsStats/CMSSW_4_1_3/src/HiggsAnalysis/CombinedLimit/data/PDFs/");
  //w->addClassDeclImportDir("/afs/cern.ch/user/b/bonato/scratch0/PhysAnalysis/CMSSW_4_2_3_patch5/src/ZJetsAnalysis/ZJetsAnalysisV1/test/statistical_tools/PDFs/");
  w->addClassDeclImportDir("../PDFs/");

  w->importClassCode(RooFermi::Class(),kTRUE);
  w->importClassCode("RooFermi",kTRUE);
  w->importClassCode(RooRelBW::Class(),kTRUE);
  w->importClassCode("RooRelBW",kTRUE);
  w->importClassCode(RooDoubleCB::Class(),kTRUE);
  w->importClassCode("RooDoubleCB",kTRUE);
  w->importClassCode(RooCB::Class(),kTRUE);
  w->importClassCode("RooCB",kTRUE);
  w->importClassCode(Triangle::Class(),kTRUE);
  w->importClassCode("Triangle",kTRUE);
  //fro roorodenbach!!!!!!!!!!!!!!!!!!!!!!
  //w->importClassCode(RooRodenbach::Class(),kTRUE);
  //w->importClassCode("RooRodenbach",kTRUE);
  w->import(background);
  w->import(signal);
  // w->import(signal_ggH);
  // w->import(signal_VBF);
  w->import(*dataset_obs);

  

  //string outFileName="datacards_20110803_epsrotatedRange/"+str_massH+"/hzz2l2q_"+str_chan+str_btag+".input.root";
  string outFileName="datacards_20110929_CLs/"+str_massH+"/hzz2l2q_"+str_chan+str_btag+".input.root";
  
  TFile* outFile = new TFile(outFileName.c_str(),"RECREATE");
  w->Write();
  outFile->Close();

  //calculate expected bkg events
  //eps functions ////////////////////////////
  RooRealVar CMS_hzz2l2q_mZZfull("CMS_hzz2l2q_mZZfull", "zz inv mass",183.0 ,800.0);
  //expo functions
  //RooRealVar CMS_hzz2l2q_mZZfull("CMS_hzz2l2q_mZZfull", "zz inv mass",230.0 ,800.0);

  //expo////////////////////////////////////////////////////////////
  //RooExponential backgroundFull("backgroundFull","Exponential background over Full range",CMS_hzz2l2q_mZZfull,slope);
  
  //eps ////////////////////////////////////////////////////////////////////////////////////////
  RooGenericPdf fermiFull("fermiFull","fermi function","1/(1+exp((@1-@0)/@2))",RooArgList(CMS_hzz2l2q_mZZfull,cutOff_BKG,beta_BKG));
  RooCB CBbkgFull("CBbkgFull","Crystal ball for background",CMS_hzz2l2q_mZZfull,m,wdth,alpha,n, theta);
  RooProdPdf backgroundFull("backgroundFull","backgroundFull",RooArgSet(fermiFull,CBbkgFull));

  //new shape andrew////////////////////////////////////////////////////////////////////////////////
  /*RooRodenbach RodFull("RodFull","Rod",CMS_hzz2l2q_mZZfull,m,wdth,alpha);
  RooGenericPdf fermiFull("fermiFull","fermi function","1/(1+exp((@1-@0)/@2))",RooArgList(CMS_hzz2l2q_mZZfull,cutOff,beta));
  RooProdPdf backgroundFull("backgroundFull","backgroundFull",RooArgSet(fermiFull,RodFull));
  */
  vector<double> EvtNorm;
  //first muon then electrons ///for expo /////////////////////////////////
  
  /*EvtNorm.push_back(chan==1? 228.10 : 200.12 );  // 0btag 
  EvtNorm.push_back(chan==1? 230.80 : 195.80 );  // 1btag 
  EvtNorm.push_back(chan==1?  16.82 :  13.79 );  // 2btag
  */
  //for eps///////////////////////////
  /*EvtNorm.push_back(chan==1? 345.7 : 286.4 );  // 0btag 
  EvtNorm.push_back(chan==1? 376.4 : 334.7 );  // 1btag 
  EvtNorm.push_back(chan==1? 24.3 : 20.3 );  // 2btag*/

  //for LP
  EvtNorm.push_back(chan==1? 575.85 : 490.54 );  // 0btag 
  EvtNorm.push_back(chan==1? 606.78 : 526.86 );  // 1btag 
  EvtNorm.push_back(chan==1? 41.14 : 35.37 );  // 2btag

  string mzzcut2="CMS_hzz2l2q_mZZfull>"+ossmzz1.str(); 
  mzzcut2+="&&CMS_hzz2l2q_mZZfull<"+ossmzz2.str();
  RooDataHist *BkgHisto = backgroundFull.generateBinned(CMS_hzz2l2q_mZZfull,EvtNorm.at(btag),kTRUE,kFALSE);  
  exp_yield=float( BkgHisto->sumEntries(mzzcut2.c_str() )  );
  cout<<"MH"<<massH<<"  With this cut: "<<mzzcut2.c_str()<<"  ===> "<<exp_yield<<"   TOT "<< BkgHisto->sumEntries( )<<"  mZZ<300 "<< BkgHisto->sumEntries("CMS_hzz2l2q_mZZfull<300.0" )<<endl;

  delete file;
  
}

double readVal(int btag, int chan, double massH,std::string varname){
  int masses[18] = {190,200,210,230,250,275,300,325,350,375,400,425,475,500,525,550,575,600};
  int nsamples= 18;

  RooRealVar var(varname.c_str(),varname.c_str(),0.);
  RooArgSet paramsup, paramslow;

  paramsup.add(var);
  paramslow.add(var);

  char filename[200];

  //which files to read?
  for(int i =0 ; i <18 ; i++){
    if(masses[i]==massH){//direct Match
      sprintf(filename,"fitresults/out-%d-%s-btag%d.config",masses[i],"EM",btag);
      //std::cout << filename << " : " << paramsup.readFromFile(filename, "READ", "Parameters") <<std::endl;
      paramsup.readFromFile(filename, "READ", "Parameters");
      return var.getVal();
    }
  }

  //no direct match => interpolation
  int indexlow = -1;
  int indexhigh= -1;
  for(int i =0 ; i <18 ; i++){
    if(masses[i]>massH){
      indexhigh=i;
      break;
    }
  }
  for(int i =17 ; i >-1 ; i--){
    if(masses[i]<massH){
      indexlow=i;
      break;
    }
  }
  if(indexlow==-1 || indexhigh== -1){
    std::cout << "Out of Range"<< std::endl;
    exit(1);
  }

  //std::cout << indexlow << " " << indexhigh <<std::endl;

  sprintf(filename,"fitresults/out-%d-%s-btag%d.config",masses[indexlow],"EM",btag);
  paramsup.readFromFile(filename, "READ", "Parameters");
  double low = var.getVal();
  sprintf(filename,"fitresults/out-%d-%s-btag%d.config",masses[indexhigh],"EM",btag);
  paramsup.readFromFile(filename, "READ", "Parameters");
  double high = var.getVal();
  
  double deltaM = masses[indexhigh] - masses[indexlow];
  
  return (massH-masses[indexlow])/deltaM*(high-low) + low;
  
}

