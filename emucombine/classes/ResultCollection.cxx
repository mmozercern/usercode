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
  samplemasse(0),samplemassmu(0),
  widthe(0),widthmu(0),widthboth(0),
  widthepull(0),widthmupull(0),widthbothpull(0),
  sigvspeake(0),sigvspeakmu(0),sigvspeakboth(0),
  sigvswide(0),sigvswidmu(0),sigvswidboth(0)
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
  if(widthe ) delete widthe ;
  if(widthmu ) delete widthmu ;
  if(widthboth ) delete widthboth ;
  if(widthepull ) delete widthepull ;
  if(widthmupull ) delete widthmupull ;
  if(widthbothpull ) delete widthbothpull ;
  if(sigvspeake ) delete   sigvspeake ;
  if(sigvspeakmu ) delete     sigvspeakmu ;
  if(sigvspeakboth ) delete     sigvspeakboth ;
  if(sigvswide ) delete     sigvswide ;
  if(sigvswidmu ) delete     sigvswidmu ;
  if(sigvswidboth ) delete     sigvswidboth ;

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
    nsigepull = new TH1F("nsigepull","n signal pull e",nbins,-1.5,1.5);
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
    nbkgepull = new TH1F("nbkgepull","n background pull e",nbins,-1.5,1.5);
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
void ResultCollection::setwidthhistos(int whichvar,int nbins,Model& model){
  if(whichvar & 1)
    widthe = new TH1F("widthe","BW gamma e",nbins, 0,200);
  if(whichvar & 2)
    widthmu = new TH1F("widthmu","BW gamma mu",nbins, 0,200);
  if((whichvar & 1) && (whichvar & 2))
    widthboth = new TH1F("widthboth","BW gamma both",nbins, 0,200);
}
void ResultCollection::setwidthnormhistos(int whichvar,int nbins,Model& model){
  if(whichvar & 1)
    widthepull = new TH1F("widthepull","BW gamma e pull",nbins, -3,3);
  if(whichvar & 2)
    widthmupull = new TH1F("widthmupull","BW gamma mu pull",nbins, -3,3);
  if((whichvar & 1) && (whichvar & 2))
    widthbothpull = new TH1F("widthbothpull","BW gamma both pull",nbins, -3,3);
}
void ResultCollection::set2d(int whichvar, int nbinsx,int nbinsy, float sigmaend,Model& model){
  if(whichvar & 1){
    sigvspeake = new TH2F("sigvspeake","sigvspeake",nbinsx,0,sigmaend,nbinsy,model.mass->getMin(),model.mass->getMax());
    sigvswide  = new TH2F("sigvswide","sigvswide ",nbinsx,0,sigmaend,nbinsy,model.gamma->getMin(),model.gamma->getMax());
  }
  if(whichvar & 2){
    sigvspeakmu = new TH2F("sigvspeakmu","sigvspeakmu",nbinsx,0,sigmaend,nbinsy,model.mass->getMin(),model.mass->getMax());
    sigvswidmu  = new TH2F("sigvswidmu","sigvswidmu ",nbinsx,0,sigmaend,nbinsy,model.gamma->getMin(),model.gamma->getMax());
  }
  if((whichvar & 1) && (whichvar & 2)){
    sigvspeakboth = new TH2F("sigvspeakboth","sigvspeakboth",nbinsx,0,sigmaend,nbinsy,model.mass->getMin(),model.mass->getMax());
    sigvswidboth  = new TH2F("sigvswidboth","sigvswidboth ",nbinsx,0,sigmaend,nbinsy,model.gamma->getMin(),model.gamma->getMax());
  }
} 


