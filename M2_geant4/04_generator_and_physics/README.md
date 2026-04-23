# Lezione 4 - Generatori primari e fisica della simulazione
Modulo 2 - Geant4

Questo esempio estende il modulo precedente introducendo due nuovi temi centrali in una simulazione Monte Carlo:

- la **generazione dei primari**;
- la **configurazione della fisica**.

---

## Descrizione della simulazione

La simulazione usa lo stesso calorimetro dei moduli precedenti:

- un **mondo** riempito di aria;
- un **calorimetro** a cristalli di **PbWO4**;
- **12 piani** alternati X/Y;
- **16 barre per piano**;
- dimensioni di ciascuna barra:
  - 2 cm × 32.015 cm × 2 cm;
- gap di **10 μm** tra barre e tra piani.

Lo scoring resta invariato:

- l'energia depositata viene accumulata con la **SteppingAction**;
- per ogni evento si salva un array ROOT:
  - `caloEdep[12][16]`
- si salvano inoltre le variabili del primario realmente generate:
  - `genEnergy`
  - `genX`, `genY`, `genZ`
  - `genTheta`, `genPhi`

---

## Struttura del codice

I file principali coinvolti in questo modulo sono:

### `PrimaryGeneratorAction`

Sostituisce il `G4ParticleGun` con il **G4GeneralParticleSource (GPS)**.

Il GPS:

- genera il primario;
- viene configurato quasi interamente da macro;
- permette di usare distribuzioni di posizione, angolo ed energia molto più generali.

Nel metodo `GeneratePrimaries()` il codice:

1. genera il vertice primario con il GPS;
2. legge dall'evento i parametri effettivamente estratti;
3. passa tali parametri all'`EventAction` per salvarli nel file ROOT.

### `PhysicsList`

In questo modulo la fisica è definita in modo **modulare e completamente esplicito nel codice**.
In simulazioni avanzate si può configurare un Messenger per modificare le liste da macro.

#### Struttura della physics list

La fisica è costruita combinando diversi moduli:

- **fisica elettromagnetica (EM)** -> sempre presente
- **decadimenti**
- **fisica adronica**
- **fisica dei neutroni a bassa energia (HP)**
- **fisica ottica**

Tutti questi blocchi sono controllati tramite variabili booleane nel codice:

```cpp
useHadronic
useNeutronHP
useDecay
useRadioactiveDecay
useOptical
```

---

### Fisica elettromagnetica

È il blocco fondamentale della simulazione.

Descrive le interazioni di:

- elettroni
- positroni
- fotoni

Processi inclusi:

- ionizzazione
- bremsstrahlung
- scattering multiplo
- produzione di coppie
- effetto fotoelettrico
- scattering Compton

#### Opzioni disponibili

Nel codice è possibile scegliere tra diversi modelli:

- `G4EmStandardPhysics` -> configurazione standard
- `G4EmStandardPhysics_option4` -> più accurata (più lenta)
- `G4EmLivermorePhysics` -> migliore a basse energie
- `G4EmPenelopePhysics` -> alternativa dettagliata

---

### Fisica dei decadimenti

Attivata con:

```cpp
useDecay = true
```

Include:

- decadimento di particelle instabili (µ, π, ecc.)

---

### Decadimenti radioattivi

Attivati con:

```cpp
useRadioactiveDecay = true
```

Include:

- decadimenti nucleari
- emissione di particelle e gamma da nuclei instabili

---

### Fisica adronica

Attivata con:

```cpp
useHadronic = true
```

Include:

- interazioni nucleari
- produzione di secondari (neutroni, protoni, mesoni)
- sviluppo di sciami adronici

Modelli utilizzati:

- `FTFP_BERT`:
  - **BERT (Bertini cascade)** a bassa energia
  - **FTF (Fritiof string model)** ad alta energia

---

### Neutroni a bassa energia (HP)

Attivati con:

```cpp
useNeutronHP = true
```

Include:

