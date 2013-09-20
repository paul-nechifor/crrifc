#ifndef _VECINI_H
#define _VECINI_H

/**
 *  Intoarce 1 daca teritoriile t1 si t2 sunt invecinate.
 */
int suntVecini(char t1, char t2);
/**
 *  In 'ter' pune un vector cu trei elemente (nu string) care contine
 *  teritorii ce nu se invecineaza.
 */
void teritoriiDeStart(char* ter);
/**
 *  Pentru toate teritoriile din 'ter' intoarce toate teritoriile 
 *  cu care se invecineaza fiecare, cu exceptia celor care se afla
 *  in 'ter'. Astfel pot sa aflu pe cine poate sa atace un jucator.
 */
void veciniPentru(char* ter, int nrter, char* veci, int* nrveci);
/**
 *  Scoate din 'ter' toate teritoriile din 'scazator'
 */
void scoateTeritorii(char* ter, int* nrter, char* scazator, int nrscazator);

#endif //_VECINI_H
