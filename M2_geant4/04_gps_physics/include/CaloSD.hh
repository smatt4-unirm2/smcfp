#ifndef CaloSD_h
#define CaloSD_h 1

#include "G4VSensitiveDetector.hh"
#include <map>
#include <utility>

class CaloHitsCollection;
class G4Step;
class G4HCofThisEvent;

class CaloSD : public G4VSensitiveDetector
{
public:
  explicit CaloSD(const G4String& name);
  ~CaloSD() override = default;

  void Initialize(G4HCofThisEvent* hce) override;
  G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;
  void EndOfEvent(G4HCofThisEvent*) override;

private:
  CaloHitsCollection* fHitsCollection = nullptr;
  std::map<std::pair<int,int>, int> fIndexMap;
  int fHCID = -1;
};

#endif
