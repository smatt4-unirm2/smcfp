#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

// Elettromagnetica
#include "G4EmStandardPhysics.hh"
// Alternative utili per esercizi:
// #include "G4EmStandardPhysics_option4.hh"
// #include "G4EmLivermorePhysics.hh"
// #include "G4EmPenelopePhysics.hh"

#include "G4EmExtraPhysics.hh"

// Decadimenti
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

// Ottica
#include "G4OpticalPhysics.hh"

// Adronica
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"

// ============================================================================
// CONFIGURAZIONE DIDATTICA
// ============================================================================
// Qui gli studenti possono attivare/disattivare blocchi di fisica ricompilando.
//
// Suggerimento per gli esercizi:
// - tenere sempre attiva la EM
// - provare a spegnere/accendere:
//     * fisica adronica
//     * neutroni HP
//     * decadimenti
//     * decadimenti radioattivi
//     * fisica ottica
// ============================================================================

// Tipo di fisica elettromagnetica
static constexpr G4int kVerbose = 1;

// Toggle principali
static constexpr G4bool useHadronic          = true;
static constexpr G4bool useNeutronHP         = false;
static constexpr G4bool useDecay             = true;
static constexpr G4bool useRadioactiveDecay  = false;
static constexpr G4bool useOptical           = false;
static constexpr G4bool useNeutronTrackCut   = false;

// Se vuoi far sperimentare anche diverse EM, puoi cambiare qui:
// 0 = G4EmStandardPhysics
// 1 = G4EmStandardPhysics_option4
// 2 = G4EmLivermorePhysics
// 3 = G4EmPenelopePhysics
static constexpr G4int emChoice = 0;

// ----------------------------------------------------------------------------

PhysicsList::PhysicsList()
 : G4VModularPhysicsList()
{
  SetVerboseLevel(kVerbose);
  defaultCutValue = 0.7 * mm;

  // --------------------------------------------------------------------------
  // 1) FISICA ELETTROMAGNETICA
  // --------------------------------------------------------------------------
  // Sempre presente in questo esempio.
  // È la parte minima necessaria per e-, e+, gamma, ecc.
  switch (emChoice) {
    case 0:
      RegisterPhysics(new G4EmStandardPhysics(kVerbose));
      break;

    /*
    case 1:
      RegisterPhysics(new G4EmStandardPhysics_option4(kVerbose));
      break;

    case 2:
      RegisterPhysics(new G4EmLivermorePhysics(kVerbose));
      break;

    case 3:
      RegisterPhysics(new G4EmPenelopePhysics(kVerbose));
      break;
    */

    default:
      RegisterPhysics(new G4EmStandardPhysics(kVerbose));
      break;
  }

  // Processi extra per gamma e leptoni
  RegisterPhysics(new G4EmExtraPhysics(kVerbose));

  // --------------------------------------------------------------------------
  // 2) DECADIMENTI
  // --------------------------------------------------------------------------
  if (useDecay) {
    RegisterPhysics(new G4DecayPhysics(kVerbose));
  }

  if (useRadioactiveDecay) {
    RegisterPhysics(new G4RadioactiveDecayPhysics(kVerbose));
  }

  // --------------------------------------------------------------------------
  // 3) FISICA ADRONICA
  // --------------------------------------------------------------------------
  // Se useHadronic = false, restano solo EM + eventuali decadimenti.
  //
  // Per i neutroni a bassa energia:
  // - useNeutronHP = false  -> fisica standard
  // - useNeutronHP = true   -> elasticità neutronica con modelli HP
  //
  // Questa è una scelta didattica semplice e leggibile.
  if (useHadronic) {

    if (useNeutronHP) {
      RegisterPhysics(new G4HadronElasticPhysicsHP(kVerbose));
    } else {
      RegisterPhysics(new G4HadronElasticPhysics(kVerbose));
    }

    RegisterPhysics(new G4HadronPhysicsFTFP_BERT(kVerbose));
    RegisterPhysics(new G4StoppingPhysics(kVerbose));
    RegisterPhysics(new G4IonPhysics(kVerbose));

    if (useNeutronTrackCut) {
      RegisterPhysics(new G4NeutronTrackingCut(kVerbose));
    }
  }

  // --------------------------------------------------------------------------
  // 4) FISICA OTTICA
  // --------------------------------------------------------------------------
  if (useOptical) {
    RegisterPhysics(new G4OpticalPhysics(kVerbose));
  }

  PrintConfiguration();
}

// ----------------------------------------------------------------------------

void PhysicsList::PrintConfiguration() const
{
  G4cout << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << " PhysicsList configuration" << G4endl;
  G4cout << "------------------------------------------------------------" << G4endl;
  G4cout << " EM choice              : " << emChoice << G4endl;
  G4cout << " Hadronic               : " << useHadronic << G4endl;
  G4cout << " Neutron HP             : " << useNeutronHP << G4endl;
  G4cout << " Decay                  : " << useDecay << G4endl;
  G4cout << " Radioactive decay      : " << useRadioactiveDecay << G4endl;
  G4cout << " Optical                : " << useOptical << G4endl;
  G4cout << " Neutron tracking cut   : " << useNeutronTrackCut << G4endl;
  G4cout << " Default cut value      : " << defaultCutValue/mm << " mm" << G4endl;
  G4cout << "============================================================" << G4endl;
  G4cout << G4endl;
}
