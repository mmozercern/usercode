#include "Model.h"
#include "DataSetHelper.h"
#include "ResultCollection.h"
#include "RooPolynomial.h"
#include <iostream>
#include <sstream>

using namespace RooFit;

Model* model;
DataSetHelper* data;
ResultCollection* results;
TH1F* scan_sb,*scan_n,*scan_sig,*scan_noe;

void test(int it,RooCmdArg* arg1,RooCmdArg* arg2,RooCmdArg* arg3,RooCmdArg* arg4,RooCmdArg* arg5,RooCmdArg* arg6,char* file1, char* file2){
  model = new Model(1,1,false);
  data = new DataSetHelper;
  results = new ResultCollection;

  model->mass->setRange(250,2000);
  model->peak->setRange(250,2000);
  model->gamma->setRange(0.01,200);
  model->truewidth=30;
  model->fitwithwidth=0.01;
  model->truerese=0.02;//0.005;
  model->fitwithrese=0.02;//0.005;
  model->effratio->setVal(0.7);
  model->effratio->setValueDirty();
  //model->bkgslopmu->setVal();
  model->trueeff=0.7;
  model->fitwitheff=0.7;
  model->truepeak=1200; 
  model->fitwithpeak=1200;
  model->trueshift=1.0;
  model->fitwithshift=1.0;
  model->massshift->setRange(0.8,1.2);
  model->normbkge->setRange(0,200);

  results->setsighistos(1,10000,0,8);              // significance histos
  results->setpeakhistos(1,200,*model);                  // histograms for peak position ranges from mass
  results->setpeaknormhistos(1,80);                     // pull-plots for peak position
  results->setnsighistos(1,50,0,25);             // n-signal
  results->setnsignormhistos(1,50);                     // pull-plots for n-signal
  results->setnbkghistos(1,50,0,50);             // n-bkg
  results->setnbkgnormhistos(1,50);             // n-bkg
  results->setwidthhistos(1,50,*model);
  results->setwidthnormhistos(1,50,*model);
  results->set2d(1,50,50,8,*model);
  
   model->options.Add(arg1);
   model->options.Add(arg2);
   model->options.Add(arg3);
   model->options.Add(arg4);
   model->options.Add(arg5);
   model->options.Add(arg6);

   scan_sb = new TH1F("sb","sb",500,250,2000);
   scan_n  = new TH1F("n","n",500,250,2000);
   scan_sig = new TH1F("sig","sig",500,250,2000);
   scan_noe = new TH1F("noe","noe",500,250,2000);

   data->readfromtxt(file1,file2,*model,3);

   for(int i = 1; i < 501 ; i++ ){
     
     model->fitwithpeak=250.+i*1750./500.;
     model->fit(*data);
     
     double norm = dynamic_cast<RooAbsReal*>(model->fitrese->floatParsFinal().find("norm"))->getVal();
     double normerr = dynamic_cast<RooRealVar*>(model->fitrese->floatParsFinal().find("norm"))->getError();
     double bckg = dynamic_cast<RooAbsReal*>(model->fitrese->floatParsFinal().find("normbkge"))->getVal();
     double sig = sqrt(2*fabs(model->fitresbe->minNll() - model->fitrese->minNll()));
     model->mass->setVal(model->fitwithpeak);
     
     scan_sb->SetBinContent(i,norm/(bckg*model->bkge->getVal(&RooArgSet(*(model->mass)))));
     scan_n->SetBinContent(i,norm);
     scan_sig->SetBinContent(i,sig);
     scan_noe->SetBinContent(i,norm/normerr);

     //results->fillhistos(*model,*data);    
   }

   model->peak->setConstant(kFALSE);
   model->fit(*data);
   std::cout << "Z_e :" << sqrt(2*fabs(model->fitresbe->minNll() - model->fitrese->minNll())) << std::endl;
   std::cout << "Z_mu :" << sqrt(2*fabs(model->fitresbmu->minNll() - model->fitresmu->minNll()))<< std::endl;;
   std::cout << "Z_both :" << sqrt(2*fabs(model->fitresbboth->minNll() - model->fitresboth->minNll()))<< std::endl;;

}

