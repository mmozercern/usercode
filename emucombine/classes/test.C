#include "Model.h"
#include "DataSetHelper.h"
#include "ResultCollection.h"

using namespace RooFit;

Model* model;
DataSetHelper* data;
ResultCollection* results;


void test(int it){
  model = new Model(1,1,false);
  data = new DataSetHelper;
  results = new ResultCollection;

  model->mass->setRange(400,2000);
  model->peak->setRange(400,2000);
  model->effratio->setVal(0.7);
  model->effratio->setValueDirty();
  model->trueeff=0.7;
  model->fitwitheff=0.7;
  model->truepeak=1100;
  model->fitwithpeak=1000;
  model->trueshift=0.90909;
  model->fitwithshift=1.;


  results->setsighistos(3,50,0,20);              // significance histos
  results->setpeakhistos(3,200,*model);                  // histograms for peak position ranges from mass
  results->setpeaknormhistos(3,20);                     // pull-plots for peak position
  results->setnsighistos(3,50,0,20);             // n-signal
  results->setnsignormhistos(3,50);                     // pull-plots for n-signal
  results->setnbkghistos(3,50,0,10);             // n-bkg
  results->setnbkgnormhistos(3,50);                     // pull-plots for n-bkg
  
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
   //   model->options.Add(&arg6);
   //   model->options.Add(&arg7);

  for(int i = 0; i< it ;i++){
    //        data->generatefrompdf(2*16.06, 2*23 , 2*5.2 , 2*6.5 ,*model,3);
    data->generatefrompdf(16.06, 23 , 5.2 , 6.5 ,*model,3);
    model->fit(*data);
    results->fillhistos(*model,*data);
  }


}

void lookelsewhere(int it){
  model = new Model(1,2,true);
  data = new DataSetHelper;
  results = new ResultCollection;

  model->mass->setRange(400,2000);
  model->peak->setRange(400,2000);
  model->massshift->setRange(0.8,1.2);
  model->effratio->setVal(0.7);
  model->effratio->setValueDirty();
  model->trueeff=0.7;
  model->fitwitheff=0.7;

  results->setsighistos(3,200,0,8);              // significance histos
  results->setpeakhistos(3,200,*model);                  // histograms for peak position ranges from mass
  results->setpeaknormhistos(3,20);                     // pull-plots for peak position
  results->setnsighistos(3,50,0,20);             // n-signal
  results->setnsignormhistos(3,50);                     // pull-plots for n-signal
  results->setnbkghistos(3,50,0,10);             // n-bkg
  results->setnbkgnormhistos(3,50);                     // pull-plots for n-bkg
  
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
   //   model->options.Add(&arg6);
   //   model->options.Add(&arg7);

  for(int i = 0; i< it ;i++){
    data->generatefrompdf( 0 , 0 , 5.2 , 6.5 ,*model,3);
    model->fit(*data);
    results->fillhistos(*model,*data);
  }


}
