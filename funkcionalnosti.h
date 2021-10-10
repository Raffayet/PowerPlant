#ifndef FUNKCIONALNOSTI_H
#define FUNKCIONALNOSTI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include "Elektrana.h"

FILE *otvaranjeDatoteke(char *imeFajla);
void formiranjeDatoteke(char *imeFajla);
SLOG *trazenjeSloga(FILE *fajl, char *evidencioniBroj);
void unosSloga(FILE *fajl, SLOG *slog);
void prikazSvihSlogova(FILE *fajl);
void prikazSloga(SLOG *slog);
void prikazSvihSlogovaSaOgranicenjima(FILE *fajl);
void prikazSlogaSaOgranicenjima(SLOG *slog, double ogranicenje);

#endif
