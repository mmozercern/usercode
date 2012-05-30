#ifndef HiggsAna_HLLJJCommon_PowhegWeightProducer_h
#define HiggsAna_HLLJJCommon_PowhegWeightProducer_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"

#include "MMozer/powhegweight/interface/pwhg_wrapper.h"


class PowhegWeightProducer : public edm::EDProducer {
public:
  explicit PowhegWeightProducer(const edm::ParameterSet&);
  ~PowhegWeightProducer();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag genpTag_;
  int           pdgId_;

  double mh_;
  double gh_;
  double mt_;
  int BWflag_;

  pwhegwrapper weightalgo_;
    
};


#endif
