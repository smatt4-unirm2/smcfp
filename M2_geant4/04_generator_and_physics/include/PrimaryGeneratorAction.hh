#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4GeneralParticleSource;
class EventAction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  explicit PrimaryGeneratorAction(EventAction* eventAction);
  ~PrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event* event) override;

private:
  // General Particle Source (GPS): generatore molto più flessibile del
  // semplice G4ParticleGun. Tutta la configurazione del fascio può essere
  // fatta da macro senza ricompilare il codice.
  G4GeneralParticleSource* fGPS = nullptr;

  // Puntatore all'EventAction: lo usiamo per salvare, evento per evento,
  // i parametri effettivamente generati dal GPS.
  EventAction* fEventAction = nullptr;
};

#endif
