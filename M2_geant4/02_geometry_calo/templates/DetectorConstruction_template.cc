#include "DetectorConstruction_template.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"

DetectorConstructionTemplate::DetectorConstructionTemplate(G4bool useOpticalProperties)
  : fUseOpticalProperties(useOpticalProperties)
{}

DetectorConstructionTemplate::~DetectorConstructionTemplate()
{
  delete fRotZ90;
}

G4VPhysicalVolume* DetectorConstructionTemplate::Construct()
{
  DefineMaterials();

  // TODO 1:
  // Creare il mondo.

  fRotZ90 = new G4RotationMatrix();
  fRotZ90->rotateZ(90.0 * deg);

  // TODO 2:
  // Costruire in modo modulare:
  //   crystal -> planeX -> planeY -> calorimeter

  // TODO 3:
  // Posizionare il calorimetro nel mondo.

  // TODO 4:
  // Aggiungere attributi di visualizzazione.

  return nullptr;
}

void DetectorConstructionTemplate::DefineMaterials()
{
  auto* nist = G4NistManager::Instance();
  fWorldMat = nist->FindOrBuildMaterial("G4_AIR");
  fGapMat = fWorldMat;
  fCrystalMat = nist->FindOrBuildMaterial("G4_PbWO4");

  if (fUseOpticalProperties) {
    DefineOpticalProperties();
  }
}

void DetectorConstructionTemplate::DefineOpticalProperties()
{
  // TODO:
  // Creare una G4MaterialPropertiesTable e assegnarla a fCrystalMat.
}

G4LogicalVolume* DetectorConstructionTemplate::BuildCrystal()
{
  // TODO:
  // Creare il solido della barra e il volume logico del cristallo.
  return nullptr;
}

G4LogicalVolume* DetectorConstructionTemplate::BuildPlaneX(G4LogicalVolume* crystalLV)
{
  // TODO:
  // Creare il volume logico del piano X e posizionare 16 barre lungo x.
  return nullptr;
}

G4LogicalVolume* DetectorConstructionTemplate::BuildPlaneY(G4LogicalVolume* crystalLV)
{
  // TODO:
  // Creare il volume logico del piano Y, ruotare le barre e posizionarle lungo y.
  return nullptr;
}

G4LogicalVolume* DetectorConstructionTemplate::BuildCalorimeter(G4LogicalVolume* planeXLV,
                                                                G4LogicalVolume* planeYLV)
{
  // TODO:
  // Creare il volume madre del calorimetro e alternare i piani X/Y lungo z.
  return nullptr;
}
