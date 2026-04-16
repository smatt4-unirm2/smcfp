#include "EventAction.hh"

#include "RunAction.hh"

#include "G4Event.hh"

EventAction::EventAction(RunAction* runAction)
  : fRunAction(runAction)
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  // All'inizio di ogni evento azzeriamo l'accumulatore dell'energia
  // depositata e le variabili del primario.
  Reset();
}

void EventAction::EndOfEventAction(const G4Event*)
{
  // Alla fine dell'evento passiamo i dati accumulati alla RunAction,
  // che li scriverà nel TTree ROOT.
  fRunAction->FillEvent(*this);
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

void EventAction::SetPrimary(G4double energy,
                             G4double x0,
                             G4double y0,
                             G4double z0,
                             G4double theta,
                             G4double phi)
{
  fPrimaryEnergy = energy;
  fPrimaryX0 = x0;
  fPrimaryY0 = y0;
  fPrimaryZ0 = z0;
  fPrimaryTheta = theta;
  fPrimaryPhi = phi;
}

void EventAction::Reset()
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
