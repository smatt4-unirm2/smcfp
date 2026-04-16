#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction(G4bool useOpticalProperties)
  : fUseOpticalProperties(useOpticalProperties)
{}

DetectorConstruction::~DetectorConstruction()
{
  delete fRotZ90;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // ================================================================
  // 1) MATERIALI
  // ================================================================
  // In questa fase lavoriamo solo sulla geometria, ma definiamo già
  // i materiali in modo realistico. Usiamo aria per mondo e gap, e
  // PbWO4 per i cristalli del calorimetro.
  DefineMaterials();

  // ================================================================
  // 2) MONDO
  // ================================================================
  // Il mondo è volutamente molto più grande del calorimetro per evitare
  // che il fascio o eventuali secondarie escano subito dalla geometria.
  const auto worldSizeXY = 1.0 * m;
  const auto worldSizeZ  = 1.0 * m;

  auto* worldSolid = new G4Box("World",
                               worldSizeXY / 2.0,
                               worldSizeXY / 2.0,
                               worldSizeZ  / 2.0);

  auto* worldLV = new G4LogicalVolume(worldSolid, fWorldMat, "World");

  auto* worldPV = new G4PVPlacement(nullptr,
                                    G4ThreeVector(),
                                    worldLV,
                                    "World",
                                    nullptr,
                                    false,
                                    0,
                                    true);

  // La rotazione di 90 gradi attorno a z viene usata per i piani "Y-view".
  fRotZ90 = new G4RotationMatrix();
  fRotZ90->rotateZ(90.0 * deg);

  // ================================================================
  // 3) COSTRUZIONE MODULARE DEL CALORIMETRO
  // ================================================================
  // L'idea didattica è costruire la geometria per livelli:
  //   cristallo -> piano -> calorimetro
  // in modo che gli studenti vedano chiaramente come si riusano i volumi.
  auto* crystalLV = BuildCrystal();
  auto* planeXLV  = BuildPlaneX(crystalLV);
  auto* planeYLV  = BuildPlaneY(crystalLV);
  auto* caloLV    = BuildCalorimeter(planeXLV, planeYLV);

  // Posizioniamo il calorimetro al centro del mondo.
  new G4PVPlacement(nullptr,
                    G4ThreeVector(),
                    caloLV,
                    "Calorimeter",
                    worldLV,
                    false,
                    0,
                    true);

  // ================================================================
  // 4) ATTRIBUTI DI VISUALIZZAZIONE
  // ================================================================
  // Servono solo a rendere la geometria più leggibile nel viewer.
  SetupVisualization(worldLV, caloLV, planeXLV, planeYLV, crystalLV);

  return worldPV;
}

void DetectorConstruction::DefineMaterials()
{
  auto* nist = G4NistManager::Instance();

  fWorldMat   = nist->FindOrBuildMaterial("G4_AIR");
  fGapMat     = fWorldMat;
  fCrystalMat = nist->FindOrBuildMaterial("G4_PbWO4");

  if (fUseOpticalProperties) {
    DefinePbWO4OpticalProperties();
  }
}

void DetectorConstruction::DefinePbWO4OpticalProperties()
{
  // ----------------------------------------------------------------
  // Proprietà ottiche di esempio per PbWO4.
  // ----------------------------------------------------------------
  // Scopo didattico:
  // - mostrare come si collega una MaterialPropertiesTable al materiale;
  // - permettere, insieme a G4OpticalPhysics, la propagazione dei fotoni
  //   ottici e la generazione di luce di scintillazione.
  //
  // I valori qui sotto sono volutamente semplici e ragionevoli per un
  // esercizio introduttivo; possono essere raffinati in esercizi successivi.
  // ----------------------------------------------------------------
  const G4int nEntries = 3;

  G4double photonEnergy[nEntries] = {
    2.0 * eV,
    2.6 * eV,
    3.2 * eV
  };

  G4double refractiveIndex[nEntries] = {
    2.20,
    2.20,
    2.20
  };

  G4double absorptionLength[nEntries] = {
    24.0 * cm,
    24.0 * cm,
    24.0 * cm
  };

  // Spettro relativo, normalizzato arbitrariamente.
  G4double scintComponent[nEntries] = {
    0.20,
    1.00,
    0.35
  };

  auto* mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
  mpt->AddProperty("ABSLENGTH", photonEnergy, absorptionLength, nEntries);
  mpt->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintComponent, nEntries);

  // Alcune costanti utili per la scintillazione.
  mpt->AddConstProperty("SCINTILLATIONYIELD", 200. / MeV);
  mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10.0 * ns);
  mpt->AddConstProperty("SCINTILLATIONYIELD1", 1.0);

  fCrystalMat->SetMaterialPropertiesTable(mpt);
}

G4LogicalVolume* DetectorConstruction::BuildCrystal()
{
  // ----------------------------------------------------------------
  // CRISTALLO BASE
  // ----------------------------------------------------------------
  // Una barra di PbWO4 ha dimensioni:
  //   x = 2 cm
  //   y = 32.015 cm
  //   z = 2 cm
  // Il lato lungo è y.
  // ----------------------------------------------------------------
  auto* crystalSolid = new G4Box("CrystalSolid",
                                 fBarSizeX / 2.0,
                                 fBarSizeY / 2.0,
                                 fBarSizeZ / 2.0);

  auto* crystalLV = new G4LogicalVolume(crystalSolid, fCrystalMat, "CrystalLV");
  return crystalLV;
}

