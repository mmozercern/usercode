// -*- C++ -*-
//
// Package:    ElectrowekaAnalysis/ZEE
// Class:      SinPhiMETMHT
// 
/*

 Description: <one line class summary>
 Fills Efficiency vectors for backwars compatibility with VecBos code
 
*/

#include "ElectroWeakAnalysis/ZEE/interface/SinPhiMETMHT.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "CommonTools/CandUtils/interface/Booster.h"

SinPhiMETMHT::SinPhiMETMHT(const edm::ParameterSet& pset)
{
  //read the conifguration
  
  metIn = pset.getParameter<edm::InputTag>("metIn"); 
  zIn   = pset.getParameter<edm::InputTag>("zIn");

  produces<pat::CompositeCandidateCollection>();
}



SinPhiMETMHT::~SinPhiMETMHT()
{
    // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to for each event  ------------
void SinPhiMETMHT::produce(edm::Event& iEvent, const edm::EventSetup& es)
{
  //find njets for binning
  edm::Handle< edm::View<reco::CompositeCandidate> > ZColl;
  edm::Handle< edm::View<pat::MET> > METColl;
  iEvent.getByLabel(zIn,ZColl);
  iEvent.getByLabel(metIn,METColl);

  //loop over Z's and add data
  pat::CompositeCandidateCollection out;

  for(edm::View<reco::CompositeCandidate>::const_iterator zorig = ZColl->begin() ; zorig != ZColl->end() ; zorig++ ){
    // read MET and determine vertex used in MET reco
    pat::MET * subMET =  (*METColl)[0].clone();
    math::XYZPoint vtx = subMET->vertex();
    
    //get hold of supercluster positions at get their 4-vectors
    reco::SuperClusterRef cluster1 = (dynamic_cast<const pat::Electron*>( zorig->daughter(0)->masterClone().get() ))->superCluster();
    reco::SuperClusterRef cluster2 = (dynamic_cast<const pat::Electron*>( zorig->daughter(1)->masterClone().get() ))->superCluster();

    math::XYZVector direction1 = cluster1->position() - vtx;
    math::XYZVector momentum1  = direction1.unit() * cluster1->energy();
    reco::Particle::LorentzVector p41(momentum1.x(), momentum1.y(), momentum1.z(), cluster1->energy() );

    math::XYZVector direction2 = cluster2->position() - vtx;
    math::XYZVector momentum2  = direction2.unit() * cluster2->energy();
    reco::Particle::LorentzVector p42(momentum2.x(), momentum2.y(), momentum2.z(), cluster2->energy() );
    
    reco::Particle::LorentzVector p4Z = p41+p42;
    math::XYZVector boostvector = momentum1 + momentum2;

    //subtract Z from calo MET
    subMET->setP4( (*METColl)[0].p4() + p4Z  );
 
 
    // boost MET
    pat::MET * boostSubMET = subMET->clone();
    
    Booster booster( boostvector );
    booster.set(*boostSubMET);
    
    //clone Z and add sinphi variable
    pat::CompositeCandidate newZ( *zorig ); 
    newZ.addUserFloat( "sinPhiMETMHT", sin(subMET->phi() - boostSubMET->phi()));
    out.push_back(newZ);
    
    delete boostSubMET;
    delete subMET;
    
  }

  std::auto_ptr<pat::CompositeCandidateCollection> outPointer(new pat::CompositeCandidateCollection(out));
  iEvent.put(outPointer);

}
  


//define this as a plug-in
DEFINE_FWK_MODULE(SinPhiMETMHT);
