# Lezione 3 - Scoring 
Modulo 2 - Geant4

Questo esempio estende il modulo `02_geometry_calo` aggiungendo **lo scoring**, senza modificare la geometria, il generatore primario o i file macro di base.

L'obiettivo didattico è mostrare come registrare in un file ROOT, **evento per evento**, l'energia totale depositata in ciascun cristallo del calorimetro usando una catena di user action molto semplice:

- `PrimaryGeneratorAction`
- `EventAction`
- `SteppingAction`
- `RunAction`

Lo scoring viene invece effettuato direttamente nello `SteppingAction`, che legge il volume attraversato a ogni step e accumula l'energia depositata nel corrispondente elemento del calorimetro.

---

## Descrizione della simulazione

La geometria è quella del modulo precedente:

- un **mondo** riempito di aria;
- un **calorimetro** centrato nel mondo;
- **12 piani** lungo l'asse `z`;
- ogni piano contiene **16 barre di PbWO4**;
- ogni barra ha dimensioni:
  - `2 cm × 32.015 cm × 2 cm`;
- i piani sono alternati:
  - **piano X**: barre lunghe lungo `y`, segmentazione lungo `x`;
  - **piano Y**: barre ruotate di 90° attorno a `z`, segmentazione lungo `y`;
- il gap tra barre e tra piani è di **10 μm**.

La novità di questo modulo è il salvataggio dei dati di scoring in un file ROOT.

---

## Cosa viene scritto nel file ROOT

Per ogni evento viene creato un record nel `TTree` chiamato `t`, contenuto nel file:

```txt
scoring_calo.root
```

ATTENZIONE: il nome del file è hardcoded nel `RunAction.cc`, quindi viene sovrascritto ogni volta che si esegue la simulazione e non è possibile cambiarlo. Cambiare il nome tra una simulazione e l'altra per evitarlo

Le informazioni salvate sono:

- `caloEdep[12][16]`
- `genEnergy`
- `genX`
- `genY`
- `genZ`
- `genTheta`
- `genPhi`

### Significato della matrice `Edep`

La branch principale è:

```cpp
caloEdep[12][16]
```

che rappresenta l'energia totale depositata in ciascun cristallo durante un singolo evento.

La convenzione adottata è:

- `piano = 0, ..., 11` nel verso positivo di `z`;
- `cristallo = 0, ..., 15`
  - nel verso positivo di `x` per i piani X;
  - nel verso positivo di `y` per i piani Y.

Questa convenzione coincide con la struttura geometrica definita nel `DetectorConstruction`, dove:

- il **copy number del piano** viene assegnato nel loop di posizionamento dei piani nel calorimetro;
- il **copy number del cristallo** viene assegnato nel loop di posizionamento delle barre nel piano.

Di conseguenza, lo `SteppingAction` può ricostruire gli indici geometrici direttamente dalla gerarchia dei volumi (`touchable history`) senza bisogno di hit collection.

> Nota importante: nella geometria ci sono **12 piani** e **16 cristalli per piano**, quindi gli indici validi sono `0..11` e `0..15`.

---

## Struttura del codice

### `PrimaryGeneratorAction`

Questa classe continua a definire il fascio primario, esattamente come nel modulo precedente.

In aggiunta, prima di generare il vertice primario, salva nell'`EventAction` i parametri del primario corrente:

- energia;
- posizione iniziale;
- angoli `theta` e `phi` della direzione iniziale.

In questo modo tutte le informazioni evento per evento vengono raccolte nello stesso contenitore logico.

---

### `EventAction`

Questa è la classe centrale per l'accumulo dei dati di un singolo evento.

Contiene:

- una matrice `fEdep[12][16]`;
- le variabili del primario;
- i metodi per:
  - azzerare i dati all'inizio di ogni evento;
  - sommare contributi di energia depositata;
  - passare i risultati finali alla `RunAction`.

Dal punto di vista concettuale, `EventAction` svolge il ruolo di **contenitore per gli osservabili dell'evento**.

