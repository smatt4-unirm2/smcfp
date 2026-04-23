#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"

// TODO:
// Inserire qui gli #include necessari per il generatore scelto.
//
// Esempi:
// #include "G4GeneralParticleSource.hh"
// #include "G4ParticleGun.hh"
// #include "G4ParticleTable.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4ThreeVector.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  // TODO:
  // Costruire qui il generatore scelto.
  //
  // Esempi:
  // fGPS = new G4GeneralParticleSource();
  // fGun = new G4ParticleGun(1);

  // TODO:
  // Impostare qui una configurazione di default minimale,
  // se si vuole avere una sorgente già pronta senza macro.
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  // TODO:
  // Deallocare qui il generatore.
  //
  // Esempi:
  // delete fGPS;
  // delete fGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // TODO:
  // Generare qui il vertice primario dell'evento.
  //
  // Esempi:
  // fGPS->GeneratePrimaryVertex(event);
  // fGun->GeneratePrimaryVertex(event);

}
