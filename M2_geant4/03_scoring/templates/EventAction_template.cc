#include "EventAction.hh"

#include "RunAction.hh"
#include "G4Event.hh"

EventAction::EventAction(RunAction* runAction)
  : fRunAction(runAction)
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  // TODO:
  // azzerare qui tutti gli accumulatori dell'evento
}

void EventAction::EndOfEventAction(const G4Event*)
{
  // TODO:
  // passare qui i dati evento per evento alla RunAction
  // includendo i dati del generatore
}

void EventAction::AddEdep(G4int, G4int, G4double)
{
  // TODO:
  // sommare l'energia depositata nel cristallo corretto
}
