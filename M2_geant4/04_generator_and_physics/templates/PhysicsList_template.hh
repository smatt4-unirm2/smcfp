#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4GenericMessenger;
class G4VPhysicsConstructor;
class G4DecayPhysics;
class G4OpticalPhysics;

class PhysicsList : public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList() override;

  void ConstructParticle() override;
  void ConstructProcess() override;
  void SetCuts() override;

  void SetPreset(const G4String& preset);
  void SetEMModel(const G4String& model);
  void SetEnableHadronic(G4bool value);
  void SetEnableOptical(G4bool value);
  void PrintConfiguration() const;

private:
  void BuildEMPhysics();

private:
  // TODO: aggiungere qui un messenger per controllare la fisica da macro
  G4GenericMessenger* fMessenger = nullptr;

  // TODO: parametri da esporre agli studenti
  G4String fPreset = "standard";
  G4String fEMModel = "standard_opt4";
  G4bool fEnableHadronic = true;
  G4bool fEnableOptical = false;

  // TODO: costruttori di fisica da istanziare in base alla configurazione
  G4VPhysicsConstructor* fEMPhysics = nullptr;
  G4DecayPhysics* fDecayPhysics = nullptr;
  G4OpticalPhysics* fOpticalPhysics = nullptr;
};

#endif
