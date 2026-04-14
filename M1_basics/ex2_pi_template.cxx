#include <cmath>
#include <iostream>
#include <vector>

#include "TRandom3.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TFile.h"

int main() {
    constexpr int N_TOTAL = 1000000;
    constexpr int SEED = 12345;

    // Generatore ROOT
    TRandom3 rng(SEED);

    // TODO:
    // Definire due istogrammi TH2D:
    // - h2_in  per i punti dentro il cerchio
    // - h2_out per i punti fuori dal cerchio
    //
    // Esempio:
    // TH2D h2_in("h2_in", "Punti dentro; x; y", 200, -1., 1., 200, -1., 1.);

    // TODO:
    // Definire tre vettori std::vector<double> per salvare:
    // - N
    // - pi stimato
    // - differenza relativa

    int nInside = 0;

    for (int i = 1; i <= N_TOTAL; ++i) {

        // TODO:
        // Generare x e y uniformi in [-1, 1] usando TRandom3
        double x = 0.0;
        double y = 0.0;

        // TODO:
        // Verificare se il punto è dentro il cerchio
        if (x * x + y * y <= 1.0) {
            ++nInside;

            // TODO:
            // Riempire h2_in
        } else {
            // TODO:
            // Riempire h2_out
        }

        // TODO:
        // Ogni certo numero di eventi, calcolare:
        // pi_est = 4.0 * nInside / i
        // diff_rel = |pi_est - pi_vero| / pi_vero
        // e salvarli nei vettori
    }

    // TODO:
    // Calcolare la stima finale di pi
    double pi_est = 0.0;

    std::cout << "Pi stimato = " << pi_est << std::endl;

    // TODO:
    // Costruire i grafici TGraph:
    // - g_pi_stimato
    // - g_convergenza_pi

    TFile fout("output_pi.root", "RECREATE");
    if (fout.IsZombie()) {
        std::cerr << "Errore nell'apertura del file output_pi.root\n";
        return 1;
    }

    // TODO:
    // Scrivere istogrammi e grafici nel file ROOT

    fout.Close();
    return 0;
}
