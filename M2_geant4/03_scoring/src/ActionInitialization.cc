#include "ActionInitialization.hh"

#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

void ActionInitialization::Build() const
{
  // Le user action coinvolte nello scoring sono:
  // - PrimaryGeneratorAction: genera il primario e ne salva i parametri;
  // - EventAction: contiene gli accumulatori evento per evento;
  // - SteppingAction: somma l'energia depositata nei cristalli;
  // - RunAction: apre il file ROOT e scrive il TTree.
  auto* runAction = new RunAction();
  SetUserAction(runAction);

  auto* eventAction = new EventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new PrimaryGeneratorAction(eventAction));
  SetUserAction(new SteppingAction(eventAction));
}
