#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction() = default;
  ~DetectorConstruction() override = default;

  G4VPhysicalVolume* Construct() override;

private:
  void DefineMaterials();
  void BuildLayer(G4LogicalVolume* mother, int layerID, bool xView);

  G4Material* fWorldMat = nullptr;
  G4Material* fAbsorberMat = nullptr;

  G4LogicalVolume* fBarLV = nullptr;
};

#endif
