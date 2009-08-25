#include "DataSetHelper.h"
#include "RooGlobalFunc.h"
#include "Model.h"
#include "TreeLoader.h"

using namespace RooFit ;

DataSetHelper::DataSetHelper():
  datae(0),databe(0),datase(0),     
  datamu(0),databmu(0),datasmu(0),  
  combdata(0),                  
  meane(0),meanmu(0),meanbe(0),meanbmu(0),meanse(0),meansmu(0),
  fnameetot(""),fnamemutot(""),     
  fnamebkge(""),fnamesige(""),      
  fnamebkgmu(""),fnamesigmu(""),     
  branche(""),branchmu(""),         
  leafe(""),leafmu(""),             
  nsige(0),nbkge(0),nsigmu(0),nbkgmu(0),ntote(0),ntotmu(0),
  filereadere(0),filereadermu(0),  
  filereaderbe(0),filereaderbmu(0),
  filereaderse(0),filereadersmu(0),
  rand(0)
{
}

DataSetHelper::~DataSetHelper(){
  clear(); // delete datasets;
  if(filereadere)  delete filereadere;
  if(filereadermu) delete filereadermu;
  if(filereaderbe) delete filereaderbe;
  if(filereaderbmu)delete filereaderbmu;
  if(filereaderse) delete filereaderse;
  if(filereadersmu)delete filereadersmu;
}

void DataSetHelper::clear(){
  if(datae) delete datae;
  if(databe) delete databe;
  if(datase) delete datase;
  if(datamu) delete datamu;
  if(databmu) delete databmu;
  if(datasmu) delete datasmu;
  if(combdata) delete combdata;
}

void DataSetHelper::initfileread(){
  if( branche.length()>2 && leafe.length()>2 ){ //branch and leaf name defined for electrons
    if(fnameetot.length()>2){ 
      filereadere = new TreeLoader();
      filereadere->loadfile(fnameetot.c_str(),branche.c_str(),leafe.c_str());
    }
    if(fnamebkge.length()>2){ 
      filereaderbe = new TreeLoader();
      filereaderbe->loadfile(fnamebkge.c_str(),branche.c_str(),leafe.c_str());
    }
    if(fnamesige.length()>2){
      filereaderse = new TreeLoader();
      filereaderse->loadfile(fnamesige.c_str(),branche.c_str(),leafe.c_str());      
    }
  }
  if( branchmu.length()>2 && leafmu.length()>2 ){ //branch and leaf name defined for muons
    if(fnamemutot.length()>2){
      filereadermu = new TreeLoader();
      filereadermu->loadfile(fnamemutot.c_str(),branchmu.c_str(),leafmu.c_str());
    }
    if(fnamebkgmu.length()>2){
      filereaderbmu = new TreeLoader();
      filereaderbmu->loadfile(fnamebkgmu.c_str(),branchmu.c_str(),leafmu.c_str());
    }     
    if(fnamesigmu.length()>2){
      filereadersmu = new TreeLoader();
      filereadersmu->loadfile(fnamesigmu.c_str(),branchmu.c_str(),leafmu.c_str());      
    }
  }
}

//generate signal and background together from the sum-pdf
void DataSetHelper::generatefrompdf(float meane, float meanmu, Model& model, int whichvar){
  clear();
  //reset variable to true values
  model.peak->setVal(model.truepeak);
  model.peak->setValueDirty();
  model.effratio->setVal(model.trueeff);
  model.effratio->setValueDirty();
  model.massshift->setVal(model.trueshift);
  model.massshift->setValueDirty();
  model.bkgslopmu->setVal(model.trueslopemu);
  model.bkgslopmu->setValueDirty();
  model.gamma->setVal(model.truewidth);
  model.gamma->setValueDirty();
  model.sigmarele->setVal(model.truerese);
  model.sigmarele->setValueDirty();
  model.sigmarelmu->setVal(model.trueresmu);
  model.sigmarelmu->setValueDirty();

  if(whichvar & 1){ // fill electrons
    ntote = rand.Poisson(meane);
    
    if(ntote == 0)
      datae = new RooDataSet("datae","datae",RooArgSet(*(model.mass)));
    else
      datae = model.sume->generate(RooArgSet(*(model.mass)),ntote);
  }

  if(whichvar & 2){ // fill muons 
    ntotmu = rand.Poisson(meanmu);
        
    if(ntotmu == 0)
      datamu = new RooDataSet("datamu","datamu",RooArgSet(*(model.mass)));
    else
      datasmu = model.summu->generate(RooArgSet(*(model.mass)),ntotmu);
  }

  if(whichvar & 1  && whichvar & 2){ //fill both
    combdata = new RooDataSet("combData","combined data",*(model.mass),
			      Index(model.sample),Import("e",*datae),Import("mu",*datamu)) ;
  }
}

void DataSetHelper::generatefrompdf(float meansige, float meansigmu, float meanbkge, float meanbkgmu, 
				    Model& model, int whichvar){
  //save poisson means for generation
  meanse=meansige;
  meansmu=meansigmu;
  meanbe=meanbkge;
  meanbmu=meanbkgmu;

  int actualse = rand.Poisson(meanse);
  int actualsmu = rand.Poisson(meansmu);
  int actualbe = rand.Poisson(meanbe);
  int actualbmu = rand.Poisson(meanbmu);
    
  generatefrompdffix(actualse,actualsmu,actualbe,actualbmu, model, whichvar);

}

