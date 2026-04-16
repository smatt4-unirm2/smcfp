#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;
class EventAction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  explicit PrimaryGeneratorAction(EventAction* eventAction);
  ~PrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event* event) override;

private:
  G4ParticleGun* fGun = nullptr;
  EventAction* fEventAction = nullptr;
};

#endif
