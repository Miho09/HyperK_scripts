

#include <stdio.h>
#include <stdlib.h>
#include <TH3.h>
#include <TH1.h>
#include <math.h>
#include <vector>
// #include <tuple>
using std::vector;
// using std::tuple;
// using std::get;



void capQ_coor(char *filename=NULL) {
  /* A simple script to plot aspects of phototube hits
   * This code is rather cavalier; I should be checking return values, etc.
   * First revision 6-24-10 David Webber
   *
   * I like to run this macro as
   * $ root -l -x 'read_PMT.C("../wcsim.root")'
   */



  gROOT->Reset();
  char* wcsimdirenv;
  wcsimdirenv = getenv ("WCSIMDIR");
  if(wcsimdirenv !=  NULL){
    gSystem->Load("${WCSIMDIR}/libWCSimRoot.so");
  }else{
    gSystem->Load("../libWCSimRoot.so");
  }
  gStyle->SetOptStat(1);

  TFile *f;
  if (filename==NULL){
    f = new TFile("../wcsim.root");
  }else{
    f = new TFile(filename);
  }
  if (!f->IsOpen()){
    cout << "Error, could not open input file: " << filename << endl;
    return -1;
  }

  TTree  *wcsimT = f->Get("wcsimT");

  WCSimRootEvent *wcsimroothyperevent = new WCSimRootEvent();
  wcsimT->SetBranchAddress("wcsimrootevent",&wcsimroothyperevent);

  TTree  *wcsimGeoT = (TTree*) f->Get("wcsimGeoT");

  WCSimRootGeom *wcsimrootgeom = 0;
  wcsimGeoT->SetBranchAddress("wcsimrootgeom",&wcsimrootgeom);
  // cout << "wcsimrootgeom value: " << wcsimrootgeom << endl;
  // cout << "getentry: " << wcsimGeoT->GetEntries() << endl;
  wcsimGeoT->GetEntry(0);

  // Force deletion to prevent memory leak when issuing multiple
  // calls to GetEvent()
  wcsimT->GetBranch("wcsimrootevent")->SetAutoDelete(kTRUE);

  //--------------------------
  // As you can see, there are lots of ways to get the number of hits.
  cout << "Nb of entries " << wcsimT->GetEntries() << endl;

  //-----------------------

  // TH1D *Q_coor = new TH1D("Q_coor","Q_coor", 100,0,100);
  // Q_coor->SetXTitle("q_coor against Number of PMTs");


  const long unsigned int nbEntries = wcsimT->GetEntries();
  vector <double> arr(0);
  vector<vector<double> >  hist(115;arr);


  for(long unsigned int iEntry = 0; iEntry < nbEntries; iEntry++){
    // Point to event iEntry inside WCSimTree
    wcsimT->GetEvent(iEntry);
    // Nb of Trigger inside the event
    const unsigned int nbTriggers = wcsimroothyperevent->GetNumberOfEvents();
    const unsigned int nbSubTriggers = wcsimroothyperevent->GetNumberOfSubEvents();

    for(long unsigned int iTrig = 0; iTrig < nbTriggers; iTrig++){
      WCSimRootTrigger *wcsimrootevent = wcsimroothyperevent->GetTrigger(iTrig);

      // RAW HITS
      int ncherenkovdigihits = wcsimrootevent->GetNcherenkovdigihits();
        for (int i = 0; i < ncherenkovdigihits; i++){
          WCSimRootCherenkovDigiHit *hit = (WCSimRootCherenkovDigiHit*)
          (wcsimrootevent->GetCherenkovDigiHits()->At(i));

          WCSimRootCherenkovDigiHit *cDigiHit = wcsimrootevent->GetCherenkovDigiHits()->At(i);

          double PhotoElectrons = cDigiHit->GetQ();

          int tubeId = hit->GetTubeId();
          double timing = hit->GetT();
          // cout << "Tube ID: " << tubeId << endl;
          WCSimRootPMT pmt = wcsimrootgeom->GetPMT(tubeId);
          // double pmtX = pmt.GetPosition(0);
          // double pmtY = pmt.GetPosition(1);
          double pmtZ = pmt.GetPosition(2);
          double real_z = pmtZ + 2750.;
          double index = real_z / 5500;
          double new_index = index * 110;
          double f_index = floor(index);


          float K_value = 0.0004;
          float d_w = (real_z) / (cos(TMath::Pi() * 43/180));
          float L_att = 70.;
          double expon = exp(d_w / L_att);
          float f_theta = pow(cos(TMath::Pi() * 47/180), 2);
          float q_coor = (K_value * PhotoElectrons * d_w * expon) / f_theta;

          // std::tuple<double, double> data(pmtZ, q_coor);
          hist[f_index].push_back(q_coor);

          // Q_coor->Fill(f_sector, charge);

          } // END FOR RAW HITS


    } // END FOR iTRIG

  } // END FOR iENTRY

  vector <double> all_Q_coor(110);

  for (int bins=0; bins < hist.size(); ++bins){
    double N_pmt = hist[bins].size();
    double sum_q = 0;
    for (int n_PMT=0; n_PMT < hist[bins].size(); ++n_PMT){
      sum_q += hist[bins][n_PMT];

    }

    all_Q_coor[bins]= sum_q / N_pmt;


  }

  // TCanvas *c1 = new TCanvas("c1");
  //
  //  Q_coor->Draw();
  //  c1->Update();
  //  Q_coor->SetMarkerStyle(3);

   // T->Draw("Cost:Age>>hist","","goff");



  }
