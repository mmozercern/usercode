void plot(TGraph* te, TGraph* tm, TGraph* tb,float ze,float zm, float zb){
  
  TH1F* dummy = new TH1F("Dummy","Dummy",1,0,6);

  dummy->GetXaxis()->SetTitle("Z");
  dummy->GetYaxis()->SetTitle("true significance [#sigma ]");
  dummy->SetMinimum(0);
  dummy->SetMaximum(6);
  
  dummy->Draw();

  TLine line;

  te->SetLineColor(2);
  te->SetLineWidth(2);
  te->Draw("L,same");

  double* ex = te->GetX();
  double* ey = te->GetY();
  float truee;
  for(int i=0; i< te->GetN(); i++){
    if (ex[i]>ze){
      truee=ey[i];
      break;
    }
  }
  line.SetLineColor(2);
  line.DrawLine(ze,0,ze,truee);
  line.DrawLine(0,truee,ze,truee);
  std::cout << "Elec true sig: " << truee << std::endl;

  tm->SetLineColor(4);
  tm->SetLineWidth(2);
  tm->Draw("L,same");

  double* mx = tm->GetX();
  double* my = tm->GetY();
  float truem;
  for(int i=0; i< tm->GetN(); i++){
    if (mx[i]>zm){
      truem=my[i];
      break;
    }
  }

  line.SetLineColor(4);
  line.DrawLine(zm,0,zm,truem);
  line.DrawLine(0,truem,zm,truem);
  std::cout << "Muon true sig: " << truem << std::endl;


  tb->SetLineColor(1);
  tb->SetLineWidth(2);
  tb->Draw("L,same");

  double* bx = tb->GetX();
  double* by = tb->GetY();
  float trueb;
  for(int i=0; i< tb->GetN(); i++){
    if (bx[i]>zb){
      trueb=by[i];
      break;
    }
  }
  line.SetLineColor(1);
  line.DrawLine(zb,0,zb,trueb);
  line.DrawLine(0,trueb,zb,trueb);
  std::cout << "Comb true sig: " << trueb << std::endl;

}
