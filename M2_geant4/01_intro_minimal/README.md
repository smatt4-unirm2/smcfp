# Lezione 1 - Introduzione a Geant4
Modulo 2 - Geant4

Questo esempio ha lo scopo di introdurre la struttura **minima** di un'applicazione Geant4. Include:

- come è organizzato un codice Geant4;
- quali sono le classi fondamentali;
- come costruire una geometria semplice;
- come generare particelle;
- come compilare ed eseguire una simulazione;
- come usare macro e interfaccia grafica.

---

## Descrizione della simulazione

La simulazione rappresenta un caso molto semplice:

- un **mondo** (World) riempito di aria;
- un **blocco cubico di PbWO4** al centro;
- un **elettrone da 1 GeV** sparato lungo l'asse z.

---

## Struttura del codice

Il progetto contiene le seguenti classi:

- `main.cpp`
- `DetectorConstruction`
- `ActionInitialization`
- `PrimaryGeneratorAction`
- `RunAction`

Queste rappresentano il **minimo indispensabile** per una simulazione Geant4.

### 1. DetectorConstruction

Responsabile di:

- definire i **materiali**;
- costruire la **geometria**;
- posizionare i volumi.

Nel codice:

- World: cubo di 1 m (aria)
- Absorber: cubo di 20 cm (tungsteno)

Modifica questa classe per cambiare geometria o materiali.

### 2. PrimaryGeneratorAction

Definisce la particella primaria:

- tipo: elettrone (`e-`)
- energia: 1 GeV
- posizione: (0, 0, -40 cm)
- direzione: lungo +z

Modifica qui per cambiare fascio o sorgente.

### 3. ActionInitialization

Collega tutte le azioni utente:

- PrimaryGeneratorAction
- RunAction

Punto centrale dove registrare nuove azioni.

---

### ➕ Estensione: EventAction e SteppingAction

Per introdurre lo **scoring**, sono state aggiunte due classi:

- `EventAction`
- `SteppingAction`

Queste permettono di raccogliere osservabili fisiche durante la simulazione.

---

### 4. RunAction

Eseguita:

- all'inizio del run
- alla fine del run

Utilizzata per:

- output su file
- analisi dati
- istogrammi

---

### 5. main.cpp

Gestisce:

- run manager (modalità seriale)
- inizializzazione della fisica (`FTFP_BERT`)
- visualizzazione
- macro o sessione interattiva

### 6. Fisica utilizzata

Physics list:

```
FTFP_BERT
```
Utilizzata per simulazioni generiche, che include, nelle interazioni inelastiche adrone-nucleo il modello Fritiof Parton (FTF) e il modello Bertini.

---

## Scoring e output ROOT

La simulazione produce un file `scoring.root` contenente un **TTree** con una entry per evento.
Variabili salvate:

- `eventID` : identificatore evento
- `primaryE_MeV` : energia del primario
- `edep_MeV` : energia totale depositata nel blocco
- `edepHad_MeV` : energia depositata da adroni
- `edepEle_MeV` : energia depositata da elettroni/positroni
- `edepGam_MeV` : energia depositata da gamma
- `trackLength_mm` : lunghezza totale delle tracce cariche
- `nStepsAbs` : numero di step nel blocco
- `nSecondaries` : numero di secondarie prodotte
- `maxStepEdep_MeV` : massimo deposito in un singolo step

Lo scoring è implementato tramite:

- `SteppingAction`: misura quantità locali ad ogni step
- `EventAction`: accumula e salva i risultati per evento


Attenzione: il file ha un nome di default, in questo caso, poiché si utilizza il `G4AnalysisManager` per gestire il file ROOT, il nome può essere cambiato da macro con il comando 

```txt
/analysis/setFileName file_name #senza estensione .root, gestita da un altro comando
```

Vedremo nella lezione 03_scoring che questo non è l'unico modo per gestire i file di output.
Ovviamente se il nome del file non viene cambiato tra una macro e l'altra, questo viene sovrascritto. 

---

## Compilazione

Il progetto Geant4 utilizza CMake per gestire la compilazione del progetto. Analizzare il file 
`CMakeLists.txt` per capire com'è costruito.

Per compilare il progetto:
```bash
mkdir build #costruisce la cartella di build in cui verranno messi tutti i file 
cd build    #mi posiziono nella cartella di build
cmake ..    #invoco CMake (.. indica la cartella dove si trova il CMakeLists principale)
make        #il CMake costruisce un makefile con tutte le dipendenze richieste, il comando make compila
```
---

## Esecuzione

### Modalità interattiva (GUI)

Dalla cartella di build eseguire:

``` bash
./calo
```

Avvia il programma ed esegue automaticamente:

```
macro/init_vis.mac
```

