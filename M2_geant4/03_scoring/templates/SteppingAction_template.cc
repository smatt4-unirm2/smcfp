#include "SteppingAction.hh"

#include "EventAction.hh"

#include "G4Step.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
  : fEventAction(eventAction)
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // TODO:
  // 1) leggere l'energia depositata nello step
  // 2) scartare gli step con edep nulla
  // 3) identificare il volume corrente
  // 4) verificare che il volume sia un cristallo
  // 5) ricavare piano e cristallo dai copy number
  // 6) chiamare EventAction::AddEdep(...)
}
