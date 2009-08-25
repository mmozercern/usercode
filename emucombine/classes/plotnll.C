{
model->peak->setConstant(kFALSE);
model->norm->setConstant(kFALSE);
model->normbkge->setConstant(kFALSE);
model->normbkgmu->setConstant(kFALSE);
model->massshift->setConstant(kTRUE);
int nmax = scan_sig->GetMaximumBin();

RooPlot* frame  = model->peak->frame();
RooPlot* frames = model->norm->frame(0,10);
RooPlot* dataframe  = model->mass->frame();
RooPlot* dataframmu  = model->mass->frame();

model->peak->setVal(250 +nmax/500.*(2000.-250));
model->effratio->setVal(1.);
rese = model->sume->fitTo(*(data->datae),Minos(kTRUE),Hesse(kTRUE),Optimize(kFALSE),Save(kTRUE));
double erre=model->peak->getError();
double peake = model->peak->getVal();
double nerrhi   = model->norm->getAsymErrorHi()/model->effratio->getVal();
double nerrlo   = model->norm->getAsymErrorLo()/model->effratio->getVal();
double ne       = model->norm->getVal();

data->datae->plotOn(dataframe);
model->sume->plotOn(dataframe,LineColor(2));

nll = model->sume->createNLL(*(data->datae),Extended(kTRUE),Optimize(kFALSE));
nll->plotOn(frame,LineColor(2));
nll->plotOn(frames,LineColor(2));

model->effratio->setVal(model->fitwitheff);
model->peak->setVal(250 +nmax/500.*(2000.-250));
resmu = model->summu->fitTo(*(data->datamu),Minos(kTRUE),Hesse(kTRUE),Optimize(kFALSE),Save(kTRUE));
double errm=model->peak->getError();
double peakm = model->peak->getVal();
double nmurrhi   = model->norm->getAsymErrorHi();
double nmurrlo   = model->norm->getAsymErrorLo();
double nmu       = model->norm->getVal();

data->datamu->plotOn(dataframmu);
model->summu->plotOn(dataframmu);

nllm = model->summu->createNLL(*(data->datamu),Extended(kTRUE),Optimize(kFALSE));
nllm->plotOn(frame);
nllm->plotOn(frames);


if( rese->minNll() < resmu->minNll()){
  frame->SetMinimum(rese->minNll() - 5);
  frames->SetMinimum(rese->minNll() - 5); 
}
else{
  frame->SetMinimum(resmu->minNll() - 5);
  frames->SetMinimum(resmu->minNll() - 5);
}


// frame->SetMinimum(resmu->minNll() - 1);
// frame->SetMaximum(resmu->minNll() + 1 );


frame->SetXTitle("inv. mass [GeV]");
frame->SetYTitle("-log(likelihood)");
frame->Draw();
TLine line;
line.SetLineColor(2);
line.DrawLine(peake-erre,frame->GetMinimum(),peake-erre,frame->GetMaximum());
line.DrawLine(peake+erre,frame->GetMinimum(),peake+erre,frame->GetMaximum());
line.SetLineColor(4);
line.DrawLine(peakm-errm,frame->GetMinimum(),peakm-errm,frame->GetMaximum());
line.DrawLine(peakm+errm,frame->GetMinimum(),peakm+errm,frame->GetMaximum());

// frame->GetXaxis()->SetRangeUser(1000,1500);
// line.DrawLine(1000,resmu->minNll()+0.5,1500,resmu->minNll()+0.5);

new TCanvas;
frames->SetXTitle("# events");
frames->SetYTitle("-log(likelihood)");
frames->Draw();
TLine line;
line.SetLineColor(2);
line.DrawLine(ne+nerrlo,frames->GetMinimum(),ne+nerrlo,frames->GetMaximum());
line.DrawLine(ne+nerrhi,frames->GetMinimum(),ne+nerrhi,frames->GetMaximum());
//line.DrawLine(0,rese->minNll()+0.5,10,rese->minNll()+0.5);
line.SetLineColor(4);
line.DrawLine(nmu+nmurrlo,frames->GetMinimum(),nmu+nmurrlo,frames->GetMaximum());
line.DrawLine(nmu+nmurrhi,frames->GetMinimum(),nmu+nmurrhi,frames->GetMaximum());
//line.DrawLine(0,resmu->minNll()+0.5,10,resmu->minNll()+0.5);

TCanvas* cfit=new TCanvas;
cfit->Divide(1,2);
cfit->cd(1);
dataframe->Draw();
cfit->cd(2);
dataframmu->Draw();

new TCanvas;
RooPlot* frame2  = model->peak->frame();
RooPlot* frames2 = model->norm->frame(0,20);
RooPlot* dataframe2 = model->mass->frame();
RooPlot* dataframmu2 = model->mass->frame();

resb = model->sumboth->fitTo(*(data->combdata),Minos(kTRUE),Hesse(kTRUE),Optimize(kFALSE),Save(kTRUE));
double errb=model->peak->getError();
double peakb = model->peak->getVal();

data->datae->plotOn(dataframe2);
model->sume->plotOn(dataframe2,LineColor(2));
data->datamu->plotOn(dataframmu2);
model->summu->plotOn(dataframmu2,LineColor(4));


nllb = model->sumboth->createNLL(*(data->combdata),Extended(kTRUE),Optimize(kFALSE));
nllb->plotOn(frame2,LineColor(1));
nllb->plotOn(frames2,LineColor(1));

frame2->SetMinimum(resb->minNll() - 5);
frame2->SetXTitle("inv. mass [GeV]");
frame2->SetYTitle("-log(likelihood)");
frame2->Draw();

new TCanvas;
frames2->SetMinimum(resb->minNll() - 5);
frames2->SetXTitle("signal amplitude");
frames2->SetYTitle("-log(likelihood)");
frames2->Draw();


TCanvas* cfit2=new TCanvas;
cfit2->Divide(1,2);
cfit2->cd(1);
dataframe2->Draw();
cfit2->cd(2);
dataframmu2->Draw();





new TCanvas;
model->massshift->setConstant(kFALSE);
nll2d = model->sumboth->createNLL(*(data->combdata),Extended(kTRUE),Optimize(kFALSE));

TH2F* nllhisto = (TH2F*)nll2d->createHistogram("nll2dhisto",*(model->peak),YVar(*(model->massshift),Binning(101,0.9,1.1)));
nllhisto->Draw("color");

line.SetLineColor(1);
line.DrawLine(250,1,2000,1);

model->norm->setConstant(kTRUE);
model->normbkge->setConstant(kTRUE);
model->normbkgmu->setConstant(kTRUE);

resfix = model->sumboth->fitTo(*(data->combdata),Minos(kTRUE),Hesse(kTRUE),Optimize(kFALSE),Save(kTRUE));

TMarker* m1= new TMarker();
m1->SetMarkerStyle(24);
m1->SetMarkerSize(2);
m1->DrawMarker(model->peak->getVal(),model->massshift->getVal());

model->norm->setConstant(kFALSE);
model->normbkge->setConstant(kFALSE);
model->normbkgmu->setConstant(kFALSE);

resfloat = model->sumboth->fitTo(*(data->combdata),Minos(kTRUE),Hesse(kTRUE),Optimize(kFALSE),Save(kTRUE));

m1->SetMarkerStyle(25);
m1->DrawMarker(model->peak->getVal(),model->massshift->getVal());


}
