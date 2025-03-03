# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Automatische Erkennung aller Unterordner
SUBDIRS := train_1 train_2 mainboard

# Unterordner, die ausgeschlossen werden sollen (Standard: leer)
EXCLUDE ?=

# Filtere ausgeschlossene Unterordner heraus
FILTERED_SUBDIRS := $(filter-out $(EXCLUDE), $(SUBDIRS))

# Ziel: Kompiliere alle Unterordner
all:
	@for dir in $(FILTERED_SUBDIRS); do \
	    echo "Kompiliere $$dir"; \
	    $(MAKE) -C $$dir; \
	done

# Clean in allen Unterordnern
clean:
	@for dir in $(FILTERED_SUBDIRS); do \
	    echo "Bereinige $$dir"; \
	    $(MAKE) -C $$dir clean; \
	done
