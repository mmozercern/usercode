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

TH1F* deltam,*deltamerr;
TH1F* deltan,*deltanerr;


void test(int it,RooCmdArg* arg1,RooCmdArg* arg2,RooCmdArg* arg3,RooCmdArg* arg4,RooCmdArg* arg5,RooCmdArg* arg6,float peak,float ne, float nmu){
  model = new Model(1,1,false);
  data = new DataSetHelper;
  results = new ResultCollection;

  model->mass->setRange(250,2000);
  model->peak->setRange(250,2000);
  model->peak->setConstant(kFALSE);
  model->gamma->setRange(0.01,200);
  model->truewidth=0.01;//30;
  model->fitwithwidth=0.01;
  model->truerese=0.02;//0.005;
  model->fitwithrese=0.02;//0.005;
  model->effratio->setVal(0.7);
  model->effratio->setValueDirty();
  //model->bkgslopmu->setVal();
  model->trueeff=0.7;
  model->fitwitheff=0.7;
  model->truepeak=1200; 
  model->fitwithpeak=peak;
  model->trueshift=1.0;
  model->fitwithshift=1.0;
  model->massshift->setRange(0.8,1.2);
  model->normbkge->setRange(0,200);

  results->setsighistos(3,10000,0,8);              // significance histos
  results->setpeakhistos(3,200,*model);                  // histograms for peak position ranges from mass
  results->setpeaknormhistos(3,80);                     // pull-plots for peak position
  results->setnsighistos(3,50,0,25);             // n-signal
  results->setnsignormhistos(3,50);                     // pull-plots for n-signal
  results->setnbkghistos(3,50,0,50);             // n-bkg
  results->setnbkgnormhistos(3,50);             // n-bkg
  results->setwidthhistos(3,50,*model);
  results->setwidthnormhistos(3,50,*model);
  results->set2d(1,50,50,8,*model);
  
 
   model->options.Add(arg1);
   model->options.Add(arg2);
   model->options.Add(arg3);
   model->options.Add(arg4);
   model->options.Add(arg5);
   model->options.Add(arg6);


   deltam = new TH1F("deltam","deltam",100,-100,100);
   deltamerr = new TH1F("deltamerr","deltamerr",100,-50,50);
   deltan = new TH1F("deltan","deltan",100,-50,50);
   deltanerr = new TH1F("deltanerr","deltanerr",100,-50,50);

   std::cout<< "start loop" <<std::endl;
   for(int i = 0; i< it ;i++){
     data->generatefrompdf(ne,nmu,23.4,23.4,*model,3);
     model->fit(*data);

     double peake = dynamic_cast<RooAbsReal*>(model->fitrese->floatParsFinal().find("peak"))->getVal();
     double peakerr = dynamic_cast<RooRealVar*>(model->fitrese->floatParsFinal().find("peak"))->getError();
     double peakmu = dynamic_cast<RooAbsReal*>(model->fitresmu->floatParsFinal().find("peak"))->getVal();
     double peakmurr = dynamic_cast<RooRealVar*>(model->fitresmu->floatParsFinal().find("peak"))->getError();

     double norme = dynamic_cast<RooAbsReal*>(model->fitrese->floatParsFinal().find("norm"))->getVal();
     double normerr = dynamic_cast<RooRealVar*>(model->fitrese->floatParsFinal().find("norm"))->getError();
     double normmu = dynamic_cast<RooAbsReal*>(model->fitresmu->floatParsFinal().find("norm"))->getVal();
     double normmurr = dynamic_cast<RooRealVar*>(model->fitresmu->floatParsFinal().find("norm"))->getError();

     results->fillhistos(*model,*data);

     deltam->Fill(peake-peakmu);
     deltamerr->Fill((peake-peakmu)/sqrt(peakerr*peakerr+peakmurr*peakmurr));
     deltan->Fill(norme-normmu);
     deltanerr->Fill((norme-normmu)/sqrt(normerr*normerr+normmurr*normmurr));
     
     
   }



}

