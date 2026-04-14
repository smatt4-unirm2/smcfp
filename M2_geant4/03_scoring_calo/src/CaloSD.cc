#include "CaloSD.hh"
#include "CaloHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHandle.hh"

CaloSD::CaloSD(const G4String& name)
: G4VSensitiveDetector(name)
{
  collectionName.insert("CaloHitsCollection");
}

void CaloSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection = new CaloHitsCollection(SensitiveDetectorName, collectionName[0]);
  fIndexMap.clear();

  if (fHCID < 0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  hce->AddHitsCollection(fHCID, fHitsCollection);
}

G4bool CaloSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  const auto edep = step->GetTotalEnergyDeposit();
  if (edep <= 0.) return false;

  const auto touchable = step->GetPreStepPoint()->GetTouchableHandle();
  const int copyNo = touchable->GetCopyNumber();
  const int layerID = copyNo / 100;
  const int barID = copyNo % 100;
  const auto key = std::make_pair(layerID, barID);

  int idx = -1;
  auto it = fIndexMap.find(key);
  if (it == fIndexMap.end()) {
    auto* hit = new CaloHit();
    hit->SetIDs(layerID, barID);
    idx = fHitsCollection->insert(hit) - 1;
    fIndexMap[key] = idx;
  } else {
    idx = it->second;
  }

  (*fHitsCollection)[idx]->AddEdep(edep);
  return true;
}

void CaloSD::EndOfEvent(G4HCofThisEvent*)
{
}
