#include "MMozer/powhegweight/plugins/PowhegWeightProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/MakerMacros.h"


PowhegWeightProducer::PowhegWeightProducer(const edm::ParameterSet& iConfig):genpTag_(iConfig.getParameter<edm::InputTag>("src")),
								     pdgId_(iConfig.getParameter<int>("pdgId")),
								     mh_(iConfig.getParameter<double>("mh")),
								     gh_(iConfig.getParameter<double>("gh")),
								     mt_(iConfig.getParameter<double>("mt")),
								     BWflag_(iConfig.getParameter<int>("BWflag")),
								     weightalgo_()					     
{
  produces<double>();
}

PowhegWeightProducer::~PowhegWeightProducer(){}

void PowhegWeightProducer::endJob(){}
void PowhegWeightProducer::beginJob(){}

void PowhegWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup&) {

  std::auto_ptr<double> weight (new double);
  (*weight) = 1.;

  if(!iEvent.isRealData()){ // only look for MC
    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel(genpTag_, genParticles);
    unsigned int gensize = genParticles->size();
    for (unsigned int i = 0; i<gensize; ++i) {
      if ((*genParticles)[i].pdgId() == pdgId_){ // found the Higgs
	double m = (*genParticles)[i].mass();
	(*weight) = weightalgo_.getweight( mh_,gh_,mt_,m,BWflag_ );	  
	break;
      }
    }
  }

  iEvent.put(weight);
}

DEFINE_FWK_MODULE(PowhegWeightProducer);
