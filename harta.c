#include <curses.h>
#include "harta.h"
#include "macro.h"
#include "culori.h"

char liniiHarta[42][120] = {
"                                       ####                           #####                                          ",
"                                     ###AA#####                    ####BBB###                                        ",
"                                  ####AAAAAAAA######       #########BBBBBBBB###                                      ",
"                          #########AAAAAAAAAAAAAAAA##### ###BBBBBBBBBBBBBBBBBB##                                     ",
"                         ##K##AAAAAAAAAAAAAaAAAAAAAAAAA###BBBBBBBBBbBBBBBBBBBBB####                                  ",
"                       ###KK###AAAAAAAAAAAAAAAAAAAAAAAAA##BBBBBBBBBBBBBBBBB#####CC##                                 ",
"                      ##KKKKKK################AAAAAAAAAA##BBBBBBBBBBBBBBB##CCCCCCCC###                               ",
"                    ###KKKKKKKKKKKKKKKKKKKKK##AAAAAAAAA##BBBBBB###########CCCCCcCCCCC####                            ",
"                   ##KKKKKKKKKKKKKKKKKKKKKKKK##AAAAAAA###########CCCCCCCCCCCCCCCCCCCCCCC##                           ",
"                  ##KKKKKKKKKKKKKkKKKKKKKKKKKK##AAA####OOOOOOOO##CCCCCCCCCCCCCCCC######CC##                          ",
"               #####KKKKKKKKKKKKKKKKKKKKKKKKK##AA###OOOOOOOOOOOO##CCCCCCC#########DDDD#######                        ",
"             ###JJJ#######KKKKK####KKKKKKKKKK#####OOOOOOOOOOOOOOO#########DDDDDDDDDDDDDDDDDD##                       ",
"            ##JJJJJJJJJJJ####K##LL########KKK#OOOOOOOOOOOOoOOOOOOO##DDDDDDDDDDDDDDDDDDDDDDDD##                       ",
" ##### ######JJJJJJJJJJJJJJJ###LLLLLLLLLL######OOOOOOOOOOOOOOOOOOOO###DDDDDDDDDDdDDDDDDDDDDD##                       ",
"##JJJ###JJJJJJJJJJJJJJJJJJ###LLLLLLLLLLLLLLLLL#######OOOOOOOOOOOOOOOO##DDDDDDDDDDDDDDDDDDDDD##                       ",
" ##JJJJJJJJJJJJJJJJJJJJJJ##LLLLLLLLLLLLLLLLLLLLLLLL###########OOOOOOOOO##DDDDD################                       ",
"   ##JJJJJJJJJJJJJJJJJJJ##LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL##OOOOOOOOO#######EEEEEEEEEEEEE##                       ",
"    ##JJJJJJJJJjJJJJJJJJJ##LLLLLLLLLLlLLLLLLLLLLLLLLLLLLLLLLLL##OOOOOOO##EEEEEEEEEEEeEEEEEEEE##                      ",
"    ##JJJJJJJJJJJJJJJJJJJJ##LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL####OO####EEEEEEEEEEEEEEEEEEEE##                     ",
"   ##JJJJJJJJJJJJJJJJJJJJJ##LLLLLLLLLLLLLLL###############LLLLLLLLLL###NN#####EEEEEEEEEEEEEEEE##                     ",
"    ##JJJJJJJJJJJJJJJJJJJJJ##LLLLLLLLLL#####MMMMMMMMMMMMM##LL##########NNNNNN######EE#####EEEE####        ########   ",
"     ####JJJJJJJJJJJJJJJJJ##############I##MMMMMMMMMMMMMMM####MMMMMMMM##NNNNNNNNNN####NNN######FF###  ####FFFFFFF##  ",
"        ####JJJJJJJJJJJJJJ##IIIIIIIIIIIII##MMMMMMMMMMMMMMMMMMMMMMMMMMM####NNNNNNNNNNNNNNNNNN##FFFFF####FFFFFFFFFFF## ",
"         ##JJJJJJJJJJJJJJ##IIIIIIIIIIIII###MMMMMMMMMMMMMmMMMMMMMMMMMMMMMM##NNNNNNNNNNNNNNNNNNN##FFFFFFFFFFFFFFFFFFF##",
"         ##JJJJJJJJJJJJ###IIIIIIIIIIIII##MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM##NNNNNNNnNNNNNNNNNNN##FFFFFFFFFFFFFFFFFF# ",
"        ##JJJJJJJJJJJ##IIIIIIIIIIIIIIII##MMMMM####MMMMMMMMMMMMMMMMMMMMMMMMM###NNNNNNNNNNNNNNNNN##FFFFFFFFFF##FFFF### ",
"         ###JJJJJJ#####IIIIIIIIIIIIIIII##MMMM##II##MMMMMMMMMMMMMMMMMMM#####GG####################FFFFFFFFF########   ",
"           ########   ###IIIIIIIIIIIIIII#MMMM##IIII##MMMMMMMMMMMMMM###HH##GGGGGGGGGGGGGGGGGGGG##FFFFFFFFF##          ",
"                     ####IIIIIIIiIIIIIII######IIIII##MMMMMM#########HHHHH##GGGGGGGGGGGGGGGGGGGG##FFFFfFFFF#          ",
"                     #IIIIIIIIIIIIIIIIIIIIIIIIIIIII#########HHHHHHHHHHHHHH##GGGGGGGGgGGGGGGGGGGG#FFFFFFFFF##         ",
"                     ##IIIIIIIIIIIIIIIIIIIIIIIIIIIIII##HHHHHHHHHHHHHHHHHHH##GGGGGGGGGGGGGGGGGGG##FFFFFFFF##          ",
"                      ##IIIIIIIIIIIIIIIIIIIIIIIIIIII##HHHHHHHHHHHHHHHHHHH##GGGGGGGGGGGGGGGGG####FFFFFFFFF##          ",
"                       ###IIIIIIIIIIIIIIIIIIIIIIIIII##HHHHHHHHHHHHHHHHHHHH##GGGGGGGGGGGGG###FFFFFFFFFFFFFF##         ",
"                         ###IIIIIIIIIIIIIIIIIIIIII###HHHHHHHHHhHHHHHHHHHHH###GGG##########FFFFFFFFFFFFFFFF##         ",
"                         ##IIIIIIIIIIIIIIIIIIIIII##HHHHHHHHHHHHHHHHHHHH##########        #########FFFFFFFFF##        ",
"                          ########IIIIIIIIIIIIIII##HHHHHHHHHHHHHHHHHH###                         ##FFFFFFFF##        ",
"                                 #######IIIIIIIIII##HHHHHHHHHHHHHHHH##                            ######FFF##        ",
"                                       ##################HHHHHHHHH###                                  #####         ",
"                                                        ###########                                                  "
};

