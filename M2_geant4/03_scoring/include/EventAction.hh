#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
class RunAction;

class EventAction : public G4UserEventAction
{
public:
  static constexpr G4int kNPlanes = 12;
  static constexpr G4int kNBars   = 16;

  explicit EventAction(RunAction* runAction);
  ~EventAction() override = default;

  void BeginOfEventAction(const G4Event* event) override;
  void EndOfEventAction(const G4Event* event) override;

  void AddEdep(G4int plane, G4int crystal, G4double edep);

  const G4double (&GetEdep() const)[kNPlanes][kNBars] { return fEdep; }

private:
  void Reset();

private:
  RunAction* fRunAction = nullptr;

  // Energia depositata in ciascun cristallo dell'evento corrente.
  // Convenzione di indicizzazione:
  //   - piano    = 0, ..., 11 (verso +z)
  //   - cristallo = 0, ..., 15 (verso +x nei piani X, verso +y nei piani Y)
  G4double fEdep[kNPlanes][kNBars] = {{0.0}};
};

#endif
