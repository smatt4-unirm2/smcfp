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

## Creazione assignment GitHub Classroom

1. Assicurarsi che `main` contenga solo il materiale da distribuire
2. Creare un assignment in GitHub Classroom
3. Usare questa repository come starter code

---

## Note importanti

- Non inserire mai le soluzioni nel branch `main`
- Non lavorare direttamente su `main`
- Usare sempre `staff`
- Controllare sempre il branch:

```bash
git branch
```

---

## Riepilogo

```bash
git checkout main
git checkout staff -- <cartella>
git commit -m "Release"
```
