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

  // TODO: metodo da chiamare per accumulare l'energia depositata
  void AddEdep(G4int plane, G4int crystal, G4double edep);

  // TODO: metodo da usare per salvare le variabili del primario
  void SetPrimary(G4double energy,
                  G4double x0,
                  G4double y0,
                  G4double z0,
                  G4double theta,
                  G4double phi);

private:
  // TODO: aggiungere l'accumulatore evento per evento
  // Esempio: G4double fEdep[kNPlanes][kNBars];

  // TODO: aggiungere qui le variabili del primario
  // Esempio: energia, posizione, angoli

  RunAction* fRunAction = nullptr;
};

#endif
