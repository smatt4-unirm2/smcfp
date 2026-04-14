#include "EventAction.hh"
#include "CaloHit.hh"

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

void EventAction::EndOfEventAction(const G4Event* event)
{
  auto* hce = event->GetHCofThisEvent();
  if (!hce) return;

  static int hcID = -1;
  if (hcID < 0) {
    hcID = G4SDManager::GetSDMpointer()->GetCollectionID("BarSD/CaloHitsCollection");
  }

  auto* hc = static_cast<CaloHitsCollection*>(hce->GetHC(hcID));
  if (!hc) return;

  G4double totalEdep = 0.;
  for (size_t i = 0; i < hc->GetSize(); ++i) {
    totalEdep += (*hc)[i]->GetEdep();
  }

  G4cout << "Event " << event->GetEventID()
         << " total Edep in bars = " << totalEdep / MeV << " MeV" << G4endl;
}
