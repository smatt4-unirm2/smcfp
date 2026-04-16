# Lezione 1 - Introduzione a Geant4
Modulo 2 - Geant4

Questo esempio ha lo scopo di introdurre la struttura **minima** di un'applicazione Geant4, ma con una geometria un po' più ricca del caso precedente. Include:

- come è organizzato un codice Geant4;
- quali sono le classi fondamentali;
- come costruire una geometria modulare;
- come definire materiali e proprietà ottiche;
- come ripetere volumi e usare rotazioni;
- come compilare ed eseguire una simulazione;
- come usare macro e interfaccia grafica.

---

## Descrizione della simulazione

La simulazione rappresenta un calorimetro segmentato molto semplice, costruito con una struttura modulare:

- un **mondo** (World) riempito di aria;
- un **cristallo** elementare di **PbWO4**;
- un **piano** formato da **16 barre**;
- un **calorimetro** formato da **12 piani**;
- i piani sono disposti in modo alternato: **X, Y, X, Y, ...**;
- tra barre adiacenti e tra piani adiacenti è presente un **gap di 10 um**;
- la particella primaria è un **elettrone da 1 GeV** sparato lungo l'asse z.

Le dimensioni della singola barra sono:

- **2 cm** lungo x;
- **32.015 cm** lungo y;
- **2 cm** lungo z.

I piani "Y" sono ottenuti ruotando le barre di **90 gradi attorno all'asse z**.

---

## Struttura del codice

Il progetto contiene le seguenti classi:

- `main.cc`
- `DetectorConstruction`
- `ActionInitialization`
- `PrimaryGeneratorAction`
- `RunAction`

Queste rappresentano il **minimo indispensabile** per una simulazione Geant4 eseguibile in cui il focus sia sulla geometria.

### 1. DetectorConstruction

Responsabile di:

- definire i **materiali**;
- definire eventuali **proprietà ottiche**;
- costruire la **geometria**;
- posizionare i volumi;
- impostare la **visualizzazione**.

In questo esempio la costruzione è organizzata in modo modulare:

- `BuildCrystal()`
- `BuildPlaneX()`
- `BuildPlaneY()`
- `BuildCalorimeter()`

Questa scelta è molto utile didatticamente perché separa chiaramente i diversi livelli della geometria.

#### Geometria costruita

- **World**: cubo di 1 m riempito di aria
- **Crystal**: barra di PbWO4
- **Plane X**: 16 barre distribuite lungo x
- **Plane Y**: 16 barre ruotate di 90° e distribuite lungo y
- **Calorimeter**: 12 piani alternati lungo z

#### Materiali

Nel codice vengono usati:

- `G4_AIR` per il mondo e per i gap;
- `G4_PbWO4` per i cristalli.

#### Proprietà ottiche

Nel `DetectorConstruction` è presente un'opzione booleana per assegnare al PbWO4 una `G4MaterialPropertiesTable` con proprietà ottiche di base:

- indice di rifrazione (`RINDEX`)
- lunghezza di assorbimento (`ABSLENGTH`)
- spettro di scintillazione (`SCINTILLATIONCOMPONENT1`)
- yield e costanti temporali di scintillazione

Questa parte è pensata per poter introdurre, quando serve, anche la propagazione dei fotoni ottici.

#### Visualizzazione

La visualizzazione è stata migliorata usando `G4VisAttributes`:

- cristalli **azzurrini e trasparenti**;
- volumi madre intermedi **invisibili**;
- gap tra cristalli lasciati visibili solo come spazio vuoto.

### 2. PrimaryGeneratorAction

Definisce la particella primaria:

- tipo: elettrone (`e-`)
- energia: 1 GeV
- posizione: `(0, 0, -25 cm)`
- direzione: lungo `+z`

Modifica qui per cambiare fascio o sorgente.

### 3. ActionInitialization

Collega tutte le azioni utente:

- `PrimaryGeneratorAction`
- `RunAction`

In questa versione **non** sono presenti classi di scoring dedicate, perché il focus dell'esercizio è soltanto la costruzione della geometria.

### 4. RunAction

Eseguita:

- all'inizio del run
- alla fine del run

In questa versione stampa solo messaggi di start/end run.

### 5. main.cc

Gestisce:

- run manager;
- inizializzazione della fisica;
- eventuale attivazione della fisica ottica;
- visualizzazione;
- macro o sessione interattiva.

### 6. Fisica utilizzata

Physics list di base:

```txt
FTFP_BERT
```

Quando `kEnableOpticalPhysics = true` in `main.cc`, viene anche registrata:

```txt
G4OpticalPhysics
```

Questo permette di usare le proprietà ottiche definite nel PbWO4.

---

## Geometria: idee importanti da osservare nel codice

Questo esempio è utile per introdurre alcuni concetti geometrici fondamentali di Geant4.

### Volume solido, logico e fisico

Per ogni elemento si distinguono tre livelli:

- **solido**: definisce la forma (`G4Box`)
- **volume logico**: associa materiale e attributi
- **volume fisico**: posiziona il volume dentro un volume madre

### Volumi madre e volumi figli

La gerarchia usata è:

```txt
World
└── Calorimeter
    ├── Plane X
    │   └── 16 Crystal
    └── Plane Y
        └── 16 Crystal
```

Naturalmente nel calorimetro reale i piani X e Y si alternano lungo z.

### Ripetizione di volumi

Le barre vengono create una sola volta come volume logico e poi posizionate più volte con un ciclo `for`.

Questo è un punto essenziale da far capire agli studenti:

