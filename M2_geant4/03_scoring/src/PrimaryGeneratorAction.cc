#include "PrimaryGeneratorAction.hh"

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction(EventAction* eventAction)
  : fEventAction(eventAction)
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
  // Salviamo nel contenitore dell'evento le informazioni del primario.
  const G4double energy = fGun->GetParticleEnergy();
  const G4ThreeVector position = fGun->GetParticlePosition();
  const G4ThreeVector direction = fGun->GetParticleMomentumDirection();

  const G4double theta = direction.theta();
  const G4double phi   = direction.phi();

  fEventAction->SetPrimary(energy,
                           position.x(),
                           position.y(),
                           position.z(),
                           theta,
                           phi);

  fGun->GeneratePrimaryVertex(event);
}
