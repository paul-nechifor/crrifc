#include <curses.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "macro.h"
#include "protocolp.h"
#include "vecini.h"

struct jucator
{
	char nume[NUME_MAX + 1];
	int nrTeritorii;
	char teritorii[15];
	int puncte;
	int iclient; /* canalul in care scrie serverul si citeste clientul */
	int iserver; /* canalul din care citeste serverul si scrie clientul */
};

/* date globale */
struct jucator jucatori[3];
int nrJucatori = 0;
int conect;
int alegeTeritoriu[3], nrAlegeri = 0, trimiteAlegerea = 0;
int ordineAtac[9] = {0,1,2,   1,2,0,   2,0,1};
int ataculNr = 0;
int nrPrimitAlegeri = 3;

/* functii */
void asteaptaConectarea(void);
void mesajeInitiale(void);
void mainLoop(void);
void castigatorIntrebareRapida(int* rasp, int* jucRasp, int corect);
int conecteazaJucator(char * nume);
void trimiteAlegereaTeritoriului(void);

int main(int argc, char *argv[])
{
	int ret, i;
	char ter[3];

	/* insamantez randul */
	srand(time(NULL));

	if (argc < 2)
	{
		printf("Utilizare: %s [NUMEJOC]\n", argv[0]);
		exit(1);
	}

	/* creez canalul de conectare */
	ret = mkfifo(argv[1], 0666);
	if ((ret == -1) && (ret != EEXIST))
	{
		printf("Eroare la crearea canalului de conectare '%s'.\n", argv[1]);
		exit(1);
	}
	
	/* ma conectez */
	if ( (conect = open(argv[1], O_RDONLY)) == -1)
	{
		printf("Eroare la deschiderea canalului de conectare '%s'.\n", argv[1]);
		exit(1);
	}

	printf("Deblocare.\n");

	/* asteapta conectarea a trei jucatori */
	asteaptaConectarea();

	/* inchide canalul de conectare si sterge-l */
	if (close(conect) == -1)
	{
		printf("Eroare la inchiderea canalului de conectare '%s'.\n", argv[1]);
		exit(1);
	}
	if (unlink(argv[1]) == -1)
	{
		printf("Eroare la stergerea canalului de conectare '%s'.\n", argv[1]);
		exit(1);
	}
	
	printf("S-au conectat cei trei jucatori.\n");

	/* trimite numele, punctele si teritoriile de start*/
	mesajeInitiale();

	/* bucla principala */
	mainLoop();

	/* inchide canalele si sterge-le */

	return 0;
}
void asteaptaConectarea(void)
{
	int mesg;

	for (;;)
	{
		usleep(100000); /* 0,1 secunde */
		mesg = citeste(conect);
		switch (mesg)
		{
			case NIMIC: /* nu s-a citit nimic */
				break;
			case C_CONE:
				printf("Vrea sa se conecteze '%s'.\n", campuri[1]);
				if (conecteazaJucator(campuri[1]) == -1)
				{
					printf("Nu se poate conecta jucatorul '%s'.\n", campuri[1]);
					break;
				}
				printf("S-a conectat jucatorul '%s'. Acum sunt %d jucatori\n", campuri[1], nrJucatori);
				scrie(1, jucatori[nrJucatori-1].iclient, "AIIN");
				
				if (nrJucatori == 3) return; /* iesi din functie pentru ca s-au conectat toti jucatorii */
				break;
			default:
				printf("Nu recunosc mesajul '%s'.\n", campuri[0]);
				break;
		}
	}
}
void mesajeInitiale(void)
{
	char pcte[20], ter[3], nr[3][2] = {"1", "2", "3"};
	int i, j;

	sprintf(pcte, "%d|%d|%d", jucatori[0].puncte, jucatori[1].puncte, jucatori[2].puncte);
	
	/* trimite numele si punctele */
	for (i=0; i<3; i++)
	{
		scrie(4, jucatori[i].iclient, "JUCS", jucatori[0].nume, jucatori[1].nume, jucatori[2].nume);	
		scrie(2, jucatori[i].iclient, "PUNS", pcte);
	}

	teritoriiDeStart(ter);
	printf("Teritoriile de start vor fi [%c%c%c].\n", ter[0], ter[1], ter[2]);

	pcte[1] = 0;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
		{
			pcte[0] = ter[i];
			scrie(3, jucatori[j].iclient, "JALT", nr[i], pcte);
		}

	/* pune-i fiecarui jucator teritoriul lui */
	for (i=0; i<3; i++)
	{
		jucatori[i].nrTeritorii = 1;
		jucatori[i].teritorii[0] = ter[i];
	}
}
void mainLoop(void)
{
	int i, j, mesg;
	int parteaCuceriri = 1, asteaptaRaspuns = 0;
	int nrIntr = 1;

	int rasp[3]; /* ce raspuns am primit */
	int jucRasp[3]; /* cine a dat raspunsul */
	int nrRasp; /* cate raspunsuri s-au dat */

	char intrebarea[200];
	int corect;

	char nb[20];

	printf("Am intrat in bucla \"infinita\".\n");

	for (;;)
	{
		usleep(100000);

		/* ascult mesajele ========================================================================================== */
		for (i=0; i<3; i++)
		{
			mesg = citeste(jucatori[i].iserver);

			switch (mesg)
			{
				case NIMIC:
					break;
				case C_TRIR:
					rasp[nrRasp] = atoi(campuri[1]);
					jucRasp[nrRasp] = i;
					nrRasp++;
					printf("Intr. rap. %d: rasp %d: Jucatorul %d a raspuns %s.\n", nrIntr, nrRasp, i+1, campuri[1]);
					break;
				
				case C_TRIG:
					break;

				case C_ALEG:
					/* trebuie sa verific daca are voie sa aleaga acest teritoriu */
					printf("Jucatorul %d a ales teritoriul '%s'.\n", i+1, campuri[1]);
					jucatori[i].teritorii[ jucatori[i].nrTeritorii ] = campuri[1][0]; /* primul caracter */
					jucatori[i].nrTeritorii++;
					trimiteAlegerea = 1;
					nrPrimitAlegeri++;
					sprintf(nb, "%d|%c", i+1, campuri[1][0]);
					for (j=0; j<3; j++)
						scrie(2, jucatori[j].iclient, "JALT", nb);
					break;

				case C_ATAC:
					break;

				default:
					printf("Nu recunosc mesajul '%s'.\n", campuri[0]);
					break;
			}
		}

		/* starea curenta ========================================================================================== */
		if (parteaCuceriri)
		{
			if (asteaptaRaspuns)
			{
				if (nrRasp == 3 && nrAlegeri == 0)
				{
					/* verifica cine a dat raspunsul corect */
					castigatorIntrebareRapida(rasp, jucRasp, corect);
					asteaptaRaspuns = 0;
					/* urmatoarea intrebare o trimit dupa ce am primit toate teritoriile  */
				}
			}
			else if (nrPrimitAlegeri == 3)
			{
				if (nrIntr < 4)
				{
					/* pune o intrebare kapida */
					getIntrebareRapida(intrebarea, &corect);
					printf("Pun intrebarea rapida: [[[[%s]]]] care are raspunsul '%d'.\n", intrebarea, corect);
					for (i=0; i<3; i++)
						scrie(2, jucatori[i].iclient, "INTR", intrebarea);

					asteaptaRaspuns = 1;
					nrPrimitAlegeri = 0;
					nrRasp = 0;
				}
				else
				{
					parteaCuceriri = 0;
					/* incepe partea a doua */
				}
			}
		}
		else 
		{
			/*
			prima data vad cine ataca pe cine
			verific daca acela poate sa atace (daca are teritorii)
			spun tuturor ca jucatorul x trebuie sa leaga un teritoriu de atacat
			ii spun sa aleaga un teritoriu de atacat
			- dupa ce am primit teritoriul de atacat
				

			*/
		}
		if (trimiteAlegerea)
		{
			if (nrAlegeri > 0)
			{
				nrAlegeri--;
				trimiteAlegerea = 0;
				trimiteAlegereaTeritoriului();
			}
			else
			{
				printf("S-a terminat alegerea teritoriilor. Acum trebuie sa pun alta intrebare.\n");
				trimiteAlegerea = 0;
				nrIntr++; /* urmatoarea intrebare */
			}
		}
	}
}
void trimiteAlegereaTeritoriului(void)
{
	
	char veci[20], lung[10];
	int nrveci, i, j, k, l, ocupat;

	i = alegeTeritoriu[nrAlegeri]; /* i este indicele jucatorului care trebuie sa faca alegerea */
	veciniPentru(jucatori[i].teritorii, jucatori[i].nrTeritorii, veci, &nrveci);
	/* trebuie scoase teritoriile care sunt detinute de alt jucator */
	for (j=0; j<3; j++)
		if (j != i) scoateTeritorii(veci, &nrveci, jucatori[j].teritorii, jucatori[j].nrTeritorii);
	/* daca nu are niciun teritoriu, alege unul neocupat la intamplare */
	if (nrveci == 0)
		for (j=0; j<15; j++)
		{
			ocupat = 0;
			for (k=0; k<3; k++)
				for (l=0; l<jucatori[k].nrTeritorii; l++)
					if (jucatori[k].teritorii[l] == (char)(j+'A'))
						{ ocupat = 1; break; }
			if (!ocupat)
				{ veci[0] = (char)(j+'A'); nrveci = 1; break; }
		}
	veci[nrveci] = 0;

	printf("Jucatorul %d trebuie sa aleaga un teritoriu dintre [%s].\n", i+1, veci);

	sprintf(lung, "%d", nrveci);
	scrie(3, jucatori[i].iclient, "TBAL", veci, lung);

	sprintf(lung, "%d", i+1);
	for (j=0; j<3; j++)
		scrie(2, jucatori[i].iclient, "JALG", lung);
}
void castigatorIntrebareRapida(int* rasp, int* jucRasp, int corect)
{
	int i, j, aux;
	int raspPentru[3]; /* indicele reprezinta jucatorul */
	int topv[3];
	int topj[3];
	char textul[100];

	for (i=0; i<3; i++)
		raspPentru[jucRasp[i]] = rasp[i];
	
	for (i=0; i<3; i++)
		topv[i] = abs(raspPentru[i] - corect) * 10;
	
	/* adaug ordinea venirii raspunsurilor */
	for (i=0; i<3; i++)
		topv[jucRasp[i]] += i;

	for (i=0; i<3; i++)
		topj[i] = i;

	for (i=0; i<2; i++)
		for (j=i+1; j<3; j++)
			if (topv[i] > topv[j])
			{
				aux = topv[i]; topv[i] = topv[j]; topv[j] = aux;
				aux = topj[i]; topj[i] = topj[j]; topj[j] = aux;
			}
	/* trimit raspunsul */
	sprintf(textul, "%d|%d|%d|%d", corect, raspPentru[0], raspPentru[1], raspPentru[2]);
	for (i=0; i<3; i++)
		scrie(2, jucatori[i].iclient, "RASR", textul);

	/* bag in stiva alegerea de teritorii */
	alegeTeritoriu[2] = alegeTeritoriu[1] = topj[0];
	alegeTeritoriu[0] = topj[1];
	nrAlegeri = 3;
	trimiteAlegerea = 1;

	/* maresc punctele */
	jucatori[topj[0]].puncte += 400;
	jucatori[topj[1]].puncte += 200;

	printf("Castigator este jucatorul %d. Pe locul doi este %d.\n", topj[0]+1, topj[1]+1);

	/* trimit noul punctaj */
	sprintf(textul, "%d|%d|%d", jucatori[0].puncte, jucatori[1].puncte, jucatori[2].puncte);
	for (i=0; i<3; i++)
		scrie(2, jucatori[i].iclient, "PUNS", textul);
}
int conecteazaJucator(char * nume)
{
	char fis[50];

	if (strlen(nume) > NUME_MAX)
	{
		printf("Numele '%s', este prea mare (<=%d).\n", nume, NUME_MAX);
		return -1;
	}
	strcpy(jucatori[nrJucatori].nume, nume);

	/* deschidere input client (a fost creat de client) */
	strcpy(fis, "iclient_");
	strcat(fis, nume);
	if ( (jucatori[nrJucatori].iclient = open(fis, O_WRONLY)) == -1)
	{
		printf("Nu s-a putut deschide fisierul de input client '%s'.\n", fis);
		return -1;
	}

	/* creare input server si deschiderea */
	strcpy(fis, "iserver_");
	strcat(fis, nume);
	if ( mkfifo(fis, 0666) == -1)
	{
		printf("Eroare la crearea canalului de input server '%s'.\n", fis);
		return -1;
	}
	if ( (jucatori[nrJucatori].iserver = open(fis, O_RDONLY | O_NONBLOCK)) == -1)
	{
		printf("Eroare la deschiderea canalului de input server '%s'.\n", fis);
		return -1;
	}
	
	jucatori[nrJucatori].puncte = 1000;
	jucatori[nrJucatori].nrTeritorii = 0;

	nrJucatori++;
	return 0;
}