- sezioni d'urto dettagliate per neutroni lenti
- dati tabulati (cross section realistiche)

Effetti:

- maggiore accuratezza
- aumento del tempo di calcolo

---

### Fisica ottica

Attivata con:

```cpp
useOptical = true
```

Include:

- produzione di fotoni di scintillazione
- trasporto ottico
- assorbimento

Richiede:

- proprietà ottiche dei materiali definite

---

### Neutron tracking cut (opzionale)

Può essere attivato per limitare:

- il tempo di tracking dei neutroni lenti

---

## `RunAction`

Mantiene il file ROOT di output e aggiunge un **messenger minimale**:

```text
/analysis/setFileName nomefile.root
```

Se questo comando non viene usato, il nome di default è:

```text
scoring_calo.root
```

## `main.cc`

In questo file vengono definiti:

- il **generatore random**;
- la geometria;
- la physics list;
- le user action;
- la gestione di macro e visualizzazione.

---

## Generazione dei primari

Nel modulo precedente il primario era generato con `G4ParticleGun`.
In questo modulo usiamo invece il **General Particle Source**.

Il vantaggio principale è che il generatore può essere controllato quasi interamente da macro, lasciando il codice C++ molto pulito.

### Comandi GPS più utili

Di seguito è riportata una selezione ragionata dei comandi GPS più utili per il laboratorio. Lo scopo non è sostituire la documentazione ufficiale, ma fornire un promemoria operativo.

### 1. Scelta della particella

```text
/gps/particle e-
/gps/particle e+
/gps/particle gamma
/gps/particle proton
```

### 2. Posizione della sorgente

#### Sorgente puntiforme

```text
/gps/pos/type Point
/gps/pos/centre 0 0 -25 cm
```

#### Fascio gaussiano

```text
/gps/pos/type Beam
/gps/pos/shape Circle
/gps/pos/centre 0 0 -25 cm
/gps/pos/sigma_x 1 mm
/gps/pos/sigma_y 1 mm
```

#### Sorgente piana

```text
/gps/pos/type Plane
/gps/pos/shape Square
/gps/pos/centre 0 0 -25 cm
/gps/pos/halfx 5 mm
/gps/pos/halfy 5 mm
```

### 3. Distribuzione angolare

#### Direzione fissa

```text
/gps/direction 0 0 1
```

#### Fascio divergente gaussiano

```text
/gps/ang/type beam2d
/gps/ang/sigma_x 1 deg
/gps/ang/sigma_y 1 deg
```

#### Distribuzione isotropa limitata in theta

```text
/gps/ang/type iso
/gps/ang/mintheta 0 deg
/gps/ang/maxtheta 10 deg
/gps/ang/minphi 0 deg
/gps/ang/maxphi 360 deg
```

### 4. Energia

#### Energia monocromatica

```text
/gps/ene/type Mono
/gps/ene/mono 1 GeV
```

#### Distribuzione uniforme

```text
/gps/ene/type Lin
/gps/ene/min 100 MeV
/gps/ene/max 1 GeV
/gps/ene/gradient 0
/gps/ene/intercept 1
```

#### Spettro arbitrario da istogramma

```text
/gps/ene/type Arb
/gps/hist/type arb
/gps/hist/file macro/spectrum_hist.dat
/gps/hist/inter Lin
```

Il comando `/gps/hist/file` legge un file **ASCII** contenente coppie `ascissa peso`; non legge direttamente un istogramma ROOT. Per questo nel modulo è incluso uno script ROOT di conversione. 

---

## Esempio 1: fascio tipo acceleratore

La macro:

```text
macro/beam_accelerator.mac
```

mostra un fascio elettronico:

- quasi collimato;
- monocromatico;
- centrato sull'asse del calorimetro;
- incidente lungo +z.

È l'esempio più vicino a un fascio da linea di acceleratore.

---

## Esempio 2: spettro energetico da istogramma

La macro:

```text
macro/beam_spectrum.mac
```

mostra come usare uno spettro energetico arbitario.

