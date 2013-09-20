#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "culori.h"
#include "harta.h"
#include "interfata.h"
#include "protocolp.h"
#include "macro.h"

int culoriTeritorii[15];
int input, output, conect;

void asteaptaAcceptarea(void);
void dimensiuneTerminal(void);
void reafiseazaHarta(void);
void mainLoop(void);

int main(int argc, char *argv[])
{
	int i, ret;
	char fis[50];

	if (argc < 3)
	{
		printf("Utilizare: %s NUME JOC\n", argv[0]);
		exit(1);
	}

	initscr();
	start_color();
	initializeazaCulori();
	
	/* asteapta sa mareasca terminalul inainte de a continua */
	dimensiuneTerminal();
	
	/* creaza si deschide canalul de intrare client */
	strcpy(fis, "iclient_");
	strcat(fis, argv[1]);
	ret = mkfifo(fis, 0666);
	if (ret == -1 && ret != EEXIST)
		DIE("Eroare la crearea canalului de input");
	if ( (input = open(fis, O_RDONLY | O_NONBLOCK)) == -1)
		DIE("Eroare la deschiderea canalului de input");

	/* deschide canalul de conectare server */
	if ( (conect = open(argv[2], O_WRONLY)) == -1)
		DIE("Nu exista acest joc\n");

	/* scrie cererea de conectare */
	scrie(2, conect, "CONE", argv[1]);

	/* inchide canalul de conectare */
	if (close(conect) == -1) 
		DIE("Nu s-a putut inchide canalul serverului");

	/* asteapta sa primiesti mesajul de conectare */
	asteaptaAcceptarea();

	/* deschide canalul de output */
	strcpy(fis, "iserver_");
	strcat(fis, argv[1]);
	if ( (output = open(fis, O_WRONLY)) == -1)
		DIE("Eroare la deschiderea canalului de output");

	/* initializeaza teritoriile */
	for (i=0; i<15; i++)
		culoriTeritorii[i] = COL_NECUCERIT;

	creazaInterfata("", "", "");
	reafiseazaHarta();
	status("Am fost acceptat pe server.");
	mesajServer("Incepe jocul.");

	/* bucla principala */
	mainLoop();

	endwin();
	return 0;
}
void mainLoop(void)
{
	int mesg, rasp, col;
	char textul[100];

	for (;;)
	{
		usleep(100000);
		mesg = citeste(input);
		switch (mesg)
		{
			case NIMIC:
				break;

			case S_JUCS:
				mesajServer("Numele jucatorilor");
				creazaInterfata(campuri[1], campuri[2], campuri[3]);
				break;

			case S_PUNS:
				mesajServer("Actualizarea punctelor");
				puncteJucator(1, atoi(campuri[1]));
				puncteJucator(2, atoi(campuri[2]));
				puncteJucator(3, atoi(campuri[3]));
				break;

			case S_INTR:
				mesajServer("Raspunde la intrebare");
				rasp = puneIntrebareRapida(campuri[1]);
				sprintf(textul, "%d", rasp);
				scrie(2, output, "TRIR", textul);
				sprintf(textul, "Am trimis raspunsul '%d' la server.", rasp);
				status(textul);
				break;

			case S_INTG:
				break;

			case S_RASR:
				mesajServer("Rezultatele");
				afiseazaRaspunsRapid(atoi(campuri[1]), atoi(campuri[2]), atoi(campuri[3]), atoi(campuri[4]));
				reafiseazaHarta();
				break;

			case S_RASG:
				break;

			case S_JALG:
				sprintf(textul, "Jucatorul %s trebuie sa aleaga un teritoriu", campuri[1]);
				mesajServer(textul);
				break;

			case S_TBAL:
				mesajServer("Trebuie sa alegi un teritoriu");
				textul[0] = teritoriuDeAles(campuri[1], atoi(campuri[2]));
				textul[1] = 0;
				reafiseazaHarta();
				scrie(2, output, "ALEG", textul);
				break;

			case S_JALT:
				sprintf(textul, "Jucatorul %s are teritoriul '%s'.", campuri[1], campuri[2]);
				mesajServer(textul);
				rasp = atoi(campuri[1]);
				switch (rasp)
				{
					case 1: col = COL_J1; break;
					case 2: col = COL_J2; break;
					case 3: col = COL_J3; break;
					default: endwin(); printf("Nu exista alt jucator\n"); exit(1);
				}
				modificaTeritoriu(campuri[2][0], col);
				culoriTeritorii[campuri[2][0]-'A'] = col;
				break;

			case S_JATC:
				break;

			case S_TBAT:
				break;

			case S_JATT:
				break;

			default:
				printw("Nu recunosc mesajul '%s'.\n", campuri[0]);
				break;
		}
	}
}
void asteaptaAcceptarea(void)
{
	int i, mesg;
	for (i=0; i<100; i++)
	{
		usleep(100000);
		mesg = citeste(input);
		switch (mesg)
		{
			case NIMIC:
				break;
			case S_AIIN:
				printw("Ai intrat.\n");
				/* intoarce-te cu succes */
				return;
			default:
				printw("Nu recunost mesajul '%s'.\n", campuri[0]);
				break;
		}
	}
	/* am primit mesajul de conectare in 10 secunde */
	DIE("Nu am primit mesajul de conectare\n");
}
void dimensiuneTerminal(void)
{
	int lin, col;
	do
	{
		/* oare de ce nu trebuie sa dau adresele pentru lin si col */
		getmaxyx(stdscr, lin, col);
		if (lin < MIN_LINES || col < MIN_COLS)
		{
			move(0, 0);
			printw("Trebuie sa ai terminalul de minim %03d linii si %03d coloane.\n"
				"Terminalul tau are %03d linii si %03d coloane.\nApasa o tasta dupa"
				"ce ai marit fereastra.", MIN_LINES, MIN_COLS, lin, col);
			refresh();
			getch();
		}
	}
	while (lin < MIN_LINES || col < MIN_COLS);
}
void reafiseazaHarta(void)
{
	int i;
	afiseazaFundal();
	afiseazaGranita();
	for (i=0; i<15; i++) 
		afiseazaTeritoriu((char)i + 'A', culoriTeritorii[i]);
}