void afiseazaGranita(void)
{
	int i, j, muta;
	attrset(COLOR_PAIR(COL_GRANITA));
	for (i=0; i<HLIN; i++)
	{
		muta = 1;
		for (j=0; j<HCOL; j++)
			if (liniiHarta[i][j] == '#')
			{
				if (muta) move(i+START_LIN, j+START_COL);
				addch(' ');
				muta = 0;
			}
			else muta = 1;
	}
	CAPAT();
	refresh();
}
void afiseazaFundal(void)
{
	int i, j;
	///bkgd(COLOR_PAIR(COL_FUNDAL));
	attrset(COLOR_PAIR(COL_FUNDAL));
	
	for (i=1; i<MIN_LINES-2; i++)
	{
		move(i, 0);
		for (j=0; j<MIN_COLS; j++)
			addch(' ');
	}

	CAPAT();
	refresh();
}
void afiseazaTeritoriu(char t, int culoare)
{
	int i, j, muta;
	attrset(COLOR_PAIR(culoare));
	for (i=0; i<HLIN; i++)
	{
		muta = 1;
		for (j=0; j<HCOL; j++)
			if (toupper(liniiHarta[i][j]) == t)
			{
				if (muta) move(i+START_LIN, j+START_COL);
				if (!(liniiHarta[i][j] & 32)) addch(' ');
				else addch(t);
				muta = 0;
			}
			else muta = 1;
	}
	CAPAT();
	refresh();
}
void modificaTeritoriu(char t, int culoare)
{
	int i, j, muta, aFost;
	attrset(COLOR_PAIR(culoare));
	for (i=0; i<HLIN; i++)
	{
		aFost = 0;
		muta = 1;
		for (j=0; j<HCOL; j++)
			if (toupper(liniiHarta[i][j]) == t)
			{
				if (muta) move(i+START_LIN, j+START_COL);
				if (!(liniiHarta[i][j] & 32)) addch(' ');
				else addch(t);
				muta = 0;
				aFost = 1;
			}
			else muta = 1;
		if (aFost)
		{
			CAPAT();
			refresh();
			napms(60);
		}
	}
	CAPAT();
	refresh();
}
