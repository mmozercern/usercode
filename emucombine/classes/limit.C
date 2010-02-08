#include "Model.h"
#include "DataSetHelper.h"
#include "ResultCollection.h"
#include <iostream>
#include <sstream>

using namespace RooFit;

Model* model;
DataSetHelper* data;
ResultCollection* results;
TGraph* exclusionslopee;
TGraph* exclusionslopemu;
TGraph* exclusionslopeboth;

RooGaussian* smear;
RooProdPdf* signaleprod;
RooAbsPdf* signalint;
RooRealVar* smearwid;

TH1F* resolution;

float findfraction(TH1F* inhist,float thresh){
  int binnr = inhist->FindBin(thresh);
  return inhist->Integral(1,binnr)/inhist->GetEntries();
}

void test(int it, float lumi=50.){
  model = new Model(1,1,false);
  data = new DataSetHelper;
  results = new ResultCollection;

  model->mass->setRange(300,2000);
  model->peak->setRange(300,2000);
  //model->gamma->setRange(0.01,200);
  //model->truewidth=0.01;
  //model->fitwithwidth=0.01;
  model->truerese=0.005;
  model->fitwithrese=0.005;
  model->effratio->setVal(0.7);
  model->effratio->setValueDirty();
  //model->effratio->setRange(0.0001,1000);
  model->trueeff=0.7;
  model->fitwitheff=0.7;
  model->truepeak=1000; 
  model->fitwithpeak=1000;
  model->trueshift=1.0;
  model->fitwithshift=1.0;
  model->massshift->setRange(0.8,1.2);
  model->normbkge->setRange(0,200);


  results->setsighistos(3,120000,0,30);              // significance histos
  results->setpeakhistos(3,200,*model);                  // histograms for peak position ranges from mass
  results->setpeaknormhistos(3,80);                     // pull-plots for peak position
  results->setnsighistos(3,50,0,25);             // n-signal
  results->setnsignormhistos(3,50);                     // pull-plots for n-signal
  results->setnbkghistos(3,50,0,50);             // n-bkg
  results->setnbkgnormhistos(3,50);             // n-bkg
  results->setwidthhistos(3,50,*model);
  results->setwidthnormhistos(3,50,*model);
  results->set2d(3,50,50,8,*model);
  results->setmassshift(50,*model);                    
  results->seteffratio(50,*model);                    

  RooCmdArg arg1=Minos(kFALSE);
  RooCmdArg arg2=Hesse(kFALSE);
  RooCmdArg arg3=PrintLevel(-1);
  RooCmdArg arg4=Save(kTRUE);
  RooCmdArg arg5=Extended(kTRUE);
  RooCmdArg arg6=Optimize(kFALSE);
  RooCmdArg arg7=Strategy(1);

  model->options.Add(&arg1);
  model->options.Add(&arg2);
  model->options.Add(&arg3);
  model->options.Add(&arg4);
  model->options.Add(&arg5);
  model->options.Add(&arg6);

  TRandom3 rand(0);

//   RooVoigtian* unsmeared = model->signale;

//   model->sigmarele->setRange(0.001,0.05);
//   smearwid = new RooRealVar("smearwid","smearwid",0.004);
//   smear = new RooGaussian("smear","smeared resolution",*(model->sigmarele),RooConst(model->truerese),*smearwid);
//   model->sigmae     = new RooProduct("sigmaex","gaus sigma e",RooArgSet(*(model->sigmarele),*(model->peak))); 
//   model->signale    = new RooVoigtian("signalex","signal pdf e",*(model->mass),*(model->shiftedpeak),*(model->gamma),*(model->sigmae));
//   signaleprod = new RooProdPdf("signalprod","signal prod",RooArgSet(*smear,*(model->signale)));
//   signalint = signaleprod->createProjection(RooArgSet(*(model->sigmarele)));

//   model->extsige=new RooExtendPdf("extsigex","ext sig e",*signalint,*(model->norme));
//   model->sume = new RooAddPdf("sumex","signal + bkg e",RooArgList(*(model->extsige),*(model->extbkge)));

//   model->signale= unsmeared;
  
  for(int i = 0; i< it ;i++){
    float shift1  =1;//rand.Gaus(1.,0.3);
    float shift1m =1;//rand.Gaus(1.,0.3);
    float shift2  =1;//rand.Gaus(1.,0.3);
    float shift3  =1;//rand.Gaus(1.,0.05);
    
    float lumi2 = lumi  * shift1 *shift2;    

    //float r = rand.Gaus(0.0,1.);
    //float smear=exp(1.24605*r-3.7363);    //lognormal mode=0.005, variance 0.01 
    //float smear=exp(1.01401*r-4.2608);    //lognormal mode=0.005, variance 0.001 = 0.032 "width"
    //float smear=exp(0.745547*r-4.73483);  // mode=0.005 var = 0.0001
    //float smear=exp(0.448628*r-5.1067);  // mode=0.005 var = 0.00001
    //model->truerese=smear;
    
    //float rm = rand.Gaus(0.0,1.);
    //float smearm=exp(0.626539*rm-2.13266);   //lognormal mode=0.08, variance 0.01
    //float smearm=exp(0.327573*rm-2.41855);   //lognormal mode=0.08, variance 0.001
    //float smearm=exp(0.121788*rm-2.5182);    // mode=0.08, var = 0.0001 
    //float smearm=exp(0.0394209*rm-2.52417);    // mode=0.08, var = 0.00001 
    //model->trueresmu=smearm;

    float shift = rand.Gaus(1.0,0.025);
    model->trueshift=shift;

    data->generatefrompdf(15./50.*lumi2,21./50.*lumi*shift1m*shift2 , 26.*shift2*shift3,37.*shift2 ,*model,3);

    model->fitwithshift=1.;
    //model->fitwithrese=0.005;
    //model->fitwithresmu=0.08;

    model->fit(*data);  
    results->fillhistos(*model,*data);
  }
}

