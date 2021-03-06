#include <stdio.h>
#include <stdlib.h>

#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>


using namespace std;


void three_dim_charge_plot(char *filename=NULL) {
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

  // TH3D *coor_q = new TH3D("coorq", "Cartesian coordinates with charge",1000,-4000,4000,1000,-4000,4000,1000,-4000,4000);
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
      int ncherenkovdigihits = wcsimrootevent->GetNcherenkovdigihits();
      ofstream xfile, yfile, zfile, qfile;
      xfile.open ("x_three_dim_charge_plot.txt");
      yfile.open ("y_three_dim_charge_plot.txt");
      zfile.open ("z_three_dim_charge_plot.txt");
      qfile.open ("q_three_dim_charge_plot.txt");
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
            double pmtX = pmt.GetPosition(0);
            double pmtY = pmt.GetPosition(1);
            double pmtZ = pmt.GetPosition(2);
            // cout << "pmtX: " << << pmtX <<  endl;
            xfile << " " << pmtX << " ";
            yfile << " " << pmtY << " ";
            zfile << " " << pmtZ << " ";
            qfile << " " << charge << " ";
            // , pmtY, pmtZ, charge;
            // cout << "3" << endl;


            // cout << "5" << endl;

            // return 0;

// int main ()
// {
//     int i=0, x=0, y=0;
//     ofstream myfile;
//     myfile.open ("example1.txt");
//
//     for (int j=0; j < 3; j++)
//     {
//         myfile  << i ;
//         myfile  << " " << x;
//         myfile  << " " << y << endl;
//         i++;
//         x = x + 2;
//         y = x + 1;
//     }
//     myfile.close();
//     return 0;
// }

            // ofstream myfile;
            // myfile.open ("three_dim_charge_plot.txt");
            // myfile << pmtX, pmtY, pmtZ, charge;
            // myfile.close();

          // cout << "Y value: " << pmtY << endl;

            // coor_q->Fill(pmtX, pmtY, pmtZ, charge);

          } // END FOR RAW HITS
          // myfile.close();
          xfile.close();
          yfile.close();
          zfile.close();
          qfile.close();


    } // END FOR iTRIG

  } // END FOR iENTRY


//    TH1 *temp;
      //coor_q->Draw("colz");




  }
