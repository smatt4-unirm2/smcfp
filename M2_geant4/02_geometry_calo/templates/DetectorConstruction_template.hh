#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4VPhysicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction(G4bool useOpticalProperties = false);
  ~DetectorConstruction() override;

  G4VPhysicalVolume* Construct() override;

private:
  void DefineMaterials();
  void DefinePbWO4OpticalProperties();
  G4VPhysicalVolume* DefineVolumes();

private:
  G4bool fUseOpticalProperties = false;
  G4bool fCheckOverlaps = true;

  G4Material* fWorldMaterial   = nullptr;
  G4Material* fGapMaterial     = nullptr;
  G4Material* fCrystalMaterial = nullptr;

  G4RotationMatrix* fRotZ90 = nullptr;

  G4int    fNofPlanes       = 12;
  G4int    fNofBarsPerPlane = 16;

  G4double fBarSizeX = 2.0 * cm;
  G4double fBarSizeY = 32.015 * cm;
  G4double fBarSizeZ = 2.0 * cm;
  G4double fGapSize  = 10.0 * um;
};

#endif
