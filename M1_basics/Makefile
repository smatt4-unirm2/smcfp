# Makefile Universale per compilare file .cxx nella directory corrente

# 1. Trova tutti i file sorgente .cxx nella directory corrente.
#    Il pattern `*.cxx` cerca direttamente nella directory dove viene lanciato make.
SOURCES := $(wildcard *.cxx)

# 2. Definisci i nomi degli eseguibili.
#    Ora gli eseguibili avranno lo stesso nome dei sorgenti, ma senza l'estensione .cxx,
#    e verranno creati nella directory corrente.
TARGETS := $(patsubst %.cxx,%,$(SOURCES))

# 3. Flags di debugging/profiling (lasciati come nell'originale)
# DEBUG = -g -pg

# 4. Determina l'architettura (come nell'originale)
UNAME_S := $(shell uname -s)

# 5. Compilatore e flags per Clang (macOS)
ifeq ($(UNAME_S), Darwin)
	CXX := clang++
	CC  := clang
	# std=c++11 è preferibile a std=c++0x per maggiore compatibilità futura
	FLAGS := -std=c++17 -O2 $(DEBUG)
endif

# 6. Compilatore e flags per Gnu (Linux)
ifeq ($(UNAME_S), Linux)
	CXX := g++
	CC  := gcc
	# std=c++11 è preferibile a std=c++0x per maggiore compatibilità futura
	FLAGS := -std=c++17 -O2 $(DEBUG)
endif

# Aggiungi qui eventuali altre configurazioni specifiche per altri OS se necessario.

ROOTMODE ?= auto

ifeq ($(ROOTMODE), auto)
	FLAGS += $(shell root-config --cflags 2>/dev/null || true)
	LIBS  += $(shell root-config --libs 2>/dev/null || true)
else
	ROOTSYS := /Users/matt/root_install_628
	FLAGS   += -I$(ROOTSYS)/include
	LIBS    += -L$(ROOTSYS)/lib \
		-lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer \
		-lRint -lPostscript -lMatrix -lPhysics -lThread -pthread -lm \
		-ldl -rdynamic -lSpectrum
endif
	LIBS   += -lSpectrum
# Nota: `2>/dev/null || true` serve a sopprimere gli errori se `root-config` non è trovato
# e a far sì che make non fallisca immediatamente, ma continui senza quelle flag.

# 8. Altre librerie di terze parti (decommenta se ti servono)
# LIBS  += -lboost_filesystem

# 9. Regola 'all': Compila tutti gli eseguibili definiti in TARGETS.
all: $(TARGETS)

# 10. Regola di pattern per la compilazione:
#     Prende un file sorgente (es. foo.cxx) e crea l'eseguibile (es. foo).
#     `$@` è il nome del target (eseguibile)
#     `$<` è il nome della prima dipendenza (file sorgente)
$(TARGETS): %: %.cxx
	@echo "Compiling $< to $@"
	$(CXX) $(FLAGS) -o $@ $< $(LIBS)

# 11. Regola 'clean': Rimuove tutti gli eseguibili e i file oggetto temporanei (se ne avessi).
#     Dato che non stiamo creando .o intermedi, rimuoviamo solo i TARGETS.
clean:
	@echo "Cleaning compiled targets..."
	@rm -f $(TARGETS)
	@echo "Clean complete."

.PHONY: all clean
