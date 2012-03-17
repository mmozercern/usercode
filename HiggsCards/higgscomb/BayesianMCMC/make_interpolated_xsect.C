#include <vector>
#include <fstream>
#include <Riostream.h>

#include "TGraph.h"
#include "TLegend.h"
#include "TF1.h"
#include "TAxis.h"
#include "TCanvas.h"

void make_interpolated_xsect(string inf="./RSGravXSectTimesBRToZZ_Pythia_c05.txt", string outf="tmp.out");
double powlaw(double *xx,double *par){
  double x=xx[0];
  return par[0]*pow(x,par[1]);
}

double expLev(double *xx,double *par){
  double x=xx[0];
  const double e=exp(1.0);
  return par[0]*pow(e,-1.0*(x-par[3])/(par[1] + par[2]*(x-par[3])));
}

void expo_interpolation(double m1,double m2, double s1, double s2, double newM, double &newS){

  if(m1>m2){
    double tmps=s1;
    double tmpm=m1;
    s1=s2;
    m1=m2;
    s2=tmps;
    m2=tmpm;
  }
  double deltaM=m2-m1;
  double alpha=(log(s2)-log(s1)) / deltaM;
  newS=s1*pow(exp(newM-m1),alpha);

}

double linear_interp( double s2, double s1, double mass, double m2, double m1 ) {
  if(m1>m2){
    double tmps=s1;
    double tmpm=m1;
    s1=s2;
    m1=m2;
    s2=tmps;
    m2=tmpm;
  }
  return (s1 + ( s2-s1 ) * ( mass-m1 ) / ( m2-m1 ));

}

