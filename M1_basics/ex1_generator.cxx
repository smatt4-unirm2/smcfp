#include <cmath>
#include <iostream>
#include <iomanip>

#include "TRandom3.h"
#include "TRandomGen.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TStyle.h"

int main() {
    constexpr int N_EVENTS = 100000;
    constexpr unsigned int SEED = 42;

    TRandom3 rng3(SEED);
    TRandomRanlux48 ranlux48(SEED);

    TH1D *h_uniform_r3 = new TH1D("h_uniform_r3", "Uniforme U(0,1) con TRandom3;x;Conteggi", 100, -0.1, 1.1);
    TH1D *h_uniform_ranlux48 = new TH1D("h_uniform_ranlux48", "Uniforme U(0,1) con RanLux48;x;Conteggi", 100, -0.1, 1.1);

    TH1D *h_gauss_r3 = new TH1D("h_gauss_r3", "Gaussiana N(0,1) con TRandom3;x;Conteggi", 120, -5.0, 5.0);
    TH1D *h_gauss_ranlux48 = new TH1D("h_gauss_ranlux48", "Gaussiana N(0,1) con RanLux48;x;Conteggi", 120, -5.0, 5.0);

    TH2D *h2_corr = new TH2D("h2_corr", "Gaussian Correlation Plot; RanLux48; TRandom3", 120, -5, 5, 120, -5, 5); 


    for (int i = 0; i < N_EVENTS; ++i) {
        h_uniform_r3->Fill(rng3.Uniform(0.0, 1.0));
        h_uniform_ranlux48->Fill(ranlux48.Rndm());

        double gr3 = rng3.Gaus(0.0, 1.0);
        double grl = ranlux48.Gaus(0.0, 1.0);
        h_gauss_r3->Fill(gr3);
        h_gauss_ranlux48->Fill(grl);
        h2_corr->Fill(grl, gr3);
    }

    std::cout << "=== Esercizio 1: generatori pseudo-random ROOT ===\n";
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\n[Uniforme con TRandom3]\n";
    std::cout << "Media = " << h_uniform_r3->GetMean() << " (atteso: 0.5)\n";
    std::cout << "RMS   = " << h_uniform_r3->GetRMS()  << " (atteso: 0.288675)\n";

    std::cout << "\n[Uniforme con RanLux48]\n";
    std::cout << "Media = " << h_uniform_ranlux48->GetMean() << " (atteso: 0.5)\n";
    std::cout << "RMS   = " << h_uniform_ranlux48->GetRMS()  << " (atteso: 0.288675)\n";

    std::cout << "\n[Gaussiana con TRandom3]\n";
    std::cout << "Media = " << h_gauss_r3->GetMean() << " (atteso: 0)\n";
    std::cout << "RMS   = " << h_gauss_r3->GetRMS()  << " (atteso: 1)\n";

    std::cout << "\n[Gaussiana con RanLux48]\n";
    std::cout << "Media = " << h_gauss_ranlux48->GetMean() << " (atteso: 0)\n";
    std::cout << "RMS   = " << h_gauss_ranlux48->GetRMS()  << " (atteso: 1)\n";

    gStyle->SetOptStat(1110);

    TFile *fout = new TFile("output_generator.root", "RECREATE");
    if (fout->IsZombie()) {
        std::cerr << "Errore: impossibile aprire output_generator.root\n";
        return 1;
    }

    h_uniform_r3->SetLineColor(kBlue + 2);
    h_uniform_r3->SetLineWidth(2);
    h_uniform_ranlux48->SetLineColor(kRed + 1);
    h_uniform_ranlux48->SetLineWidth(2);

    h_gauss_r3->SetLineColor(kBlue + 2);
    h_gauss_r3->SetLineWidth(2);
    h_gauss_ranlux48->SetLineColor(kRed + 1);
    h_gauss_ranlux48->SetLineWidth(2);

    TCanvas *c_uniform_compare = new TCanvas("c_uniform_compare", "Confronto uniformi", 900, 700);
    h_uniform_r3->Draw("HIST");
    h_uniform_ranlux48->Draw("HIST SAME");
    TLegend *leg_u = new TLegend(0.60, 0.75, 0.88, 0.88);
    leg_u->AddEntry(h_uniform_r3, "TRandom3", "l");
    leg_u->AddEntry(h_uniform_ranlux48, "RanLux48", "l");
    leg_u->Draw();

    TCanvas *c_gauss_compare = new TCanvas("c_gauss_compare", "Confronto gaussiane", 900, 700);
    h_gauss_r3->Draw("HIST");
    h_gauss_ranlux48->Draw("HIST SAME");
    TLegend *leg_g = new TLegend(0.60, 0.75, 0.88, 0.88);
    leg_g->AddEntry(h_gauss_r3, "TRandom3", "l");
    leg_g->AddEntry(h_gauss_ranlux48, "RanLux48", "l");
    leg_g->Draw();


    TCanvas *c_gauss_corr = new TCanvas("c_gauss_corr", "Correlazione gaussiane", 900, 700);
    h2_corr->Draw("colz");


    h_uniform_r3->Write();
    h_uniform_ranlux48->Write();
    h_gauss_r3->Write();
    h_gauss_ranlux48->Write();
    h2_corr->Write();
    c_uniform_compare->Write();
    c_gauss_compare->Write();
    c_gauss_corr->Write();
    
    fout->Close();

    std::cout << "\nOutput scritto su: "<<fout->GetName()<<"\n";
    return 0;
}
