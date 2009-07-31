#include "TreeLoader.h"
#include <sstream>

TreeLoader::TreeLoader():
  tree(0),
  file(0),
  nevents(0),
  neventsused(0)
{
}

TreeLoader::~TreeLoader(){
  if(file) file->Close();
  //if(tree) delete tree;
}

void TreeLoader::loadfile(const char* filename,const char* treename,const char* branchname){
  if(tree || file){
    std::cout << "resetting file" << std::endl;
    if(file) file->Close();
    if(tree) delete tree;
    file=0;tree=0;
  }

  file = TFile::Open(filename,"read");
  if(file==0){std::cout << "couldn't open file" << std::endl; return;}

  tree=(TTree*)file->Get(treename);
  if(tree==0){std::cout << "couldn't retrieve tree" << std::endl; return;}

  nevents= tree->GetEntriesFast();
  tree->SetBranchAddress(branchname,&mass,&massbranch);
}

RooDataSet* TreeLoader::getevents(int n,RooRealVar &var){
  if(neventsused+n>nevents){std::cout <<"out of events" << std::endl; return 0;}

  std::stringstream title;
  title << "events nr " << neventsused << " to " <<  neventsused + n;
  
  RooDataSet* returnpointer = new RooDataSet(title.str().c_str(),title.str().c_str(),var);

  for(int i = neventsused ; i < neventsused + n ; i++){
    tree->GetEntry(i);
    var.setVal(mass);
    //std::cout << mass << std::endl;
    returnpointer->add(var);
  }
  neventsused+=n;

  return returnpointer;
}
