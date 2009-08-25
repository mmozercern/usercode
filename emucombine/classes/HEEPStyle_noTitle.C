{


gROOT->SetStyle("Plain");

gStyle->SetCanvasColor(0);
gStyle->SetPalette(1,0);
gStyle->SetOptTitle(1);
gStyle->SetTitleFillColor(0);
gStyle->SetStatColor(0);
gStyle->SetOptFit(1111);
gStyle->SetCanvasBorderMode(0);
gStyle->SetTitleBorderSize(0);
 gStyle->SetOptStat(0); 
  
gStyle->SetTitleXOffset(.9);
gStyle->SetTitleXSize(0.047);
gStyle->SetTitleYOffset(1.3);
gStyle->SetTitleYSize(0.047);
 gStyle->SetOptTitle(0);

gStyle->SetTitleX(0.04);
gStyle->SetTitleY(0.99);
gStyle->SetTitleW(0.88);
gStyle->SetTitleH(0.06);

 gStyle->SetAxisColor(1, "XYZ");
 gStyle->SetStripDecimals(kTRUE);
 gStyle->SetTickLength(0.03, "XYZ");
 gStyle->SetNdivisions(510, "XYZ");
 gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
 gStyle->SetPadTickY(1);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);

  gStyle->SetPadTopMargin(0.09);

 
  gStyle->SetPadBottomMargin(0.105);

  gStyle->SetPadLeftMargin(0.13);

  gStyle->SetPadRightMargin(0.07);

}
