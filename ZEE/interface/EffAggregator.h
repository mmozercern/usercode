#ifndef EffAggregator_H
#define EffAggregator_H


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include "TMath.h"
//
// class decleration
//

class EffAggregator : public edm::EDAnalyzer {
   public:
      explicit EffAggregator(const edm::ParameterSet&);
      ~EffAggregator();


   private:
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      bool fill_eselection(const edm::Event&, int, int);
      bool fill_genericselection(const edm::Event&, int, int);
      void count(int, int);
      // ----------member data --------------------------
      
      edm::InputTag jetsIn;
      std::string jetLabel; // label for trees 

      std::vector<int> cutVals;
      std::vector<float> order;
      std::vector<std::string> cutTypes;
      std::vector<edm::InputTag> tightIn;
      std::vector<edm::InputTag> looseIn;
      int cuts; // for backwards compatibility 20 for W 18 for Z
      
      float counts[20][6];
      
};

#endif
