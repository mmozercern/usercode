#ifndef HiggsAna_HLLJJCommon_TongguangReader_h
#define HiggsAna_HLLJJCommon_TongguangReader_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"


class TongguangReader : public edm::EDProducer {
public:
  explicit TongguangReader(const edm::ParameterSet&);
  ~TongguangReader();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag genpTag_;
  int           pdgId_;

  std::string   filename_;

  std::vector<double> bincenters_;
  std::vector<double> weight_;
  std::vector<double> weightP_;
  std::vector<double> weightM_;
  

};


#endif
