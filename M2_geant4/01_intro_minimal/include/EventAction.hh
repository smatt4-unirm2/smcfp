#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

class EventAction : public G4UserEventAction
{
public:
  EventAction() = default;
  ~EventAction() override = default;

  void BeginOfEventAction(const G4Event* event) override;
  void EndOfEventAction(const G4Event* event) override;

  void AddEdep(G4double value) { fTotalEdep += value; }
  void AddEdepHad(G4double val) { fEdepHad += val; }
  void AddEdepEle(G4double val) { fEdepEle += val; }
  void AddEdepGam(G4double val) { fEdepGam += val; }
  void AddTrackLength(G4double value) { fTotalTrackLength += value; }
  void AddAbsorberStep() { ++fNStepsInAbsorber; }
  void AddSecondaries(G4int value) { fNSecondaries += value; }
  void UpdateMaxStepEdep(G4double value);

private:
  G4double fPrimaryEnergy = 0.0;
  G4double fTotalEdep = 0.0;
  G4double fEdepHad = 0.;
  G4double fEdepEle = 0.;
  G4double fEdepGam = 0.;
  G4double fTotalTrackLength = 0.0;
  G4double fMaxStepEdep = 0.0;
  G4int    fNStepsInAbsorber = 0;
  G4int    fNSecondaries = 0;
};

#endif
