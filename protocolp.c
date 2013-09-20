#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <curses.h>
#include "protocolp.h"
#include "macro.h"

extern char mesaj[MMESAJ];
extern char **campuri;
extern int ncamp;

void scrie(int n, int fifo, ...)
{
	int i;
	va_list args;

	strcpy(mesaj, "");

	va_start(args, n);
	for (i=0; i<n; i++)
	{
		if (i) strcat(mesaj, "|");
		strcat(mesaj, va_arg(args, char*));
	}
	va_end(args);

	strcat(mesaj, "\n");
	
	/* mesajul este scris tot de o data pentru a fi atomic */
	if (write(fifo, mesaj, strlen(mesaj)) == -1) DIE("scrie()");
}
int citeste(int fifo)
{
	int n, l=0, i, k, ret=0;
	char c;
	
	n = read(fifo, &c, 1);
	if ( n == -1 )
	{
		if (errno == EAGAIN) return NIMIC;
		perror("citeste(): read():");
		exit(1);
	}
	
	if (n == 0) return NIMIC;

	mesaj[l++] = c;
	ncamp = 1;
	for(;;)
	{
		if (read(fifo, &c, 1) < 1) DIE("Eroare. Nu mai sunt caractere de citit si nu s-a dat de '\\n'");
		if (c == '|') ncamp++;
		if (c != '\n')
		{
			if (l >= MMESAJ-2) DIE("Eroare. Lungimea mesajului nu poate fi atat de mare.");
			mesaj[l++] = c;
		}
		else break;
	}

	mesaj[l] = 0;		
	if (campuri) free(campuri);
	campuri = (char**) malloc(ncamp * sizeof(char *));
	
	campuri[0] = &mesaj[0];
	for (i=1, k=1; i<l; i++)
	{
		if (mesaj[i] == '|')
		{
			mesaj[i] = 0;
			campuri[k++] = &mesaj[i+1];
		}
	}
	if (strlen(campuri[0]) != 4)
	{
		endwin();
		printf("Eroare. Primul camp ('%s')trebuie sa aiba mereu 4 litere.\n", campuri[0]);
		exit(1);
	}

	for (i=0, ret=0; i<4; i++)
		ret = (ret<<8) | campuri[0][i];
	return ret;
}