### Passaggio preliminare

Se si dispone di un file ROOT chiamato, ad esempio:

```text
spectrum_hist.root
```

contenente un istogramma `TH1`, è necessario prima convertirlo in un file ASCII leggibile dal GPS.

Nel modulo è incluso lo script:

```text
macro/convert_root_hist_to_gps.C
```

Esempio di uso:

```bash
root -l -q 'macro/convert_root_hist_to_gps.C("spectrum_hist.root", "hSpectrum", "macro/spectrum_hist.dat")'
```

Dopo la conversione, la macro GPS può leggere:

```text
macro/spectrum_hist.dat
```

---

## Random generator

Nel `main.cc` il motore random viene definito esplicitamente:

```cpp
G4Random::setTheEngine(new CLHEP::RanecuEngine);
```

Questo è uno dei possibili generatori random disponibile in Geant4. 

### Alternative possibili

Nel codice sono commentate anche alcune alternative:

```cpp
G4Random::setTheEngine(new CLHEP::MixMaxRng);
G4Random::setTheEngine(new CLHEP::MTwistEngine);
```

Abbiamo visto che generatori diversi hanno caratteristiche (velocità, randomicità etc.) diverse. L'accuratezza della simulazione dipende anche dalla scelta del generatore.

### Random Seeds

Il generatore random ha bisogno di seed da cui partire per la generazione. Una sequenza di numeri random estratti dipende dal seed, e in particolare, usando lo stesso seed, la sequenza di numeri sarà la stessa.  
In Geant4 è possibile usare da macro il comando standard:

```text
/random/setSeeds 12345 67890
```

Questo comando è molto utile per:

- riprodurre un run
- confrontare configurazioni diverse

In particolare quando si lanciano diversi run in modalità parallela (simulazioni molto intense per gli esperimenti), lanciare tutti i run con il seed standard produce tutti file identici. In questo caso avere la possibilità di cambiare il seed da macro è utile per evitare di produrre N volte la stessa simulazione.

---

## Scoring

Lo scoring resta basato su `SteppingAction`.

Per ogni step:

1. si legge l'energia depositata;
2. si controlla se il volume è un cristallo;
3. si ricavano gli indici `piano` e `cristallo` dai copy number;
4. si accumula l'energia nella matrice evento per evento.

Questo approccio è stato mantenuto volutamente perché:

- è semplice da leggere;
- non introduce ancora sensitive detector e hit collections;
- è sufficiente per un primo studio quantitativo del calorimetro.

---

## File ROOT di output

Il file ROOT contiene un `TTree` con:

- `caloEdep[12][16]`
- `genEnergy`
- `genX`, `genY`, `genZ`
- `genTheta`, `genPhi`

Le unità salvate sono:

- energia in **MeV**;
- posizione in **mm**;
- angoli in **gradi**.

---

## Compilazione

```bash
mkdir build
cd build
cmake ..
make
```

---

## Esecuzione

### Modalità interattiva

```bash
./calo
```

### Modalità batch

```bash
./calo macro/run.mac
```

Esempi:

```bash
./calo macro/beam_accelerator.mac
./calo macro/beam_spectrum.mac
./calo macro/run_mult.mac
```
---

## Esercizi proposti

1. Confrontare la deposizione totale per:
   - elettroni;
   - fotoni;
   - positroni.

2. Studiare come cambia il profilo longitudinale cambiando:
   - energia del fascio;
   - posizione iniziale;
   - divergenza angolare.

3. Confrontare i risultati ottenuti con:
   - `standard`
   - `emonly`
   - `livermore`
   - `penelope`

4. Attivare la fisica ottica e verificare come cambia il tempo di calcolo e il numero di tracce secondarie.

5. Creare un proprio spettro energetico in ROOT, convertirlo in formato ASCII e usarlo come input del GPS.

6. Modificare `run_mult.mac` in modo da lanciare più campagne di run con combinazioni diverse di particella, posizione e seed.