- la geometria è **modulare**;
- i volumi logici possono essere **riusati**;
- ciò che cambia tra i diversi posizionamenti è soprattutto:
  - la posizione;
  - eventualmente la rotazione;
  - il `copy number`.

### Rotazioni

Per ottenere i piani ortogonali si usa una `G4RotationMatrix` ruotata di 90° attorno a z.

Questo permette di riusare la stessa barra anche per il piano orientato nell'altra vista.

---

## Compilazione

Il progetto Geant4 utilizza CMake per gestire la compilazione del progetto. Analizzare il file
`CMakeLists.txt` per capire com'è costruito.

Per compilare il progetto:
```bash
mkdir build
cd build
cmake ..
make
```

---

## Esecuzione

### Modalità interattiva (GUI)

Dalla cartella di build eseguire:

```bash
./calo
```

Avvia il programma ed esegue automaticamente:

```txt
macro/init_vis.mac
```

Questa modalità è molto utile per:

- controllare se la geometria è costruita correttamente;
- verificare rotazioni e spaziature;
- fare debug dei volumi madre e figli;
- visualizzare pochi eventi.

### Modalità batch (macro)

Le macro sono comandi in sequenza che danno istruzioni a Geant per eseguire la simulazione. Controllare l'esempio `macro/run.mac` per alcuni dei comandi.

Esempio:

```txt
/run/initialize             # Inizializza geometria e fisica
/vis/open OGL               # Apre la finestra grafica
/vis/drawVolume             # Disegna la geometria
/run/beamOn 10              # Lancia 10 eventi
```

Per eseguire il programma in batch mode con una macro, usare:

```bash
./calo macro/run1.mac
```

Nota: una macro si può eseguire anche dal visualizzatore con

```txt
/control/execute path/to/macro.mac
```

### Comandi aggiuntivi di setup

```txt
# --- Visualizzazione ---

/vis/viewer/set/style surface              # Mostra superfici solide
/vis/viewer/set/style wireframe            # Mostra solo i bordi
/vis/viewer/zoom 1.5                       # Zoom della scena
/vis/viewer/set/viewpointThetaPhi 70 20    # Cambia punto di vista

# --- Scena ---

/vis/scene/add/axes 0 0 0 20 cm            # Aggiunge assi cartesiani
/vis/scene/add/trajectories smooth         # Mostra traiettorie lisce
/tracking/storeTrajectory 1                # Salva le traiettorie degli eventi

# --- Generatore ---

/gun/particle e-                           # Seleziona elettrone
/gun/particle gamma                        # Seleziona fotone gamma
/gun/energy 500 MeV                        # Imposta energia del fascio
/gun/position 0 0 -30 cm                   # Sposta la sorgente
/gun/direction 0 0 1                       # Direzione lungo +z

# --- Run ---

/run/printProgress 100                     # Stampa progresso ogni 100 eventi
/run/beamOn 1000                           # Esegue 1000 eventi
```

---

## Template per gli studenti

Nel progetto è presente anche una cartella:

```txt
templates/
```

con un file `DetectorConstruction_template.cc/.hh` pensato come scheletro iniziale.

L'idea è far partire gli studenti da una struttura già organizzata, ma con i punti principali ancora da completare:

- definizione del mondo;
- costruzione del cristallo;
- costruzione dei piani;
- costruzione del calorimetro;
- aggiunta delle proprietà ottiche;
- impostazione della visualizzazione.

---

## Esercizi proposti

### Esercizio 1
Modificare il numero di barre per piano, portandolo da 16 a 20.

Domande guida:

- come cambia la dimensione trasversa del piano?
- il calorimetro entra ancora comodamente nel mondo?

### Esercizio 2
Modificare il gap tra i cristalli da 10 um a 100 um.

Osservare:

- come cambia la geometria totale;
- quanto diventano visibili i gap nel viewer.

### Esercizio 3
Cambiare le dimensioni della barra, ad esempio aumentando lo spessore lungo z.

Domande guida:

- quanto cresce la profondità totale del calorimetro?
- occorre modificare la posizione iniziale del fascio?

### Esercizio 4
Sostituire il materiale dei cristalli con un altro materiale NIST e confrontare la densità.

Esempi possibili:

- `G4_PbWO4`
- `G4_BGO`
- `G4_CESIUM_IODIDE`

### Esercizio 5
Disattivare le proprietà ottiche mettendo il flag booleano a `false` e confrontare il comportamento del programma.

### Esercizio 6
Aggiungere un involucro esterno al calorimetro, ad esempio una sottile scatola di alluminio.

Suggerimento:

- creare un nuovo volume logico madre attorno al calorimetro;
- lasciare il calorimetro attuale come volume figlio.

### Esercizio 7
Provare a colorare in modo diverso i piani X e i piani Y.

Questo esercizio è utile per capire meglio la differenza tra:

- riuso dello stesso volume logico;
- necessità di creare volumi logici distinti quando si vogliono attributi grafici diversi.

---

## Osservazioni finali

Questo esempio è pensato come passaggio intermedio tra:

- una simulazione con un singolo blocco omogeneo;
- una simulazione più realistica con rivelatori segmentati e scoring.

L'obiettivo principale qui è capire bene:

- la gerarchia dei volumi;
- il riuso di solidi e volumi logici;
- le rotazioni;
- la costruzione parametrica della geometria;
- la separazione tra geometria, generatore e azioni utente.

Una volta compresa bene questa parte, si potrà reintrodurre in modo naturale anche la parte di scoring.
