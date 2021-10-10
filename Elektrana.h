#define FAKTORBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE "*"

typedef struct DateTime {
	int dan;
	int mesec;
	int godina;
	int sati;
	int minuti;
} DATETIME;

typedef struct Slog {
	char evidencioniBroj[5+1]; //koristi se kao kljuc sloga
	char lokacija[50+1];
	char tip[5+1];
	DATETIME datumVremeDolaska;
	double snaga;
} SLOG;

typedef struct Blok {
	SLOG slogovi[FAKTORBLOKIRANJA];
} BLOK;
