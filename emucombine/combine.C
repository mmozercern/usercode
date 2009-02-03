#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooGenericPdf.h"
#include "RooDataSet.h"
#include "RooExtendPdf.h"
#include "RooFitResult.h"
#include "RooGlobalFunc.h"
#include "RooBreitWigner.h"
#include "RooGaussModel.h"
#include "RooGaussian.h"
#include "RooNumConvPdf.h"
#include "RooFFTConvPdf.h"
#include "RooAddPdf.h"
#include "RooProduct.h"
#include "RooFitResult.h"
#include "RooMsgService.h"
#include "RooSimultaneous.h"
#include "RooPlot.h"
#include "RooProdPdf.h"
#include "RooEBackground.h"
#include "RooMuBackground.h"
#include "RooLinkedList.h"
#include "RooCmdArg.h"
#include "RooPolynomial.h"

#include "TRandom3.h"
#include "TH1F.h"
#include "TCanvas.h"

TH1F* numsige;
TH1F* numbe;
TH1F* numsigmu;
TH1F* numbmu;
TH1F* numsigboth;
TH1F* significancee;
TH1F* significancemu;
TH1F* significanceboth;
TH1F* numsigeboth; 
TH1F* numbeboth;   
TH1F* numsigmuboth;
TH1F* numbmuboth;  
TH1F* numsigavboth;
TH1F* effratioboth;

TH1F* massshifte;
TH1F* peakfite;
TH1F* peakfitesig;
TH1F* massshiftboth;


using namespace RooFit ;

/////////////////////////////////////////////////////////////////////////////
//-fix constrained fits
//-fix integrated fit for efficiency ratio
//-speedup integrated fits (generate from different function?)
//-
//
//////////////////////////////////////////////////////////////////////////////


