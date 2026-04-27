## Installazione di Geant4

Su macchine basate su Linux.
Istruzioni di riferimento su 
https://geant4.web.cern.ch/documentation/dev/ig_html/InstallationGuide/installguide.html#buildandinstall


Download del codice sorgente da:
https://geant4.web.cern.ch/download

Scaricare l'ultima release in formato `tar.gz` sotto `Source Code` 
Nel nostro caso è `geant4-v11.4.1`

Supponiamo che l'area di lavoro sia `/path/to`, questo consiglio di metterlo nella propia home

Copiamo e spacchettiamo il sorgente
```bash
mv geant4-v11.4.1 /path/to
tar xvzf geant4-v11.4.1
```

A questo punto creiamo le cartelle di build e install

```bash
mkdir geant4-v11.4.1-build
mkdir geant4-v11.4.1-install
cd geant4-v11.4.1-build
```

Ora lanciare il comando `cmake` con le opzioni di compilazione:
```bash
cmake -DCMAKE_INSTALL_PREFIX=/path/to/geant4-v11.4.1-install /path/to/geant4-v11.4.1 -DGEANT4_INSTALL_DATA=ON -DGEANT4_BUILD_MULTITHREADED=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_QT_QT5=ON -DGEANT4_USE_SYSTEM_EXPAT=OFF
```

Ora, se tutte le dipendenze sono presenti il comando dovrebbe procedere senza problemi. Se invece dà errore perché manca qualcosa allora procedere ad installare le dipendenze (in questo è utile cercare l'errore su google per vedere come installare le librerie mancanti). Probabilmente una libreria che manca è quella grafica che si installa con 

```bash
sudo apt install qt6-base-dev qt6-base-dev-tools qt6-tools-dev qt6-tools-dev-tools
```

Una volta che `cmake` completa senza errori finisce con la frase:

```txt
-- Build files have been written to: /path/to/geant4-v11.4.1-build
```

a questo punto compilare con 

```bash
make -j N
```

dove N è il numero di core del processore per mandare processi paralleli. Se il pc non è nuovissimo 4 va bene.

Una volta completata la compilazione, per installare lanciare

```bash
make install
```

In questo modo i file verranno copiati nella cartella di installazione.
Geant ora è installato.

## Sourcing

Per richiamare le variabili d'ambiente (e per dire a cmake dove trovare Geant quando compiliamo i progetti) fare

```bash
source /path/to/geant4-v11.4.1-install/share/Geant4/geant4make/geant4make.sh
```

questa cosa va fatta ogni volta che si apre un terminale. Per evitare di doverlo fare a mano, inserire il comando nel file `.bashrc`:
```bash
nano ~/.bashrc
#si apre bashrc, aggiungere in fondo la riga del source
#chiudere con CTRL+X e dare Y e poi Invio per salvare il file
```

## Esecuzione esempio

Ora, in un'area diversa da quella di installazione, che può essere una generica cartella `simulations`, copiare un file di esempio:

```bash
cd ~/Desktop/simulazioni
cp -r /path/to/geant4-v11.4.1-install/share/Geant4/examples/basic/B4/B4c . #questo copia la cartella B4c in quest'area

cd B4c    		#entro nella cartella
mkdir build 	#creo la cartella per i file compilati
cd build 		#e ci vado dentro

cmake .. 		#creo i file di compilazione con cmake (.. dice a cmake che la cartella sorgente è quella superiore)
#quando cmake finisce senza errori dare
make  			#questo compila il codice, dovrebbe creare un eseguibile che si chiama exampleB4c

./exampleB4c 	#lancia l'esempio
```

Ora, se tutto è compilato bene si dovrebbe aprire un visualizzatore con un rivelatore a layer. Premendo il pulsante "play" verde in alto circa al centro, il codice simula una particella e il visualizzatore dovrebbe mostrare correttamente le tracce della particella e delle secondarie simulate.


