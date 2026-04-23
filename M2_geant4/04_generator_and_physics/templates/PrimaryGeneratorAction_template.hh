#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4GeneralParticleSource;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event* event) override;

private:
  // TODO:
  // Scegliere qui il generatore primario.
  //
  // Esempi:
  // G4ParticleGun* fGun = nullptr;
  // G4GeneralParticleSource* fGPS = nullptr;

};

#endif
