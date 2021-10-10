#include "stdio.h"
#include "stdlib.h"

#include "funkcionalnosti.h"

//NAPOMENA! - test datoteka: "data"

int main() {

	int running = 1;
	int unosKorisnika;

	FILE *fajl = NULL;

	char imeAktivneDatoteke [20];
	imeAktivneDatoteke[0] = NULL;

	while (running) {
		printf("Odaberite opciju:\n");
		printf("1 - Otvaranje datoteke\n");
		printf("2 - Prikaz naziva aktivne datoteke\n");
		printf("3 - Formiranje datoteke\n");
		printf("4 - Trazenje sloga u aktivnoj datoteci\n");
		printf("5 - Upis novog sloga\n");
		printf("6 - Prikaz svih slogova\n");
		printf("7 - Prikaz svih aktuelnih slogova (sa ogranicenjima)\n");
		printf("8 - Izlaz\n");
		if (fajl == NULL) {
			printf("!!! PAZNJA: datoteka jos uvek nije otvorena !!!\n");
		}
		scanf("%d", &unosKorisnika);
		getc(stdin);

		switch(unosKorisnika) {
			case 1:
				{
					char imeFajla[20];
					printf("Unesite ime datoteke za otvaranje: ");
					scanf("%s", &imeFajla[0]);
					fajl = otvaranjeDatoteke(imeFajla);

					for (int i = 0; i < sizeof(imeFajla); i++){

                        imeAktivneDatoteke[i] = imeFajla[i];
					}

					printf("\n");
					break;
				}
			case 2:
				{
                    if (imeAktivneDatoteke[0] == NULL){

                        printf("Nema aktivnih datoteka! \n");
                        break;
                    }

                    printf("Ime aktivne datoteke: %s", imeAktivneDatoteke);
                    printf("\n");
                    break;

				}
			case 3:
				{
                    char imeFajla[20];
					printf("Unesite ime datoteke za kreiranje: ");
					scanf("%s", imeFajla);
					formiranjeDatoteke(imeFajla);
					printf("\n");
					break;
				}
			case 4:
				{

				    if (imeAktivneDatoteke[0] == NULL){

                        printf("Nema aktivnih datoteka! \n");
                        break;
                    }

                    char evidencioniBroj[5+1];
					printf("Unesite evidencioni broj trazenog prijema: ");
					scanf("%s", evidencioniBroj);
					SLOG *slog = trazenjeSloga(fajl, evidencioniBroj);
					if (slog == NULL) {
                        printf("Nema tog sloga u datoteci.\n");
					} else {
                        printf("BL SL Evid. broj   Lokacija   Tip   Dat.Vrem.Pustanja u rad  Snaga\n");
                        prikazSloga(slog);
                        printf("\n");
					}
					printf("\n");
					break;
				}
			case 5:
				{
				    if (imeAktivneDatoteke[0] == NULL){

                        printf("Nema aktivnih datoteka! \n");
                        break;
                    }

					SLOG slog;
					printf("Evidencioni broj (5 cifara): ");
					scanf("%s", slog.evidencioniBroj);
					printf("Lokacija (najvise 50 karaktera): ");
					scanf("%s", slog.lokacija);
					printf("Tip (tacno 5 karaktera): ");
					scanf("%s", slog.tip);
					printf("Datum i vreme dolaska (dd-mm-YYYY HH:mm): ");
					scanf("%d-%d-%d %d:%d",
						&slog.datumVremeDolaska.dan,
						&slog.datumVremeDolaska.mesec,
						&slog.datumVremeDolaska.godina,
						&slog.datumVremeDolaska.sati,
						&slog.datumVremeDolaska.minuti);
					printf("Snaga (do 100 000 000 000 W): ");
					scanf("%lf", &slog.snaga);
					unosSloga(fajl, &slog);
					printf("\n");
					break;
				}
			case 6:
				{
					prikazSvihSlogova(fajl);
					printf("\n");
					break;
				}
			case 7:
				{
                    prikazSvihSlogovaSaOgranicenjima(fajl);
                    printf("\n");
                    break;
				}

			case 8:
				{
					running = 0;
					if (fajl != NULL) {
						fclose(fajl);
					}
				}
		}
	}

	return 0;

}