void make_interpolated_xsect(string inf, string outf){
  ifstream fin(inf.c_str(),ios::in);
  float mH, CS;
  const float BRZZ2l2q=0.0937;

  vector<float> v_mh, v_xs;
  while(fin.good()){
    fin >> mH>> CS;
    v_mh.push_back(mH);
    v_xs.push_back(CS);
  }
  const int nM=v_mh.size();
  float m_arr[nM],xs_arr[nM];
  for(int i=0;i<nM;i++){
    m_arr[i]=v_mh.at(i);
    xs_arr[i]=v_xs.at(i);
  }

  TGraph *gr_xs=new TGraph(nM,m_arr,xs_arr);
  gr_xs->SetName("rs_grav_xsect_LO");
  gr_xs->SetMarkerColor(kBlue);
  gr_xs->SetMarkerStyle(24);
  gr_xs->GetXaxis()->SetTitle("M_{G} [GeV]");
  gr_xs->GetYaxis()->SetTitle("#sigma_{LO}(pp #rightarrow G #rightarrow ZZ) [pb]");
  TF1 *fitfunc=new TF1("powfit",powlaw,m_arr[0],m_arr[nM-1],2);

  //  fitfunc->SetParameter(0,);
  fitfunc->SetParameter(1,-2.0);
  fitfunc->SetParName(0,"Norm");
  fitfunc->SetParName(1,"Alpha");
  //  fitfunc->SetParName(0,"Norm");
  fitfunc->SetParLimits(1,-10.0,0.0);
  cout<<"N pars: "<<fitfunc->GetNumberFreeParameters()<<endl;


  TF1 *fitfunc2=new TF1("expfit","expo",m_arr[0],m_arr[nM-1]);
  fitfunc2->SetLineColor(kBlue);
  TF1 *fitfunc3=new TF1("expLevfit",expLev,m_arr[0],m_arr[nM-1],4);
  fitfunc3->SetParameter(0,  10 );
  fitfunc3->SetParameter(1,  1.0 /0.0147943 );
  fitfunc3->SetParameter(2,  0.0 );
  fitfunc3->SetParameter(3,  200.0 );
  fitfunc3->FixParameter(3,  180.0 );
  fitfunc3->SetParName(0,"Norm");
  fitfunc3->SetParName(1,"Sigma");
  fitfunc3->SetParName(2,"Alpha");
  fitfunc3->SetParName(3,"m");

  fitfunc2->SetLineColor(kBlue);
  fitfunc2->SetLineColor(kOrange);
  gr_xs->SetMinimum(0.4);
  // gr_xs->Fit(fitfunc,"W+");
  // gr_xs->Fit(fitfunc2,"W+");
  // gr_xs->Fit(fitfunc3,"W+");

  float m_arr2[nM*2],xs_arr2[nM*2],xs_arr3[nM*2];
 
  for(int i=0;i<nM-1;i++){
    double deltaM=0.0;
    //    if(i!=nM-1)deltaM=v_mh.at(i+1)-v_mh.at(i);
    // else deltaM=0.0;
    deltaM=v_mh.at(i+1)-v_mh.at(i);
    for(int j=0;j<4;j++){
    m_arr2[i*2+j]=v_mh.at(i)+j*deltaM/2.0;
    double newXS=0.0;
    expo_interpolation(v_mh.at(i),v_mh.at(i+1),  v_xs.at(i), v_xs.at(i+1),m_arr2[i*2+j] , newXS);    
    xs_arr2[i*2+j]=newXS;
    


    xs_arr3[i*2+j]= linear_interp(v_xs.at(i+1),v_xs.at(i), m_arr2[i*2+j] ,v_mh.at(i+1),v_mh.at(i));
    }
  }
 TGraph *gr_xs2=new TGraph(nM*2,m_arr2,xs_arr2);
  gr_xs2->SetName("rs_grav_xsect_EXPINTERPOL");
  gr_xs2->SetMarkerColor(kViolet);
  gr_xs2->SetMarkerStyle(21);

  TGraph *gr_xs3=new TGraph(nM*2,m_arr2,xs_arr3);
  gr_xs3->SetName("rs_grav_xsect_LININTERPOL");
  gr_xs3->SetMarkerColor(kGreen+3);
  gr_xs3->SetMarkerStyle(23);
  gr_xs3->SetMarkerSize(1.0);

  TLegend *l=new TLegend(0.7,0.8,0.95,0.99);
  l->AddEntry(gr_xs,"Tabulated XSECT","P");
  l->AddEntry(gr_xs3,"Lin interpolation","P");
  l->AddEntry(gr_xs2,"Exp interpolation","P");
  TCanvas *ctmp=new TCanvas("fff","rrrrr",800,800);
  ctmp->cd();
  gr_xs->Draw("AP");
  fitfunc->SetLineColor(kRed);
  gr_xs3->Draw("P");
  gr_xs2->Draw("P");
  gr_xs->Draw("P");
  l->Draw();
  ctmp->SaveAs("RSXSECT_fitRes.root");
  gPad->SetLogy();
 ctmp->SaveAs("RSXSECT_fitRes.eps");
 ctmp->SaveAs("RSXSECT_fitRes.png");

  delete gr_xs; delete gr_xs2;




  ofstream outfile(outf.c_str(),ios::out);
  ifstream mfile("./list_masses.txt",ios::in);
  double mm;
  while(mfile.good()){
    mfile>>mm;
    cout<<"Interpolating mass "<<mm<<"  --> "<<flush;
    //find bin in original xsect
    bool found =false;
    int i0=0,im=-1;
    while( !found ){
      if(i0>=v_mh.size()-1)break;
      if(mm==v_mh.at(v_mh.size()-1) ){//last value found
	  found=true;
	  cout<<"M found at index "<<v_mh.size()-1<<" (m="<<v_mh.at(v_mh.size()-1)<<")"<<flush;
	  im=v_mh.size()-1;	
      }
      else{
	if(mm<v_mh.at(i0+1) ){
	  found=true;
	  cout<<"M found at index "<<i0<<" (m="<<v_mh.at(i0)<<")"<<flush;
	  im=i0;
	}
      }
      i0++;
    }
    if(!found)cout<<" MASS NOT FOUND !!! "<<flush;
    double newXS=-99.0;
    const double fbTopb=1000.0;
    expo_interpolation(v_mh.at(im),v_mh.at(im+1),  v_xs.at(im), v_xs.at(im+1),mm , newXS);
     // newXS= linear_interp(v_xs.at(im+1),v_xs.at(im), mm ,v_mh.at(im+1),v_mh.at(im));
    outfile<<mm<<"\t"<<newXS/fbTopb<<endl;
    //    cout<<mm<<"\t"<<newXS<<endl;
  }//end loop on masses in mfile
  outfile.close();

  delete ctmp;
}
