#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TStyle.h"
#include "TTree.h"

void analysis_calo(const char* filename="scoring_calo.root")
{
  TFile* f = TFile::Open(filename);
  if (!f || f->IsZombie()) {
    std::cout << "Errore apertura file!" << std::endl;
    return;
  }

  TTree* t = (TTree*)f->Get("t");
  if (!t) {
    std::cout << "Tree 't' non trovato!" << std::endl;
    return;
  }

  const int nPlanes = 12;
  const int nBars   = 16;

  double edep[nPlanes][nBars];

  t->SetBranchAddress("caloEdep", edep);

  // =========================
  // 1) GENERATORE
  // =========================
  TCanvas* cGen = new TCanvas("cGen","Generatore",1200,800);
  cGen->Divide(2,2);

  cGen->cd(1);
  t->Draw("genEnergy>>hE(500,0,2000)");

  cGen->cd(2);
  t->Draw("genY:genX>>hXY(200,-10,10,200,-10,10)","","COLZ");

  cGen->cd(3);
  t->Draw("genTheta>>hTheta(1000,0,190)");

  cGen->cd(4);
  t->Draw("genPhi>>hPhi(1000,-190,190)");

  // =========================
  // 2) ENERGIA TOTALE DEPOSITATA
  // =========================
  TH1D* hTotE = new TH1D("hTotE",
                         "Total Energy Deposit;E_{tot} [MeV]; Counts [-]",
                         100,0,2000);

  Long64_t nentries = t->GetEntries();

  for (Long64_t i=0; i<nentries; i++) {
    t->GetEntry(i);

    double Etot = 0.0;

    for (int p=0; p<nPlanes; p++) {
      for (int b=0; b<nBars; b++) {
        Etot += edep[p][b];
      }
    }

    hTotE->Fill(Etot);
  }

  TCanvas* cTot = new TCanvas("cTot","Total Edep",700,500);
  hTotE->Draw();

  // =========================
  // 3) ENERGIA MEDIA PER PIANO
  // =========================
  TH1D* hPlane = new TH1D("hPlane",
                          "Average Energy Deposit per Plane;Plane;#LTE#GT [MeV]",
                          nPlanes,0,nPlanes);

  for (Long64_t i=0; i<nentries; i++) {
    t->GetEntry(i);

    for (int p=0; p<nPlanes; p++) {
      double Eplane = 0.0;

      for (int b=0; b<nBars; b++) {
        Eplane += edep[p][b];
      }

      hPlane->Fill(p, Eplane);
    }
  }

  hPlane->Scale(1.0/nentries);

  TCanvas* cPlane = new TCanvas("cPlane","Energia per piano",700,500);
  hPlane->Draw("HIST");

  // =========================
  // 4) DISTRIBUZIONE LATERALE DELLO SCIAME
  // =========================
  TH1D* hSigmaVsPlane = new TH1D("hSigmaVsPlane",
                                 "Average Width per Plane;Plane;#LT#sigma_{lat}#GT",
                                 nPlanes,0,nPlanes);

  TH1D* hNhitsVsPlane = new TH1D("hNhitsVsPlane",
                                 "Average crystals hit per plane;Plane;#LTN_{hit}#GT",
                                 nPlanes,0,nPlanes);

  const double threshold = 0.1; // MeV

  for (Long64_t i=0; i<nentries; i++) {
    t->GetEntry(i);

    for (int p=0; p<nPlanes; p++) {
      double sumE = 0.0;
      double mean = 0.0;
      int nhit = 0;

      for (int b=0; b<nBars; b++) {
        double e = edep[p][b];
        sumE += e;
        mean += b * e;

        if (e > threshold) nhit++;
      }

      if (sumE <= 0.0) continue;

      mean /= sumE;

      double var = 0.0;
      for (int b=0; b<nBars; b++) {
        double e = edep[p][b];
        var += e * (b - mean) * (b - mean);
      }

      var /= sumE;
      double sigma = std::sqrt(var);

      hSigmaVsPlane->Fill(p, sigma);
      hNhitsVsPlane->Fill(p, nhit);
    }
  }

  hSigmaVsPlane->Scale(1.0/nentries);
  hNhitsVsPlane->Scale(1.0/nentries);

  // =========================
  // MAPPE 2D SEPARATE: PIANI PARI / PIANI DISPARI
  // =========================
  TH2D* hMapEven = new TH2D("hMapEven",
                            "Average xtal energy - even planes (Y);Bar;Plane",
                            nBars,0,nBars,nPlanes,0,nPlanes);

  TH2D* hMapOdd = new TH2D("hMapOdd",
                           "Average xtal energy - odd planes (X);Bar;Plane",
                           nBars,0,nBars,nPlanes,0,nPlanes);

 
  double avgE[nPlanes][nBars] = {0.0};
  for (Long64_t i=0; i<nentries; i++) {
    t->GetEntry(i);

    for (int p=0; p<nPlanes; p++) {
      for (int b=0; b<nBars; b++) {
        avgE[p][b] += edep[p][b];
        if (p % 2 == 0) {
          hMapEven->Fill(b, p, edep[p][b]);
        } else {
          hMapOdd->Fill(b, p, edep[p][b]);
        }
      }
    }
  }

  hMapEven->Scale(1.0/nentries);
  hMapOdd->Scale(1.0/nentries);


  TCanvas* cLat = new TCanvas("cLat","Shower Lateral Distribution",1200,800);
  cLat->Divide(2,2);

  cLat->cd(1);
  gPad->SetLogz();
  hMapEven->Draw("COLZ");

  cLat->cd(2);
  gPad->SetLogz();
  hMapOdd->Draw("COLZ");

  cLat->cd(3);
  hSigmaVsPlane->Draw("HIST");

  cLat->cd(4);
  hNhitsVsPlane->Draw("HIST");


  // =========================
  // 6) PSEUDO-RICOSTRUZIONE 3D
  // =========================
  TH3D* h3_voxel = new TH3D("h3_voxel",
    "3D shower reconstruction;Plane;X;Y",
    nPlanes,0,nPlanes,
    nBars,0,nBars,
    nBars,0,nBars);

  for (int p=0; p<nPlanes-1; p++) {

    if (p % 2 != 0) continue; // usa solo coppie (pari, dispari)

    for (int by=0; by<nBars; by++) {
      double Ey = avgE[p][by];
      if (Ey <= 0) continue;

      for (int bz=0; bz<nBars; bz++) {
        double Ez = avgE[p+1][bz];
        if (Ez <= 0) continue;

        double E3 = Ey * Ez; // combinazione semplice

        h3_voxel->Fill(p, bz, by, E3);
      }
    }
  }

  TCanvas* c3D_C = new TCanvas("c3D_C","Sciame 3D",900,700);
  h3_voxel->Draw("BOX2Z");

  std::cout << "Analisi completata." << std::endl;
}
