#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


DetectorConstruction::DetectorConstruction(G4bool useOpticalProperties)
 : G4VUserDetectorConstruction(),
   fUseOpticalProperties(useOpticalProperties),
   fCheckOverlaps(true)
{
}


DetectorConstruction::~DetectorConstruction()
{
  delete fRotZ90;
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterials();
  return DefineVolumes();
}


void DetectorConstruction::DefineMaterials()
{
  auto nistManager = G4NistManager::Instance();

  // TODO: definire i materiali usati nella simulazione
  fWorldMaterial   = nistManager->FindOrBuildMaterial("G4_AIR");
  fGapMaterial     = nistManager->FindOrBuildMaterial("G4_AIR");
  fCrystalMaterial = nistManager->FindOrBuildMaterial("G4_PbWO4");

  // TODO: attivare opzionalmente le proprietà ottiche
  if (fUseOpticalProperties) {
    DefinePbWO4OpticalProperties();
  }
}


void DetectorConstruction::DefinePbWO4OpticalProperties()
{
  // TODO: aggiungere proprietà ottiche al PbWO4
  // Esempio:
  // - indice di rifrazione
  // - lunghezza di assorbimento
  // - scintillazione
}


G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // TODO:
  // 1) costruire il mondo
  // 2) costruire il calorimetro
  // 3) costruire il cristallo base
  // 4) costruire un piano X con 16 cristalli
  // 5) costruire un piano Y ruotando i cristalli di 90 gradi
  // 6) alternare 12 piani nel calorimetro

  // Esempio di struttura:
  //
  // World
  //   └── Calorimeter
  //         ├── PlaneX
  //         │     ├── Crystal
  //         │     ├── Crystal
  //         │     └── ...
  //         ├── PlaneY
  //         │     ├── Crystal (ruotato)
  //         │     ├── Crystal (ruotato)
  //         │     └── ...
  //         └── ...

  // Ricordarsi sempre di ritornare il world fisico
  return nullptr;
}
