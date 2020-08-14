// author:  j.nagle
// date  :  04/02/2020
// goal  :  code uses a TH2D (2-d ROOT histogram with energy densities) as input (i.e. from your code)
//          and then calculates the eccentricity moments and directions
//          A good paper to read on the definitions is here (https://arxiv.org/abs/1003.0194)
//          This is the "famous" Alver and Roland paper with 1000+ citations!

#include "TH1D.h"
#include "TRandom.h"
#include <time.h>
#include <iostream>

void e_psi_code() {

  //This is outside the loop to create an array for all of the moments

  //Specify number of events and initialize code running time

  time_t start_time;
  
  start_time = time(NULL);

  int nev = 100;
  double eps_2_orig[nev];
  double eps_2_mod[nev];
  double eps_3_orig[nev];
  double eps_3_mod[nev];

  // TCanvas* c1 = new TCanvas("c1", "Canvas", 500, 500);
  TH1D* hist_e2_orig = new TH1D("orig_2","#varepsilon_{2} Comparison",100, 0.0, 1.0);
  TH1D* hist_e2_mod = new TH1D("mod_2","hist_mod_2",100, 0.0, 1.0);

  // TCanvas* c2 = new TCanvas("c2", "Canvas", 500, 500);
  TH1D* hist_e3_orig = new TH1D("orig_3","#varepsilon_{3} Comparison",100, 0.0, 1.0);
  TH1D* hist_e3_mod = new TH1D("mod_3","hist_mod_3",100, 0.0, 1.0);

  for (int ev = 0; ev < nev; ev++) {

    //==================================================================================================
    // File I/O
    // you can hardcode these of pass them in as arguments, or paste this code into your own code!
    //==================================================================================================
    char rootfile[100];
    char histname[100];
    
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPalette(1);

    sprintf(rootfile,"DensityPlot_size100_only.root");

    for (int i =0; i <=1; i++) {
      if (i == 0) {
        sprintf(histname,"Energy_Dens_orig_ev_%d_size_100", ev);
      }
      else {
        sprintf(histname,"Energy_Dens_mod_ev_%d_size_100", ev);
      }

      TFile *f = new TFile(rootfile);
      if (!f) {
        cout << "ERROR - NO ROOT FILE FOUND = " << rootfile << endl;
        return; // error, no file found
      }

      TH2D *h = static_cast <TH2D *> (f->Get(histname));
      if (!h) {
        cout << "ERROR - NO HISTOGRAM FOUND = " << histname << endl;
        return; // error, no histogram with this name found
      }

      int nbinsX = h->GetNbinsX();
      int nbinsY = h->GetNbinsY();

      //==================================================================================================  
      // now one needs to calculate the center-of-mass in x,y of the energy distribution
      //==================================================================================================
      double meanx = 0.0;
      double meany = 0.0;
      double energytotal = 0.0;

      // note that ROOT histogram bins start at 1 (not zero - urg!)
      for (int ix=1;ix<=nbinsX;ix++) {
        for (int iy=1;iy<=nbinsY;iy++) 
        {
          double weight = h->GetBinContent(ix,iy);
          energytotal += weight;
          meanx += h->GetXaxis()->GetBinCenter(ix) * weight;
          meany += h->GetYaxis()->GetBinCenter(iy) * weight;
        }
      }
    


      meanx = meanx / energytotal;
      meany = meany / energytotal;

      // now calculate eccentricities
      double sinsum[10];
      double cossum[10];
      double normsum[10];
      
      // 10 calculations for the first 10 eccentricity moments
      for (int j=0;j<10;j++) {
        sinsum[j]  = 0.0;
        cossum[j]  = 0.0;
        normsum[j] = 0.0;
      }

      for (int ix=1;ix<=nbinsX;ix++) {
        for (int iy=1;iy<=nbinsY;iy++) {
          
          double xtemp = h->GetXaxis()->GetBinCenter(ix) - meanx;
          double ytemp = h->GetYaxis()->GetBinCenter(iy) - meany;
          double phitemp = TMath::ATan2(ytemp,xtemp);
          double weight = h->GetBinContent(ix,iy);

          // calculate moments just 2-6 and put then in the arrays by exact value (even though starting at zero)
          for (int j=2;j<=6;j++) {
          	// epsilon_n weighted by r^n
          	double rtemp = TMath::Power( TMath::Power(xtemp,2) + TMath::Power(ytemp,2) , ((float)j)/2.0 );
          	sinsum[j] += weight * rtemp * TMath::Sin(((float)j)*phitemp);
          	cossum[j] += weight * rtemp * TMath::Cos(((float)j)*phitemp);
          	normsum[j] += weight * rtemp;
          }
          
        }
      }

      // if (ev == 10) {
      //   printf("sinsum[2] = %lf",sinsum[2]);
      //   printf("cossum[2] = %lf",cossum[2]);
      //   printf("normsum[2] = %lf",normsum[2]);
      // }



      // now that we have the mean, calculate the various eccentricity angles/moments
      // A = epsilon_N cos(N*psiN) = sum (r^2 cos(N*phi)) / sum(r^2)
      // B = epsilon_N sin(N*psiN) = sum (r^2 sin(N*phi)) / sum(r^2)
      // psiN = (atan2(B/A))/N
      // epsilon_N = A / cos(N*psiN)

      //In this case we just want e2s and e3s, so j=3 to j=3 only 

      for (int j=2;j<=3;j++) {

        double moment = (double) j;
        
        double eccentricity = TMath::Sqrt(cossum[j]*cossum[j] + sinsum[j]*sinsum[j]) / normsum[j];
        double psi = (atan2(sinsum[j],cossum[j]) + TMath::Pi()) / moment;
        
        if(j==2) {
          if (i==0) {

            
            eps_2_orig[ev] = eccentricity;
            hist_e2_orig -> Fill(eccentricity);

          }

          else {
            eps_2_mod[ev] = eccentricity;
            hist_e2_mod -> Fill(eccentricity);
            //printf("%lf \n", eccentricity);
          }
        }

        else {
          if (i==0) {
            eps_3_orig[ev] = eccentricity;
            hist_e3_orig -> Fill(eccentricity);

          }

          else {
            eps_3_mod[ev] = eccentricity;
            hist_e3_mod -> Fill(eccentricity);
            //printf("%lf \n", eccentricity);
          }

        }
      }

      //printf("%lf \n", eps_2_orig[ev]);
      // printf("Mod_%lf", eps_2_mod[ev]);
    // if (i == 0) {
    //   printf("%lf \n",normsum[2]);
    // }

    }
  }
  // gStyle->SetOptStat(000001111);
  // gStyle->SetOptTitle(1);


  TCanvas* c1 = new TCanvas("c1", "Canvas_e2s", 500, 500);

  hist_e2_orig->SetLineColor(kRed);
  hist_e2_orig->SetLineWidth(3);
  hist_e2_orig->Draw();

  hist_e2_mod->SetLineColor(kBlue);
  hist_e2_mod->SetLineWidth(3);
  hist_e2_mod->Draw("SAME");

  //hist_e2_orig -> SetTitle("#varepsilon_{2} Comparison")
  hist_e2_orig->GetXaxis()->SetTitle("#varepsilon_{2}");
  hist_e2_orig->GetYaxis()->SetTitle("Count");

  TLegend *tleg_2 = new TLegend(0.25,0.8,0.9,0.9,"MAGMA","brNDC");
  char label_2[100];
  sprintf(label_2,"MAGMA #varepsilon_{2}, #mu=%3.2f, #sigma=%3.2f",hist_e2_orig->GetMean(), hist_e2_orig->GetRMS());
  tleg_2->AddEntry(hist_e2_orig, label_2, "l");
  tleg_2->SetTextSize(0.03);
  
  sprintf(label_2,"MAGMA modified #varepsilon_{2}, #mu=%3.2f, #sigma=%3.2f",hist_e2_mod->GetMean(), hist_e2_mod->GetRMS());
  tleg_2->AddEntry(hist_e2_mod,label_2,"l");
  tleg_2->Draw();

  TCanvas* c2 = new TCanvas("c2", "Canvas_e3s", 500, 500);

  hist_e3_orig->SetLineColor(kRed);
  hist_e3_orig->SetLineWidth(3);
  hist_e3_orig->Draw();

  hist_e3_mod->SetLineColor(kBlue);
  hist_e3_mod->SetLineWidth(3);
  hist_e3_mod->Draw("SAME");

  //hist_e3_orig -> SetTitle("#varepsilon_{3} Comparison")
  hist_e3_orig->GetXaxis()->SetTitle("#varepsilon_{3}");
  hist_e3_orig->GetYaxis()->SetTitle("Count");

  TLegend *tleg_3 = new TLegend(0.25,0.8,0.9,0.9,"MAGMA","brNDC");
  char label_3[100];
  sprintf(label_3,"MAGMA #varepsilon_{3}, #mu = %3.2f, #sigma = %3.2f",hist_e3_orig->GetMean(), hist_e3_orig->GetRMS());
  tleg_3->AddEntry(hist_e3_orig, label_3, "l");
  tleg_3->SetTextSize(0.03);

  sprintf(label_3,"MAGMA modified #varepsilon_{3}, #mu = %3.2f, #sigma = %3.2f",hist_e3_mod->GetMean(), hist_e3_mod->GetRMS());
  tleg_3->AddEntry(hist_e3_mod,label_3,"l");
  tleg_3->Draw();

  time_t finish_time;
  finish_time = time(NULL);

  cout << "It took " << finish_time - start_time << "seconds to run " << nev << "events";

  // for an individual event (i.e. a single TH2D) one can print the eccentricity value and draw the psi vector
  // direction -- that is useful particular for debugging

  // one can also loop over many events and plot the distribution of eccentricity (2) == elliptical moment
  // and eccentricity (3) == triangularity moment

  // QUESTIONS ARE MOST WELCOME!

}