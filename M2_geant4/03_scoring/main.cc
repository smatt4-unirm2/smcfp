#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

int main(int argc, char** argv)
{
  // Mettere a false per disattivare in blocco le parti ottiche:
  // - proprietà ottiche del PbWO4
  // - fisica ottica
  constexpr G4bool kEnableOpticalPhysics = false;

  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

  runManager->SetUserInitialization(new DetectorConstruction(kEnableOpticalPhysics));

  auto* physicsList = new FTFP_BERT();
  if (kEnableOpticalPhysics) {
    physicsList->RegisterPhysics(new G4OpticalPhysics());
  }
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new ActionInitialization());

  auto* visManager = new G4VisExecutive();
  visManager->Initialize();

  auto* uiManager = G4UImanager::GetUIpointer();

  if (argc == 1) {
    auto* ui = new G4UIExecutive(argc, argv);
    uiManager->ApplyCommand("/control/execute macro/init_vis.mac");
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
