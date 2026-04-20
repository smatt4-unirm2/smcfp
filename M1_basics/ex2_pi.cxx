#include <cmath>
#include <iostream>
#include <vector>

#include "TRandom3.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2D.h"
#include "TLine.h"
#include "TMath.h"
#include "TStyle.h"

void ex2_pi(){
    int N_TOTAL = 1000000;
    int N_SCATTER_SAVE = 50000;
    int SEED = 12345;

    TRandom3 rng(SEED);

    TH2D h2_in("h2_in", "Punti dentro il cerchio; x; y", 200, -1., 1., 200, -1., 1.);
    TH2D h2_out("h2_out", "Punti fuori dal cerchio; x; y", 200, -1., 1., 200, -1., 1.);
    h2_in.SetDirectory(nullptr);
    h2_out.SetDirectory(nullptr);

    std::vector<double> vN;
    std::vector<double> vPi;
    std::vector<double> vRelDiff;
    vN.reserve(N_TOTAL / 1000 + 1);
    vPi.reserve(N_TOTAL / 1000 + 1);
    vRelDiff.reserve(N_TOTAL / 1000 + 1);

    int nInside = 0;

    for (int i = 1; i <= N_TOTAL; ++i) {
        double x = rng.Uniform(-1.0, 1.0);
        double y = rng.Uniform(-1.0, 1.0);
        double r2 = x * x + y * y;

        if (r2 <= 1.0) {
            ++nInside;
            if (i <= N_SCATTER_SAVE) h2_in.Fill(x, y);
        } else {
            if (i <= N_SCATTER_SAVE) h2_out.Fill(x, y);
        }

        if (i >= 100 && (i % 1000 == 0 || i == N_TOTAL)) {
            double pi_est = 4.0 * static_cast<double>(nInside) / static_cast<double>(i);
            double diff_rel = std::abs(pi_est - TMath::Pi()) / TMath::Pi();

            vN.push_back(static_cast<double>(i));
            vPi.push_back(pi_est);
            vRelDiff.push_back(diff_rel);
        }
    }

    double pi_est = 4.0 * static_cast<double>(nInside) / static_cast<double>(N_TOTAL);

    std::cout << "=== Esercizio 2: Stima di pi ===\n";
    std::cout << "Punti totali         : " << N_TOTAL << "\n";
    std::cout << "Punti dentro cerchio : " << nInside << "\n";
    std::cout << "Pi stimato           : " << pi_est << "\n";
    std::cout << "Pi vero              : " << TMath::Pi() << "\n";
    std::cout << "Diff. relativa       : " << std::abs(pi_est - TMath::Pi()) / TMath::Pi() << "\n";

    TGraph g_pi_stimato(static_cast<int>(vN.size()), vN.data(), vPi.data());
    g_pi_stimato.SetName("g_pi_stimato");
    g_pi_stimato.SetTitle("Valore stimato di #pi;N;#pi_{stimato}");
    g_pi_stimato.SetLineColor(kBlue + 2);
    g_pi_stimato.SetLineWidth(2);

    TGraph g_convergenza_pi(static_cast<int>(vN.size()), vN.data(), vRelDiff.data());
    g_convergenza_pi.SetName("g_convergenza_pi");
    g_convergenza_pi.SetTitle("Differenza relativa sulla stima di #pi;N;|#pi_{stim}-#pi|/#pi");
    g_convergenza_pi.SetLineColor(kRed + 1);
    g_convergenza_pi.SetLineWidth(2);

    gStyle->SetOptStat(0);

    TFile fout("output_pi.root", "RECREATE");
    if (fout.IsZombie()) {
        std::cerr << "Errore nell'apertura del file output_pi.root\n";
        return;
    }

    TCanvas c_scatter("c_scatter_pi", "Punti nel quadrato", 900, 800);
    h2_out.SetMarkerStyle(6);
    h2_out.SetMarkerColor(kGray + 1);
    h2_out.Draw("SCAT");
    h2_in.SetMarkerStyle(6);
    h2_in.SetMarkerColor(kAzure + 2);
    h2_in.Draw("SCAT SAME");

    TCanvas c_pi("c_pi_stimato", "Stima di pi", 900, 700);
    g_pi_stimato.Draw("AL");
    TLine line_pi(vN.front(), TMath::Pi(), vN.back(), TMath::Pi());
    line_pi.SetLineStyle(2);
    line_pi.Draw("SAME");

    TCanvas c_conv("c_convergenza_pi", "Convergenza", 900, 700);
    c_conv.SetLogx();
    c_conv.SetLogy();
    g_convergenza_pi.Draw("AL");

    h2_in.Write();
    h2_out.Write();
    g_pi_stimato.Write();
    g_convergenza_pi.Write();
    c_scatter.Write();
    c_pi.Write();
    c_conv.Write();

    fout.Close();
    std::cout << "Output scritto su: output_pi.root, uscita con .q\n";
}
