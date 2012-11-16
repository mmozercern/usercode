#include "MMozer/powhegweight/plugins/TongguangReader.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include <fstream>
#include "TMath.h"
#include <algorithm>

TongguangReader::TongguangReader(const edm::ParameterSet& iConfig):genpTag_(iConfig.getParameter<edm::InputTag>("src")),
								   pdgId_(iConfig.getParameter<int>("pdgId")),
								   filename_(iConfig.getParameter<edm::FileInPath>("filename").fullPath())
{
  

  double bincenter, initial, pow, powp, powm, out, outp, outm;

  std::ifstream ifs(filename_.c_str());

  while( ifs.good() ) {
    ifs >> bincenter >> initial >> pow >> powp >> powm >> out >> outp >> outm;
    
    bincenters_.push_back(bincenter);
    if(initial > 0){
      weight_.push_back(   TMath::Max(0.,out/initial) );
      weightP_.push_back(  TMath::Max(0.,outp/initial) );
      weightM_.push_back(  TMath::Max(0.,outm/initial) );
    }else{//weights are not defined if initial distribution is 0 => set weight to 0
      weight_.push_back( 0. );
      weightP_.push_back( 0. );
      weightM_.push_back( 0. );
    }

  }

  produces<double>("center");
  produces<double>("errorp");
  produces<double>("errorm");
}

TongguangReader::~TongguangReader(){}

void TongguangReader::endJob(){}
void TongguangReader::beginJob(){}

void TongguangReader::produce(edm::Event& iEvent, const edm::EventSetup&) {

  std::auto_ptr<double> weight (new double);
  std::auto_ptr<double> weightp (new double);
  std::auto_ptr<double> weightm (new double);
  (*weight) = 1.;
  (*weightp) = 1.;
  (*weightm) = 1.;
  

  if(!iEvent.isRealData()){ // only look for MC
    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel(genpTag_, genParticles);
    unsigned int gensize = genParticles->size();
    for (unsigned int i = 0; i<gensize; ++i) {
      if ((*genParticles)[i].pdgId() == pdgId_){ // found the Higgs
	double m = (*genParticles)[i].mass();
	
	if( m < bincenters_.front() || m >  bincenters_.back() ){ // set weights to 0 if out of range
	  (*weight) = 0.;
	  (*weightp) = 0.;
	  (*weightm) = 0.;
	  break;
	}
	    
   
	std::vector<double>::iterator low;
	low=lower_bound( bincenters_.begin(), bincenters_.end(),m ); 
	int lowindex=(low-  bincenters_.begin());
	if(m == *low ){//exact match
	  (*weight) = weight_[lowindex];
	  (*weightp) = weightP_[lowindex];
	  (*weightm) = weightM_[lowindex];
	}else{//linear interpolation
	  lowindex--; // lower_bound finds the first element not smaller than X
	  (*weight) = weight_[lowindex] +( m - bincenters_[lowindex] )*(weight_[lowindex+1]-weight_[lowindex])/(bincenters_[lowindex+1]-bincenters_[lowindex]) ;
	  (*weightp) = weightP_[lowindex] +( m - bincenters_[lowindex] )*(weightP_[lowindex+1]-weightP_[lowindex])/(bincenters_[lowindex+1]-bincenters_[lowindex])  ;
	  (*weightm) = weightM_[lowindex] +( m - bincenters_[lowindex] )*(weightM_[lowindex+1]-weightM_[lowindex])/(bincenters_[lowindex+1]-bincenters_[lowindex])  ;
	}

	//swap weights if necessary: 



	//std::cout << m << " " << bincenters_[lowindex] << "  " << bincenters_[lowindex+1] << " " << (*weight)  <<std::endl;

  
	break;
      }
    }
  }

  iEvent.put(weight,"center");
  iEvent.put(weightp,"errorp");
  iEvent.put(weightm,"errorm");
}

DEFINE_FWK_MODULE(TongguangReader);
