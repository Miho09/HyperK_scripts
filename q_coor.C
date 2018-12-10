#include <stdio.h>
#include <stdlib.h>

#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include <math.h>       /* sin */



using namespace std;


void q_coor(char *filename=NULL) {
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
  // cout << "1" << endl;
  WCSimRootEvent *wcsimroothyperKevent = new WCSimRootEvent();
  wcsimT->SetBranchAddress("wcsimrootevent",&wcsimroothyperKevent);

  // cout << "2" << endl;
  TTree  *wcsimGeoT = (TTree*) f->Get("wcsimGeoT");
  // cout << "3" << endl;
  WCSimRootGeom *wcsimrootgeom = 0;
  wcsimGeoT->SetBranchAddress("wcsimrootgeom",&wcsimrootgeom);
  // cout << "wcsimrootgeom value: " << wcsimrootgeom << endl;
  // cout << "getentry: " << wcsimGeoT->GetEntries() << endl;
  wcsimGeoT->GetEntry(0);
  // cout << "4" << endl;
  // Force deletion to prevent memory leak when issuing multiple
  // calls to GetEvent()
  wcsimT->GetBranch("wcsimrootevent")->SetAutoDelete(kTRUE);

  //--------------------------
  // As you can see, there are lots of ways to get the number of hits.
  cout << "Nb of entries " << wcsimT->GetEntries() << endl;

  TH1 *q_raw_hist = new TH1D("q_raw_hist", "hist",100, -100, 100);
  q_raw_hist->SetTitle("Histogram for q_raw")
  // coor_q->SetMinimum(1e-11);
  //
  // coor_q->SetYTitle("Charge distribution in 3D");
  // // graph.GetYaxis()->SetTitleOffset(1.4);
  // coor_q->SetXTitle("charge");


  const long unsigned int nbEntries = wcsimT->GetEntries();

  for(long unsigned int iEntry = 0; iEntry < nbEntries; iEntry++){
    // Point to event iEntry inside WCSimTree
    wcsimT->GetEvent(iEntry);

    // Nb of Trigger inside the event
    const unsigned int nbTriggers = wcsimroothyperKevent->GetNumberOfEvents();
    const unsigned int nbSubTriggers = wcsimroothyperKevent->GetNumberOfSubEvents();

    for(long unsigned int iTrig = 0; iTrig < nbTriggers; iTrig++){
      WCSimRootTrigger *wcsimrootevent = wcsimroothyperKevent->GetTrigger(iTrig);

      // RAW HITS
      int ncherenkovhits = wcsimrootevent->GetNcherenkovhits();
        for (int i = 0; i < ncherenkovhits; i++){
          WCSimRootCherenkovHit *chit = wcsimrootevent->GetCherenkovHits()->At(i);
    //WCSimRootChernkovDigiHit has methods GetTubeId(), GetT(), GetQ()
            WCSimRootCherenkovHitTime *cHitTime = wcsimrootevent->GetCherenkovHitTimes()->At(i);
            // WCSimRootCherenkovHitTime has methods GetTubeId(), GetTruetime()

            // WCSimRootCherenkovDigiHit *cDigiHit = wcsimrootevent->GetCherenkovDigiHits()->At(i);
            //WCSimRootChernkovDigiHit has methods GetTubeId(), GetT(), GetQ()
            // QvsT->Fill(cDigiHit->GetT(), cDigiHit->GetQ());
            WCSimRootPMT pmt = wcsimrootgeom->GetPMT(tubeId);

            double pmtX = pmt.GetPosition(0);
            double pmtY = pmt.GetPosition(1);
            double pmtZ = pmt.GetPosition(2);

            double K_value = (1 / 2500);
            double PhotoElectrons = chit->GetTotalPe(1);
            double d_w = pmtZ / cos(pi * 43/180)
            double L_att = 7
            double expon = exp(d_w / L_att)
            // double ang_acc =
            double q_coor = K_value * PhotoElectrons * d_q * expon

            // double charge = hit->GetQ();
            // int tubeId = cHitTime -> GetTubeId();
            // double timing = hit->GetT();
            // // cout << "Tube ID: " << tubeId << endl;
            // WCSimRootPMT pmt = wcsimrootgeom->GetPMT(tubeId);

            q_raw_hist->Fill(q_coor);

          } // END FOR RAW HITS
          // myfile.close();
          // xfile.close();
          // yfile.close();
          // zfile.close();
          // qfile.close();


    } // END FOR iTRIG

  } // END FOR iENTRY


   TH1 *temp;
      q_raw_hist->SetFillColor(2);

      // gStyle->SetNumberContours(112);
      // gStyle->SetPalette(112);
      // coor_q->Draw("SURF2");
      q_raw_hist->Draw();




  }