che è la macro per impostare la visualizzazione. Questa modalità permette di accedere al visualizzatore OpenGL, utile per fare debug della geometria e per visualizzare pochi eventi (e.g. per figure nei paper).

### Modalità batch (macro)

Le macro sono comandi in sequenza che danno istruzioni a Geant per eseguire la simulazione. Controllare l'esempio macro/run.mac per alcuni dei comandi.

Esempio:

```txt
/run/initialize             # Inizializza geometria e fisica
/vis/open OGL               # Apre la finestra grafica (OpenGL)
/vis/drawVolume             # Disegna la geometria
/run/beamOn 10              # Lancia 10 eventi
```

Per eseguire il programma in batch mode (senza avviare la visualizzazione) con una macro, usare:

``` bash
./calo run.mac
```
In questo modo il programma esegue le istruzioni e simula il numero di particelle specificate dal comando `/run/beamOn`.

Nota: una macro si può eseguire anche dal visualizzatore con

```
/control/execute path/to/macro.mac
```

questo è possibile quando nella macro ci sono pochi eventi o ci sono solo istruzioni semplici e.g. impostare la visualizzazione o il generatore.

### Comandi aggiuntivi di setup

```txt
# --- Visualizzazione ---

/vis/viewer/set/style surface        # Mostra superfici solide
/vis/viewer/set/style wireframe      # Mostra solo i bordi (utile per debug)
/vis/viewer/zoom 1.5                 # Zoom della scena
/vis/viewer/set/viewpointThetaPhi 70 20   # Cambia punto di vista

# --- Scena ---

/vis/scene/add/axes 0 0 0 20 cm      # Aggiunge assi cartesiani
/vis/scene/add/trajectories smooth   # Mostra traiettorie delle particelle

# --- Traiettorie ---

/vis/modeling/trajectories/create/drawByCharge   # Colora per carica
/vis/modeling/trajectories/drawByCharge-0/set 0 green   # Neutre verdi
/vis/modeling/trajectories/drawByCharge-0/set -1 red   # Negativi rossi
/vis/modeling/trajectories/drawByCharge-0/set 1 blue  # Positivi blu

# --- Controllo run ---

/run/beamOn 100              # Lancia 100 eventi
/run/verbose 1               # Output base del run
/event/verbose 1             # Output per evento
/tracking/verbose 1          # Output dettagliato delle tracce (molto verboso!)

# --- Controllo geometria ---

/geometry/test/run           # Test automatico della geometria
/geometry/test/verbose 1     # Livello di dettaglio del test

# --- Aggiornamento scena ---

/vis/viewer/flush            # Forza aggiornamento della finestra grafica
```

Attenzione: `/tracking/verbose` produce molto output che rallenta la simulazione, è utile solo per debug rapidi in situazioni di test

### Comandi utili per la sorgente (/gun)
Nota: funzionano solo quando si utilizza G4ParticleGun, approfondiremo nella lezione sui generatori primari

Questi comandi permettono di controllare il **particle gun direttamente da macro**, senza modificare il codice.

```txt
# Tipo di particella
/gun/particle e-              # Elettrone
/gun/particle gamma           # Fotone
/gun/particle mu-             # Muone

# Energia
/gun/energy 1 GeV             # Imposta energia
/gun/energy 500 MeV

# Posizione iniziale
/gun/position 0 0 -40 cm      # Posizione della sorgente

# Direzione del momento
/gun/direction 0 0 1          # Lungo asse z
/gun/direction 1 0 0          # Lungo asse x

# Numero di particelle per evento
/gun/number 1

# Polarizzazione (opzionale)
/gun/polarization 0 1 0
```
---

## Analisi con ROOT

Aprire il file:

```bash
root scoring.root
```

Comandi base:

```C++
t->Print();                      // struttura del tree
t->Scan();                       // stampa eventi
```

Esempi di plot:

```C++
t->Draw("edep_MeV");             // energia totale
t->Draw("edepEle_MeV");          // componente elettromagnetica
t->Draw("edepHad_MeV");          // componente adronica
t->Draw("trackLength_mm");       // lunghezza tracce
```

Correlazioni:

```C++
t->Draw("edepEle_MeV:edepHad_MeV");
```

Istogramma con binning:

```C++
t->Draw("edep_MeV>>h(100,0,1000)");
```

## Esercizi

- compilare ed eseguire la simulazione
- familiarizzare con il visualizzatore
- lanciare una particella di prova
- lanciare una macro esempio
- cambiare posizione, energia, particella
- lanciare una macro con due test a energia diversa
- aprire scoring.root con ROOT
- esplorare il tree (t->Print())
- fare l’istogramma di edep_MeV
- confrontare edepEle_MeV e edepHad_MeV per un fascio di protoni e di elettroni