G4LogicalVolume* DetectorConstruction::BuildPlaneX(G4LogicalVolume* crystalLV)
{
  // ----------------------------------------------------------------
  // PIANO X
  // ----------------------------------------------------------------
  // In questo piano le barre NON sono ruotate.
  // Quindi il lato lungo resta lungo y, mentre i 16 cristalli vengono
  // distribuiti lungo x con gap di 10 um.
  // ----------------------------------------------------------------
  const auto planeSizeX = fNBarsPerPlane * fBarSizeX + (fNBarsPerPlane - 1) * fGap;
  const auto planeSizeY = fBarSizeY;
  const auto planeSizeZ = fBarSizeZ;

  auto* planeSolid = new G4Box("PlaneXSolid",
                               planeSizeX / 2.0,
                               planeSizeY / 2.0,
                               planeSizeZ / 2.0);

  auto* planeLV = new G4LogicalVolume(planeSolid, fGapMat, "PlaneXLV");

  for (G4int iBar = 0; iBar < fNBarsPerPlane; ++iBar) {
    const auto xPos = -planeSizeX / 2.0 + fBarSizeX / 2.0 + iBar * (fBarSizeX + fGap);

    new G4PVPlacement(nullptr,
                      G4ThreeVector(xPos, 0., 0.),
                      crystalLV,
                      "CrystalInPlaneX",
                      planeLV,
                      false,
                      iBar,
                      true);
  }

  return planeLV;
}

G4LogicalVolume* DetectorConstruction::BuildPlaneY(G4LogicalVolume* crystalLV)
{
  // ----------------------------------------------------------------
  // PIANO Y
  // ----------------------------------------------------------------
  // Qui riusiamo lo stesso cristallo, ma lo ruotiamo di 90 gradi attorno
  // a z. In questo modo il lato lungo, che prima era lungo y, va lungo x.
  // I 16 cristalli vengono poi distribuiti lungo y.
  // ----------------------------------------------------------------
  const auto planeSizeX = fBarSizeY;
  const auto planeSizeY = fNBarsPerPlane * fBarSizeX + (fNBarsPerPlane - 1) * fGap;
  const auto planeSizeZ = fBarSizeZ;

  auto* planeSolid = new G4Box("PlaneYSolid",
                               planeSizeX / 2.0,
                               planeSizeY / 2.0,
                               planeSizeZ / 2.0);

  auto* planeLV = new G4LogicalVolume(planeSolid, fGapMat, "PlaneYLV");

  for (G4int iBar = 0; iBar < fNBarsPerPlane; ++iBar) {
    const auto yPos = -planeSizeY / 2.0 + fBarSizeX / 2.0 + iBar * (fBarSizeX + fGap);

    new G4PVPlacement(fRotZ90,
                      G4ThreeVector(0., yPos, 0.),
                      crystalLV,
                      "CrystalInPlaneY",
                      planeLV,
                      false,
                      iBar,
                      true);
  }

  return planeLV;
}

G4LogicalVolume* DetectorConstruction::BuildCalorimeter(G4LogicalVolume* planeXLV,
                                                        G4LogicalVolume* planeYLV)
{
  // ----------------------------------------------------------------
  // CALORIMETRO COMPLETO
  // ----------------------------------------------------------------
  // Il calorimetro contiene 12 piani:
  //   X, Y, X, Y, ...
  // con gap di 10 um tra un piano e il successivo.
  //
  // Notare che i piani X e Y hanno la stessa dimensione esterna:
  // 32.015 cm x 32.015 cm x 2 cm.
  // Questo rende naturale l'alternanza dei piani nel volume madre.
  // ----------------------------------------------------------------
  const auto planeSizeXY = fBarSizeY;
  const auto planeSizeZ  = fBarSizeZ;
  const auto caloSizeX   = planeSizeXY;
  const auto caloSizeY   = planeSizeXY;
  const auto caloSizeZ   = fNPlanes * planeSizeZ + (fNPlanes - 1) * fGap;

  auto* caloSolid = new G4Box("CalorimeterSolid",
                              caloSizeX / 2.0,
                              caloSizeY / 2.0,
                              caloSizeZ / 2.0);

  auto* caloLV = new G4LogicalVolume(caloSolid, fGapMat, "CalorimeterLV");

  for (G4int iPlane = 0; iPlane < fNPlanes; ++iPlane) {
    const auto zPos = -caloSizeZ / 2.0 + planeSizeZ / 2.0 + iPlane * (planeSizeZ + fGap);

    auto* currentPlaneLV = (iPlane % 2 == 0) ? planeXLV : planeYLV;
    const auto planeName = (iPlane % 2 == 0) ? "PlaneX" : "PlaneY";

    new G4PVPlacement(nullptr,
                      G4ThreeVector(0., 0., zPos),
                      currentPlaneLV,
                      planeName,
                      caloLV,
                      false,
                      iPlane,
                      true);
  }

  return caloLV;
}

void DetectorConstruction::SetupVisualization(G4LogicalVolume* worldLV,
                                              G4LogicalVolume* caloLV,
                                              G4LogicalVolume* planeXLV,
                                              G4LogicalVolume* planeYLV,
                                              G4LogicalVolume* crystalLV)
{
  // Colore azzurrino trasparente per i cristalli.
  auto* crystalVis = new G4VisAttributes(G4Colour(0.35, 0.85, 1.00, 0.05));
  crystalVis->SetVisibility(true);
  crystalVis->SetForceWireframe(true);
  crystalVis->SetForceSolid(false);

  // I volumi madre intermedi sono invisibili: così si vedono bene i gap d'aria
  // senza disegnare box aggiuntivi che disturbano la vista.
  auto* invisibleVis = new G4VisAttributes();
  invisibleVis->SetVisibility(false);

  worldLV->SetVisAttributes(invisibleVis);
  caloLV->SetVisAttributes(invisibleVis);
  planeXLV->SetVisAttributes(invisibleVis);
  planeYLV->SetVisAttributes(invisibleVis);
  crystalLV->SetVisAttributes(crystalVis);
}
