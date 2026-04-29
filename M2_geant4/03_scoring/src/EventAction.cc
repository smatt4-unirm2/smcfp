#include "EventAction.hh"
#include "RunAction.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ThreeVector.hh"
#include "G4Event.hh"

EventAction::EventAction(RunAction* runAction)
  : fRunAction(runAction)
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  // All'inizio di ogni evento azzeriamo l'accumulatore dell'energia
  // depositata e le variabili del primario.
  for (G4int iPlane = 0; iPlane < kNPlanes; ++iPlane) {
    for (G4int iBar = 0; iBar < kNBars; ++iBar) {
      fEdep[iPlane][iBar] = 0.0;
    }
  }
}

void EventAction::EndOfEventAction(const G4Event* event)
{
  G4double primaryEnergy = 0.0;
  G4double primaryX0 = 0.0;
  G4double primaryY0 = 0.0;
  G4double primaryZ0 = 0.0;
  G4double primaryTheta = 0.0;
  G4double primaryPhi = 0.0;

  auto* vertex = event->GetPrimaryVertex(0);
  if (vertex != nullptr) {
    auto* primary = vertex->GetPrimary();
    if (primary != nullptr) {
      primaryEnergy = primary->GetKineticEnergy();
      primaryX0 = vertex->GetX0();
      primaryY0 = vertex->GetY0();
      primaryZ0 = vertex->GetZ0();

      G4ThreeVector direction(primary->GetPx(),
                              primary->GetPy(),
                              primary->GetPz());

      primaryTheta = direction.theta();
      primaryPhi   = direction.phi();
    }
  }

  fRunAction->FillEvent(*this,
                        primaryEnergy,
                        primaryX0,
                        primaryY0,
                        primaryZ0,
                        primaryTheta,
                        primaryPhi);
}

void EventAction::AddEdep(G4int plane, G4int crystal, G4double edep)
{
  if (plane < 0 || plane >= kNPlanes) {
    return;
  }

  if (crystal < 0 || crystal >= kNBars) {
    return;
  }

  fEdep[plane][crystal] += edep;
}