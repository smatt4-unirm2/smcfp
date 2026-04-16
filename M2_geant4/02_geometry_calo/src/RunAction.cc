#include "RunAction.hh"

#include "G4Run.hh"
#include "G4ios.hh"

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4cout << "### Run start" << G4endl;
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4cout << "### Run end" << G4endl;
}
