#ifndef _CONST_H
#define _CONST_H

/* cat de mare trebuie sa fie terminalul pentru a putea rula corect */
#define MIN_LINES 44
#define MIN_COLS 117

/* lungimea maxima a numelui */
#define NUME_MAX 12

/* timpul maxim de raspuns */
#define TIMP_MAX_INTR 15
#define TIMP_MAX_INTG 15

/* muta cursorul din clale */
#define CAPAT() move(MIN_LINES-1, MIN_COLS-1)

/* termina programul inchizand curses corect */
#define DIE(x) { endwin(); perror(x); exit(1); }

/* mesajele pe care le trimite clientul */
#define C_CONE 1129270853
#define C_TRIR 1414678866
#define C_TRIG 1414678855
#define C_ALEG 1095517511
#define C_ATAC 1096040771

/* mesajele pe care le trimite serverul */
#define S_AIIN 1095321934
#define S_SNTJ 1397642314
#define S_JUCS 1247101779
#define S_PUNS 1347767891
#define S_INTR 1229870162
#define S_INTG 1229870151
#define S_RASR 1380012882
#define S_RASG 1380012871
#define S_JALG 1245793351
#define S_TBAL 1413628236
#define S_JALT 1245793364
#define S_JATC 1245795395
#define S_TBAT 1413628244
#define S_JATT 1245795412

#endif //_CONST_H
