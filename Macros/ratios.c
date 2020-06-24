void ratios() {

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

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

  TLegend *tleg = new TLegend(0.5,0.5,0.9,0.9,"Ratio v_{3}{2} / v_{2}{2}","brNDC");
  tleg->AddEntry(t0ratio,"IP-Jazma (A #times B)","l");
  tleg->AddEntry(t1ratio,"IP-Jazma (A #times B_{WS} + A_{WS} #times B)","l");
  tleg->AddEntry(t2ratio,"IP-Jazma (#sqrt{A #times B})","l");
  tleg->AddEntry(t3ratio,"MAGMA (A #times B_{WS} + A_{WS} #times B)","l"); 
  tleg->AddEntry(t4ratio,"MAGMA (A #times B)","l"); 
  tleg->Draw("same");
  
}
