#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterials();

  constexpr int nLayers = 12;
  constexpr int nBars = 16;
  const G4double gap = 10.0 * um;

  const G4double barX = 2.0 * cm;
  const G4double barY = 32.015 * cm;
  const G4double barZ = 2.0 * cm;

  const G4double layerThickness = barZ;
  const G4double caloXY = barY + 2.0 * cm;
  const G4double caloZ = nLayers * layerThickness + (nLayers - 1) * gap + 2.0 * cm;

  auto* worldS = new G4Box("World", 1.0*m, 1.0*m, 1.0*m);
  auto* worldLV = new G4LogicalVolume(worldS, fWorldMat, "World");
  auto* worldPV = new G4PVPlacement(nullptr, {}, worldLV, "World", nullptr, false, 0, true);

  auto* caloS = new G4Box("Calorimeter", caloXY/2., caloXY/2., caloZ/2.);
  auto* caloLV = new G4LogicalVolume(caloS, fWorldMat, "Calorimeter");
  new G4PVPlacement(nullptr, {}, caloLV, "Calorimeter", worldLV, false, 0, true);

  auto* barS = new G4Box("Bar", barX/2., barY/2., barZ/2.);
  fBarLV = new G4LogicalVolume(barS, fAbsorberMat, "Bar");

  for (int i = 0; i < nLayers; ++i) {
    BuildLayer(caloLV, i, (i % 2 == 0));
  }

  return worldPV;
}

void DetectorConstruction::BuildLayer(G4LogicalVolume* mother, int layerID, bool xView)
{
  constexpr int nBars = 16;
  const G4double gap = 10.0 * um;
  const G4double barX = 2.0 * cm;
  const G4double barY = 32.015 * cm;
  const G4double barZ = 2.0 * cm;

  const G4double pitch = barX + gap;
  const G4double totalZ = 12 * barZ + 11 * gap;
  const G4double z0 = -totalZ/2. + barZ/2.;
  const G4double zPos = z0 + layerID * (barZ + gap);

  auto* rot = new G4RotationMatrix();
  if (!xView) {
    rot->rotateZ(90.0 * deg);
  }

  for (int ib = 0; ib < nBars; ++ib) {
    const G4double offset = (ib - (nBars - 1) / 2.0) * pitch;
    G4ThreeVector pos = xView ? G4ThreeVector(offset, 0., zPos)
                              : G4ThreeVector(0., offset, zPos);
    const int copyNo = 100 * layerID + ib;
    new G4PVPlacement(xView ? nullptr : rot, pos, fBarLV, "Bar", mother, false, copyNo, true);
  }
}

void DetectorConstruction::DefineMaterials()
{
  auto* nist = G4NistManager::Instance();
  fWorldMat = nist->FindOrBuildMaterial("G4_AIR");
  fAbsorberMat = nist->FindOrBuildMaterial("G4_W");
}