---

### `SteppingAction`

Questa classe implementa lo scoring vero e proprio.

Per ogni step:

1. legge l'energia depositata con:

```cpp
step->GetTotalEnergyDeposit()
```

2. scarta gli step con energia nulla;
3. identifica il volume corrente usando il `PreStepPoint`;
4. verifica che il volume sia uno dei cristalli del calorimetro;
5. ricava:
   - l'indice del cristallo dal `copy number` del volume corrente;
   - l'indice del piano dal `copy number` del volume madre;
6. aggiunge il contributo nella cella corretta della matrice `Edep[piano][cristallo]`.

Questo è il passaggio chiave del modulo: la geometria non serve solo a visualizzare il rivelatore, ma fornisce anche l'informazione necessaria per costruire una struttura dati fisicamente interpretabile.

---

### `RunAction`

La `RunAction` si occupa della scrittura su file ROOT.

All'inizio del run:

- apre il file `calo_scoring.root`;
- crea il `TTree` `t`;
- definisce le branch.

Alla fine di ogni evento, tramite il metodo `FillEvent(...)`, copia nei buffer del tree:

- la matrice `Edep[12][16]`;
- i parametri del primario.

Alla fine del run:

- scrive il tree su disco;
- chiude il file ROOT.

---

### `ActionInitialization`

Questa classe costruisce e collega tra loro le user action.

L'ordine logico è il seguente:

1. `RunAction`
2. `EventAction`
3. `PrimaryGeneratorAction`
4. `SteppingAction`

In particolare:

- `PrimaryGeneratorAction` riceve un puntatore a `EventAction`, così può salvare le variabili del primario;
- `SteppingAction` riceve un puntatore a `EventAction`, così può aggiornare la matrice delle energie depositate;
- `EventAction` riceve un puntatore a `RunAction`, così può chiedere la scrittura dei dati a fine evento.

Questa architettura è volutamente semplice e molto utile da discutere a lezione, perché mostra chiaramente il flusso delle informazioni in una simulazione Geant4.

---

## Compilazione

Questo modulo usa Geant4 e ROOT.

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

Al termine del run verrà prodotto il file:

```txt
calo_scoring.root
```

nella directory da cui si esegue il programma.

---

## Esempio di lettura del file ROOT

Da ROOT si può controllare il contenuto del tree con:

```cpp
TFile* f = TFile::Open("calo_scoring.root");
TTree* t = (TTree*)f->Get("t");
t->Print();
```

Per esempio, per leggere l'energia depositata nel cristallo `3` del piano `5`:

```cpp
t->Draw("Edep[5][3]");
```

Per vedere la somma di tutta l'energia depositata nel calorimetro in un evento si può costruire una variabile derivata in analisi, sommando tutti gli elementi della matrice.

---

## Commento sulla numerazione

La numerazione dei cristalli non viene assegnata “a mano” in un file esterno, ma deriva direttamente dal modo in cui i volumi sono posizionati.

### Piani

Nel `DetectorConstruction` i piani sono creati in un loop del tipo:

```cpp
for (G4int iPlane = 0; iPlane < fNofPlanes; ++iPlane)
```

Il `copy number` associato a quel posizionamento coincide quindi con l'indice del piano nella matrice `Edep`.

### Cristalli

All'interno di ciascun piano, i cristalli sono anch'essi creati in un loop:

```cpp
for (G4int iBar = 0; iBar < fNofBarsPerPlane; ++iBar)
```

Il `copy number` del cristallo coincide quindi con l'indice del secondo asse della matrice `Edep`.

---

## File principali aggiunti in questo modulo

Rispetto al modulo `02_geometry_calo`, i file nuovi o modificati per lo scoring sono:

### Nuovi file

- `include/EventAction.hh`
- `src/EventAction.cc`
- `include/SteppingAction.hh`
- `src/SteppingAction.cc`

### File modificati

