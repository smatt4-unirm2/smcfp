#include "PrimaryGeneratorAction.hh"

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(EventAction* eventAction)
  : fEventAction(eventAction)
{
  // TODO:
  // 1) creare il GPS
  // 2) definire un default sensato (particella, energia, posizione, direzione)
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fGPS;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // TODO:
  // 1) generare il vertice primario con il GPS
  // 2) leggere dall'evento i parametri effettivamente generati
  // 3) salvare questi parametri nell'EventAction
}
