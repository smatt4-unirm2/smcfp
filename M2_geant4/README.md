# SMCFP — Lezioni Geant4

Questo repository contiene quattro lezioni progressive sull'uso di **Geant4** per la simulazione di rivelatori, rivolte a dottorandi di fisica.

Ogni cartella è un progetto Geant4 indipendente con il proprio `CMakeLists.txt`, sorgenti e macro.

---

## Struttura

| Cartella | Contenuto |
|---|---|
| `01_intro_minimal` | Struttura minima di un'applicazione Geant4: world, singolo blocco di PbWO₄, particle gun |
| `02_geometry_calo` | Geometria modulare: calorimetro a barre di PbWO₄, 12 layer × 16 barre |
| `03_scoring_calo` | Sensitive detector e raccolta degli hit integrati |
| `04_gps_physics` | General Particle Source (GPS) e confronto di physics list |

Ogni lezione ha un proprio `README.md` con la descrizione dettagliata della simulazione e istruzioni di compilazione.

---

## Requisiti

- CMake ≥ 3.16
- Compilatore C++17 (GCC ≥ 9 o Clang ≥ 10)
- Geant4 ≥ 11.x installato e configurato (vedi sezione successiva)

---

## Installazione di Geant4

Il modo più rapido è compilare Geant4 dai sorgenti con le opzioni minime necessarie.

```bash
# Scarica i sorgenti (sostituisci la versione se necessario)
wget https://gitlab.cern.ch/geant4/geant4/-/archive/v11.2.2/geant4-v11.2.2.tar.gz
tar xzf geant4-v11.2.2.tar.gz
cd geant4-v11.2.2

# Configura la build
cmake -B build \
  -DCMAKE_INSTALL_PREFIX=$HOME/geant4 \
  -DGEANT4_INSTALL_DATA=ON \
  -DGEANT4_USE_QT=ON \
  -DGEANT4_USE_OPENGL_X11=ON \
  -DCMAKE_BUILD_TYPE=Release

# Compila e installa (usa tutti i core disponibili)
cmake --build build --parallel $(nproc)
cmake --install build
```

> **Nota:** `GEANT4_INSTALL_DATA=ON` scarica automaticamente i dataset di fisica (~2 GB). Su cluster, è preferibile usare dati già installati e puntarli con `GEANT4_DATA_DIR`.

---

## Sourcing dell'ambiente Geant4

Prima di compilare o eseguire qualsiasi progetto, è necessario caricare le variabili d'ambiente di Geant4. Va fatto **in ogni nuova sessione di shell**:

```bash
source $HOME/geant4/bin/geant4.sh        # bash/zsh
```

oppure, se usi `csh/tcsh`:

```bash
source $HOME/geant4/bin/geant4.csh
```

Per evitare di ripeterlo ogni volta, aggiungilo al tuo `.bashrc` o `.zshrc`:

```bash
echo 'source $HOME/geant4/bin/geant4.sh' >> ~/.bashrc
```

Verifica che tutto sia a posto con:

```bash
geant4-config --version
```

---

## Compilare ed eseguire una lezione

```bash
cd 01_intro_minimal
cmake -B build -DGeant4_DIR=$(geant4-config --prefix)/lib/Geant4-*/
cmake --build build --parallel $(nproc)
cd build
./calo macro/run1.mac
```

Il procedimento è identico per le lezioni successive.
