#include "PrimaryGeneratorAction.hh"

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(EventAction* eventAction)
  : fEventAction(eventAction)
{
  // --------------------------------------------------------------------------
  // GENERAL PARTICLE SOURCE
  // --------------------------------------------------------------------------
  // Usiamo GPS al posto di G4ParticleGun perché permette di controllare
  // direttamente da macro:
  //   - tipo di particella;
  //   - posizione della sorgente;
  //   - distribuzione angolare;
  //   - spettro energetico;
  //   - più sorgenti, distribuzioni arbitrarie, istogrammi, ecc.
  //
  // --------------------------------------------------------------------------
  //fGPS = new G4GeneralParticleSource();

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fGPS;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // --------------------------------------------------------------------------
  // Il GPS genera uno o più vertici primari nell'evento corrente.
  // --------------------------------------------------------------------------
  //fGPS->GeneratePrimaryVertex(event);
}
