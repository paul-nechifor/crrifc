#ifndef _INTERFATA_H
#define _INTERFATA_H

/* de unde incepe afisarea */
#define START_PUNCTE 0
#define START_MESAJE 42

/* de unde incep intrebarile */
#define START_INTR_LIN 18
#define START_INTR_COL 7
#define START_INTG_LIN 15
#define START_INTG_COL 7

/* de unde incepe afisarea raspunsurilor */
#define START_AFISR_LIN 7
#define START_AFISR_COL 34
#define START_AFISG_LIN 7
#define START_AFISG_COL 34

/* de unde incepe alegerea si atacarea de teritorii */
#define START_TER_LIN 2
#define START_TER_COL 2

/* ce lungime au liniile */
#define LUNGIME 117

void creazaInterfata(char* j1, char* j2, char* j3);
void puncteJucator(int nr, int puncte);

void status(char* mesaj);
void mesajServer(char* mesaj);

void chenarColorat(int lin, int col, int llin, int lcol, int culoare, char* text);
void conturColorat(int lin, int col, int llin, int lcol, int culoare);

int puneIntrebareRapida(char* intrebare);
int puneIntrebareGrila(char* intrebare, char* v1, char* v2, char* v3, char* v4);

void afiseazaRaspunsRapid(int corect, int j1, int j2, int j3);
void afiseazaRaspunsGrila(int corect, int j1, int j2, int j3);

char teritoriuDeAtacat(char* pos, int nr);
char teritoriuDeAles(char* pos, int nr);

#endif //_INTERFATA_H
