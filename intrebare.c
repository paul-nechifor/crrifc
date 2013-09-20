#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "intrebare.h"
#include "macro.h"

void getIntrebareRapida(char text[200], int* corect)
{
	int i, j, index, bd, poz, nr, n, marime;
	char buf[2048], numar[20];

	/* deschide fisierul cu indecsi */
	if ( (index = open("rapide.idx", O_RDONLY)) == -1)
		DIE("getIntrebareRapida() E01");
	
	/* afla numarul de intrebari calculand marimea fisieului index */
	lseek(index, 0, SEEK_END);
	marime = lseek(index, 0, SEEK_CUR) / 4;

	nr = rand() % marime;
	lseek(index, nr * 4, SEEK_SET);

	/* citeste pozitia pe care sa o citesti in 'baza de date' */
	if (read(index, &poz, 4) < 0) DIE("getIntrebareRapida() E02");
	if (close(index) == -1) DIE("getIntrebareRapida() E03");

	/* deschid 'baza de date' */
	if ( (bd = open("rapide.txt", O_RDONLY)) == -1)
		DIE("getIntrebareRapida() E04");
	lseek(bd, poz, SEEK_SET);
	if ( (n = read(bd, buf, 2048)) == -1)
		DIE("getIntrebareRapida() E05");

	/* copiaza textul */
	i = 0;
	while (buf[i] != '\n')
	{
		text[i] = buf[i];
		i++;
	}
	text[i] = 0;
	i++;
	j = 0;

	/* citeste stringul cu numarul si transformal in int */
	while (buf[i] != '\n')
	{
		numar[j] = buf[i];
		i++;
		j++;
	}
	numar[j] = 0;
	*corect = atoi(numar);

	if (close(bd) == -1) DIE("getIntrebareRapida() E06");
}
void getIntrebareGrila(char text[200], char ras[4][100], int* corect)
{
	int i, j, s, index, bd, poz, nr, n, marime;
	char buf[2048];

	/* deschide fisierul cu indecsi */
	if ( (index = open("grila.idx", O_RDONLY)) == -1)
		DIE("getIntrebareGrila() E01");
	
	/* afla numarul de intrebari calculand marimea fisieului index */
	lseek(index, 0, SEEK_END);
	marime = lseek(index, 0, SEEK_CUR) / 4;

	nr = rand() % marime;
	lseek(index, nr * 4, SEEK_SET);

	/* citeste pozitia pe care sa o citesti in 'baza de date' */
	if (read(index, &poz, 4) < 0) DIE("getIntrebareGrila() E02");
	if (close(index) == -1) DIE("getIntrebareGrila() E03");

	/* deschid 'baza de date' */
	if ( (bd = open("grila.txt", O_RDONLY)) == -1)
		DIE("getIntrebareGrila() E04");
	
	/* citirea */
	lseek(bd, poz, SEEK_SET);
	if ( (n = read(bd, buf, 2048)) == -1)
		DIE("getIntrebareGrila() E05");

	/* citeste textul intrebarii */
	i = 0;
	while (buf[i] != '\n')
	{
		text[i] = buf[i];
		i++;
	}
	text[i] = 0;
	i++;

	/* citeste textul raspunsurilor */
	for (j=0; j<4; j++)
	{
		s = 0;
		while (buf[i] != '\n')
		{
			ras[j][s] = buf[i];
			i++;
			s++;
		}
		ras[j][s] = 0;
		i++;
	}

	/* vezi indicele raspunsului corect. Scad '1' pentru ca raspunsul sa fie de la 0 la 3*/
	*corect = buf[i] - '1';
	
	if (close(bd) == -1) DIE("getIntrebareGrila() E06");
}