void prodlim(int it, char* filename){
  TFile* outfile = TFile::Open(filename,"recreate");
  std::stringstream ss;

  for(int i =0 ;i<10;i++){
    test(it,i/2.*50./15.);
    ss.str("");
    ss << "nadd_"<< i;
    results->savehistos(*outfile,ss.str().c_str());
  }
}

void limit(char* filename){

  TFile* infile = TFile::Open(filename,"readonly");
  std::stringstream ss;

  //first run without signal

  ss << "nadd_" << 0 ;
  infile->cd(ss.str().c_str());
  TH1F* sige = (TH1F*)gDirectory->Get("sige");
  TH1F* sigmu = (TH1F*)gDirectory->Get("sigmu");
  TH1F* sigboth = (TH1F*)gDirectory->Get("sigboth");


  float mediane=0;
  float medianmu=0;
  float medianboth=0;
 //find median significance
  for(int i =1 ; i <= sige->GetNbinsX() ;i++)
    if(sige->Integral(1,i) >  sige->GetEntries()/2.){
      mediane = sige->GetBinLowEdge(i);
      break;
    }
  for(int i =1 ; i <= sigmu->GetNbinsX() ;i++)
    if(sigmu->Integral(1,i) >  sigmu->GetEntries()/2.){
      medianmu = sigmu->GetBinLowEdge(i);
      break;
    }
  for(int i =1 ; i <= sigboth->GetNbinsX() ;i++)
    if(sigboth->Integral(1,i) >  sigboth->GetEntries()/2.){
      medianboth = sigboth->GetBinLowEdge(i);
      break;
    }
  
  if(mediane < 0.01 ) mediane=0.01;
  if(medianmu < 0.01 ) medianmu=0.01;
  if(medianboth < 0.01 ) medianboth=0.01;
  std::cout << mediane << " " << medianmu << " "<<medianboth  <<std::endl;

  exclusionslopee = new TGraph(8);
  exclusionslopee->SetPoint(0,0,findfraction(sige,mediane));
  exclusionslopemu = new TGraph(8);
  exclusionslopemu->SetPoint(0,0,findfraction(sigmu,medianmu));
  exclusionslopeboth = new TGraph(8);
  exclusionslopeboth->SetPoint(0,0,findfraction(sigboth,medianboth));

  for(int i =1 ;i<10;i++){
    infile->cd("..");
    ss.str("");
    ss << "nadd_" << i ;
    infile->cd(ss.str().c_str());

    sige = (TH1F*)gDirectory->Get("sige");
    sigmu = (TH1F*)gDirectory->Get("sigmu");
    sigboth = (TH1F*)gDirectory->Get("sigboth");

    exclusionslopee->SetPoint(i,i/2.,findfraction(sige,mediane));    
    exclusionslopemu->SetPoint(i,i/2.,findfraction(sigmu,medianmu));    
    exclusionslopeboth->SetPoint(i,i/2.,findfraction(sigboth,medianboth));    
        
  }
  
}
