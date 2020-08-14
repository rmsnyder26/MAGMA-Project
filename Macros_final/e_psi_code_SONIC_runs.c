// author:  j.nagle
// date  :  04/02/2020
// goal  :  code uses a TH2D (2-d ROOT histogram with energy densities) as input (i.e. from your code)
//          and then calculates the eccentricity moments and directions
//          A good paper to read on the definitions is here (https://arxiv.org/abs/1003.0194)
//          This is the "famous" Alver and Roland paper with 1000+ citations!

void e_psi_code_SONIC_runs() {

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPalette(1);

  //==================================================================================================
  // File I/O
  // you can hardcode these of pass them in as arguments, or paste this code into your own code!
  //==================================================================================================
  char rootfile[100];
  char histname[100];
  sprintf(rootfile,"Rho_MAGMA_for_SONIC_25_26_cent_1000_runs.root");

  TFile *f = new TFile(rootfile);
  if (!f) {
    cout << "ERROR - NO ROOT FILE FOUND = " << rootfile << endl;
    return; // error, no file found
  }

//Loop over histograms in TFile and get each key name. Then find e2s and e3s of each histogram

  ofstream myfile;
  myfile.open ("1000_Ev_for_SONIC_25_26_cent.txt");

  myfile << std::left << std::setw(16) << "#Event Number";
  myfile << std::left << std::setw(16) << "e2s";
  myfile << std::left << std::setw(16) << "e3s" << "\n";

  for (auto&& keyAsObj : *f->GetListOfKeys() ){
    auto key = (TKey*) keyAsObj;
    cout << key->GetName() << endl;

    // const char *filename;
    
    // filename = key -> GetName();

    std::string fileName = key->GetName();
    int histVal = std::stoi(fileName.substr(fileName.rfind("_")+1, fileName.size()));

    printf("%d",histVal);
    // for (i = 21; i < strlen(filename); i++ ) { if (isdigit(filename[i])) {
    //     strcat(ev_num_char,filename[i]);

    // size_t i=0;
    // char ev_num_char[100];

        // printf("%c", ev_num_char);
    //   }
    // }
        // ev_num_char.append(filename[i])

    // filename = filename.substr(i, strlen(filename) - i);

    // int ev_num = atoi(filename.c_str());

    // std::cout << ev_num << std::endl;

    myfile << std::left << std::setw(16) << histVal;
    // std::string(filename,21,4);

    TH2D *h = static_cast <TH2D *> (f->Get(key->GetName()));
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
      for (int iy=1;iy<=nbinsY;iy++) {
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

        // calculate moments just 2-3 and put then in the arrays by exact value (even though starting at zero)
        for (int j=2;j<=3;j++) {
  	// epsilon_n weighted by r^n
  	double rtemp = TMath::Power( TMath::Power(xtemp,2) + TMath::Power(ytemp,2) , ((float)j)/2.0 );
  	sinsum[j] += weight * rtemp * TMath::Sin(((float)j)*phitemp);
  	cossum[j] += weight * rtemp * TMath::Cos(((float)j)*phitemp);
  	normsum[j] += weight * rtemp;
        }
        
      }
    }

    // now that we have the mean, calculate the various eccentricity angles/moments
    // A = epsilon_N cos(N*psiN) = sum (r^2 cos(N*phi)) / sum(r^2)
    // B = epsilon_N sin(N*psiN) = sum (r^2 sin(N*phi)) / sum(r^2)
    // psiN = (atan2(B/A))/N
    // epsilon_N = A / cos(N*psiN)

    for (int j=2;j<=3;j++) {

      double moment = (double) j;
      
      double eccentricity = TMath::Sqrt(cossum[j]*cossum[j] + sinsum[j]*sinsum[j]) / normsum[j];
      double psi = (atan2(sinsum[j],cossum[j]) + TMath::Pi()) / moment;
      
      // cout << "e" << j << " = " << eccentricity << endl;
      // cout << "psi angle" << j << "=" << psi << endl;



      if (j==2){
        double e2 = eccentricity;
        myfile << std::left << std::setw(16) << e2;
      }

      if (j==3){
        double e3 = eccentricity;
        myfile << std::left << std::setw(16) << e3 << "\n";
      }
    }
    
  }
  myfile.close();
  // for an individual event (i.e. a single TH2D) one can print the eccentricity value and draw the psi vector
  // direction -- that is useful particular for debugging

  // one can also loop over many events and plot the distribution of eccentricity (2) == elliptical moment
  // and eccentricity (3) == triangularity moment

  // QUESTIONS ARE MOST WELCOME!

}