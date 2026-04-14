#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

class G4Event;

class EventAction : public G4UserEventAction
{
public:
  EventAction() = default;
  ~EventAction() override = default;

  void EndOfEventAction(const G4Event* event) override;
};

#endif
