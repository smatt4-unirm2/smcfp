#include "PhysicsList.hh"

PhysicsList::PhysicsList()
  : G4VModularPhysicsList()
{
  // TODO:
  // 1) impostare cut di default
  // 2) creare il messenger /physics/
  // 3) inizializzare la configurazione di default
}

PhysicsList::~PhysicsList()
{
  // TODO: deallocare i costruttori di fisica e il messenger
}

void PhysicsList::BuildEMPhysics()
{
  // TODO:
  // scegliere qui tra modelli EM alternativi:
  // - standard
  // - standard_opt4
  // - livermore
  // - penelope
}

void PhysicsList::SetPreset(const G4String&)
{
  // TODO:
  // definire alcune configurazioni pronte all'uso per gli studenti
}

void PhysicsList::SetEMModel(const G4String&)
{
  // TODO: cambiare il modello EM prima di /run/initialize
}

void PhysicsList::PrintConfiguration() const
{
  // TODO: stampare la configurazione attiva
}

void PhysicsList::ConstructParticle()
{
  // TODO: costruire le particelle necessarie
}

void PhysicsList::ConstructProcess()
{
  // TODO:
  // 1) AddTransportation()
  // 2) processi EM
  // 3) decadimenti
  // 4) eventuale adronica
  // 5) eventuale ottica
}

void PhysicsList::SetCuts()
{
  SetCutsWithDefault();
}
