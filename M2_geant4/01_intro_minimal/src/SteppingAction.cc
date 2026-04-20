#include "SteppingAction.hh"

#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
  : fEventAction(eventAction)
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  auto* preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  if (!preVolume || preVolume->GetName() != "Absorber") {
    return;
  }

  const auto edep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edep);
  if (edep > 0.) {
      auto particle = step->GetTrack()->GetDefinition();
      auto name = particle->GetParticleName();
      auto type = particle->GetParticleType();

      if (name == "e-" || name == "e+") {
          fEventAction->AddEdepEle(edep);
      }
      else if (name == "gamma") {
          fEventAction->AddEdepGam(edep);
      }
      else if (type == "baryon" || type == "meson" || type == "nucleus") {
          fEventAction->AddEdepHad(edep);
      }
  }
  fEventAction->UpdateMaxStepEdep(edep);
  fEventAction->AddAbsorberStep();

  const auto* track = step->GetTrack();
  if (track->GetDefinition()->GetPDGCharge() != 0.) {
    fEventAction->AddTrackLength(step->GetStepLength());
  }

  const auto* secondaries = step->GetSecondaryInCurrentStep();
  if (secondaries) {
    fEventAction->AddSecondaries(static_cast<G4int>(secondaries->size()));
  }
}