void  ResultCollection::fillhistos(Model& model,DataSetHelper& data){
  //std::cout << "sig" << std::endl;
  // significance histgrams (will crash if histos defined for absent fits)
  if(sige) 
    sige->Fill(sqrt(2*fabs(model.fitresbe->minNll() - model.fitrese->minNll())));
    //sige->Fill(exp(model.fitrese->minNll() - model.fitresbe->minNll()));
  if(sigmu)
    sigmu->Fill(sqrt(2*fabs(model.fitresbmu->minNll() - model.fitresmu->minNll())));
  if(sigboth)
    sigboth->Fill(sqrt(2*fabs(model.fitresbboth->minNll() - model.fitresboth->minNll())));
  //peak-positions
  //std::cout << "peak" << std::endl;
  if(peake){ 
    double peak=0,masshift=1;
    if(model.peak->isConstant())
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("peak"))->getVal();
    else
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("peak"))->getVal();
    if(model.massshift->isConstant())
      masshift = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("massshift"))->getVal();
    else if(!model.shiftprod) // don't set this variable if it's been integrated
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
  //std::cout << "peakpull" << std::endl;
  //peak-pulls
  if(peakepull){ 
    double peak=0,masshift=1,errp=1.,errm=1.;
    if(model.peak->isConstant()){
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("peak"))->getVal();
    }
    else{
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("peak"))->getVal();
      errp = dynamic_cast<RooRealVar*>(model.fitrese->floatParsFinal().find("peak"))->getError();  
    }
    if(model.massshift->isConstant())
      masshift = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("massshift"))->getVal();
    else if(!model.shiftprod){ // don't set this variable if it's been integrated
      masshift = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("massshift"))->getVal();
      errm = dynamic_cast<RooRealVar*>(model.fitrese->floatParsFinal().find("massshift"))->getError();
    }   
    peakepull->Fill( ( (peak*masshift) - model.truepeak*model.trueshift )/sqrt(errm*errm+errp*errp) );
  }
  if(peakmupull){
    if(model.peak->isConstant())
      peakmupull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresmu->constPars().find("peak"))->getVal() - model.truepeak )/model.truepeak);
    else
      peakmupull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("peak"))->getVal() - model.truepeak )/dynamic_cast<RooRealVar*>(model.fitresmu->floatParsFinal().find("peak"))->getError());
  }
  if(peakbothpull){
    if(model.peak->isConstant())
      peakbothpull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("peak"))->getVal() - model.truepeak )/model.truepeak);
    else
      peakbothpull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("peak"))->getVal() - model.truepeak )/dynamic_cast<RooRealVar*>(model.fitresboth->floatParsFinal().find("peak"))->getError());
  }  
  //std::cout << "nsig" << std::endl;
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
  //std::cout << "nsig pull" << std::endl;
  //number of signal events pull
  if(nsigepull){
    double effratio=0;
    double norm = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("norm"))->getVal();
    if(model.effratio->isConstant())
      effratio = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("effratio"))->getVal();
    else
      effratio = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("effratio"))->getVal();   
    nsigepull->Fill((norm*effratio - data.nsige)/(effratio  * (dynamic_cast<RooRealVar*>(model.fitrese->floatParsFinal().find("norm"))->getError())  )  );
  }
  if(nsigmupull){
    nsigmupull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("norm"))->getVal() - data.nsigmu)/(dynamic_cast<RooRealVar*>(model.fitresmu->floatParsFinal().find("norm"))->getError()));
  }
  if(nsigbothpull){
    double effratio=0;
    double effratioerr=0;
    if(model.effratio->isConstant()){
      effratio = dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("effratio"))->getVal();
    }else{
      effratio = dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("effratio"))->getVal(); 
      effratioerr = dynamic_cast<RooRealVar*>(model.fitresboth->floatParsFinal().find("effratio"))->getError();
    }  
    double magnitude = dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("norm"))->getVal();
    double merr = dynamic_cast<RooRealVar*>(model.fitresboth->floatParsFinal().find("norm"))->getError();
    nsigbothpull->Fill( ((1+effratio)*magnitude -  (data.nsige+data.nsigmu)) / ( sqrt( (1+effratio)*merr*(1+effratio)*merr + magnitude*magnitude*effratioerr*effratioerr  ) ) );
  }
  //std::cout << "nbkge" << std::endl;
  //number of baclkground events
  if(nbkge)
    nbkge->Fill(dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("normbkge"))->getVal());
  if(nbkgmu)
    nbkgmu->Fill(dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("normbkgmu"))->getVal());
  if(nbkgbothe)
    nbkgbothe->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("normbkge"))->getVal());
  if(nbkgbothmu)
    nbkgbothmu->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("normbkgmu"))->getVal());
  
  //std::cout << "nbkge pull" << std::endl;
  //background events: pull
  if(nbkgepull)
    nbkgepull->Fill( (dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("normbkge"))->getVal()- data.nbkge)/dynamic_cast<RooRealVar*>(model.fitrese->floatParsFinal().find("normbkge"))->getError() );
  if(nbkgmupull)
    nbkgmupull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("normbkgmu"))->getVal()- data.nbkgmu)/dynamic_cast<RooRealVar*>(model.fitresmu->floatParsFinal().find("normbkgmu"))->getError());
  if(nbkgbothpulle)
    nbkgbothpulle->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("normbkge"))->getVal()- data.nbkge)/dynamic_cast<RooRealVar*>(model.fitresboth->floatParsFinal().find("normbkge"))->getError());
  if(nbkgbothpullmu)
    nbkgbothpullmu->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("normbkgmu"))->getVal()- data.nbkgmu)/dynamic_cast<RooRealVar*>(model.fitresboth->floatParsFinal().find("normbkgmu"))->getError());
  
  //std::cout << "effratio" << std::endl;
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
    else if(!model.shiftprod) // don't set this variable if it's been integrated
      massshiftplot->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("massshift"))->getVal());
  }
  if(massshiftpull){
    if(model.massshift->isConstant())
      massshiftpull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("massshift"))->getVal() - model.trueshift)/model.trueshift);
    else if(!model.shiftprod) // don't set this variable if it's been integrated
      massshiftpull->Fill( (dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("massshift"))->getVal() - model.trueshift)/model.trueshift);			   
  }

  if(widthe){
    if(model.gamma->isConstant())
      widthe->Fill(dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("gamma"))->getVal());
    else
      widthe->Fill(dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("gamma"))->getVal());
  }
  if(widthmu){
    if(model.gamma->isConstant())
      widthmu->Fill(dynamic_cast<RooAbsReal*>(model.fitresmu->constPars().find("gamma"))->getVal());
    else
      widthmu->Fill(dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("gamma"))->getVal());
  }
  if(widthboth){
    if(model.gamma->isConstant())
      widthboth->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("gamma"))->getVal());
    else
      widthboth->Fill(dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("gamma"))->getVal());
  }
  if(widthepull){
    float err=1.,wid=1.;
    if(model.gamma->isConstant())
      wid = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("gamma"))->getVal();
    else{
      wid = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("gamma"))->getVal();
      err = dynamic_cast<RooRealVar*>(model.fitrese->floatParsFinal().find("gamma"))->getError();
    }
    widthepull->Fill( (wid- model.truewidth)/err );
  }
  if(widthmupull){
    float err=1.,wid=1.;
    if(model.gamma->isConstant())
      wid = dynamic_cast<RooAbsReal*>(model.fitresmu->constPars().find("gamma"))->getVal();
    else{
      wid = dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("gamma"))->getVal();
      err = dynamic_cast<RooRealVar*>(model.fitresmu->floatParsFinal().find("gamma"))->getError();
    }
    widthmupull->Fill( (wid- model.truewidth)/err );
  }
  if(widthbothpull){
    float err=1.,wid=1.;
    if(model.gamma->isConstant())
      wid = dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("gamma"))->getVal();
    else{
      wid = dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("gamma"))->getVal();
      err = dynamic_cast<RooRealVar*>(model.fitresboth->floatParsFinal().find("gamma"))->getError();
    }
    widthbothpull->Fill( (wid- model.truewidth)/err );
  }
  if(sigvspeake ){
    double peak=0,masshift=1;
    if(model.peak->isConstant())
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("peak"))->getVal();
    else
      peak = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("peak"))->getVal();
    if(model.massshift->isConstant())
      masshift = dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("massshift"))->getVal();
    else if(!model.shiftprod) // don't set this variable if it's been integrated
      masshift = dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("massshift"))->getVal();   
    sigvspeake->Fill(sqrt(2*fabs(model.fitresbe->minNll() - model.fitrese->minNll())),peak*masshift);
  }
  if(sigvspeakmu ) {
    if(model.peak->isConstant())
      sigvspeakmu->Fill(sqrt(2*fabs(model.fitresbmu->minNll() - model.fitresmu->minNll())),dynamic_cast<RooAbsReal*>(model.fitresmu->constPars().find("peak"))->getVal());
    else
      sigvspeakmu->Fill(sqrt(2*fabs(model.fitresbmu->minNll() - model.fitresmu->minNll())),dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("peak"))->getVal());
  }
  if(sigvspeakboth ){
    if(model.peak->isConstant())
      sigvspeakboth->Fill(sqrt(2*fabs(model.fitresbboth->minNll() - model.fitresboth->minNll())),dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("peak"))->getVal());
    else
      sigvspeakboth->Fill(sqrt(2*fabs(model.fitresbboth->minNll() - model.fitresboth->minNll())),dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("peak"))->getVal());
  }
  if(sigvswide ){
    if(model.gamma->isConstant())
      sigvswide->Fill(sqrt(2*fabs(model.fitresbe->minNll() - model.fitrese->minNll())),dynamic_cast<RooAbsReal*>(model.fitrese->constPars().find("gamma"))->getVal());
    else
      sigvswide->Fill(sqrt(2*fabs(model.fitresbe->minNll() - model.fitrese->minNll())),dynamic_cast<RooAbsReal*>(model.fitrese->floatParsFinal().find("gamma"))->getVal());
  } 
  if(sigvswidmu ){
    if(model.gamma->isConstant())
      sigvswidmu->Fill(sqrt(2*fabs(model.fitresbmu->minNll() - model.fitresmu->minNll())),dynamic_cast<RooAbsReal*>(model.fitresmu->constPars().find("gamma"))->getVal());
    else
      sigvswidmu->Fill(sqrt(2*fabs(model.fitresbmu->minNll() - model.fitresmu->minNll())),dynamic_cast<RooAbsReal*>(model.fitresmu->floatParsFinal().find("gamma"))->getVal());
  }
  if(sigvswidboth ){
    if(model.gamma->isConstant())
      sigvswidboth->Fill(sqrt(2*fabs(model.fitresbboth->minNll() - model.fitresboth->minNll())),dynamic_cast<RooAbsReal*>(model.fitresboth->constPars().find("gamma"))->getVal());
    else
      sigvswidboth->Fill(sqrt(2*fabs(model.fitresbboth->minNll() - model.fitresboth->minNll())),dynamic_cast<RooAbsReal*>(model.fitresboth->floatParsFinal().find("gamma"))->getVal());
  }
}

