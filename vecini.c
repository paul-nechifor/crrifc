#include <stdlib.h>
#include "vecini.h"

char vecini[15][8] = {
	/*A*/ {'B', 'O', 'K'},
	/*B*/ {'A', 'O', 'C'},
	/*C*/ {'B', 'O', 'D'},
	/*D*/ {'C', 'O', 'E'},
	/*E*/ {'D', 'O', 'N', 'F'},
	/*F*/ {'E', 'N', 'G'},
	/*G*/ {'H', 'M', 'N', 'F'},
	/*H*/ {'I', 'M', 'G'},
	/*I*/ {'J', 'L', 'M', 'H'},
	/*J*/ {'K', 'L', 'I'},
	/*K*/ {'A', 'O', 'L', 'J'},
	/*L*/ {'J', 'K', 'O', 'N', 'M', 'I'},
	/*M*/ {'L', 'N', 'G', 'H', 'I'},
	/*N*/ {'O', 'E', 'F', 'G', 'M', 'L'},
	/*O*/ {'K', 'A', 'B', 'C', 'D', 'E', 'N', 'L'}
};
int nvecini[15] = {3, 3, 3, 3, 4, 3, 4, 3, 4, 3, 4, 6, 5, 6, 8};

int suntVecini(char t1, char t2)
{
	int i;
	for (i=0; i<nvecini[t1-'A']; i++)
		if (vecini[t1-'A'][i] == t2)
			return 1;
	return 0;
}
void teritoriiDeStart(char* ter)
{
	/* rand trebuie sa fie initializat altundeva */
	ter[0] = (char)(rand() % 15) + 'A';
	
	do ter[1] = (char)(rand() % 15) + 'A';
	while (suntVecini(ter[0], ter[1]) ||
			ter[0] == ter[1]
			);

	do ter[2] = (char)(rand() % 15) + 'A';
	while (suntVecini(ter[0], ter[2]) || 
			suntVecini(ter[1], ter[2]) ||
			ter[0] == ter[2] || 
			ter[1] == ter[2]
			);
}
void veciniPentru(char* ter, int nrter, char* veci, int* nrveci)
{
	char litera;
	int i, j, k, gasit;

	*nrveci = 0;
	
	for (i=0; i<nrter; i++)
		for (j=0; j<15; j++)
		{
			litera = (char)j + 'A';
			if (suntVecini(ter[i], litera))
			{
				/* verifica sa nu fie in ter si sa fi fost adaugat deja in veci */
				gasit = 0;
				for (k=0; k<nrter; k++)
					if (ter[k] == litera) { gasit = 1; break; }
				if (gasit) continue;
				for (k=0; k<(*nrveci); k++)
					if (veci[k] == litera) { gasit = 1; break; }
				if (gasit) continue;
				
				veci[*nrveci] = litera;
				(*nrveci)++;
			}
		}
}
void scoateTeritorii(char* ter, int* nrter, char* scazator, int nrscazator)
{
	char nou[15];
	int i, j, gasit, nrnou = 0;
	
	for (i=0; i<(*nrter); i++)
	{
		gasit = 0;
		for (j=0; j<nrscazator; j++)
			if (ter[i] == scazator[j]) { gasit = 1; break; }
		if (!gasit)
			nou[nrnou++] = ter[i];
	}
	for (i=0; i<nrnou; i++)
		ter[i] = nou[i];
	*nrter = nrnou;
}
