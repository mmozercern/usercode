#include "ResultCollection.h"
#include "RooFitResult.h"
#include "Model.h"
#include "DataSetHelper.h"

ResultCollection::ResultCollection():
  sige(0),sigmu(0),sigboth(0),
  peake(0),peakmu(0),peakboth(0),
  peakepull(0),peakmupull(0),peakbothpull(0),
  nsige(0),nsigmu(0),nsigboth(0),
  nsigepull(0),nsigmupull(0),nsigbothpull(0),
  nbkge(0),nbkgmu(0),nbkgbothe(0),nbkgbothmu(0),
  nbkgepull(0),nbkgmupull(0),nbkgbothpulle(0),nbkgbothpullmu(0),
  effratioplot(0),effratiopull(0),
  massshiftplot(0),massshiftpull(0),
  totalmasse(0),totalmassmu(0),
  samplemasse(0),samplemassmu(0)
{}


ResultCollection::~ResultCollection(){
  clear();
}

void ResultCollection::clear(){
  if(sige ) delete sige ;
  if(sigmu ) delete sigmu ;
  if(sigboth ) delete sigboth ;
  if(peake ) delete peake ;
  if(peakmu ) delete peakmu ;
  if(peakboth ) delete peakboth ;
  if(peakepull ) delete peakepull ;
  if(peakmupull ) delete peakmupull ;
  if(peakbothpull ) delete peakbothpull ;
  if(nsige ) delete nsige ;
  if(nsigmu ) delete nsigmu ;
  if(nsigboth ) delete nsigboth ;
  if(nsigepull ) delete nsigepull ;
  if(nsigmupull ) delete nsigmupull ;
  if(nsigbothpull ) delete nsigbothpull ;
  if(nbkge ) delete nbkge ;
  if(nbkgmu ) delete nbkgmu ;
  if(nbkgbothe ) delete nbkgbothe ;
  if(nbkgbothmu ) delete nbkgbothmu ;
  if(nbkgepull ) delete nbkgepull ;
  if(nbkgmupull ) delete nbkgmupull ;
  if(nbkgbothpulle ) delete nbkgbothpulle ;
  if(nbkgbothpullmu ) delete nbkgbothpullmu ;
  if(effratioplot ) delete effratioplot ;
  if(effratiopull ) delete effratiopull ;
  if(massshiftplot ) delete massshiftplot ;
  if(massshiftpull ) delete massshiftpull ;
  if(totalmasse ) delete totalmasse ;
  if(totalmassmu ) delete totalmassmu ;
  if(samplemasse ) delete samplemasse ;
  if(samplemassmu ) delete samplemassmu ;
}