// generate signal and background separately
void DataSetHelper::generatefrompdffix(int meansige, int meansigmu, int meanbkge, int meanbkgmu, 
				    Model& model, int whichvar){
  clear();
  //reset variable to true values


  // set model to values for generation
  model.peak->setVal(model.truepeak);
  model.peak->setValueDirty();
  model.effratio->setVal(model.trueeff);
  model.effratio->setValueDirty();
  model.massshift->setVal(model.trueshift);
  model.massshift->setValueDirty();
  model.bkgslopmu->setVal(model.trueslopemu);
  model.bkgslopmu->setValueDirty();
  model.gamma->setVal(model.truewidth);
  model.gamma->setValueDirty();
  model.sigmarele->setVal(model.truerese);
  model.sigmarele->setValueDirty();
  model.sigmarelmu->setVal(model.trueresmu);
  model.sigmarelmu->setValueDirty();

  if(whichvar & 1){ // fill electrons
    nsige = meansige;
    nbkge = meanbkge; 
    ntote = nsige + nbkge;
    
    if(nsige == 0)
      datase = new RooDataSet("datase","datase",RooArgSet(*(model.mass)));
    else
      datase = model.signale->generate(RooArgSet(*(model.mass)),nsige);
    if(nbkge == 0)
      databe = new RooDataSet("databe","databe",RooArgSet(*(model.mass)));
    else
      databe = model.extbkge->generate(RooArgSet(*(model.mass)),nbkge);
    
    datae = new RooDataSet("datae","datae",RooArgSet(*(model.mass)));
    datae->append(*datase);
    datae->append(*databe);
  }

  if(whichvar & 2){ // fill muons 
    nsigmu = meansigmu;
    nbkgmu = meanbkgmu;
    ntotmu = nsigmu + nbkgmu;
        
    if(nsigmu == 0)
      datasmu = new RooDataSet("datasmu","datasmu",RooArgSet(*(model.mass)));
    else
      datasmu = model.signalmu->generate(RooArgSet(*(model.mass)),nsigmu);
    if(nbkgmu == 0)
      databmu = new RooDataSet("databmu","databmu",RooArgSet(*(model.mass)));
    else
      databmu = model.extbkgmu->generate(RooArgSet(*(model.mass)),nbkgmu);
    
    datamu = new RooDataSet("datamu","datamu",RooArgSet(*(model.mass)));
    datamu->append(*datasmu);
    datamu->append(*databmu);
  }

  if(whichvar & 1  && whichvar & 2){ //fill both
    combdata = new RooDataSet("combData","combined data",*(model.mass),
			      Index(model.sample),Import("e",*datae),Import("mu",*datamu)) ;
  }
}

// read data from files (background/signal separate)
void DataSetHelper::readfromfile(float meansige, float meansigmu, float meanbkge, float meanbkgmu, 
		  Model& model, int whichvar){      
  //reset datasets
  clear();
  if(whichvar & 1){
    int nsige = rand.Poisson(meansige);
    int nbkge = rand.Poisson(meanbkge);
    if(nsige == 0)
      datase = new RooDataSet("datase","datase",RooArgSet(*(model.mass)));
    else
      datase = filereaderse->getevents(nsige,*(model.mass));
    if(nbkge == 0)
      databe = new RooDataSet("databe","databe",RooArgSet(*(model.mass)));
    else
      databe = filereaderbe->getevents(nbkge,*(model.mass));
    
    if(datase !=0 && databe!=0){ //only merge after successfull read
      datae = new RooDataSet("datae","datae",RooArgSet(*(model.mass)));
      datae->append(*datase);
      datae->append(*databe);
    }
    else
      datae=0;
    
  }
  if(whichvar &2){
    int nsigmu = rand.Poisson(meansigmu);
    int nbkgmu = rand.Poisson(meanbkgmu);
     if(nsigmu == 0)
      datasmu = new RooDataSet("datasmu","datasmu",RooArgSet(*(model.mass)));
    else
      datasmu = filereadersmu->getevents(nsigmu,*(model.mass));
    if(nbkgmu == 0)
      databmu = new RooDataSet("databmu","databmu",RooArgSet(*(model.mass)));
    else
      databmu = filereaderbmu->getevents(nsigmu,*(model.mass));
    
    if(datasmu !=0 && databmu!=0){ //only merge after successfull read
      datamu = new RooDataSet("datamu","datamu",RooArgSet(*(model.mass)));
      datamu->append(*datasmu);
      datamu->append(*databmu);
    }
    else
      datamu=0;   
  }
  if(whichvar & 1  && whichvar & 2){ //fill both
    combdata = new RooDataSet("combData","combined data",*(model.mass),
			      Index(model.sample),Import("e",*datae),Import("mu",*datamu)) ;
  }
  
}

// read data from files (background/sginal together)
void DataSetHelper::readfromfile(float meane ,float meanmu ,Model& model ,int whichvar) {
  //reset datasets
  clear();

  if(whichvar & 1){
    int ne = rand.Poisson(meane);
    datae = filereadere->getevents(ne,*(model.mass));
  }
  if(whichvar &2){
    int nmu = rand.Poisson(meanmu);
    datamu = filereadermu->getevents(nmu,*(model.mass));
  }
  if(whichvar & 1  && whichvar & 2){ //fill both
    combdata = new RooDataSet("combData","combined data",*(model.mass),
			      Index(model.sample),Import("e",*datae),Import("mu",*datamu)) ;
  }
}




void DataSetHelper::readfromtxt(char* ename, char* muname,Model& model ,int whichvar) {
  //reset datasets
  clear();

  if(whichvar & 1){
    assert(ename != NULL);
    datae = RooDataSet::read(ename,RooArgSet(*(model.mass)));
  }
  if(whichvar &2){
    assert(muname != NULL);
    datamu = RooDataSet::read(muname,RooArgSet(*(model.mass)));
  }
  if(whichvar & 1  && whichvar & 2){ //fill both
    combdata = new RooDataSet("combData","combined data",*(model.mass),
			      Index(model.sample),Import("e",*datae),Import("mu",*datamu)) ;
  }
}




