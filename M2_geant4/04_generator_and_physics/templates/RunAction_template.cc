#include "RunAction.hh"

RunAction::RunAction()
{
  // TODO:
  // creare qui un messenger minimale con il comando
  //   /analysis/setFileName nome.root
}

RunAction::~RunAction()
{
  // TODO: deallocare messenger e file di output
}

void RunAction::SetOutputFileName(const G4String&)
{
  // TODO: salvare il nome del file impostato da macro
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  // TODO: aprire il file di output
}

void RunAction::EndOfRunAction(const G4Run*)
{
  // TODO: scrivere e chiudere il file di output
}

void RunAction::Book()
{
  // TODO:
  // 1) aprire il file ROOT
  // 2) creare il TTree
  // 3) definire le branch del primario e dello scoring
}
