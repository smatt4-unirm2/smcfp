# Git Workflow per il corso SMCFP

Questo documento descrive il workflow Git utilizzato per gestire il materiale del corso tramite GitHub e GitHub Classroom.

L’obiettivo è mantenere separati:

- il materiale completo (docente)
- il materiale rilasciato progressivamente agli studenti

---

## Struttura dei branch

La repository utilizza due branch principali:

### `staff`

Contiene **tutto il materiale del corso**, inclusi:

- tutte le lezioni
- versioni complete degli esercizi
- eventuali soluzioni
- materiale in sviluppo

Questo è il branch su cui lavora il docente.

---

### `main`

Contiene solo il materiale **rilasciato agli studenti**.

Questo branch viene aggiornato progressivamente durante il corso ed è utilizzato come base per gli assignment GitHub Classroom.

---

## Principio di funzionamento

Il branch `main` viene aggiornato copiando selettivamente contenuto dal branch `staff`.

Non si cancellano file da `main`:  
si aggiunge nuovo materiale oppure si aggiornano cartelle già esistenti.

---

## Workflow

### 1. Lavorare sul materiale (branch `staff`)

```bash
git checkout staff
git add .
git commit -m "Aggiornamento materiale"
git push
```

---

### 2. Pubblicare materiale per gli studenti

```bash
git checkout main
git checkout staff -- M1_basics
git add .
git commit -m "Release M1 basics"
git push
```

---

## Rilasci successivi

```bash
git checkout main
git checkout staff -- M2_geant4/01_intro
git add .
git commit -m "Add Geant4 intro"
git push
```

---

## Aggiornare materiale già pubblicato

```bash
git checkout main
git checkout staff -- M1_basics
git add .
git commit -m "Fix M1 basics"
git push
```

---
## Riepilogo

```bash
git checkout main
git checkout staff -- <cartella>
git commit -m "Release"
```

---

# Accesso alle macchine:

Accedere tramite browser andando su classrooms.rmlab.infn.it e inserendo le credenziali della webmail INFN (per utenti esterni le credenziali arrivate via email).
Una volta entrati fare il source delle variabili d'ambiente
```bash
source /data-labs/C2602/setenv.sh
```

Per entrare via ssh (solo da rete INFNdot1x):
```bash
ssh -XY utente@c2602-01.classrooms.rmlab.infn.it
```

Per copiare con `scp`:

```bash
#dal pc alla macchina remota
scp /path/al/file utente@c2602-01.classrooms.rmlab.infn.it:/home/utente/path

#dalla macchina al pc locale
scp utente@c2602-01.classrooms.rmlab.infn.it:/home/utente/path /path/al/file
```
