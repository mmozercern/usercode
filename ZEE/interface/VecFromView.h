#ifndef VecFromView_H
#define VecFromView_H


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include "TMath.h"
//
// class decleration
//

class VecFromView : public edm::EDProducer {
   public:
      explicit VecFromView(const edm::ParameterSet&);
      ~VecFromView();


   private:
      virtual void produce(edm::Event&, const edm::EventSetup&);
      //      virtual void endJob() ;

      // ----------member data --------------------------

      edm::InputTag inTag;

};

#endif
