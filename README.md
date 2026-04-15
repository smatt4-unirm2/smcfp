
# Corso Strumenti Montecarlo per la Fisica delle Particelle
Dottorato di Ricerca in Fisica

M. Sorbara, T. Minniti, F. Palma

---
Questa repository contiene il materiale didattico e gli esercizi del corso **Strumenti Montecarlo per la Fisica delle Particelle (SMCFP)**.  
Il corso introduce i principi delle simulazioni Montecarlo applicate alla fisica delle particelle e ai rivelatori, con un focus su due toolkit ampiamente utilizzati nella ricerca:

- **Geant4** per simulazioni di rivelatori e fisica delle alte energie
- **FLUKA** per simulazioni di interazione radiazione–materia e applicazioni di fisica medica e radioprotezione

Il corso prevede due progetti:
- simulazione di un **rivelatore a scintillatore per particelle** (Geant4)
- simulazione di un **profilo di dose in un fantoccio d’acqua** (FLUKA)

Per conseguire i crediti relativi all'esame gli studenti dovranno risolvere un esercizio sulla misura di quantità fisiche in ognuna delle due simulazioni.

---

### Calendario e argomenti
Le lezioni si tengono in **Aula Caccin (Dipartimento di Fisica, Piano -1, di fronte al corridoio D0)** dalle **15:00 alle 18:00**.

Il calendario è il seguente:

| Giorno | Argomenti trattati | Esercitazione |
|------|--------------------|--------------|
| **Lunedì 20 Aprile** | Introduzione al metodo Montecarlo: numeri casuali, distribuzioni, concetto di evento, errori statistici | Esercizi in C++: generazione numeri casuali, stima di π con Montecarlo, distribuzione esponenziale |
| **Giovedì 23 Aprile** | Introduzione a Geant4: struttura di una simulazione, ciclo Run/Event/Track, compilazione ed esecuzione di un esempio | Compilazione ed esecuzione di un esempio Geant4, utilizzo delle macro e visualizzazione della geometria |
| **Lunedì 27 Aprile** | Geometria e materiali in Geant4: solidi, volumi logici e fisici, definizione del world volume | Modifica della geometria dell'esempio Geant4, inserimento di un rivelatore semplice |
| **Giovedì 30 Aprile** | Raccolta dati e analisi in Geant4: energia depositata, sensitive detector, output della simulazione | Registrazione dell’energia depositata nel rivelatore e analisi delle distribuzioni |
| **Lunedì 4 Maggio** | Generazione di particelle e modelli fisici in Geant4: Particle Gun, physics lists | Definizione del fascio incidente, variazione del tipo di particella e dell’energia |
| **Giovedì 7 Maggio** | Introduzione a FLUKA: filosofia del codice, struttura degli input file, utilizzo di FLAIR | Esecuzione di una simulazione semplice e visualizzazione della geometria |
| **Lunedì 18 Maggio** | Geometria e fascio in FLUKA: definizione del phantom e della sorgente di particelle | Costruzione di un phantom d’acqua e definizione del fascio incidente |
| **Giovedì 21 Maggio** | Scoring e analisi dei risultati in FLUKA: distribuzione di dose e profilo di dose | Calcolo del profilo di dose nel phantom e studio della dipendenza dalla profondità |

Eventuali lezioni di recupero:

* Lunedì 25 Maggio
* Giovedì 28 Maggio


---

# Struttura della repository

La repository è organizzata in tre sezioni principali:

1. **Esercizi introduttivi in C++**
2. **Progetto Geant4**
3. **Progetto FLUKA**

Gli esercizi sono organizzati per lezione e progettati per essere **incrementali**: il codice sviluppato nelle prime lezioni viene esteso e modificato nelle successive.

```

SMCFP_main
|
├── M1_basics
│
├── M2_geant4
│ 		├── base
│ 		└── full
│
└── M3_fluka
		  ├── base
		  └── full
```
---

# M1_basics

Questa cartella contiene esercizi introduttivi in **C++** utilizzati nella prima lezione per introdurre i concetti fondamentali delle simulazioni Montecarlo:

- generazione di numeri casuali uniformi
- stima di π con metodo Montecarlo
- generazione di distribuzioni esponenziali

---

# M2_geant4

Contiene il materiale relativo alle simulazioni con **Geant4**. Il lavoro è organizzato in due parti:

## base

Questa cartella contiene un **esempio minimale di Geant4**, derivato da uno degli esempi ufficiali. Questo esempio verrà utilizzato per:

- familiarizzare con l'ambiente Geant4
- visualizzare la geometria
- modificare parametri base
- eseguire simulazioni con macro

## full

Questa cartella contiene il **progetto completo sviluppato durante il corso**.

Il progetto è basato su una versione estesa dell'esempio `B1` e rappresenta una simulazione semplificata di un **rivelatore per particelle**.
Componenti principali del codice:

- **DetectorConstruction**  
  definizione della geometria del rivelatore

- **PrimaryGeneratorAction**  
  generazione delle particelle incidenti

- **PhysicsList**  
  definizione dei modelli fisici

- **RunAction / EventAction / SteppingAction**  
  gestione della simulazione e raccolta dei dati

---

# M3_fluka

Contiene gli esempi e gli esercizi relativi all'utilizzo di **FLUKA**. Il lavoro è organizzato in due parti:

## base

Contiene un **esempio minimale FLUKA** utilizzato per:

- introdurre la struttura dei file di input
- modificare l'interfaccia grafica **FLAIR**
- visualizzare la geometria e i suoi errori
- modificare parametri base dell'input

## full

Questa cartella contiene la simulazione completa sviluppata durante il corso.

Il progetto consiste nella simulazione di un **fantoccio ad acqua irradiato con un fascio di particelle**, con l'obiettivo di calcolare il **profilo di dose**.

Elementi principali della simulazione:

- definizione della geometria del phantom
- definizione dei materiali
- definizione del fascio incidente
- scoring della dose

---



