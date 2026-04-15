
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


Questa repository verrà utilizzata per condividere il codice relativo alle esercitazioni e verrà aggiornata di volta in volta con gli strumenti di cui avrete bisogno.

---

# Lezione 0

E' semplicemente un messaggio di benvenuto per determinare se tutto il sistema funziona. Dovete semplicemente fare un edit del file Welcome.txt seguendo le istruzioni al suo interno.
Potete fare edit online del file tramite l'interfaccia web di GitHub, oppure fare il pull della repository sul vostro PC (dopo aver installato git):

```bash
git clone git@github.com:smatt4-unirm2/m0-welcome-<user>.git
cd m0-welcome-<user>
```
Modificare il file
```bash
nano Welcome.txt #o qualsiasi editor
```

E infine fate il commit della repository
```bash
git add Welcome.txt #per aggiungere ai file del commit il file Welcome.txt
git commit -m "Messaggio per identificare il commit"
git push
```



