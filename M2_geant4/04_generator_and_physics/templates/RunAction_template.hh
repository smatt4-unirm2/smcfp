#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4GenericMessenger;
class G4Run;
class TFile;
class TTree;
class EventAction;

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction() override;

  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;

  void FillEvent(const EventAction& eventAction);
  void SetOutputFileName(const G4String& fileName);

private:
  void Book();

private:
  // TODO: messenger per esporre /analysis/setFileName
  G4GenericMessenger* fMessenger = nullptr;

  // TODO: fallback se il nome non viene impostato da macro
  G4String fOutputFileName = "scoring_calo.root";

  TFile* fOutputFile = nullptr;
  TTree* fTree = nullptr;
};

#endif
