#define MMESAJ 4096
#define NIMIC 123

/* variabile globale */
char mesaj[MMESAJ];
char **campuri;
int ncamp;

/**
 * Functia scrie() are numar variabil de parametrii char*. Ii scrie pe toti in canalul fifo.
 * Exemplu:
 *      scrie(2, canal, "CONN", "paul.nechifor");    va scrie
 *      "CONN|paul.nechifor\n"
 * ar trebui sa verific daca este '\n' printre paremetii
 */
void scrie(int n, int fifo, ...);
/**
 * Functia citeste() citeste din canalul fifo un singur mesaj.
 * Daca nu a fost gasit niciun mesaj intoarce NIMIC.
 * Dupa citire, numarul de mesaje se va afla in variabila globala 'ncamp' si campurile in 
 * variabila globala 'campuri'.
 */
int citeste(int fifo);
