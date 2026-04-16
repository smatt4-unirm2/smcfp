#include "EventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4SystemOfUnits.hh"

void EventAction::BeginOfEventAction(const G4Event* event)
{
  fPrimaryEnergy = 0.0;
  fTotalEdep = 0.0;
  fEdepHad = 0.;
  fEdepEle = 0.;
  fEdepGam = 0.;
  fTotalTrackLength = 0.0;
  fMaxStepEdep = 0.0;
  fNStepsInAbsorber = 0;
  fNSecondaries = 0;

  if (event->GetNumberOfPrimaryVertex() > 0) {
    auto* vertex = event->GetPrimaryVertex(0);
    if (vertex && vertex->GetNumberOfParticle() > 0) {
      auto* primary = vertex->GetPrimary(0);
      if (primary) {
        fPrimaryEnergy = primary->GetKineticEnergy();
      }
    }
  }
}

void EventAction::EndOfEventAction(const G4Event* event)
{
  auto* analysisManager = G4AnalysisManager::Instance();

  analysisManager->FillNtupleIColumn(0, event->GetEventID());
  analysisManager->FillNtupleDColumn(1, fPrimaryEnergy / MeV);
  analysisManager->FillNtupleDColumn(2, fTotalEdep / MeV);
  analysisManager->FillNtupleDColumn(3, fEdepHad / MeV);
  analysisManager->FillNtupleDColumn(4, fEdepEle / MeV);
  analysisManager->FillNtupleDColumn(5, fEdepGam / MeV);
  analysisManager->FillNtupleDColumn(6, fTotalTrackLength / mm);
  analysisManager->FillNtupleIColumn(7, fNStepsInAbsorber);
  analysisManager->FillNtupleIColumn(8, fNSecondaries);
  analysisManager->FillNtupleDColumn(9, fMaxStepEdep / MeV);
  analysisManager->AddNtupleRow();
}

void EventAction::UpdateMaxStepEdep(G4double value)
{
  if (value > fMaxStepEdep) {
    fMaxStepEdep = value;
  }
}
