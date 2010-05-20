#ifndef SinPhiMETMHT_H
#define SinPhiMETMHT_H


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include "TMath.h"
//
// class decleration
//

class SinPhiMETMHT : public edm::EDProducer {
   public:
      explicit SinPhiMETMHT(const edm::ParameterSet&);
      ~SinPhiMETMHT();


   private:
      virtual void produce(edm::Event&, const edm::EventSetup&);
      //      virtual void endJob() ;

      // ----------member data --------------------------
      
      edm::InputTag metIn;
      edm::InputTag zIn;
      
};

#endif
