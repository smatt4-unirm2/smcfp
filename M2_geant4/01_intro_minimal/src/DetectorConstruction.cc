#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterials();

  const auto worldSize = 1.0 * m;
  const auto boxSize   = 20.0 * cm;

  auto* worldS = new G4Box("World", worldSize/2., worldSize/2., worldSize/2.);
  auto* worldLV = new G4LogicalVolume(worldS, fWorldMat, "World");
  auto* worldPV = new G4PVPlacement(nullptr, {}, worldLV, "World", nullptr, false, 0, true);

  auto* absorberS = new G4Box("Absorber", boxSize/2., boxSize/2., boxSize/2.);
  auto* absorberLV = new G4LogicalVolume(absorberS, fAbsorberMat, "Absorber");

  new G4PVPlacement(nullptr, {}, absorberLV, "Absorber", worldLV, false, 0, true);

  return worldPV;
}

void DetectorConstruction::DefineMaterials()
{
  auto* nist = G4NistManager::Instance();
  fWorldMat = nist->FindOrBuildMaterial("G4_AIR");
  fAbsorberMat = nist->FindOrBuildMaterial("G4_PbWO4");
}
