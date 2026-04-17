#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "CLHEP/Random/RanecuEngine.hh"
#include "CLHEP/Random/MixMaxRng.hh"
#include "CLHEP/Random/MTwistEngine.hh"
#include "Randomize.hh"

int main(int argc, char** argv)
{
  // TODO:
  // scegliere esplicitamente un motore random.
  // Alternative utili da provare:
  //   G4Random::setTheEngine(new CLHEP::MixMaxRng);
  //   G4Random::setTheEngine(new CLHEP::MTwistEngine);
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

  // TODO: inizializzare geometria, fisica e user actions
  runManager->SetUserInitialization(new DetectorConstruction(true));
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());

  auto* visManager = new G4VisExecutive();
  visManager->Initialize();

  auto* uiManager = G4UImanager::GetUIpointer();

  // TODO: gestire modalità batch e interattiva

  delete visManager;
  delete runManager;
  return 0;
}
