#include "ActionInitialization.hh"

#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

void ActionInitialization::Build() const
{
  // --------------------------------------------------------------------------
  // Le user action sono organizzate in modo modulare:
  //
  // - RunAction:
  //   gestisce il file ROOT di output e il messenger /analysis/setFileName;
  //
  // - EventAction:
  //   contiene gli accumulatori evento per evento e i parametri del primario;
  //
  // - PrimaryGeneratorAction:
  //   usa il GPS per generare il primario e salvare i parametri realmente
  //   estratti nel singolo evento;
  //
  // - SteppingAction:
  //   esegue lo scoring dell'energia depositata nei cristalli.
  // --------------------------------------------------------------------------
  auto* runAction = new RunAction();
  SetUserAction(runAction);

  auto* eventAction = new EventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new PrimaryGeneratorAction(eventAction));
  SetUserAction(new SteppingAction(eventAction));
}
