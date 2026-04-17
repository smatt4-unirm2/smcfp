#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class TFile;
class TTree;
class EventAction;

class RunAction : public G4UserRunAction
{
public:
  static constexpr G4int kNPlanes = 12;
  static constexpr G4int kNBars   = 16;

  RunAction();
  ~RunAction() override;

  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;

  void FillEvent(const EventAction& eventAction,
               G4double primaryEnergy,
               G4double primaryX0,
               G4double primaryY0,
               G4double primaryZ0,
               G4double primaryTheta,
               G4double primaryPhi);

private:
  void Book();
  void ResetBranches();

private:
  TFile* fOutputFile = nullptr;
  TTree* fTree = nullptr;

  G4double fEdep[kNPlanes][kNBars] = {{0.0}};

  G4double fPrimaryEnergy = 0.0;
  G4double fPrimaryX0     = 0.0;
  G4double fPrimaryY0     = 0.0;
  G4double fPrimaryZ0     = 0.0;
  G4double fPrimaryTheta  = 0.0;
  G4double fPrimaryPhi    = 0.0;
};

#endif
