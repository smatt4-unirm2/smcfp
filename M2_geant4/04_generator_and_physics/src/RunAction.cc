#include "RunAction.hh"

#include "EventAction.hh"

#include "G4GenericMessenger.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "TFile.h"
#include "TTree.h"

RunAction::RunAction()
{
  // --------------------------------------------------------------------------
  // UIMessenger minimale per il nome del file di output.
  // --------------------------------------------------------------------------
  // Esempio d'uso da macro:
  //   /analysis/setFileName prova.root
  //
  // In assenza del comando si usa il fallback definito in fOutputFileName.
  // --------------------------------------------------------------------------
  fMessenger = new G4GenericMessenger(this, "/analysis/", "Analysis control");
  fMessenger->DeclareMethod("setFileName", &RunAction::SetOutputFileName,
                            "Set ROOT output file name.");
}

RunAction::~RunAction()
{
  delete fMessenger;
  delete fOutputFile;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  Book();
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4cout << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << ">>> End of run" << G4endl;
  G4cout << ">>> Writing and closing output file: " << fOutputFileName << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << G4endl;

  if (fOutputFile != nullptr) {
    fOutputFile->cd();
    fTree->Write();
    fOutputFile->Close();
  }
}

void RunAction::FillEvent(const EventAction& eventAction,
                          G4double primaryEnergy,
                          G4double primaryX0,
                          G4double primaryY0,
                          G4double primaryZ0,
                          G4double primaryTheta,
                          G4double primaryPhi)
{
  ResetBranches();

  const auto& edep = eventAction.GetEdep();
  for (G4int iPlane = 0; iPlane < kNPlanes; ++iPlane) {
    for (G4int iBar = 0; iBar < kNBars; ++iBar) {
      fEdep[iPlane][iBar] = edep[iPlane][iBar] / MeV;
    }
  }

  fPrimaryEnergy = primaryEnergy / MeV;
  fPrimaryX0     = primaryX0 / mm;
  fPrimaryY0     = primaryY0 / mm;
  fPrimaryZ0     = primaryZ0 / mm;
  fPrimaryTheta  = primaryTheta / deg;
  fPrimaryPhi    = primaryPhi / deg;

  fTree->Fill();
}

void RunAction::Book()
{
  delete fOutputFile;
  fOutputFile = nullptr;

  fOutputFile = TFile::Open(fOutputFileName.c_str(), "RECREATE");

  G4cout << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << ">>> Begin of run" << G4endl;
  G4cout << ">>> Opening output file: " << fOutputFile->GetName() << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << G4endl;

  fTree = new TTree("t", "Event-by-event calorimeter scoring");

  // Energia depositata: Edep[piano][cristallo]
  fTree->Branch("caloEdep", fEdep, "caloEdep[12][16]/D");

  // Variabili del primario effettivamente generate dal GPS.
  fTree->Branch("genEnergy", &fPrimaryEnergy, "genEnergy/D");
  fTree->Branch("genX",      &fPrimaryX0,     "genX/D");
  fTree->Branch("genY",      &fPrimaryY0,     "genY/D");
  fTree->Branch("genZ",      &fPrimaryZ0,     "genZ/D");
  fTree->Branch("genTheta",  &fPrimaryTheta,  "genTheta/D");
  fTree->Branch("genPhi",    &fPrimaryPhi,    "genPhi/D");

  ResetBranches();
}

void RunAction::ResetBranches()
{
  for (G4int iPlane = 0; iPlane < kNPlanes; ++iPlane) {
    for (G4int iBar = 0; iBar < kNBars; ++iBar) {
      fEdep[iPlane][iBar] = 0.0;
    }
  }

  fPrimaryEnergy = 0.0;
  fPrimaryX0 = 0.0;
  fPrimaryY0 = 0.0;
  fPrimaryZ0 = 0.0;
  fPrimaryTheta = 0.0;
  fPrimaryPhi = 0.0;
}
