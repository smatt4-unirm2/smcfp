# Lezione 2 - Geometria modulare di un calorimetro in Geant4
Modulo 2 - Geant4

Questo esempio introduce la costruzione **modulare** di un rivelatore in Geant4.

L'obiettivo è costruire un **calorimetro a barre di PbWO4** organizzato in:

- cristalli (unità base)
- piani (insiemi di cristalli)
- calorimetro (insieme di piani alternati)

---

## Descrizione della simulazione

La geometria implementata è la seguente:

- un **mondo** riempito di aria;
- un **calorimetro** centrato nel mondo;
- il calorimetro è composto da:
  - **12 piani**
  - ogni piano contiene **16 barre di PbWO4**
- dimensioni di ogni barra:
  - 2 cm x 32.015 cm x 2 cm
- i piani sono alternati:
  - piano X barre lungo y, segmentazione in x
  - piano Y barre ruotate di 90 deg, segmentazione in y
- gap tra barre e tra piani: **10 μm**

---

## Struttura del codice

La costruzione segue lo schema tipico Geant4:

### DetectorConstruction

- `Construct()`
  - chiama:
    - `DefineMaterials()`
    - `DefineVolumes()`

### DefineMaterials()

- definisce:
  - aria (`G4_AIR`)
  - PbWO4 (`G4_PbWO4`)
- opzionalmente:
  - proprietà ottiche del PbWO4

### DefineVolumes()

Costruisce la geometria in modo gerarchico:

```
World
 └── Calorimeter
       ├── PlaneX
       │     ├── Crystal
       │     ├── Crystal
       │     └── ...
       ├── PlaneY
       │     ├── Crystal (ruotato)
       │     └── ...
       └── ...
```

---

### Separazione materiali / geometria

- `DefineMaterials()` -> materiali
- `DefineVolumes()` -> geometria

Questo è lo stile raccomandato negli esempi ufficiali.

---

### Riutilizzo dei volumi

- un solo `LogicalVolume` per il cristallo
- riutilizzato in:
  - piano X
  - piano Y (ruotato)

---

### Rotazioni

Per i piani Y:

```cpp
fRotZ90 = new G4RotationMatrix();
fRotZ90->rotateZ(90.*deg);
```

---

### Costruzione modulare

- cristallo → `G4Box`
- piano → contenitore con loop
- calorimetro → loop sui piani

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
./calo run.mac
```

---

## Esercizi

1. Cambiare il numero di piani
2. Cambiare il numero di barre per piano
3. Aumentare il gap
4. Cambiare materiale (es. G4_Pb)
5. Rendere visibili i piani
6. Aggiungere proprietà ottiche più realistiche

---

## Nota

In questa fase:
- **nessuno scoring**
- focus solo su geometria e visualizzazione
