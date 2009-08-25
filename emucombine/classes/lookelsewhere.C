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

void test(int it, float lumi=1.){
  model = new Model(1,1,false);
  data = new DataSetHelper;
  results = new ResultCollection;

  model->mass->setRange(250,2000);
  model->peak->setRange(250,2000);
  model->peak->setConstant(kFALSE);
  model->gamma->setRange(0.01,200);
  model->truewidth=30;
  model->fitwithwidth=0.01;
  model->truerese=0.005;
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
  model->normbkgmu->setRange(0,200);

  results->setsighistos(3,10000,0,10);              // significance histos
  results->setpeakhistos(3,200,*model);                  // histograms for peak position ranges from mass
  results->setpeaknormhistos(3,80);                     // pull-plots for peak position
  results->setnsighistos(3,10,0,25);             // n-signal
  results->setnsignormhistos(3,50);                     // pull-plots for n-signal
  results->setnbkghistos(3,50,0,200);             // n-bkg
  results->setnbkgnormhistos(3,50);             // n-bkg
  results->set2d(3,50,50,8,*model);


  
  RooCmdArg arg1=Minos(kFALSE);  
  RooCmdArg arg2=Hesse(kFALSE);
  RooCmdArg arg3=PrintLevel(-1);
  RooCmdArg arg4=Save(kTRUE);
  RooCmdArg arg5=Extended(kTRUE);
  RooCmdArg arg6=Optimize(kFALSE);
  model->options.Add(&arg1);
  model->options.Add(&arg2);
  model->options.Add(&arg3);
  model->options.Add(&arg4);
  model->options.Add(&arg5);
  model->options.Add(&arg6);

  for(int i = 0; i< it ;i++){
    data->generatefrompdf(0,0,24.3,24.3,*model,3);
    model->fit(*data);
    results->fillhistos(*model,*data);
  }

  TFile* outfile = TFile::Open("test.root","recreate");
  results->savehistos(*outfile,"tail");

}

    