void ResultCollection::setsighistos(int whichvar,int nbins,float highbin,float lowbin){
  if(whichvar & 1)
    sige = new TH1F("sige","significnance e",nbins,highbin,lowbin);
  if(whichvar & 2)
    sigmu = new TH1F("sigmu","significnance mu",nbins,highbin,lowbin);
  if((whichvar & 1) && (whichvar & 2))
    sigboth = new TH1F("sigboth","significnance both",nbins,highbin,lowbin);
}
void ResultCollection::setpeakhistos(int whichvar,int nbins,Model& model){
  if(whichvar & 1)
    peake = new TH1F("peake","peak position e",nbins,model.mass->getMin(),model.mass->getMax());
  if(whichvar & 2)
    peakmu = new TH1F("peakmu","peak position mu",nbins,model.mass->getMin(),model.mass->getMax());
  if((whichvar & 1) && (whichvar & 2))
    peakboth = new TH1F("peakboth","peak position both",nbins,model.mass->getMin(),model.mass->getMax());
}
void ResultCollection::setpeaknormhistos(int whichvar,int nbins){
  if(whichvar & 1)
    peakepull = new TH1F("peakepull","peak pull e",nbins,-3.,3.);
  if(whichvar & 2)
    peakmupull = new TH1F("peakmupull","peak pull mu",nbins,-3.,3.);
  if((whichvar & 1) && (whichvar & 2))
    peakbothpull = new TH1F("peakbothpull","peak pull both",nbins,-3.,3.);
}
void ResultCollection::setnsighistos(int whichvar,int nbins,float highbin,float lowbin){
  if(whichvar & 1)
    nsige = new TH1F("nsige","n signal e",nbins,highbin,lowbin);
  if(whichvar & 2)
    nsigmu = new TH1F("nsigmu","n signal mu",nbins,highbin,lowbin);
  if((whichvar & 1) && (whichvar & 2))
    nsigboth = new TH1F("nsigboth","n signal both",nbins,highbin,lowbin);
}
void ResultCollection::setnsignormhistos(int whichvar,int nbins){
  if(whichvar & 1)
    nsigepull = new TH1F("nsigepull","n signal pull e",nbins,-3.,3.);
  if(whichvar & 2)
    nsigmupull = new TH1F("nsigmupull","n signal pull mu",nbins,-3.,3.);
  if((whichvar & 1) && (whichvar & 2))
    nsigbothpull = new TH1F("nsigbothpull","n signal pull both",nbins,-3.,3.);
}
void ResultCollection::setnbkghistos(int whichvar,int nbins,float highbin,float lowbin){
  if(whichvar & 1)
    nbkge = new TH1F("nbkge","n background e",nbins,highbin,lowbin);
  if(whichvar & 2)
    nbkgmu = new TH1F("nbkgmu","n background mu",nbins,highbin,lowbin);
  if((whichvar & 1) && (whichvar & 2)){
    nbkgbothe  = new TH1F("nbkgbothe ","n background both (e) ",nbins,highbin,lowbin);
    nbkgbothmu = new TH1F("nbkgbothmu","n background both (mu)",nbins,highbin,lowbin);
  }
}
void ResultCollection::setnbkgnormhistos(int whichvar,int nbins){
  if(whichvar & 1)
    nbkgepull = new TH1F("nbkgepull","n background pull e",nbins,-3.,3.);
  if(whichvar & 2)
    nbkgmupull = new TH1F("nbkgmupull","n background pull mu",nbins,-3.,3.);
  if((whichvar & 1) && (whichvar & 2)){
    nbkgbothpulle  = new TH1F("nbkgbothpulle ","n background pull both (e) ",nbins,-3.,3.);
    nbkgbothpullmu = new TH1F("nbkgbothpullmu","n background pull both (mu)",nbins,-3.,3.);
  }
}
void ResultCollection::seteffratio(int nbins,Model& model){
  effratioplot = new TH1F("effratioplot","efficiency ratio",nbins,model.effratio->getMin(),model.effratio->getMax());
  effratiopull = new TH1F("effratiopull","efficiency ratio pull",nbins,-3.,3.);
}
void ResultCollection::setmassshift(int nbins,Model& model){
  massshiftplot = new TH1F("massshiftplot","mass shift",nbins,model.massshift->getMin(),model.massshift->getMax());
  massshiftpull = new TH1F("massshiftpull","mass shift pull",nbins,-3.,3.);
}
void ResultCollection::settotalmass(int whichvar,int nbins,Model& model){
  if(whichvar & 1)
    totalmasse = new TH1F("totalmasse","speactrum e (total)",nbins,model.mass->getMin(),model.mass->getMax());
  if(whichvar & 2)
    totalmassmu = new TH1F("totalmassmu","speactrum mu (total)",nbins,model.mass->getMin(),model.mass->getMax());
}
void ResultCollection::setsamplemass(int whichvar,int nbins,Model& model){
  if(whichvar & 1)
    samplemasse = new TH1F("samplemasse","speactrum e (sample)",nbins,model.mass->getMin(),model.mass->getMax());
  if(whichvar & 2)
    samplemassmu = new TH1F("samplemassmu","speactrum mu (sample)",nbins,model.mass->getMin(),model.mass->getMax());
}

