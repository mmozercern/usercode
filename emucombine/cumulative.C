#include "TH1F.h"

TH1F* cumulate(TH1F* inhist){
  TH1F* cumulative;
  cumulative = new TH1F(*inhist);
  cumulative->Reset();
  int n=inhist->GetNbinsX();
  cumulative->GetBinContent(n,inhist->GetBinContent(n));
  for(int i = n-1 ;i >0 ;i--)
    cumulative->SetBinContent(i,inhist->GetBinContent(i)+cumulative->GetBinContent(i+1));

  return cumulative;
}



TGraph* correspond(TH1F* inhist){
  TH1F* gaushist;

  //fill histogram with gaussian values
  gaushist = new TH1F(*inhist);
  gaushist->Reset();
  int n=inhist->GetNbinsX();
  for(int i = 1; i<=n;i++)
    gaushist->SetBinContent(i,TMath::Gaus(gaushist->GetBinLowEdge(i) ,0.,1.));

  TH1F* cumulgaushist = cumulate(gaushist);
  TH1F* cumuldatahist = cumulate(inhist);

  cumulgaushist->Scale(1./cumulgaushist->GetBinContent(1));
  cumuldatahist->Scale(1./cumuldatahist->GetBinContent(1));

  TCanvas* c = new TCanvas("test","test",800,800) ;
  c->Divide(2,2) ;
  c->cd(1) ; cumulgaushist->Draw() ;
  c->cd(2) ; cumuldatahist->Draw() ;

  float* datasig = new float [inhist->GetNbinsX()];
  float* dataprob = new float [inhist->GetNbinsX()];
  float* realsig = new float [inhist->GetNbinsX()];
  float* realprob = new float [inhist->GetNbinsX()];

  float* correspond = new float [inhist->GetNbinsX()];

  bool first =true;
  int maxnum=1;
  for(int i = 1; i<=n;i++){
    datasig[i]=cumuldatahist->GetBinLowEdge(i);
    dataprob[i]=cumuldatahist->GetBinContent(i);
    realsig[i]=cumulgaushist->GetBinLowEdge(i);
    realprob[i]=cumulgaushist->GetBinContent(i);
    if(first == true &&  dataprob[i]==0){
      maxnum=i;
      first = false;
    }
  }
  std::cout << maxnum << std::endl;

  for(int i =1 ; i < maxnum;i++){
    int j=1;
    while(j<=n && dataprob[i]<realprob[j]) j++;
    correspond[i]=realsig[j];
  }
   
  TGraph* tmp = new TGraph(maxnum-1,datasig,correspond);
  
  c->cd(3);
  tmp->Draw("A*");

  return tmp;

}
