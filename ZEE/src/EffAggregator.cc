// -*- C++ -*-
//
// Package:    ElectrowekaAnalysis/ZEE
// Class:      EffAggregator
// 
/*

 Description: <one line class summary>
 Fills Efficiency vectors for backwars compatibility with VecBos code
 
*/

#include "ElectroWeakAnalysis/ZEE/interface/EffAggregator.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TString.h"
#include "TH1F.h"


EffAggregator::EffAggregator(const edm::ParameterSet& pset)
{
  //read the conifguration
  
  jetsIn = pset.getParameter<edm::InputTag>("jetsIn"); // jet collection for Njet counting
  jetLabel = pset.getParameter<std::string>("jetLabel");
  cuts   = pset.getParameter<int>("ncutsintree");

  ////////////////////////////////////////////////////////////
  //         Read in the Vector of Parameter Sets.          //
  //           Information for each filter-step             //
  ////////////////////////////////////////////////////////////
  std::vector<edm::ParameterSet> filters = 
    pset.getParameter<std::vector<edm::ParameterSet> >("filters");

  for(std::vector<edm::ParameterSet>::iterator filterconf = filters.begin() ; filterconf != filters.end() ; filterconf++){
    order.push_back(filterconf->getParameter<double>("order"));
    cutVals.push_back( filterconf->getParameter<int>("cutVal") );
      
    //get input collections and cut type
    tightIn.push_back(filterconf->getParameter<edm::InputTag>("tightIn"));
    looseIn.push_back(filterconf->getParameter<edm::InputTag>("looseIn"));
    cutTypes.push_back(filterconf->getParameter<std::string>("type"));
    
  } // END of loop over parameter sets

}



