#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"

RunAction::RunAction()
{
  auto* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetFileName("scoring");

  analysisManager->CreateNtuple("t", "Event-by-event scoring in the absorber block");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleDColumn("primaryE_MeV");
  analysisManager->CreateNtupleDColumn("edep_MeV");
  analysisManager->CreateNtupleDColumn("edepHad_MeV");
  analysisManager->CreateNtupleDColumn("edepEle_MeV");
  analysisManager->CreateNtupleDColumn("edepGam_MeV");
  analysisManager->CreateNtupleDColumn("trackLength_mm");
  analysisManager->CreateNtupleIColumn("nStepsAbs");
  analysisManager->CreateNtupleIColumn("nSecondaries");
  analysisManager->CreateNtupleDColumn("maxStepEdep_MeV");
  analysisManager->FinishNtuple();
}

RunAction::~RunAction(){}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4cout << "### Run start" << G4endl;
  G4AnalysisManager::Instance()->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  G4cout << "### Run end" << G4endl;
  G4cout << "Output ROOT scritto in: scoring.root" << G4endl;
}
