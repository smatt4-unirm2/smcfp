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
  // TODO: usare GPS al posto di G4ParticleGun
  G4GeneralParticleSource* fGPS = nullptr;

  // TODO: salvare qui il puntatore all'EventAction per memorizzare
  // energia, posizione e direzione del primario generato.
  EventAction* fEventAction = nullptr;
};

#endif
