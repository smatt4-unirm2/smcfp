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
  void SetPrimary(G4double energy,
                  G4double x0,
                  G4double y0,
                  G4double z0,
                  G4double theta,
                  G4double phi);

  const G4double (&GetEdep() const)[kNPlanes][kNBars] { return fEdep; }
  G4double GetPrimaryEnergy() const { return fPrimaryEnergy; }
  G4double GetPrimaryX0() const { return fPrimaryX0; }
  G4double GetPrimaryY0() const { return fPrimaryY0; }
  G4double GetPrimaryZ0() const { return fPrimaryZ0; }
  G4double GetPrimaryTheta() const { return fPrimaryTheta; }
  G4double GetPrimaryPhi() const { return fPrimaryPhi; }

private:
  void Reset();

private:
  RunAction* fRunAction = nullptr;

  // Energia depositata in ciascun cristallo dell'evento corrente.
  // Convenzione di indicizzazione:
  //   - piano    = 0, ..., 11 (verso +z)
  //   - cristallo = 0, ..., 15 (verso +x nei piani X, verso +y nei piani Y)
  G4double fEdep[kNPlanes][kNBars] = {{0.0}};

  // Variabili del primario salvate evento per evento.
  G4double fPrimaryEnergy = 0.0;
  G4double fPrimaryX0     = 0.0;
  G4double fPrimaryY0     = 0.0;
  G4double fPrimaryZ0     = 0.0;
  G4double fPrimaryTheta  = 0.0;
  G4double fPrimaryPhi    = 0.0;
};

#endif