EffAggregator::~EffAggregator()
{
    // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to for each event  ------------
void EffAggregator::analyze(const edm::Event& iEvent, const edm::EventSetup& es)
{
  //find njets for binning
  edm::Handle< edm::View<pat::Jet> > jetCounter;
  iEvent.getByLabel(jetsIn,jetCounter);

  std::cout << std::endl;
  for( edm::View<pat::Jet>::const_iterator jet = jetCounter->begin() ; jet!=jetCounter->end();jet++){
    std::cout << jet->hasCorrFactors() << " "  << jet->corrStep().c_str() << " XXX" ;
 //     std::cout << jet->phi() << " " << jet->eta() << " "  <<jet->pt() << std::endl;
//     std::cout << jet->energyFractionHadronic() <<std::endl ;

  }

  
  
  int njet = jetCounter->size();
  if(njet > 5) njet = 5; //prevent overflow
  
  //count all events
  count(njet,0);
  
  std::cout << "njet: " << njet <<std::endl;

  //std::cout <<"Analyzer: " << njet << "jets"<< std::endl;
  // loop over all cuts
  bool pass = true;
  for(unsigned int i = 0; i < cutTypes.size();i++){
    //std::cout << "look at filter: " << i << " with type " << cutTypes[i] << std::endl;
    if(cutTypes[i]=="e")            {pass = fill_eselection(iEvent,i,njet);}
    else if (cutTypes[i]=="e ex")   {pass = fill_eselection(iEvent,i,njet);}
    else if (cutTypes[i]=="trigger"){pass = fill_eselection(iEvent,i,njet);}
    else if (cutTypes[i]=="jet")    {} // do nothing
    else if (cutTypes[i]=="mee")    {pass = fill_genericselection(iEvent,i,njet);}
    else if (cutTypes[i]=="gen")    {pass = fill_genericselection(iEvent,i,njet);}
    else
      std::cout << "not implemeneted yet" << std::endl;
    
    if(!pass) break; // stop the loop at the first failing cut
  }
  
  if(pass)  // passed all cuts
    count(njet,cuts-1);
}
  
bool EffAggregator::fill_eselection(const edm::Event& iEvent,int icut, int njet){

  bool pass=false;
  // open electron collections
  edm::Handle< edm::View<reco::Candidate> > loosee;
  edm::Handle< edm::View<reco::Candidate> > tighte;
  iEvent.getByLabel(tightIn[icut],tighte);
  iEvent.getByLabel(looseIn[icut],loosee);

  if( (tighte->size() > 0 ) &&                                                   //at least one tight
      (((int)(tighte->size() + loosee->size())) >= cutVals[icut])) pass = true;  // and possibly more
  //std::cout << "tight: "<< tighte->size() << " loose: " << loosee->size() << " order: " << order[icut] << std::endl;
  
  //for(edm::View<pat::Electron>::const_iterator e = tighte->begin(); e != tighte->end(); e++){
  //    if( order[icut]==5 && pass)
  //    std::cout << " XXX " << tighte->size() <<  " loose: " << loosee->size() << " at least one" << (tighte->size() > 0) << std::endl;
      //std::cout <<"EB: " << e->isEB() << " EE: " << e->isEE() << " eta: " << e->eta() << " gap: " << e->isGap() << std::endl;
      //std::cout << "pt: " << e->pt() <<  " sihih: " << e->sigmaIetaIeta() <<" deta: " <<  e->deltaEtaSuperClusterTrackAtVtx() << "dphi:"<< e->deltaPhiSuperClusterTrackAtVtx() << std::endl; }
  //  std::cout << "phi: " << e->phi() <<  " eta: " << e->eta() << std::endl; 
  //}
  //std::cout << "loose" << std::endl;
  //for(edm::View<pat::Electron>::const_iterator e = loosee->begin(); e != loosee->end(); e++){
  //  std::cout << "phi: " << e->phi() <<  " eta: " << e->eta() << " "  << e->electronID("eidRobustLoose") << std::endl; 
  //}


  if( ((int)(order[icut]*10))%10 == 0 && order[icut]<cuts) // only fill for whole numbered steps
    if(pass)
      count(njet,(int)order[icut]);

  return pass;
}

bool EffAggregator::fill_genericselection(const edm::Event& iEvent,int icut, int njet){

  bool pass=false;
  // open electron collections
  edm::Handle< edm::View<reco::Candidate> > gen;
  iEvent.getByLabel(tightIn[icut],gen);

  std::cout << "generic: " << order[icut]  << std::endl;
  if(order[icut] == 1){
    for( edm::View<reco::Candidate>::const_iterator jet = gen->begin() ; jet!=gen->end(); jet++)    
      std::cout << "gen ele:" << jet->phi() << " " << jet->eta() << " "  <<jet->pt() << std::endl;
  }

  if( (int)gen->size() >= cutVals[icut] ) pass = true;  

  if( ((int)(order[icut]*10))%10 == 0 && order[icut]<cuts) // only fill for whole numbered steps
    if(pass)
      count(njet,(int)order[icut]);

  return pass;
}

void EffAggregator::count(int njet, int bin){
  switch (njet){
  case 5: counts[bin][5] +=1.;
  case 4: counts[bin][4] +=1.;
  case 3: counts[bin][3] +=1.;
  case 2: counts[bin][2] +=1.;
  case 1: counts[bin][1] +=1.;
  default : counts[bin][0] +=1.; 
  }
}

void EffAggregator::endJob()
{
  // write to file
  edm::Service<TFileService> fs;
  
  for(int njets=0;njets<6;njets++){
    TString branchTitle("");
    if(cuts==20)//W
      branchTitle+="W_";
    else
      branchTitle+="Z_";

    branchTitle += njets;
    branchTitle += jetLabel.c_str();
    branchTitle += "_counter";

    TTree* tree =fs->make<TTree>(branchTitle,"Efficiency Counters");
       
    
    float nSel[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int nCuts=cuts; 
        
    tree->Branch("nCuts",&nCuts,"nCuts/I");
    tree->Branch("nSel",nSel,"nSel[nCuts]/F");
    
    for(int i=0;i<cuts;i++) nSel[i]=counts[i][njets];
    
    //find the highest non-zero step and fill values values for missing cuts
    int nmax = 0;
    for(int n=cuts-1; n >=0 ; n--){ // loop over cuts 
      if (nSel[n]>0) {
	nmax=n;
	break;
      }	
    }
    for(int i=1 ; i< nmax;i++){
      if (nSel[i]==0)
	nSel[i]=nSel[i-1];
    }
        
    tree->Fill();
  }

}



//define this as a plug-in
DEFINE_FWK_MODULE(EffAggregator);