void  ResultCollection::savehistos(char*){}               
void  ResultCollection::savehistos(TFile& outfile,char* subname){
  outfile.cd();
  outfile.mkdir(subname);
  outfile.cd(subname);
  if(sige) sige->Write();
  if(sigmu) sigmu->Write();
  if(sigboth) sigboth->Write();
  if(peake) peake->Write();
  if(peakmu) peakmu->Write();
  if(peakboth) peakboth->Write();
  if(peakepull) peakepull->Write();
  if(peakbothpull) peakbothpull->Write();
  if(nsigepull) nsigepull->Write();
  if(nsigmupull) nsigmupull->Write();
  if(nsigbothpull) nsigbothpull->Write();
  if(nsige) nsige->Write();
  if(nsigmu) nsigmu->Write();
  if(nsigboth) nsigboth->Write();
  if(nbkge) nbkge->Write();
  if(nbkgmu) nbkgmu->Write();
  if(nbkgbothe) nbkgbothe->Write();
  if(nbkgbothmu) nbkgbothmu->Write();
  if(nbkgepull) nbkgepull->Write();
  if(nbkgmupull) nbkgmupull->Write();
  if(nbkgbothpulle) nbkgbothpulle->Write();
  if(nbkgbothpullmu) nbkgbothpullmu->Write();
  if(effratioplot) effratioplot->Write();
  if(effratiopull) effratiopull->Write();
  if(massshiftplot) massshiftplot->Write();
  if(massshiftpull) massshiftpull->Write();
  if(totalmasse) totalmasse->Write();
  if(totalmassmu) totalmassmu->Write();
  if(samplemasse) samplemasse->Write();
  if(samplemassmu) samplemassmu->Write();
  if(widthe) widthe->Write();
  if(widthmu) widthmu->Write();
  if(widthboth) widthboth->Write();
  if(widthepull) widthepull->Write();
  if(widthmupull) widthmupull->Write();
  if(widthbothpull) widthbothpull->Write();
  if(sigvspeake ) sigvspeake->Write();
  if(sigvspeakmu ) sigvspeakmu->Write();
  if(sigvspeakboth ) sigvspeakboth->Write();
  if(sigvswide ) sigvswide->Write();
  if(sigvswidmu ) sigvswidmu->Write();
  if(sigvswidboth ) sigvswidboth->Write();

  outfile.cd("..");

}              
