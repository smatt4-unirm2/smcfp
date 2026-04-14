#ifndef CaloHit_h
#define CaloHit_h 1

#include "G4THitsCollection.hh"
#include "G4VHit.hh"
#include "globals.hh"

class CaloHit : public G4VHit
{
public:
  CaloHit() = default;
  ~CaloHit() override = default;

  void AddEdep(G4double e) { fEdep += e; }
  void SetIDs(G4int layer, G4int bar) { fLayerID = layer; fBarID = bar; }

  G4double GetEdep() const { return fEdep; }
  G4int GetLayerID() const { return fLayerID; }
  G4int GetBarID() const { return fBarID; }

private:
  G4double fEdep = 0.;
  G4int fLayerID = -1;
  G4int fBarID = -1;
};

using CaloHitsCollection = G4THitsCollection<CaloHit>;

#endif
