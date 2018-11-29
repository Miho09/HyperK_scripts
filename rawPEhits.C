

#include <stdio.h>
#include <stdlib.h>
#include <TH3.h>
#include <TH1.h>


void rawPEhits(char *filename=NULL) {
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
  // cout << "Nb of entries " << wcsimT->GetEntries() << endl;

  //-----------------------


  TH1* PE_hist = new TH1I("Qraw_hist", "Qraw_hist", 100, 0.0, 100.0);

  PE_hist->SetTitle("Raw Photons Histogram");


  const long unsigned int nbEntries = wcsimT->GetEntries();

  for(long unsigned int iEntry = 0; iEntry < nbEntries; iEntry++){
    // Point to event iEntry inside WCSimTree
    wcsimT->GetEvent(iEntry);
    // Nb of Trigger inside the event
    const unsigned int nbTriggers = wcsimroothyperevent->GetNumberOfEvents();
    const unsigned int nbSubTriggers = wcsimroothyperevent->GetNumberOfSubEvents();

    for(long unsigned int iTrig = 0; iTrig < nbTriggers; iTrig++){
      WCSimRootTrigger *wcsimrootevent = wcsimroothyperevent->GetTrigger(iTrig);

      // int max=wcsimrootevent->GetNcherenkovhits();
      // for (int i = 0; i<max; i++){
      //   WCSimRootCherenkovHit *chit = wcsimrootevent->GetCherenkovHits()->At(i);
      //   PMT_hits->Fill(chit->GetTubeID());
      //   //WCSimRootCherenkovHit has methods GetTubeId(), GetTotalPe(int)
      //   PE->Fill(chit->GetTotalPe(1));
      // }


        // RAW HITS
      int ncherenkovhits = wcsimrootevent->GetNcherenkovhits();
        for (int i = 0; i < ncherenkovhits; i++){

            WCSimRootCherenkovHit *cHit = wcsimroothyperevent->GetCherenkovHits()->At(i);

            PMT_hits->Fill(chit->GetTubeID());
            //WCSimRootCherenkovHit has methods GetTubeId(), GetTotalPe(int)
            PE_hist->Fill(chit->GetTotalPe(1));
          }// END FOR RAW HITS

    } // END FOR iTRIG

  } // END FOR iENTRY

  // cout << "5" << endl;

  TCanvas *c1 = new TCanvas("c1");

   PE_hist->Draw();
   c1->Update();
   PE_hist->SetMarkerStyle(3);

   // T->Draw("Cost:Age>>hist","","goff");



  }
