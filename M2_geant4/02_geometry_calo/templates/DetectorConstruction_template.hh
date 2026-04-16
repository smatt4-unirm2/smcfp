#ifndef DetectorConstructionTemplate_h
#define DetectorConstructionTemplate_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4RotationMatrix;

class DetectorConstructionTemplate : public G4VUserDetectorConstruction
{
public:
  explicit DetectorConstructionTemplate(G4bool useOpticalProperties = false);
  ~DetectorConstructionTemplate() override;

  G4VPhysicalVolume* Construct() override;

private:
  void DefineMaterials();
  void DefineOpticalProperties();

  G4LogicalVolume* BuildCrystal();
  G4LogicalVolume* BuildPlaneX(G4LogicalVolume* crystalLV);
  G4LogicalVolume* BuildPlaneY(G4LogicalVolume* crystalLV);
  G4LogicalVolume* BuildCalorimeter(G4LogicalVolume* planeXLV, G4LogicalVolume* planeYLV);

private:
  G4bool fUseOpticalProperties = false;
  G4Material* fWorldMat = nullptr;
  G4Material* fGapMat = nullptr;
  G4Material* fCrystalMat = nullptr;
  G4RotationMatrix* fRotZ90 = nullptr;

  // Parametri da completare/modificare.
  G4double fBarSizeX = 2.0 * cm;
  G4double fBarSizeY = 32.015 * cm;
  G4double fBarSizeZ = 2.0 * cm;
  G4double fGap = 10.0 * um;

  G4int fNBarsPerPlane = 16;
  G4int fNPlanes = 12;
};

#endif
