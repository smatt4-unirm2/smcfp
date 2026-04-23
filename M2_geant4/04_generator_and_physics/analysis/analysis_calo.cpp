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
                         "Energia totale depositata per evento;E_{tot} [MeV];Eventi",
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

  TCanvas* cTot = new TCanvas("cTot","Energia totale",700,500);
  hTotE->Draw();

  // =========================
  // 3) ENERGIA MEDIA PER PIANO
  // =========================
  TH1D* hPlane = new TH1D("hPlane",
                          "Energia media depositata per piano;Piano;E media [MeV]",
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
  TH1D* hSigmaAll = new TH1D("hSigmaAll",
                             "Larghezza laterale RMS;#sigma_{lat} [indice cristallo];Piani-evento",
                             100,0,5);

  TH1D* hNhits = new TH1D("hNhits",
                          "Numero cristalli attivi per piano;N_{hit};Piani-evento",
                          17,-0.5,16.5);

  TH1D* hSigmaVsPlane = new TH1D("hSigmaVsPlane",
                                 "Larghezza laterale media per piano;Piano;#LT#sigma_{lat}#GT",
                                 nPlanes,0,nPlanes);

  TH1D* hNhitsVsPlane = new TH1D("hNhitsVsPlane",
                                 "Numero medio di cristalli attivi per piano;Piano;#LTN_{hit}#GT",
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

      hSigmaAll->Fill(sigma);
      hNhits->Fill(nhit);

      hSigmaVsPlane->Fill(p, sigma);
      hNhitsVsPlane->Fill(p, nhit);
    }
  }

  hSigmaVsPlane->Scale(1.0/nentries);
  hNhitsVsPlane->Scale(1.0/nentries);

  TCanvas* cLat = new TCanvas("cLat","Distribuzione laterale sciame",1200,800);
  cLat->Divide(2,2);

  cLat->cd(1);
  hSigmaAll->Draw();

  cLat->cd(2);
  hNhits->Draw();

  cLat->cd(3);
  hSigmaVsPlane->Draw("HIST");

  cLat->cd(4);
  hNhitsVsPlane->Draw("HIST");

  // =========================
  // 5) MAPPE 2D SEPARATE: PIANI PARI / PIANI DISPARI
  // =========================
  TH2D* hMapEven = new TH2D("hMapEven",
                            "Energia media per cristallo - piani pari;Bar;Indice piano pari",
                            nBars,0,nBars,nPlanes/2,0,nPlanes/2);

  TH2D* hMapOdd = new TH2D("hMapOdd",
                           "Energia media per cristallo - piani dispari;Bar;Indice piano dispari",
                           nBars,0,nBars,nPlanes/2,0,nPlanes/2);

  for (Long64_t i=0; i<nentries; i++) {
    t->GetEntry(i);

    for (int p=0; p<nPlanes; p++) {
      for (int b=0; b<nBars; b++) {
        if (p % 2 == 0) {
          hMapEven->Fill(b, p/2, edep[p][b]);
        } else {
          hMapOdd->Fill(b, p/2, edep[p][b]);
        }
      }
    }
  }

  hMapEven->Scale(1.0/nentries);
  hMapOdd->Scale(1.0/nentries);

  TCanvas* cMap2 = new TCanvas("cMap2","Mappa calorimetro: pari/dispari",1200,500);
  cMap2->Divide(2,1);

  cMap2->cd(1);
  hMapEven->Draw("COLZ");

  cMap2->cd(2);
  hMapOdd->Draw("COLZ");

  // =========================
  // 6) VISUALIZZAZIONE 3D SEMPLICE
  // =========================
  TH3D* h3 = new TH3D("h3",
                      "Energia media depositata;Bar;Piano;Energia [MeV]",
                      nBars,0,nBars,nPlanes,0,nPlanes,100,0,200);

  for (Long64_t i=0; i<nentries; i++) {
    t->GetEntry(i);

    for (int p=0; p<nPlanes; p++) {
      for (int b=0; b<nBars; b++) {
        h3->Fill(b, p, edep[p][b]);
      }
    }
  }

  h3->Scale(1.0/nentries);

  TCanvas* c3D = new TCanvas("c3D","Visualizzazione 3D",800,600);
  h3->Draw("LEGO2");

  std::cout << "Analisi completata." << std::endl;
}
