#include "RunAction.hh"

#include "EventAction.hh"

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "TFile.h"
#include "TTree.h"

RunAction::RunAction() = default;

RunAction::~RunAction()
{
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
  G4cout << ">>> End of run " << G4endl;
  G4cout << ">>> Writing and closing output file" << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << G4endl;

  if (fOutputFile != nullptr) {
    fOutputFile->cd();
    fTree->Write();
    fOutputFile->Close();
  }
}

void RunAction::FillEvent(const EventAction& eventAction)
{
  ResetBranches();

  // Copia dell'energia depositata dall'accumulatore evento alle branch del tree.
  const auto& edep = eventAction.GetEdep();
  for (G4int iPlane = 0; iPlane < kNPlanes; ++iPlane) {
    for (G4int iBar = 0; iBar < kNBars; ++iBar) {
      fEdep[iPlane][iBar] = edep[iPlane][iBar] / MeV;
    }
  }

  // Copia delle variabili del primario.
  fPrimaryEnergy = eventAction.GetPrimaryEnergy() / MeV;
  fPrimaryX0     = eventAction.GetPrimaryX0() / mm;
  fPrimaryY0     = eventAction.GetPrimaryY0() / mm;
  fPrimaryZ0     = eventAction.GetPrimaryZ0() / mm;
  fPrimaryTheta  = eventAction.GetPrimaryTheta() / deg;
  fPrimaryPhi    = eventAction.GetPrimaryPhi() / deg;

  fTree->Fill();
}

void RunAction::Book()
{
  // Ricreiamo il file a ogni run.
  delete fOutputFile;
  fOutputFile = TFile::Open("scoring_calo.root", "RECREATE");
  
  G4cout << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << ">>> Begin of run " << G4endl;
  G4cout << ">>> Opening output file: " << fOutputFile->GetName() << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << G4endl;

  fTree = new TTree("t", "Event-by-event calorimeter scoring");

  // Branch 2D: Edep[piano][cristallo]
  // Convenzione:
  //   - piano    = 0,...,11 lungo +z
  //   - cristallo = 0,...,15 lungo +x nei piani X, lungo +y nei piani Y
  fTree->Branch("caloEdep", fEdep, "caloEdep[12][16]/D");

  // Variabili del primario.
  fTree->Branch("genEnergy", &fPrimaryEnergy, "genEnergy/D");
  fTree->Branch("genX",     &fPrimaryX0,     "genX/D");
  fTree->Branch("genY",     &fPrimaryY0,     "genY/D");
  fTree->Branch("genZ",     &fPrimaryZ0,     "genZ/D");
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
