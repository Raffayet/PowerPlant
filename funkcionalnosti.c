#include "funkcionalnosti.h"
#include <limits.h>

FILE *otvaranjeDatoteke(char *imeFajla) {
	FILE *fajl = fopen(imeFajla, "rb+");
	if (fajl == NULL) {
		printf("Doslo je do greske! Moguce da datoteka \"%s\" ne postoji.\n", imeFajla);
	} else {
		printf("Datoteka \"%s\" otvorena.\n", imeFajla);
	}
	return fajl;
}

void formiranjeDatoteke(char *imeFajla) {
	FILE *fajl = fopen(imeFajla, "wb");
	if (fajl == NULL) {
		printf("Doslo je do greske prilikom kreiranja datoteke \"%s\"!\n", imeFajla);
	} else {
		//upisi pocetni blok
		BLOK blok;
		strcpy(blok.slogovi[0].evidencioniBroj, OZNAKA_KRAJA_DATOTEKE);
		fwrite(&blok, sizeof(BLOK), 1, fajl);
		printf("Datoteka \"%s\" uspesno kreirana.\n", imeFajla);
		fclose(fajl);
	}
}

SLOG *trazenjeSloga(FILE *fajl, char *evidencioniBroj) {
	if (fajl == NULL) {
		return NULL;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FAKTORBLOKIRANJA; i++) {
			if (strcmp(blok.slogovi[i].evidencioniBroj, OZNAKA_KRAJA_DATOTEKE) == 0) {
				//nema vise slogova
				return NULL;
			}

			if (strcmp(blok.slogovi[i].evidencioniBroj, evidencioniBroj) == 0) {
                //Ako se evidencioniBroj poklapa i slog NIJE logicki obrisan
                //(logicki obrisane slogove tretiramo kao da i ne
                //postoje u datoteci).

				SLOG *slog = (SLOG *)malloc(sizeof(SLOG));
				memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
				//printf(sizeof(slog));
				return slog;
			}
		}
	}

	return NULL;
}

void unosSloga(FILE *fajl, SLOG *slog) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	SLOG *slogStari = trazenjeSloga(fajl, slog->evidencioniBroj);
	if (slogStari != NULL) {
        //U datoteci vec postoji slog sa tim evid. brojem,
        //pa ne mozemo upisati novi slog.
        printf("Vec postoji slog sa tim evid brojem!\n");
        return;
    }

	BLOK blok;
	fseek(fajl, -sizeof(BLOK), SEEK_END); //u poslednji blok upisujemo novi slog
	fread(&blok, sizeof(BLOK), 1, fajl);

	int i;
	for (i = 0; i < FAKTORBLOKIRANJA; i++) {
		if (strcmp(blok.slogovi[i].evidencioniBroj, OZNAKA_KRAJA_DATOTEKE) == 0) {
            //Ovo je mesto gde se nalazi slog sa oznakom
            //kraja datoteke; tu treba upisati novi slog.
			memcpy(&blok.slogovi[i], slog, sizeof(SLOG));
			break;
		}
	}

	i++; //(na to mesto u bloku cemo upisati krajDatoteke)

	if (i < FAKTORBLOKIRANJA) {
        //Jos uvek ima mesta u ovom bloku, mozemo tu smestiti slog
        //sa oznakom kraja datoteke.
		strcpy(blok.slogovi[i].evidencioniBroj, OZNAKA_KRAJA_DATOTEKE);

		//Sada blok mozemo vratiti u datoteku.
		fseek(fajl, -sizeof(BLOK), SEEK_CUR);
		fwrite(&blok, sizeof(BLOK), 1, fajl);
	} else {
		//Nema vise mesta u tom bloku, tako da moramo
        //praviti novi blok u koji cemo upisati slog
        //sa oznakom kraja datoteke.

		//Prvo ipak moramo vratiti u datoteku blok
        //koji smo upravo popunili:
		fseek(fajl, -sizeof(BLOK), SEEK_CUR);
		fwrite(&blok, sizeof(BLOK), 1, fajl);

		//Okej, sad pravimo novi blok:
		BLOK noviBlok;
		strcpy(noviBlok.slogovi[0].evidencioniBroj, OZNAKA_KRAJA_DATOTEKE);
		//(vec smo na kraju datoteke, nema potrebe za fseekom)
		fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
	}

	if (ferror(fajl)) {
		printf("Greska pri upisu u fajl!\n");
	} else {
		printf("Upis novog sloga zavrsen.\n");
	}
}

void prikazSvihSlogova(FILE *fajl) {

	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;

	printf("Evid. broj        Lokacija     Tip  Dat.Vrem.Pustanja u rad  Snaga\n");
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FAKTORBLOKIRANJA; i++) {
                //strcpy(blok.slogovi[i].adresaBloka, rbBloka);
                //strcpy(blok.slogovi[i].adresaBloka, rbBloka);
			if (strcmp(blok.slogovi[i].evidencioniBroj, OZNAKA_KRAJA_DATOTEKE) == 0) {
				printf("B%d S%d *\n", rbBloka, i);
                break; //citaj sledeci blok
			}
                printf("B%d S%d ", rbBloka, i);
                prikazSloga(&blok.slogovi[i]);
                printf("\n");

		}

		rbBloka++;
	}
}

void prikazSvihSlogovaSaOgranicenjima(FILE *fajl) {

	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

    double ogranicenje;
    printf("Unesite ogranicenje za snagu u W: ");
    scanf("%lf", &ogranicenje);

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;
	printf("Evid. broj        Lokacija     Tip  Dat.Vrem.Pustanja u rad  Snaga\n");
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FAKTORBLOKIRANJA; i++) {

			if (strcmp(blok.slogovi[i].evidencioniBroj, OZNAKA_KRAJA_DATOTEKE) == 0) {
				printf("B%d S%d *\n", rbBloka, i);
                break;
			}

			else {
                printf("B%d S%d ", rbBloka, i);
                prikazSlogaSaOgranicenjima(&blok.slogovi[i], ogranicenje);
                printf("\n");
			}
		}

		rbBloka++;
	}
}

void prikazSlogaSaOgranicenjima(SLOG *slog, double ogranicenje) {

    if (&slog->snaga > &ogranicenje || &ogranicenje > 100000000000){

        printf("            Snaga premasuje ogranicenje!");

    }

    else{

    printf("%5s  %13s  %7s  %02d-%02d-%4d %02d:%02d %12d",

        slog->evidencioniBroj,
		slog->lokacija,
		slog->tip,
		slog->datumVremeDolaska.dan,
		slog->datumVremeDolaska.mesec,
		slog->datumVremeDolaska.godina,
		slog->datumVremeDolaska.sati,
		slog->datumVremeDolaska.minuti,
		slog->snaga);
    }
}


void prikazSloga(SLOG *slog) {
	printf("%5s  %13s  %7s  %02d-%02d-%4d %02d:%02d %12d",

        slog->evidencioniBroj,
		slog->lokacija,
		slog->tip,
		slog->datumVremeDolaska.dan,
		slog->datumVremeDolaska.mesec,
		slog->datumVremeDolaska.godina,
		slog->datumVremeDolaska.sati,
		slog->datumVremeDolaska.minuti,
		slog->snaga);
}


