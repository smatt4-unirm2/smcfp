# Corso Strumenti Montecarlo per la Fisica delle Particelle
Dottorato di Ricerca in Fisica
M. Sorbara, T. Minniti, F. Palma

## Esercizi Montecarlo con C++ e ROOT

Esercizi base per iniziare a familiarizzare con gli ambienti C++ applicando principi base dell'analisi Montecarlo.

### Compilazione ed esecuzione

Aprire il terminale, entrare nella cartella `M1_basics` dove è presente l'esempio `ex1_generator.cxx`. 
La compilazione è gestita da un `Makefile` che viene invocato con:
```bash
make 
```
e compila automaticamente tutti i file che si chiamano `*.cxx` e ne genera un file eseguibile.
Per eseguire l'esempio lanciare:
```bash
./ex1_generator
```
---

## Esempio 1 — Generatori di numeri pseudo-random in ROOT

File: `ex1_generator.cxx`

Questo esempio mostra come utilizzare due generatori di numeri pseudo-random disponibili in **ROOT**:

- `TRandom3`
- `RanLux48`

Il programma genera eventi secondo due distribuzioni:

- **Uniforme** U(0,1)
- **Gaussiana** N(0,1)

e li accumula in istogrammi ROOT per confrontare le proprietà statistiche dei generatori.

Vengono generati **100000 eventi** per ciascun generatore.

### Verifiche statistiche

Per la distribuzione **uniforme U(0,1)** ci si aspetta:
``
media = 0.5
RMS = 0.288675 = 1/sqrt(12)
``

Per la distribuzione **gaussiana N(0,1)**:
``
media = 0
RMS = 1
``

Il programma stampa a schermo media e RMS delle distribuzioni generate per verificare che i valori ottenuti siano compatibili con quelli attesi.

### Output

Il programma salva i risultati nel file ROOT `output_generator.root` contenente:

```C++
h_uniform_r3		//distribuzione uniforme generata con **TRandom3**
h_uniform_ranlux48 	//distribuzione uniforme generata con **RanLux48**
h_gauss_r3			//distribuzione Gaussiana generata con **TRandom3**
h_gauss_ranlux48	//distribuzione Gaussiana generata con **RanLux48**
c_uniform_compare	//canvas confronto tra distribuzioni uniformi
c_gauss_compare		//canvas confronto tra gaussiane
```
### Esecuzione

```bash
./ex1_generator
```
Aprire il file ROOT:
```bash
root -l output_generator.root
```
In ROOT:
```C++
.ls
c_uniform_compare->Draw()
c_gauss_compare->Draw()
```

## Esercizio 2 — Stima di Pi Greco con metodo Montecarlo

**Concetti:** metodo *hit-or-miss*, distribuzione uniforme, convergenza statistica.

Stimare il valore di π utilizzando un metodo Montecarlo.

Considerare il quadrato

```text
[-1,1] x [-1,1]
```

in cui è inscritto un cerchio unitario di raggio 1.  
Generare punti casuali `(x,y)` uniformemente distribuiti nel quadrato e verificare se cadono all'interno del cerchio.

Un punto è **dentro il cerchio** se:

```text
x^2 + y^2 <= 1
```

Se si generano `N_total` punti e `N_inside` cadono nel cerchio, una stima di Pi è:

```text
π ≈ 4 × N_inside / N_total
```

### Obiettivo dell'esercizio

Scrivere un programma C++ che:

1. generi **N = 1e6** punti casuali `(x,y)` uniformi nel quadrato usando **TRandom3**
2. determini se il punto cade **dentro** o **fuori** dal cerchio
3. calcoli una stima di P
4. studi la **convergenza della stima al variare di N**

### Output richiesto

Salvare i risultati in un file ROOT `output_pi.root` contenente:

- `h2_in` — scatter plot 2D dei punti **dentro** il cerchio
- `h2_out` — scatter plot 2D dei punti **fuori** dal cerchio
- `g_pi_stimato` — grafico del valore stimato di π al variare di N
- `g_convergenza_pi` — grafico della differenza relativa tra la stima e il valore vero di Pi

Suggerimento: utilizzare le classi ROOT

```text
TH2D
TGraph
TRandom3
```

per visualizzare i risultati e generare i numeri casuali.

---

## Esercizio 3 — Distribuzione esponenziale

**Concetti:** generazione di distribuzioni, fit ROOT, CDF empirica.

Generare numeri casuali secondo una **distribuzione esponenziale** usando i generatori ROOT.

Una possibilità è usare il metodo integrato:

```text
TRandom3::Exp(tau)
```

ricordando che per una distribuzione esponenziale

```text
f(x) = λ e^{-λx}
```

vale la relazione

```text
tau = 1/λ
```

Per questo esercizio usare

```text
λ = 2.5
```

### Obiettivo dell'esercizio

Scrivere un programma C++ che:

1. generi **100000 numeri casuali** secondo una distribuzione esponenziale usando **TRandom3**
2. riempia un **istogramma ROOT** con i valori generati
3. esegua un **fit** con una funzione esponenziale
4. costruisca la **CDF empirica** a partire dai dati generati
5. confronti la CDF empirica con la **CDF teorica**

La CDF teorica della distribuzione esponenziale è

```text
F(x) = 1 − e^{-λx}
```

### Output richiesto

Salvare i risultati in un file ROOT `output_exponential.root` contenente:

- `h_expo` — istogramma della distribuzione esponenziale
- `T_expo` — `TTree` con tutti i valori generati
- `g_cdf_empirica` — CDF empirica
- `g_cdf_teorica` — CDF teorica
- `c_expo_fit` — canvas con fit esponenziale
- `c_cdf` — canvas con confronto tra CDF empirica e teorica

---
