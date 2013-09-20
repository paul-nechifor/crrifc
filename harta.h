#ifndef _HARTA_H
#define _HARTA_H

/* de unde incepe afisearea partilor hartii */
#define START_LIN 2
#define START_COL 0

/* cat de mare este harta */
#define HLIN 41
#define HCOL 117

void afiseazaGranita(void);
void afiseazaFundal(void);
void afiseazaTeritoriu(char t, int culoare);
void modificaTeritoriu(char t, int culoare);

#endif //_HARTA_H
