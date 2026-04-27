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
  // Definizione dei materiali
  DefineMaterials();

  // Definizione della geometria
  return DefineVolumes();
}


void DetectorConstruction::DefineMaterials()
{
  // --------------------------------------------------------------------------
  // MATERIALI
  // --------------------------------------------------------------------------
  // In questo esercizio usiamo:
  // - aria per il mondo;
  // - aria anche per i gap tra cristalli e tra piani;
  // - PbWO4 per i cristalli del calorimetro.
  //
  // Il PbWO4 viene preso dal database NIST di Geant4.
  // --------------------------------------------------------------------------
  auto nistManager = G4NistManager::Instance();

  fWorldMaterial   = nistManager->FindOrBuildMaterial("G4_AIR");
  fGapMaterial     = nistManager->FindOrBuildMaterial("G4_AIR");
  fCrystalMaterial = nistManager->FindOrBuildMaterial("G4_PbWO4");

  if (fUseOpticalProperties) {
    DefinePbWO4OpticalProperties();
  }

  // Stampa della tabella dei materiali
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


void DetectorConstruction::DefinePbWO4OpticalProperties()
{
  // --------------------------------------------------------------------------
  // PROPRIETA' OTTICHE DEL PbWO4
  // --------------------------------------------------------------------------
  // Questa parte è pensata per uso didattico:
  // consente di associare al materiale una MaterialPropertiesTable
  // per studiare propagazione di fotoni ottici e scintillazione.
  //
  // I valori sono semplici e ragionevoli per un esempio introduttivo.
  // --------------------------------------------------------------------------
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

  G4double scintComponent[nEntries] = {
    0.20,
    1.00,
    0.35
  };

  auto* mpt = new G4MaterialPropertiesTable();

  mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
  mpt->AddProperty("ABSLENGTH", photonEnergy, absorptionLength, nEntries);
  mpt->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintComponent, nEntries);

  mpt->AddConstProperty("SCINTILLATIONYIELD", 200. / MeV);
  mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10.0 * ns);
  mpt->AddConstProperty("SCINTILLATIONYIELD1", 1.0);

  fCrystalMaterial->SetMaterialPropertiesTable(mpt);
}


