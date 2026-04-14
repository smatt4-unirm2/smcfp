#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "TRandom3.h"
#include "TFile.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TTree.h"

int main() {
    constexpr int N_EVENTS = 100000;
    constexpr double LAMBDA = 2.5;
    constexpr int SEED = 2024;

    // Generatore ROOT
    TRandom3 rng(SEED);

    // Per TRandom3::Exp(tau) vale tau = 1/lambda
    const double tau = 1.0 / LAMBDA;

    // TODO:
    // Definire un istogramma ROOT per la distribuzione esponenziale

    // TODO:
    // Definire un TTree per salvare i valori generati

    std::vector<double> values;
    values.reserve(N_EVENTS);

    // TODO:
    // Variabile da collegare al branch del TTree
    double x = 0.0;

    for (int i = 0; i < N_EVENTS; ++i) {

        // TODO:
        // Generare un numero casuale esponenziale con TRandom3
        // usando il metodo Exp(tau)
        x = 0.0;

        values.push_back(x);

        // TODO:
        // Riempire istogramma e TTree
    }

    std::sort(values.begin(), values.end());

    // TODO:
    // Costruire i vettori per la CDF empirica e teorica

    // TODO:
    // Creare i TGraph:
    // - g_cdf_empirica
    // - g_cdf_teorica

    // TODO:
    // Definire una funzione TF1 per il fit esponenziale

    TFile fout("output_exponential.root", "RECREATE");
    if (fout.IsZombie()) {
        std::cerr << "Errore nell'apertura del file output_exponential.root\n";
        return 1;
    }

    // TODO:
    // Scrivere tutti gli oggetti nel file ROOT

    fout.Close();
    return 0;
}
