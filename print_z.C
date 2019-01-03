

#include <stdio.h>
#include <stdlib.h>
#include <TH3.h>
#include <TH1.h>


void print_z(char *filename=NULL) {
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



  // TH2F *Qraw_x = new TH2F("Qraw_x","Raw Charge vs X axis",100,70,80,1000,-4000,4000);
  //
  // // TH2D *discorr = new TH3D("coorq", "Cartesian coordinates with charge",100,-4000,4000,100,-4000,4000,1000,-4000,4000);
  // // cout << "1" << endl;
  // Qraw_x->SetTitle("Charge Distribution");
  // // cout << "2" << endl;

  // graph.GetYaxis()->SetTitleOffset(1.4);


//ben added
// string rootfilename;
// cout << "Enter name of file for test root file: " << endl;
// cin >> rootfilename;
  const long unsigned int nbEntries = wcsimT->GetEntries();

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
      //WCSimRootChernkovDigiHit has methods GetTubeId(), GetT(), GetQ()
            // WCSimRootCherenkovHitTime *cHitTime = wcsimrootevent->GetCherenkovHitTimes()->At(i);
            //WCSimRootCherenkovHitTime has methods GetTubeId(), GetTruetime()

            WCSimRootCherenkovDigiHit *cDigiHit = wcsimrootevent->GetCherenkovDigiHits()->At(i);
            //WCSimRootChernkovDigiHit has methods GetTubeId(), GetT(), GetQ()
            // QvsT->Fill(cDigiHit->GetT(), cDigiHit->GetQ());

          double charge = hit->GetQ();
          int tubeId = hit -> GetTubeId();
          double timing = hit->GetT();
          // cout << "Tube ID: " << tubeId << endl;
          WCSimRootPMT pmt = wcsimrootgeom->GetPMT(tubeId);
          // double pmtX = pmt.GetPosition(0);
          // double pmtY = pmt.GetPosition(1);
          double pmtZ = pmt.GetPosition(2);
          cout << "Z axis value: " << pmtZ << endl;

            // cout << "Y value: " << pmtY << endl;
          // cout << "4" << endl;

          // Qraw_x->Fill(charge, pmtX);

          } // END FOR RAW HITS

    } // END FOR iTRIG

  } // END FOR iENTRY

  // cout << "5" << endl;

  // TCanvas *c1 = new TCanvas("c1");
  //
  //  Qraw_x->Draw();
  //  c1->Update();
  //  Qraw_x->SetMarkerStyle(3);

   // T->Draw("Cost:Age>>hist","","goff");



  }
