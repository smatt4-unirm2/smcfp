#include "RunAction.hh"

#include "EventAction.hh"

#include "G4Run.hh"
#include "G4ios.hh"

#include "TFile.h"
#include "TTree.h"

RunAction::RunAction() = default;

RunAction::~RunAction()
{
  // TODO:
  // chiudere o distruggere qui gli oggetti ROOT se necessario
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4cout << "### Run start" << G4endl;

  // TODO:
  // aprire il file ROOT e creare il TTree
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4cout << "### Run end" << G4endl;

  // TODO:
  // scrivere il TTree e chiudere il file ROOT
}

void RunAction::FillEvent(const EventAction&)
{
  // TODO:
  // copiare qui le variabili dell'evento nei buffer del tree e chiamare Fill()
}