void l3(int it, int effoption = 0, int massoption = 0, int whichvar=3, bool fitmass=false){
  //it: number of pseudoexperiments
  //effoption: 1-fixed 2-free 3-contrained 4-integrated, only 1 and 2 fully operational;
  //msasoption: 1-fixed 2-free 3-contrained 4-integrated, only 1 and 2 fully operational;
  //other values default to "fixed";
  //whichvar: bit pattern: 1=e 2=mu
  TRandom3 rand(0);

  //setup histograms
  significancee = new TH1F("significancee","significancee",50,0,5);
  numsige = new TH1F("numsige","numsige",50,-5,5);
  numbe   =  new TH1F("numbe","numbe",50,-5,5);
  significancemu = new TH1F("significancemu","significancemu",50,0,20);
  numsigmu = new TH1F("numsigmu","numsigmu",50,-10,10);
  numbmu   =  new TH1F("numbmu","numbmu",50,-10,10);
  significanceboth = new TH1F("significanceboth","significanceboth",50,0,20);
  numsigeboth = new TH1F("numsigeboth","numsigeboth",50,-10,10);
  numbeboth    =  new TH1F("numbeboth","numbeboth",50,-5,5);
  numsigmuboth = new TH1F("numsigmuboth","numsigmuboth",50,-10,10);
  numbmuboth   =  new TH1F("numbmuboth","numbmuboth",50,-10,10);
  numsigavboth = new TH1F("numsigavboth","numsigavboth",50,-5,5);
  effratioboth = new TH1F("effratioboth","efficiency ratio",20,0.,2.);
  massshifte = new TH1F("massshifte","mass shift e",20,0.6,1.4);
  massshiftboth = new TH1F("massshiftboth","mass shift both",20,0.5,2.);
  peakfite = new TH1F("peakfite","fitted mass",200,500,2000);
  peakfitesig = new TH1F("peakfitesig","fitted mass (sig >0.01)",200,500,2000);

  //deactivate generation messages
  RooMsgService::instance().setStreamStatus(1,kFALSE);

  //setup fit options
  RooLinkedList options;
  RooCmdArg arg1=Minos(kFALSE);  
  options.Add(&arg1);
  RooCmdArg arg2=Hesse(kFALSE);
  options.Add(&arg2);
  RooCmdArg arg3=PrintLevel(-1);
  options.Add(&arg3);
  RooCmdArg arg4=Save(kTRUE);
  options.Add(&arg4);
  RooCmdArg arg5=Extended(kTRUE);
  options.Add(&arg5);


  //master variable
  RooRealVar mass("mass","invariant mass",200,2000,"GeV");

  //systematics: efficiency ratio
  RooRealVar* effratio;
  RooGaussian* effratiodist=0;
  RooRealVar effratiomean("effratiomean","effratiomean",1.);
  RooRealVar effratiosigma("effratiosigma","effratiosigma",.1 );
  switch(effoption){
  case 2: //free
    effratio = new RooRealVar("effratio","effratio",1.,0.,10.); break;
  case 3: //contraint
    effratio = new RooRealVar("effratio","effratio",1.,0.,10.); 
    effratiodist = new RooGaussian("effratiodist","effratiodist",*effratio,effratiomean,effratiosigma);break;
  case 4: // integrated    
    effratio = new RooRealVar("effratio","effratio",1.,0.,10.);
    effratiodist = new RooGaussian("effratiodist","effratiodist",*effratio,effratiomean,effratiosigma);break;
  default: //default is fixed
    effratio = new RooRealVar("effratio","effratio",1.);
  }
  //systematics: mass scale
  RooRealVar* massshift;
  RooGaussian* massshiftdist=0;
  RooRealVar massshiftmean("massshiftmean","massshiftmean",1.);
  RooRealVar massshiftsigma("massshiftsigma","massshiftsigma",.1 );
  switch(massoption){
  case 2: //free
    massshift = new RooRealVar("massshift","massshift",1.,0.6,1.4); break;
  case 3: //contraint
    massshift = new RooRealVar("massshift","massshift",1.,0.6,1.4); 
    massshiftdist = new RooGaussian("massshiftdist","massshiftdist",*massshift,massshiftmean,massshiftsigma);break;
  case 4: // integrated    
    massshift = new RooRealVar("massshift","massshift",1.,0.6,1.4);
    massshiftdist = new RooGaussian("massshiftdist","massshiftdist",*massshift,massshiftmean,massshiftsigma);break;
  default: //default is fixed
    massshift = new RooRealVar("massshift","massshift",1.);
  }


  RooArgSet* constraints=0;
  if(effoption==3 and massoption ==3)
    constraints= new RooArgSet(*effratiodist,*massshiftdist);
  if(effoption==3 and massoption !=3)
    constraints= new RooArgSet(*effratiodist);
  if(effoption!=3 and massoption ==3)
    constraints= new RooArgSet(*massshiftdist);

  RooCmdArg arg6=ExternalConstraints(*constraints);
  if(effoption==3 || massoption==3)   options.Add(&arg6);

  // signal: BreitWigner convoluted with gaussian
  RooRealVar gamma("gamma","BW gamma",30);
  RooRealVar sigmamu("sigmamu","gaus sigma mu",40);
  RooRealVar sigmae("sigmae","gaus sigma e",15);
  RooRealVar peak("peak","mass peak",1000);//,500,2000);
  if(fitmass) {peak.setRange(200,2000),peak.setConstant(kFALSE);}
  RooRealVar origin("origin","zero",0);
  RooProduct shiftedpeak("shiftedpeak","shiftedpeak",RooArgSet(peak,*massshift));
  RooBreitWigner bwmu("bwmu","breit wigner mu",mass,peak,gamma);
  RooBreitWigner bwe("bwe","breit wigner e",mass,shiftedpeak,gamma);
  RooGaussModel gme("gme","det smearing e",mass,origin,sigmae);
  RooGaussModel gmmu("gmmu","det smearing mu",mass,origin,sigmamu);
  RooNumConvPdf signale("signale","signal pdf e",mass,bwe,gme);
  signale.setConvolutionWindow(origin,sigmae,3);
  RooNumConvPdf signalmu("signalmu","signal pdf mu",mass,bwmu,gmmu);
  signalmu.setConvolutionWindow(origin,sigmamu,3);

  peak.Print("v");

  //background
  RooRealVar bkgslopm("bkgsig","bkgsig",-2);
  RooMuBackground bckgmu("bckgmu","muon background",mass,bkgslopm);

  RooRealVar bckgslope("bckgslope","slope parameter",0.00291045);
  RooRealVar bckgshape("bckgshape","shape parameter",3.177);
  RooEBackground bckg("bckg","e background",mass,bckgslope,bckgshape);

  //integrate singal pdf over mass-scale uncertainty if necessary
  RooAbsPdf* shiftsige=0;
  RooAbsPdf* tmp=0;
  switch(massoption){
  case 4: // integrated 
    tmp= new RooProdPdf("temp product","temp product",RooArgSet(signale,*massshiftdist));
    shiftsige = tmp->createProjection(RooArgSet(*massshift));break;
  default: //fixed/free/constrained -> do nothing
    shiftsige = &signale;
  }
  
  //add extended terms
  RooRealVar norm("norm","norm",100,0,200);
  RooProduct norme("norme","norme",RooArgSet(norm,effratio));
  RooRealVar normbkge("normbkge","normbkge",70,0,200);
  RooRealVar normbkgmu("normbkgmu","normbkgmu",70,0,200);
  RooExtendPdf extsige("extsige","ext sig e",*shiftsige,norme);
  RooExtendPdf extsigmu("extsigmu","ext sig mu",signalmu,norm);
  RooExtendPdf extbkge("extbkge","ext bkg e",bckg,normbkge);
  RooExtendPdf extbkgmu("extbkgmu","ext bkg mu",bckgmu,normbkgmu); 

  norm.Print("v");

  // add signal and background
  RooAddPdf sume("sume","signal + bkg e",RooArgList(extsige,extbkge));
  RooAddPdf summu("summu","signal + bkg mu",RooArgList(extsigmu,extbkgmu));


  //define data types
  RooCategory sample("sample","sample") ;
  sample.defineType("e") ;
  sample.defineType("mu") ;

  // Construct a simultaneous pdf using category sample as index
  RooSimultaneous simPdf("simPdf","simultaneous pdf",sample) ;
  simPdf.addPdf(sume,"e") ;
  simPdf.addPdf(summu,"mu") ;

  RooSimultaneous simbPdf("simbPdf","simultaneous background only",sample);
  simbPdf.addPdf(extbkge,"e");
  simbPdf.addPdf(extbkgmu,"mu");


  //loop for pseudoexperiments
  for(int i = 0 ; i < it ;i++){
    if(i%10 == 0)      std::cout << i << std::endl;

    massshift->setVal(1.0);massshift->setValueDirty();
    //draw poisson variable for signal+background
    int sigmeane  = 0;//rand.Poisson( 10.);
    int bckmeane  = rand.Poisson( 70.);
    int sigmeanmu = rand.Poisson( 10.);
    int bckmeanmu = rand.Poisson( 70 );
        
    //generate signal+back sample, each poisson distributed (careful with empty sets), generate only when needed
    RooDataSet *datae,*dataeb,*datamu, *datamub;  
    if(whichvar & 1){
      if(sigmeane == 0)
	datae = new RooDataSet("datae","datae",RooArgSet(mass));
      else
	datae  = signale.generate(RooArgSet(mass),sigmeane) ; //extsige may be integrated over the masshift=> use bare signal
      if(bckmeane == 0)
	dataeb = new RooDataSet("dataeb","dataeb",RooArgSet(mass));
      else
	dataeb  = extbkge.generate(RooArgSet(mass),bckmeane) ;

      datae->append(*dataeb);
    }
    
    if(whichvar & 2){
      if(sigmeanmu ==0 )
	datamu = new RooDataSet("datamu","datamu",RooArgSet(mass));
      else    
	datamu =  extsigmu.generate(RooArgSet(mass),sigmeanmu) ;
      if(bckmeanmu ==0 )
	datamub = new RooDataSet("datamub","datamub",RooArgSet(mass));
      else
	datamub = extbkgmu.generate(RooArgSet(mass),bckmeanmu) ;

      datamu->append(*datamub);
    }
    RooDataSet* combData=0;
    if(whichvar ==3)
      combData= new RooDataSet("combData","combined data",mass,Index(sample),Import("e",*datae),Import("mu",*datamu)) ;

    //reset after generation
    massshift->setVal(1.);massshift->setValueDirty();

       

    RooFitResult* sigresb=0;
    RooFitResult* sigres=0;
    double llratio  =0;
    //fit e/mu separately
    if(whichvar & 1){
      sigresb = extbkge.fitTo(*datae,options);
      sigres = sume.fitTo(*datae,options);   
      
      
      llratio = sigresb->minNll() - sigres->minNll();
      if (llratio < 0 ) std::cout<< "WARNING e only: negative llratio"<<std::endl; 
      if (llratio < -1.e-3) std::cout<< "ERROR e: negative llratio" <<std::endl;
      
      significancee->Fill(sqrt(2*fabs(llratio)));
      numbe->Fill(normbkge.getVal()-bckmeane);
      numsige->Fill(norme.getVal()-sigmeane);
      std::cout << sqrt(2*fabs(llratio))  << " " << bckmeane << " " << sigmeane << std::endl;
      massshifte->Fill(massshift->getVal());
      peakfite->Fill(peak.getVal());
      if(sqrt(2*fabs(llratio))>0.1 ) peakfitesig->Fill(peak.getVal());
      
      //     RooPlot* frame = mass.frame();
      //     datae->plotOn(frame);
      //     sume.plotOn(frame);
      //     frame->Draw();
    
      delete sigres;
      delete sigresb;
    }

    if(whichvar & 2){
      sigresb = extbkgmu.fitTo(*datamu,options);
      sigres  = summu.fitTo(*datamu,options);
      
      llratio = sigresb->minNll() - sigres->minNll();
      if (llratio < 0 ) std::cout<< "WARNING mu only: negative llratio"<<std::endl; 
      if (llratio < -1.e-3) std::cout<< "ERROR mu: negative llratio" <<std::endl;
      
      significancemu->Fill(sqrt(2*fabs(llratio)));
      numbmu->Fill(normbkgmu.getVal()-bckmeanmu);
      numsigmu->Fill(norm.getVal()-sigmeanmu);
      delete sigres;
      delete sigresb;
    }
     

    if(whichvar ==3){
      //fit combined
      sigresb = simbPdf.fitTo(*combData,options);
      sigres  = simPdf.fitTo(*combData,options);
      
      //     sigres->Print("v");

      llratio = sigresb->minNll() - sigres->minNll();
      if (llratio < 0 ) std::cout<< "WARNING mu only: negative llratio"<<std::endl; 
      if (llratio < -1.e-3) std::cout<< "ERROR mu: negative llratio" <<std::endl;
      
      significanceboth->Fill(sqrt(2*fabs(llratio)));
      numbeboth->Fill(normbkge.getVal()-bckmeane);
      numsigeboth->Fill(norme.getVal()-sigmeane);
      numbmuboth->Fill(normbkgmu.getVal()-bckmeanmu);
      numsigmuboth->Fill(norm.getVal()-sigmeanmu);
      numsigavboth->Fill(norme.getVal()+norm.getVal() - (sigmeane+sigmeanmu));
      effratioboth->Fill(effratio->getVal());
      delete sigres;
      delete sigresb;
      
      
      //cleanup
      delete datae;
      delete dataeb;
      delete datamu;
      delete datamub;
    }

  }
//     RooPlot* frame1 = mass.frame(Bins(40),Title("e"));
//     RooPlot* frame2 = mass.frame(Bins(40),Title("mu"));
//     combData->plotOn(frame1,Cut("sample==sample::e")) ;
//     combData->plotOn(frame2,Cut("sample==sample::mu")) ;
//     simPdf.plotOn(frame1,Slice(sample,"e"),ProjWData(sample,combData)) ;
//     simPdf.plotOn(frame2,Slice(sample,"mu"),ProjWData(sample,combData)) ;
//    TCanvas* c = new TCanvas("test","test",800,400) ;
//    c->Divide(2) ;
//    c->cd(1) ; frame1->Draw() ;
//    c->cd(2) ; frame2->Draw() ;

}
