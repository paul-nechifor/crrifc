#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int i, d, n, in, out, lin=1, poz=0, afost=1;
	char buf[1024];

	in = open("grila.txt", O_RDONLY);
	out = creat("grila.idx", 0644);
	while ( (n=read(in, buf, 1024)) > 0)
	{
		for (i=0; i<n; i++)
		{
			if (lin % 6 == 1 && afost)
			{
				write(out, &poz, 4);
				printf("chr = '%c' lin=%d  poz=%d  \n", buf[i], lin, poz);
			}
			if (buf[i] == '\n')
			{
				lin++;
				afost = 1;
			}
			else afost = 0;
			poz++;
		}

	}
	close(in);
	close(out);
	return 0;
}
