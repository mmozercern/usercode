// -*- C++ -*-
//
// Package:    ElectrowekaAnalysis/ZEE
// Class:      VecFromView
//
/*

 Description: <one line class summary>
 transform RefVector/PtrVector into standard vector
 should probably be templated

*/

#include "ElectroWeakAnalysis/ZEE/interface/VecFromView.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

VecFromView::VecFromView(const edm::ParameterSet& pset)
{
  //read the conifguration

  inTag = pset.getParameter<edm::InputTag>("src");

  produces< pat::ElectronCollection >();
}



VecFromView::~VecFromView()
{
    // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to for each event  ------------
void VecFromView::produce(edm::Event& iEvent, const edm::EventSetup& es)
{

  edm::Handle< edm::View<reco::Candidate> > inColl;
  iEvent.getByLabel(inTag,inColl);
  std::auto_ptr<pat::ElectronCollection> out(new pat::ElectronCollection);
  for(edm::View<reco::Candidate>::const_iterator orig = inColl->begin() ; orig != inColl->end() ; orig++ ){
    const pat::Electron * ele = dynamic_cast<const pat::Electron *>(&(*orig));
    out->push_back(*ele);
  }
  iEvent.put(out);

}



//define this as a plug-in
DEFINE_FWK_MODULE(VecFromView);
