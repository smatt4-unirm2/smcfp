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
  RunAction();
  ~RunAction() override;

  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;

  // TODO:
  // metodo da usare per trasferire i dati dell'evento al TTree
  void FillEvent(const EventAction& eventAction);

private:
  // TODO:
  // aggiungere qui file ROOT, TTree e buffer delle branch
  TFile* fOutputFile = nullptr;
  TTree* fTree = nullptr;
};

#endif