G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // --------------------------------------------------------------------------
  // PARAMETRI GEOMETRICI
  // --------------------------------------------------------------------------
  // Il calorimetro è costituito da:
  // - 12 piani
  // - 16 barre per piano
  //
  // Ogni barra ha dimensioni:
  //   2 cm (x) x 32.015 cm (y) x 2 cm (z)
  //
  // La spaziatura:
  // - tra barre nello stesso piano
  // - tra piani successivi
  // è di 10 um.
  //
  // I piani sono alternati:
  //   X, Y, X, Y, ...
  // dove i piani Y hanno le barre ruotate di 90 gradi attorno a z.
  // --------------------------------------------------------------------------

  // Dimensione esterna di un piano:
  // lungo la direzione di segmentazione ci sono 16 barre + 15 gap
  G4double planeSizeTransverse =
      fNofBarsPerPlane * fBarSizeX + (fNofBarsPerPlane - 1) * fGapSize;

  // Per come è costruito l'esercizio, planeSizeTransverse = 32.015 cm
  // e coincide con la lunghezza della barra.
  G4double planeSizeXY = fBarSizeY;
  G4double planeThickness = fBarSizeZ;

  // Spessore totale del calorimetro
  G4double calorThickness =
      fNofPlanes * planeThickness + (fNofPlanes - 1) * fGapSize;

  // Dimensioni del calorimetro
  G4double calorSizeX = planeSizeXY;
  G4double calorSizeY = planeSizeXY;
  G4double calorSizeZ = calorThickness;

  // Dimensioni del mondo
  G4double worldSizeXY = 1.2 * planeSizeXY;
  G4double worldSizeZ  = 1.2 * calorThickness;

  // Controllo materiali
  if (!fWorldMaterial || !fGapMaterial || !fCrystalMaterial) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorConstruction::DefineVolumes()",
                "MyCode0001", FatalException, msg);
  }

  // --------------------------------------------------------------------------
  // WORLD
  // --------------------------------------------------------------------------
  auto worldS
    = new G4Box("World",
                worldSizeXY / 2.0,
                worldSizeXY / 2.0,
                worldSizeZ  / 2.0);

  auto worldLV
    = new G4LogicalVolume(worldS,
                          fWorldMaterial,
                          "World");

  auto worldPV
    = new G4PVPlacement(0,
                        G4ThreeVector(),
                        worldLV,
                        "World",
                        0,
                        false,
                        0,
                        fCheckOverlaps);

  // --------------------------------------------------------------------------
  // CALORIMETRO
  // --------------------------------------------------------------------------
  // Volume madre che contiene tutti i 12 piani.
  // --------------------------------------------------------------------------
  auto calorimeterS
    = new G4Box("Calorimeter",
                calorSizeX / 2.0,
                calorSizeY / 2.0,
                calorSizeZ / 2.0);

  auto calorimeterLV
    = new G4LogicalVolume(calorimeterS,
                          fGapMaterial,
                          "Calorimeter");

  new G4PVPlacement(0,
                    G4ThreeVector(),
                    calorimeterLV,
                    "Calorimeter",
                    worldLV,
                    false,
                    0,
                    fCheckOverlaps);

  // --------------------------------------------------------------------------
  // CRISTALLO BASE
  // --------------------------------------------------------------------------
  // Questo è il "mattone" elementare del calorimetro.
  // --------------------------------------------------------------------------
  auto crystalS
    = new G4Box("Crystal",
                fBarSizeX / 2.0,
                fBarSizeY / 2.0,
                fBarSizeZ / 2.0);

  auto crystalLV
    = new G4LogicalVolume(crystalS,
                          fCrystalMaterial,
                          "CrystalLV");

  // --------------------------------------------------------------------------
  // PIANO X
  // --------------------------------------------------------------------------
  // In questo piano le barre NON sono ruotate.
  // La segmentazione è lungo x.
  // --------------------------------------------------------------------------
  auto planeXS
    = new G4Box("PlaneX",
                planeSizeXY / 2.0,
                planeSizeXY / 2.0,
                planeThickness / 2.0);

  auto planeXLV
    = new G4LogicalVolume(planeXS,
                          fGapMaterial,
                          "PlaneXLV");

  for (G4int iBar = 0; iBar < fNofBarsPerPlane; ++iBar) {
    G4double xPos =
      -planeSizeTransverse / 2.0 + fBarSizeX / 2.0 + iBar * (fBarSizeX + fGapSize);

    new G4PVPlacement(0,
                      G4ThreeVector(xPos, 0., 0.),
                      crystalLV,
                      "CrystalX",
                      planeXLV,
                      false,
                      iBar,
                      fCheckOverlaps);
  }

  // --------------------------------------------------------------------------
  // POSIZIONAMENTO DEI PIANI NEL CALORIMETRO
  // --------------------------------------------------------------------------
  // I piani vengono alternati:
  //   piano 0 -> X
  //   piano 1 -> Y (rot 90 deg)
  //   piano 2 -> X
  //   ...
  // --------------------------------------------------------------------------

  fRotZ90 = new G4RotationMatrix();
  fRotZ90->rotateZ(90.0 * deg);

  for (G4int iPlane = 0; iPlane < fNofPlanes; ++iPlane) {

    G4double zPos =
      -calorSizeZ / 2.0 + planeThickness / 2.0 + iPlane * (planeThickness + fGapSize);

    G4RotationMatrix* currentRot = (iPlane % 2 == 0) ? nullptr : fRotZ90;
    G4String currentPlaneName       = (iPlane % 2 == 0) ? "PlaneX"  : "PlaneY";

    new G4PVPlacement(0,
                      G4ThreeVector(0., 0., zPos),
                      currentPlaneLV,
                      currentPlaneName,
                      calorimeterLV,
                      false,
                      iPlane,
                      fCheckOverlaps);
  }

  // --------------------------------------------------------------------------
  // STAMPA PARAMETRI
  // --------------------------------------------------------------------------
  G4cout
    << G4endl
    << "------------------------------------------------------------" << G4endl
    << "---> The calorimeter is made of " << fNofPlanes << " planes, each with "
    << fNofBarsPerPlane << " PbWO4 bars." << G4endl
    << "---> Bar size: "
    << fBarSizeX / cm << " cm x "
    << fBarSizeY / cm << " cm x "
    << fBarSizeZ / cm << " cm" << G4endl
    << "---> Gap between bars and between planes: "
    << fGapSize / um << " um" << G4endl
    << "------------------------------------------------------------" << G4endl;

  // --------------------------------------------------------------------------
  // VISUALIZATION ATTRIBUTES
  // --------------------------------------------------------------------------
  // Mondo e volumi madre invisibili, cristalli visibili in azzurro trasparente.
  // --------------------------------------------------------------------------
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  calorimeterLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  planeXLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  auto crystalVisAtt = new G4VisAttributes(G4Colour(0.35, 0.85, 1.00, 0.10));
  crystalVisAtt->SetVisibility(true);
  crystalVisAtt->SetForceWireframe(true);
  crystalVisAtt->SetForceSolid(false);
  crystalLV->SetVisAttributes(crystalVisAtt);

  // --------------------------------------------------------------------------
  // Sempre ritornare il world fisico
  // --------------------------------------------------------------------------
  return worldPV;
}