void  ResultCollection::fillhistos(Model& model,DataSetHelper& data){
  std::cout << "sig" << std::endl;
  // significance histgrams (will crash if histos defined for absent fits)
  if(sige)
    sige->Fill(sqrt(2*fabs(model.fitresbe->minNll() - model.fitrese->minNll())));
  if(sigmu)
    sigmu->Fill(sqrt(2*fabs(model.fitresbmu->minNll() - model.fitresmu->minNll())));
  if(sigboth)
    sigboth->Fill(sqrt(2*fabs(model.fitresbboth->minNll() - model.fitresboth->minNll())));
  //peak-positions
  std::cout << "peak" << std::endl;
  if(peake){ 
    double peak=0,masshift=1;
    if(model.peak->isConstant())
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("peak"))->getVal();
    else
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("peak"))->getVal();
    if(model.massshift->isConstant())
      masshift = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("massshift"))->getVal();
    else
      masshift = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("massshift"))->getVal();   
    peake->Fill(peak*masshift);
  }
  if(peakmu){
    if(model.peak->isConstant())
      peakmu->Fill(dynamic_cast<RooAbsReal*>(model.fitresmu->constPars().find("peak"))->getVal());
    else
      peakmu->Fill(dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("peak"))->getVal());
  }
  if(peakboth){
    if(model.peak->isConstant())
      peakboth->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("peak"))->getVal());
    else
      peakboth->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("peak"))->getVal());
  }
  std::cout << "peakpull" << std::endl;
  //peak-pulls
  if(peakepull){ 
    double peak=0,masshift=1;
    if(model.peak->isConstant())
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("peak"))->getVal();
    else
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("peak"))->getVal();
    if(model.massshift->isConstant())
      masshift = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("massshift"))->getVal();
    else
      masshift = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("massshift"))->getVal();   
    peakepull->Fill( ( (peak*masshift) - model.truepeak*model.trueshift )/model.truepeak/model.trueshift );
  }
  if(peakmupull){
    if(model.peak->isConstant())
      peakmupull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresmu->constPars().find("peak"))->getVal() - model.truepeak )/model.truepeak);
    else
      peakmupull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("peak"))->getVal() - model.truepeak )/model.truepeak);
  }
  if(peakbothpull){
    if(model.peak->isConstant())
      peakbothpull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("peak"))->getVal() - model.truepeak )/model.truepeak);
    else
      peakbothpull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("peak"))->getVal() - model.truepeak )/model.truepeak);
  }  
  std::cout << "nsig" << std::endl;
  //number of signal events
  if(nsige){
    double effratio=0;
    double norm = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("norm"))->getVal();
    if(model.effratio->isConstant())
      effratio = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("effratio"))->getVal();
    else
      effratio = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("effratio"))->getVal();   
    nsige->Fill(norm*effratio);
  }
  if(nsigmu){
    nsigmu->Fill(dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("norm"))->getVal());
  }
  if(nsigboth){
    nsigboth->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("norm"))->getVal());    
  }
  std::cout << "nsig pull" << std::endl;
  //number of signal events pull
  if(nsigepull){
    double effratio=0;
    double norm = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("norm"))->getVal();
    if(model.effratio->isConstant())
      effratio = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("effratio"))->getVal();
    else
      effratio = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("effratio"))->getVal();   
    nsigepull->Fill((norm*effratio - data.nsige)/data.nsige);
  }
  if(nsigmupull){
    nsigmupull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("norm"))->getVal() - data.nsigmu)/data.nsigmu);
  }
  if(nsigbothpull){
    double effratio=0;
    if(model.effratio->isConstant())
      effratio = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("effratio"))->getVal();
    else
      effratio = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("effratio"))->getVal();   
    nsigbothpull->Fill( ((1+effratio)*dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("norm"))->getVal() -  (data.nsige+data.nsigmu)) / (data.nsige+data.nsigmu) );
  }
  std::cout << "nbkge" << std::endl;
  //number of baclkground events
  if(nbkge)
    nbkge->Fill(dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("normbkge"))->getVal());
  if(nbkgmu)
    nbkgmu->Fill(dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("normbkgmu"))->getVal());
  if(nbkgbothe)
    nbkgbothe->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("normbkge"))->getVal());
  if(nbkgbothmu)
    nbkgbothmu->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("normbkgmu"))->getVal());
  
  std::cout << "nbkge pull" << std::endl;
  //background events: pull
  if(nbkgepull)
    nbkgepull->Fill( (dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("normbkge"))->getVal()- data.nbkge)/data.nbkge );
  if(nbkgmupull)
    nbkgmupull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("normbkgmu"))->getVal()- data.nbkgmu)/data.nbkgmu);
  if(nbkgbothpulle)
    nbkgbothpulle->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("normbkge"))->getVal()- data.nbkge)/data.nbkge);
  if(nbkgbothpullmu)
    nbkgbothpullmu->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("normbkgmu"))->getVal()- data.nbkgmu)/data.nbkgmu);
  
  std::cout << "effratio" << std::endl;
  //efficiency ratio
  if(effratioplot){
    if(model.effratio->isConstant())
      effratioplot->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("effratio"))->getVal() );
    else
      effratioplot->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("effratio"))->getVal() );
  }
  if(effratiopull){
    if(model.effratio->isConstant())
      effratiopull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("effratio"))->getVal()  - model.trueeff)/model.trueeff);
    else
      effratiopull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("effratio"))->getVal() - model.trueeff)/model.trueeff);
  }
  // massshift plots
  if(massshiftplot){
    if(model.massshift->isConstant())
      massshiftplot->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("massshift"))->getVal());
    else
      massshiftplot->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("massshift"))->getVal());
  }
  if(massshiftpull){
    if(model.massshift->isConstant())
      massshiftpull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("massshift"))->getVal() - model.trueshift)/model.trueshift);
    else
      massshiftpull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("massshift"))->getVal() - model.trueshift)/model.trueshift);			   
  }
 
}

void  ResultCollection::savehistos(char*){}               
void  ResultCollection::savehistos(TFile&){}              
