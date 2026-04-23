#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"

#include "G4RunManagerFactory.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "Randomize.hh"

int main(int argc, char** argv)
{
  // --------------------------------------------------------------------------
  // Scelta esplicita del motore random.
  // --------------------------------------------------------------------------
  // RanecuEngine è un classico motore semplice e leggero, adatto a un corso
  // introduttivo. La scelta del motore non impedisce l'uso dei comandi macro:
  //   /random/setSeeds s1 s2
  // che continuano a funzionare sul motore attivo.
  //
  // Alternative utili da mostrare agli studenti:
  //   G4Random::setTheEngine(new CLHEP::MixMaxRng);
  //   G4Random::setTheEngine(new CLHEP::MTwistEngine);
  // --------------------------------------------------------------------------
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

  // La geometria resta identica al modulo precedente.
  // Le proprietà ottiche del PbWO4 sono mantenute attive a livello di materiale,
  // così l'utente può provare a combinare GPS + fisica ottica se lo desidera.
  runManager->SetUserInitialization(new DetectorConstruction(true));

  // Physics list modulare controllabile da macro.
  runManager->SetUserInitialization(new PhysicsList());

  // User actions: scoring + generatore primario.
  runManager->SetUserInitialization(new ActionInitialization());

  auto* visManager = new G4VisExecutive();
  visManager->Initialize();

  auto* uiManager = G4UImanager::GetUIpointer();

  if (argc == 1) {
    auto* ui = new G4UIExecutive(argc, argv);
    uiManager->ApplyCommand("/control/execute macro/init_vis.mac");
    //Aggiungo un comando per eseguire la configurazione GPS di default quando sono in visualizzazione
    uiManager->ApplyCommand("/control/execute macro/default_source.mac");
    ui->SessionStart();
    delete ui;
  } else {
    G4String command = "/control/execute ";
    uiManager->ApplyCommand(command + argv[1]);
  }

  delete visManager;
  delete runManager;
  return 0;
}