- `include/PrimaryGeneratorAction.hh`
- `src/PrimaryGeneratorAction.cc`
- `include/RunAction.hh`
- `src/RunAction.cc`
- `include/ActionInitialization.hh`
- `src/ActionInitialization.cc`
- `CMakeLists.txt`

La geometria (`DetectorConstruction`) rimane invariata.

---

## Macro ROOT di analisi

Oltre al controllo diretto del `TTree` da prompt ROOT, l'esempio include una macro di analisi più completa:

```txt
analysis_calo.C
```

La macro legge il file ROOT prodotto dalla simulazione, recupera il tree `t` e costruisce alcune visualizzazioni utili per interpretare il contenuto fisico dello scoring.

La macro può essere eseguita da ROOT con:

```bash
root -l analysis_calo.C
```

oppure, specificando esplicitamente il file da analizzare:

```cpp
root -l
.L analysis_calo.C
analysis_calo("calo_scoring.root")
```

Il nome del file passato alla funzione deve coincidere con il file ROOT effettivamente prodotto dalla simulazione.

---

### Controlli sul generatore primario

La prima parte della macro visualizza le variabili salvate dal generatore primario:

- distribuzione dell'energia generata;
- distribuzione spaziale iniziale nel piano `x-y`;
- distribuzione dell'angolo polare `theta`;
- distribuzione dell'angolo azimutale `phi`.

Questi istogrammi sono utili per verificare che il fascio simulato corrisponda alla configurazione attesa.

---

### Energia totale depositata

La macro costruisce un istogramma dell'energia totale depositata evento per evento:

```cpp
TH1D* hTotE
```

Per ogni evento, tutti gli elementi della matrice

```cpp
caloEdep[12][16]
```

vengono sommati, il risultato rappresenta l'energia totale rilasciata nel calorimetro in un singolo evento.

---

### Profilo longitudinale dello sciame

La macro calcola anche l'energia media depositata in ciascun piano:

```cpp
TH1D* hPlane
```

Per ogni evento, l'energia dei 16 cristalli appartenenti allo stesso piano viene sommata. Successivamente, il contenuto dell'istogramma viene normalizzato al numero totale di eventi. Il risultato è una stima del profilo longitudinale medio dello sciame.

---

### Distribuzione laterale dello sciame

La macro studia anche la distribuzione laterale dell'energia nei cristalli di ciascun piano.

Per ogni piano e per ogni evento vengono calcolate due quantità:

```cpp
hSigmaVsPlane
hNhitsVsPlane
```

La prima rappresenta una larghezza laterale media dello sciame, calcolata come deviazione standard pesata con l'energia depositata nei cristalli. La seconda rappresenta il numero medio di cristalli colpiti per piano, usando una soglia minima di energia `threshold = 0.1 MeV`

---

### Mappe 2D dei piani pari e dispari

Poiché la geometria del calorimetro alterna piani con barre orientate lungo direzioni diverse, la macro costruisce due mappe 2D separate:

```cpp
TH2D* hMapEven
TH2D* hMapOdd
```

I piani pari e dispari vengono trattati separatamente:

- i piani pari rappresentano una proiezione trasversale;
- i piani dispari rappresentano la proiezione ortogonale.

Le mappe mostrano l'energia media depositata in funzione di:

- indice del cristallo;
- indice del piano.

---

### Pseudo-ricostruzione 3D dello sciame

L'ultima parte della macro costruisce una visualizzazione tridimensionale approssimata dello sciame:

```cpp
TH3D* h3_voxel
```

La ricostruzione usa coppie di piani consecutivi:

- un piano pari;
- il piano dispari successivo.

Poiché i due piani misurano proiezioni trasversali ortogonali, la macro combina le energie medie dei due piani per costruire una distribuzione voxelizzata:

```cpp
E3 = Ey * Ez
```

Questa non è una ricostruzione calorimetrica rigorosa, ma solo una visualizzazione.
Il grafico finale viene disegnato con:

```cpp
h3_voxel->Draw("BOX2Z");
```

