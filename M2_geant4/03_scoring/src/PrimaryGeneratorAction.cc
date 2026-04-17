#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  fGun = new G4ParticleGun(1);

  auto* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fGun->SetParticleDefinition(particle);
  fGun->SetParticleEnergy(1.0 * GeV);

  // Il fascio parte prima del calorimetro e viaggia lungo +z.
  fGun->SetParticlePosition({0., 0., -25. * cm});
  fGun->SetParticleMomentumDirection({0., 0., 1.});
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  fGun->GeneratePrimaryVertex(event);
}