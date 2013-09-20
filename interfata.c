#include <curses.h>
#include <string.h>
#include <ctype.h>
#include "interfata.h"
#include "culori.h"
#include "macro.h"

void creazaInterfata(char* j1, char* j2, char* j3)
{
	int i, pe = (LUNGIME - 3 * 25) / 6;
	
	move(START_PUNCTE, 0);

	attrset(COLOR_PAIR(COL_J1));
	for (i=0; i<pe; i++) addch(' ');
	printw("J1: %-12s  P: %04d", j1, 0);
	for (i=0; i<pe; i++) addch(' ');
	
	attrset(COLOR_PAIR(COL_J2));
	for (i=0; i<pe; i++) addch(' ');
	printw("J2: %-12s  P: %04d", j2, 0);
	for (i=0; i<pe; i++) addch(' ');
	
	attrset(COLOR_PAIR(COL_J3));
	for (i=0; i<pe; i++) addch(' ');
	printw("J3: %-12s  P: %04d", j3, 0);
	for (i=0; i<pe; i++) addch(' ');
	
	CAPAT();
	refresh();
}
void puncteJucator(int nr, int puncte)
{
	/* (2*nr-1)pe + nr*25 - 4 */
	switch (nr)
	{
		case 1: attrset(COLOR_PAIR(COL_J1)); break;
		case 2: attrset(COLOR_PAIR(COL_J2)); break;
		case 3: attrset(COLOR_PAIR(COL_J3)); break;
	}
	move(START_PUNCTE, (2*nr-1)*((LUNGIME - 3*25)/6) + nr*25 - 4 );
	printw("%04d", puncte);

	CAPAT();
	refresh();
}
void status(char* mesaj)
{
	char *m = "Status: ";
	int i, rest = LUNGIME - strlen(m) - strlen(mesaj);

	move(START_MESAJE, 0);
	attrset(COLOR_PAIR(COL_STATUS));
	printw("%s%s", m, mesaj);
	for (i=0; i<rest; i++) addch(' ');

	CAPAT();
	refresh();
}
void mesajServer(char* mesaj)
{
	char *m = "Mesaj server: ";
	int i, rest = LUNGIME - strlen(m) - strlen(mesaj);

	move(START_MESAJE + 1, 0);
	attrset(COLOR_PAIR(COL_MESAJ_SERVER));
	printw("%s%s", m, mesaj);
	for (i=0; i<rest; i++) addch(' ');

	CAPAT();
	refresh();
}
void conturColorat(int lin, int col, int llin, int lcol, int culoare)
{
	int i;
	attrset(COLOR_PAIR(culoare));

	/* conturul orizontal */
	move(lin, col);
	for (i=0; i<lcol; i++) addch(' ');
	move(lin+llin-1, col);
	for (i=0; i<lcol; i++) addch(' ');

	/* conturul vertical */
	for (i=0; i<llin; i++)
	{
		mvaddch(lin + i, col, ' ');
		mvaddch(lin + i, col + lcol -1, ' ');
	}

}
void chenarColorat(int lin, int col, int llin, int lcol, int culoare, char* text)
{
	int i, j, str;
	attrset(COLOR_PAIR(culoare));
	for (i=lin; i<lin+llin; i++)
	{
		move(i, col);
		for (j=0; j<lcol; j++) addch(' ');
	}
	str = strlen(text);
	for (i=0, j=0; i<str; i++)
	{
		if (i % lcol == 0) move(lin + (j++), col);
		addch(text[i]);
	}
}
int puneIntrebareRapida(char* intrebare)
{
	int i, j, n, ret, len, endTime;
	char c;

	/* fundalul si conturul */
	conturColorat(START_INTR_LIN, START_INTR_COL, 12, 104, COL_INTREBARE);
	chenarColorat(START_INTR_LIN + 1, START_INTR_COL + 1, 10, 102, COL_FUNDAL_INTREBARE, "");

	/* afiseaza intrebarea */
	mvaddstr(START_INTR_LIN + 2, START_INTR_COL + 43, "INTREBARE RAPIDA");
	chenarColorat(START_INTR_LIN + 4, START_INTR_COL + 2, 2, 100, COL_INTREBARE, intrebare);

	/* locul pentru raspuns */
	attrset(COLOR_PAIR(COL_RASPUNS));
	mvprintw(START_INTR_LIN + 7, START_INTR_COL + 47, "         ");

	timeout(100);
	noecho();
	endTime = time(NULL) + TIMP_MAX_INTR;
	ret = n = 0;

	while (time(NULL) <= endTime)
	{
		attrset(COLOR_PAIR(COL_FUNDAL_INTREBARE));
		mvprintw(START_INTR_LIN + 9, START_INTR_COL + 42, "Timp de raspuns: %02d", endTime - time(NULL));
		
		attrset(COLOR_PAIR(COL_RASPUNS));
		move(START_INTR_LIN + 7, START_INTR_COL + 47 + n);
		refresh();
		c = getch();

		if (c == ERR) continue; /* adica nu a fost citit niciun caracter */
		if (c == '\n') break;
		if (c == '-') /* nu pot sa fac backspace sa mearga */
		{
			n--;
			if (ret) mvaddch(START_INTR_LIN + 7, START_INTR_COL + 47 + n, ' ');
			ret /= 10;
			continue;
		}
		if (c < '0' || c > '9') continue;
		addch(c);
		n++;
		ret = ret * 10 + (c-'0');
		if (n > 8) break;
	}

	echo();
	CAPAT();
	refresh();

	return ret;
}
int puneIntrebareGrila(char* intrebare, char* v1, char* v2, char* v3, char* v4)
{
	int ret, endTime;
	char c;

	/* fundalul si conturul */
	conturColorat(START_INTG_LIN, START_INTG_COL, 16, 104, COL_INTREBARE);
	chenarColorat(START_INTG_LIN + 1, START_INTG_COL + 1, 14, 102, COL_FUNDAL_INTREBARE, "");

	/* intrebarea */
	mvaddstr(START_INTG_LIN + 2, START_INTG_COL + 43, "INTREBARE GRILA");
	chenarColorat(START_INTG_LIN + 4, START_INTG_COL + 2, 2, 100, COL_INTREBARE, intrebare);

	/* variantele de raspuns */
	chenarColorat(START_INTG_LIN + 7, START_INTG_COL + 5, 2, 46, COL_RASPUNS, v1);
	chenarColorat(START_INTG_LIN + 7, START_INTG_COL + 55, 2, 46, COL_RASPUNS, v2); 
	chenarColorat(START_INTG_LIN + 10, START_INTG_COL + 5, 2, 46, COL_RASPUNS, v3); 
	chenarColorat(START_INTG_LIN + 10, START_INTG_COL + 55, 2, 46, COL_RASPUNS, v4); 

	/* litera corespunsatoare */
	attrset(COLOR_PAIR(COL_INTREBARE));
	mvaddstr(START_INTG_LIN + 7, START_INTG_COL + 3, "a)");
	mvaddstr(START_INTG_LIN + 7, START_INTG_COL + 53, "b)");
	mvaddstr(START_INTG_LIN + 10, START_INTG_COL + 3, "c)");
	mvaddstr(START_INTG_LIN + 10, START_INTG_COL + 53, "d)");

	attrset(COLOR_PAIR(COL_FUNDAL_INTREBARE));

	timeout(333);
	noecho();
	endTime = time(NULL) + TIMP_MAX_INTG;
	ret = 0;

	while (time(NULL) <= endTime)
	{
		mvprintw(START_INTG_LIN + 13, START_INTG_COL + 42, "Timp de raspuns: %02d", endTime - time(NULL));
		CAPAT();
		refresh();
		c = getch();

		if (c == ERR) continue; /* adica nu a fost citit niciun caracter */
		if (c == '\n') break;
		if (c >= '1' && c <= '4') return c - '0';
		if (c >= 'a' && c <= 'd') return c - 'a' + 1;
		if (c >= 'A' && c <= 'D') return c - 'A' + 1;
	}
	CAPAT();
	refresh();

	return ret;
}
void afiseazaRaspunsRapid(int corect, int j1, int j2, int j3)
{
	conturColorat(START_AFISR_LIN, START_AFISR_COL, 7, 50, COL_INTREBARE);
	chenarColorat(START_AFISR_LIN + 1, START_AFISR_COL + 1, 5, 48, COL_FUNDAL_INTREBARE, "");
	mvprintw(START_AFISR_LIN + 1, START_AFISR_COL + 3, "Raspunsul corect este: %d", corect);

	attrset(COLOR_PAIR(COL_J1));
	mvprintw(START_AFISR_LIN + 3, START_AFISR_COL + 1, "Jucatorul 1: %-10d", j1); 
	attrset(COLOR_PAIR(COL_J2));
	mvprintw(START_AFISR_LIN + 4, START_AFISR_COL + 1, "Jucatorul 2: %-10d", j2);
	attrset(COLOR_PAIR(COL_J3));
	mvprintw(START_AFISR_LIN + 5, START_AFISR_COL + 1, "Jucatorul 3: %-10d", j3);

	CAPAT();
	refresh();
	napms(4000);
}
void afiseazaRaspunsGrila(int corect, int j1, int j2, int j3)
{
	conturColorat(START_AFISR_LIN, START_AFISR_COL, 7, 50, COL_INTREBARE);
	chenarColorat(START_AFISR_LIN + 1, START_AFISR_COL + 1, 5, 48, COL_FUNDAL_INTREBARE, "");
	mvprintw(START_AFISR_LIN + 1, START_AFISR_COL + 3, "Raspunsul corect este: %c)", 'a' + (char)(corect-1));

	attrset(COLOR_PAIR(COL_J1));
	move(START_AFISR_LIN + 3, START_AFISR_COL + 1);
	if (j1) printw("Jucatorul 1:   %c)          ", 'a' + (char)(j1-1));
	else printw("Jucatorul 1:   nu a raspuns");

	attrset(COLOR_PAIR(COL_J2));
	move(START_AFISR_LIN + 4, START_AFISR_COL + 1);
	if (j2) printw("Jucatorul 2:   %c)          ", 'a' + (char)(j2-1));
	else printw("Jucatorul 2:   nu a raspuns");
	
	attrset(COLOR_PAIR(COL_J3));
	move(START_AFISR_LIN + 5, START_AFISR_COL + 1);
	if (j3) printw("Jucatorul 3:   %c)          ", 'a' + (char)(j3-1));
	else printw("Jucatorul 3:   nu a raspuns");

	CAPAT();
	refresh();
	napms(4000);
}
char teritoriuDeAtacat(char* pos, int nr)
{
	int i;
	char c;

	conturColorat(START_TER_LIN, START_TER_COL, 5, 32, COL_INTREBARE);
	chenarColorat(START_TER_LIN + 1, START_TER_COL + 1, 3, 30, COL_FUNDAL_INTREBARE, "Trebuie sa ataci un teritoriu.");
	mvaddstr(START_TER_LIN + 2, START_TER_COL + 1, "Alege dintre: ");
	mvaddstr(START_TER_LIN + 3, START_TER_COL + 4, "[");
	for (i=0; i<nr; i++) addch(pos[i]);
	addch(']');
	CAPAT();
	noecho();
	refresh();

	timeout(-1);
	for (;;)
	{
		c = getch();
		for (i=0; i<nr; i++)
			if (pos[i] == toupper(c)) return toupper(c);
	}
}
char teritoriuDeAles(char* pos, int nr)
{
	int i;
	char c;

	conturColorat(START_TER_LIN, START_TER_COL, 5, 32, COL_INTREBARE);
	chenarColorat(START_TER_LIN + 1, START_TER_COL + 1, 3, 30, COL_FUNDAL_INTREBARE, "Trebuie sa alegi un teritoriu.");
	mvaddstr(START_TER_LIN + 2, START_TER_COL + 1, "Alege dintre: ");
	mvaddstr(START_TER_LIN + 3, START_TER_COL + 4, "[");
	for (i=0; i<nr; i++) addch(pos[i]);
	addch(']');
	CAPAT();
	noecho();
	refresh();

	timeout(-1);
	for (;;)
	{
		c = getch();
		for (i=0; i<nr; i++)
			if (pos[i] == toupper(c)) return toupper(c);
	}
}
