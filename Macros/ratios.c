void ratios() {

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TCanvas *c1 = new TCanvas("c","c",500,500);

  TFile *f0 = new TFile("fout_nagle0.root");
  TFile *f1 = new TFile("fout_nagle1.root");
  TFile *f2 = new TFile("fout_nagle2.root");  
  TFile *f3 = new TFile("v_n_fluctuations_MAGMA.root");  
  TFile *f4 = new TFile("v_n_fluctuations_MAGMA_mod.root");  

  TGraph *t0ratio = (TGraph*) (f0->Get("nagleratio_0"));
  TGraph *t1ratio = (TGraph*) (f1->Get("nagleratio_1"));
  TGraph *t2ratio = (TGraph*) (f2->Get("nagleratio_2"));
  TGraph *t3ratio = (TGraph*) (f3->Get("v3_over_v2_MAGMA"));
  TGraph *t4ratio = (TGraph*) (f4->Get("v3_over_v2_MAGMA_mod"));

  float ATLAS_ratio_x[29] = {2.39583333333333, 1.45833333333333, 3.4375, 4.47916666666666, 5.41666666666666, 6.45833333333333, 7.5, 8.54166666666667, 9.58333333333333, 10.5208333333333, 11.5625, 12.4999999999999, 13.4375, 14.4791666666666, 15.5208333333333, 16.4583333333333, 17.5, 18.5416666666666, 19.5833333333333, 20.5208333333333, 21.5625, 22.5, 23.5416666666666, 24.5833333333333, 25.5208333333333, 26.5625, 27.6041666666666, 28.6458333333333, 29.5833333333333};
  float ATLAS_ratio_y[29] = {1/1.18298969072164, 1/1.06701030927835, 1/1.30670103092783, 1/1.4381443298969, 1/1.54639175257731, 1/1.64690721649484, 1/1.74742268041237, 1/1.84020618556701, 1/1.91752577319587, 1/2.00257731958762, 1/2.07989690721649, 1/2.14948453608247, 1/2.20360824742268, 1/2.26546391752577, 1/2.32731958762886, 1/2.37371134020618, 1/2.42783505154639, 1/2.47422680412371, 1/2.52061855670103, 1/2.55927835051546, 1/2.59793814432989, 1/2.63659793814433, 1/2.67525773195876, 1/2.69845360824742, 1/2.72164948453608, 1/2.75257731958762, 1/2.77577319587628, 1/2.80670103092783, 1/2.8221649484536};
  
  TGraph *atlas = new TGraph(29,ATLAS_ratio_x,ATLAS_ratio_y);
  atlas ->SetMarkerStyle(24);

  t0ratio->SetLineWidth(3);
  t1ratio->SetLineWidth(3);
  t2ratio->SetLineWidth(3);
  t3ratio->SetLineWidth(3);
  t4ratio->SetLineWidth(3);

  t0ratio->SetLineColor(kRed);
  t1ratio->SetLineColor(kBlue);
  t2ratio->SetLineColor(kBlack);
  t3ratio->SetLineColor(kGreen+2);
  t4ratio->SetLineColor(kMagenta);

  TH1D *htemplate = new TH1D("htemplate","htemplate",100,0.0,100.0);
  htemplate->GetXaxis()->SetRangeUser(0.0,30.0);
  htemplate->GetXaxis()->SetTitle("Pb+Pb Centrality");
  htemplate->GetYaxis()->SetRangeUser(0.0,1.1);
  htemplate->GetYaxis()->SetTitle("v_{3}{2} / v_{2}{2}");
  htemplate->DrawCopy();
  t0ratio->Draw("l,same");
  t1ratio->Draw("l,same");
  t2ratio->Draw("l,same");
  t3ratio->Draw("l,same");  
  t4ratio->Draw("l,same");
  atlas -> Draw("p,same");

  TLegend *tleg = new TLegend(0.35,0.5,.9,.9,"Ratio v_{3}{2} / v_{2}{2}","brNDC");
  tleg->AddEntry(atlas,"ATLAS data","p"); 
  tleg->AddEntry(t3ratio,"MAGMA (A #times B_{WS} + A_{WS} #times B)","l"); 
  tleg->AddEntry(t4ratio,"MAGMA (A #times B)","l"); 
  tleg->AddEntry(t0ratio,"IP-Jazma (A #times B)","l");
  tleg->AddEntry(t1ratio,"IP-Jazma (A #times B_{WS} + A_{WS} #times B)","l");
  tleg->AddEntry(t2ratio,"IP-Jazma (#sqrt{A #times B})","l");

  tleg->Draw("same");
  
  c1 -> SaveAs("ratio_plot.pdf");
}
