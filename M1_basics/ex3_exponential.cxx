#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "TRandom3.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TTree.h"

void ex3_exponential() {
    int N_EVENTS = 100000;
    double LAMBDA = 2.5;
    int SEED = 2024;

    TRandom3 rng(SEED);
    const double tau = 1.0 / LAMBDA;

    TH1D h_expo("h_expo", "Distribuzione esponenziale; x; PDF", 100, 0.0, 3.0);
    h_expo.SetDirectory(nullptr);

    TFile fout("output_exponential.root", "RECREATE");
    if (fout.IsZombie()) {
        std::cerr << "Errore nell'apertura del file output_exponential.root\n";
        return;
    }

    std::vector<double> values;
    values.reserve(N_EVENTS);

    for (int i = 0; i < N_EVENTS; ++i) {
        double u = rng.Uniform(1e-10, 1.);
        double x=-1/LAMBDA*TMath::Log(u);
        values.push_back(x);
        h_expo.Fill(x);
    }

    std::sort(values.begin(), values.end());

    std::vector<double> xvals(values.size());
    std::vector<double> y_emp(values.size());
    std::vector<double> y_theo(values.size());

    for (std::size_t i = 0; i < values.size(); ++i) {
        xvals[i] = values[i];
        y_emp[i] = static_cast<double>(i + 1) / static_cast<double>(values.size());
        y_theo[i] = 1.0 - std::exp(-LAMBDA * values[i]);
    }

    TGraph g_cdf_empirica(static_cast<int>(xvals.size()), xvals.data(), y_emp.data());
    g_cdf_empirica.SetName("g_cdf_empirica");
    g_cdf_empirica.SetTitle("CDF empirica; x; F(x)");
    g_cdf_empirica.SetLineColor(kBlue + 2);
    g_cdf_empirica.SetLineWidth(2);

    TGraph g_cdf_teorica(static_cast<int>(xvals.size()), xvals.data(), y_theo.data());
    g_cdf_teorica.SetName("g_cdf_teorica");
    g_cdf_teorica.SetTitle("CDF teorica; x; F(x)");
    g_cdf_teorica.SetLineColor(kRed + 1);
    g_cdf_teorica.SetLineWidth(2);

    h_expo.Scale(1.0 / (h_expo.GetEntries() * h_expo.GetBinWidth(1)));

    TF1 f_exp("f_exp", "[0]*exp(-[1]*x)", 0.0, 3.0);
    f_exp.SetParameters(LAMBDA, LAMBDA);
    f_exp.SetParNames("A", "lambda");

    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(1111);

    TCanvas c_expo_fit("c_expo_fit", "Fit esponenziale", 900, 700);
    h_expo.SetLineColor(kBlue + 2);
    h_expo.SetLineWidth(2);
    h_expo.Draw("E");
    h_expo.Fit(&f_exp, "R");

    TCanvas c_cdf("c_cdf", "CDF empirica vs teorica", 900, 700);
    g_cdf_empirica.Draw("AL");
    g_cdf_teorica.Draw("L SAME");

    h_expo.Write();
    g_cdf_empirica.Write();
    g_cdf_teorica.Write();
    c_expo_fit.Write();
    c_cdf.Write();

    fout.Close();

    std::cout << "=== Esercizio 3: Distribuzione esponenziale ===\n";
    std::cout << "Eventi generati : " << N_EVENTS << "\n";
    std::cout << "Media campionata: " << h_expo.GetMean() << " (atteso: " << 1.0 / LAMBDA << ")\n";
    std::cout << "Lambda atteso   : " << LAMBDA << "\n";
    std::cout << "Output scritto su: output_exponential.root\n";
    return;
}
