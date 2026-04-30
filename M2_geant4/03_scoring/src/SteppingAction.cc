#include "SteppingAction.hh"

#include "EventAction.hh"

#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4VPhysicalVolume.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
  : fEventAction(eventAction)
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Consideriamo solo step con energia depositata non nulla.
  const G4double edep = step->GetTotalEnergyDeposit();
  if (edep <= 0.0) {
    return;
  }

  // Usiamo il volume fisico di pre-step per capire in quale volume si trova
  // la particella all'inizio dello step.

  //Ricava il volume dal pre-step-point

  if (volume == nullptr) {
    return;
  }

  // Lo scoring deve essere fatto solo nei cristalli
  // Quindi mettiamo la condizione di essere nel cristallo

  // Grazie alla gerarchia della geometria:
  //   depth 0 -> cristallo corrente
  //   depth 1 -> piano che contiene il cristallo
  //   depth 2 -> calorimetro
  // possiamo ricostruire gli indici direttamente dai copy number.
  // poi chiamiamo la funzione per accumulare l'energia depositata
}


