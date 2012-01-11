#include <cstdlib>
#include <fstream>
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1D.h"
#include "TString.h"
#include "TROOT.h"

#include "SidebandFitter.h"


int main( int argc, char* argv[] ) {

  std::string PUReweighing = "weight";
  unsigned int nToys = 500;
  
  TRandom3* random = new TRandom3(13);

  TChain* chainData = new TChain("AngularInfo");
  chainData->Add("summer11_data_highmass.root");
  chainData->Add("summer11_data_2011B.root");


  TChain* chainMC = new TChain("AngularInfo");
  std::string bgTreeName;
  chainMC->Add("summer11_TTbarIncl_highmass.root");
  chainMC->Add("summer11_ZJets_madgraph_highmass.root");
  chainMC->Add("summer11_ZZ_highmass.root");
  chainMC->Add("summer11_WZ_highmass.root");

  gROOT->cd(); //magic!



  for( unsigned ibtag=0; ibtag<3; ++ibtag ) {
    
    SidebandFitter *sf = new SidebandFitter( PUReweighing);

    char btagCut[100];
    sprintf( btagCut, "nBTags==%d", ibtag );

    TTree* treeDATA_Xbtag = chainData->CopyTree(btagCut);

    TTree* treeMC_Xbtag = chainMC->CopyTree(btagCut);

    RooWorkspace* alpha_Xbtag;
    alpha_Xbtag = sf->getAlphaFit( ibtag, "ALL", treeMC_Xbtag );

//     RooFitResult* fr = sf->fitSidebands( treeMC_Xbtag, treeDATA_Xbtag, ibtag, "ALL", alpha_Xbtag );

//     for(int i = 0 ; i <nToys ; i++) {
//       std::cout << std::endl << "[ " << ibtag << " b-tags ]  Toy: " << i << "/" << nToys << std::endl;
//       TH1D* variedHisto = sf->shuffle(alpha_Xbtag, random ,"tmp");
//       sf->fitPseudo( treeMC_Xbtag, treeDATA_Xbtag, ibtag, "ALL", variedHisto,i);
//       delete variedHisto;
//     }

//     if( nToys > 0 )
//       sf->pseudoMassge(nToys, ibtag,"ALL",fr);

//     delete fr;
//     delete sf;

    delete alpha_Xbtag;
  } //for btags

  return 0;

}



