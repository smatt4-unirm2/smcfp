#include <TFile.h>
#include <TH1.h>
#include <iostream>
#include <fstream>

void convert_root_hist_to_gps(const char* inputFile  = "spectrum_hist.root",
                              const char* histName   = "hSpectrum",
                              const char* outputFile = "macro/spectrum_hist.dat")
{
  // --------------------------------------------------------------------------
  // Converte un istogramma ROOT (TH1) in un file ASCII leggibile dal GPS.
  // --------------------------------------------------------------------------
  // Uso:
  // root -l -q 'macro/convert_root_hist_to_gps.C("spectrum_hist.root", "hSpectrum", "macro/spectrum_hist.dat")'
  // --------------------------------------------------------------------------

  TFile file(inputFile, "READ");
  if (file.IsZombie()) {
    std::cerr << "Cannot open input file: " << inputFile << std::endl;
    return;
  }

  auto* h = dynamic_cast<TH1*>(file.Get(histName));
  if (!h) {
    std::cerr << "Cannot find histogram: " << histName << std::endl;
    return;
  }

  std::ofstream out(outputFile);
  if (!out) {
    std::cerr << "Cannot open output file: " << outputFile << std::endl;
    return;
  }

  // Per /gps/hist/file servono coppie: ascissa peso.
  // Per semplicità usiamo il centro bin come ascissa e il contenuto bin come peso.
  for (int i = 1; i <= h->GetNbinsX(); ++i) {
    const double x = h->GetBinCenter(i);      // in unità Geant4 di default per energia: MeV
    const double y = h->GetBinContent(i);
    if (y > 0.0) {
      out << x << " " << y << "\n";
    }
  }

  std::cout << "Written GPS histogram file: " << outputFile << std::endl;
}
