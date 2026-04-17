#include "PhysicsList.hh"

// TODO:
// Inserire qui gli #include dei costruttori di fisica che si vogliono usare.
//
// Esempi possibili:
// #include "G4EmStandardPhysics.hh"
// #include "G4DecayPhysics.hh"
// #include "G4HadronPhysicsFTFP_BERT.hh"
// #include "G4OpticalPhysics.hh"

#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

PhysicsList::PhysicsList()
  : G4VModularPhysicsList()
{
  // TODO:
  // Impostare il livello di verbosità.
  //
  // Esempio:
  // SetVerboseLevel(1);

  // TODO:
  // Impostare il cut di default.
  //
  // Esempio:
  // defaultCutValue = 0.7 * mm;

  // TODO:
  // Registrare qui i moduli di fisica desiderati.
  //
  // Esempi:
  // RegisterPhysics(new G4EmStandardPhysics());
  // RegisterPhysics(new G4DecayPhysics());
  // RegisterPhysics(new G4HadronPhysicsFTFP_BERT());

  PrintConfiguration();
}

void PhysicsList::PrintConfiguration() const
{
  G4cout << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << " PhysicsList template configuration" << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << G4endl;

  // TODO:
  // Stampare qui la configurazione scelta, se utile.
}
