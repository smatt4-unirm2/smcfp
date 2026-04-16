#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction(G4bool useOpticalProperties = true);
  ~DetectorConstruction() override;

  G4VPhysicalVolume* Construct() override;

private:
  void DefineMaterials();
  void DefinePbWO4OpticalProperties();
  G4VPhysicalVolume* DefineVolumes();

private:
  // Flag per attivare/disattivare le proprietà ottiche del PbWO4
  G4bool fUseOpticalProperties = false;

  // Controllo overlap
  G4bool fCheckOverlaps = true;

  // Materiali
  G4Material* fWorldMaterial   = nullptr;
  G4Material* fGapMaterial     = nullptr;
  G4Material* fCrystalMaterial = nullptr;

  // Rotazione per i piani con barre ortogonali
  G4RotationMatrix* fRotZ90 = nullptr;

  // Parametri geometrici
  G4int    fNofPlanes       = 12;
  G4int    fNofBarsPerPlane = 16;

  G4double fBarSizeX = 2.0 * cm;
  G4double fBarSizeY = 32.015 * cm;
  G4double fBarSizeZ = 2.0 * cm;

  G4double fGapSize = 10.0 * um;
};

#endif
