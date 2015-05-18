/*
 * Subor:   proj3.c
 * Datum:   2013/12/06
 * Autor:   Filip Ježovica, xjezov01@stud.fit.vutbr.cz
 * Projekt: Hledání obrazců, projekt c. 3 pre predmet IZP
 * Popis:   Program v danom monochromatickom obrazku hlada najdlhsie vodorovne a zvisle usecky a najvatsie stvorce
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define CHYBA fprintf(stderr, "ERROR!! Neplatne spustenie programu! pre viac info spusti napovedu  --help    \n")
#define CHYBA_MATICA fprintf(stderr, "ERROR! matica je chybna.  Skontroluj a spust znovu\n")
#define FALSE 0
#define TRUE 1
#define FAIL 1
#define SUCCESS 0
#define INVALID printf("Invalid\n")
#define VALID printf("Valid\n")

typedef struct
{
    int rows;
    int cols;
    char *cells;
} Bitmap;

//fcie
void help();
int test(FILE *subor, Bitmap *bitmap);
int inicializuj(FILE *subor,Bitmap *bitmap);
int nacitaj(FILE *subor, Bitmap *bitmap);
char getcolor(Bitmap *bitmap, int x, int y);
int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);
int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);
int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);


int main(int argc, char *argv[])
{
    if(argc == 1)                                            //ak nemam vstupne argumenty od uzivatela programu
    {
        CHYBA;
        return FAIL;
    }

    if((strcmp(argv[1], "--help") == 0))
    {
        help();
        return SUCCESS;
    }

    if(argc == 3)
    {
        FILE *subor;                                   // otvorime subor
        if((subor = fopen(argv[2], "r")) == NULL )     // a otestujem ci sa to podarilo
        {
            fprintf(stderr,"ERROR!! Subor sa nepodarilo otvorit.\n");
            return FAIL;
        }

        Bitmap bitmap;                            // nastavim strukturu


        if((strcmp(argv[1], "--test") == 0))      // len test matice
        {
            if(test(subor, &bitmap) == FALSE)     // testuje, najprv iniciaalizuje maticu , alokuje pamet, otestuje
            {
                INVALID;
                return FAIL;
            }

            VALID;
            free(bitmap.cells);
            return SUCCESS;
        }

        if((strcmp(argv[1], "--hline") == 0))           // horizontalna  usecka
        {
            if(test(subor, &bitmap) == FALSE)           // otestujem ci je validna matica, pritom sa aj inicializuje a nacita
            {
                CHYBA_MATICA;
                return FAIL;
            }

            fclose(subor);                             // treba zavret subor

            int x1, y1, x2, y2;

            if((find_hline(&bitmap, &x1, &y1, &x2, &y2)) == TRUE)
                printf("%d %d %d %d\n", x1, y1, x2, y2);

            free(bitmap.cells);                      // treba uvolnit pamet
            return SUCCESS;
        }

        if((strcmp(argv[1], "--vline") == 0))        // vertikalna usecka
        {
            if(test(subor, &bitmap) == FALSE)        // otestujem ci je validna matica, pritom sa aj inicializuje a nacita
            {
                CHYBA_MATICA;
                return FAIL;
            }

            fclose(subor);                           // treba zavret subor

            int x1, y1, x2, y2;

            if((find_vline(&bitmap, &x1, &y1, &x2, &y2)) == TRUE)
                printf("%d %d %d %d\n", x1, y1, x2, y2);

            free(bitmap.cells);                    // treba uvolnit pamet
            return SUCCESS;
        }

        if((strcmp(argv[1], "--square") == 0))
        {
            if(test(subor, &bitmap) == FALSE)
            {
                CHYBA_MATICA;
                return FAIL;
            }

            fclose(subor);                          // treba zavret subor

            int x1, y1, x2, y2;
            if(find_square(&bitmap, &x1, &y1, &x2, &y2) == TRUE)
                printf("%d %d %d %d\n", x1, y1, x2, y2);

            free(bitmap.cells);                    // treba uvolnit pamet
            return SUCCESS;
        }
    }
    CHYBA;               //cokolvek ine nastane je to chyba
    return FAIL;
}



//definicia funkcii
void help()
{
    printf("Vitaj! Spustil si napovedu! (--help )  \n\n"
           "Autor: FILIP JEZOVICA, xjezov01@stud.fit.vutbr.cz\n  \n\n"
           "Toto je program, ktory v danom monochromatickom obrazku hlada\n"
           "najdlhsie vodorovne a zvisle usecky a najvatsie stvorce\n\n"
           "Obrazok musi byt ulozeny ako bitbitmap v textovom soubore\n"
           "vo forme obdlznikovej matice nul a jedniciek\n\n\n"
           "Argumenty pri spustani programu: \n\n"
           "    --> --square X.txt <-- Vytiskne pociatocnu a koncovu souuadnicu prveho najvecsieho stvorca\n"
           "    --> --hline X.txt  <-- Vytiskne pociatocnu a koncovu souuadnicu prvej najdlhsej horizontalnej usecky\n"
           "    --> --vline X.txt  <-- Vytiskne pociatocnu a koncovu souuadnicu prvej najdlhsej vertikalnej  usecky\n\n"
           "    --> --test X.txt   <-- Overi ci X.txt je riadnou definiciou bitmapoveho obrazka\n"
           "      *X.txt je subor - bitbitmap obrazka\n\n\n");
}


int inicializuj(FILE *subor,Bitmap *bitmap)                            // nacitam kolko riadkocv a stlpcov mame a alokujem pamet
{
    if(fscanf(subor,"%d %d",&bitmap->rows,&bitmap->cols) != 2)         // zistim aka je to matica napr 2x3
        return FALSE;

    bitmap->cells = (char *)malloc(sizeof(char) * bitmap->rows * bitmap->cols);         // alokujem pre bunky
    if(bitmap->cells == NULL)
    {
        printf("Nepodarilo sa alokovat pamet!! Skus znova..\n");
        return FALSE;
    }

    return TRUE;            // vsetko sa podarilo
}


int nacitaj(FILE *subor, Bitmap *bitmap)                        // zisti pocet riadkov a stlpcov //ok
{
    if(bitmap->rows * bitmap->cols < 2)                         // ak mam pocet buniek matice menej ako 2 chyba
        return FALSE;

    int i, k;
    for(i = 0; i < (bitmap->rows) * (bitmap->cols); i++)      // cyklus pojde tolko krat ako  poct riadkov x  pocet stlpcov
    {
        if(fscanf(subor,"%d",&k) != 1)                        // ak nenacita cislo chyba
            return FALSE;

        if(k > 1 || k < 0)                                    // cislo v matici moze byt len 0 alebo 1 ... ostatne zle
            return FALSE;

        bitmap->cells[i] = k;
    }

    char c;                                                   // overenie ci je nieco za maticu este..ak je tak chyba
    while( (c = fgetc(subor)) != EOF)                         // biele znaky povolene
        if( !isspace(c))
            return FALSE;


    return TRUE;       // vsetko ok
}


int test(FILE *subor, Bitmap *bitmap)               // otestuje danu maticu ...
{
    if(inicializuj(subor, bitmap) == FALSE)         // inicializuje + alokuje pamet
    {
        free(bitmap->cells);
        return FALSE;
    }

    if(nacitaj(subor, bitmap) == FALSE)             // nacita prvky matice
    {
        free(bitmap->cells);
        return FALSE;
    }

    return TRUE;                                    // vsetko bolo ok
}


char getcolor(Bitmap *bitmap, int x, int y)                  // vrati mi na ktorej bunde som
{
    return bitmap->cells[x*bitmap->cols+y];
}


int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)    // najde najdlhsiu horizontalnu usecku
{
    int x;
    int y;
    int tmp_startx = 0;
    int tmp_starty = 0;
    int dlzka;
    int save_dlzka = 0;

    for(x = 0; x < bitmap->rows ; x++ )           //riadok .. to je suradnica x
    {
        dlzka = 0;

        for(y = 0; y < bitmap->cols; y++)         // stlpec  .. y
        {
            if( ((bitmap->cols) - y)  == save_dlzka )   // nemusi hladat dalej
                break;

            if(getcolor(bitmap, x ,y ))           // ak mam 1 teda farbu
            {
                if(dlzka == 0)
                {
                    tmp_startx = x;               // ulozim zaciatocne suradnice
                    tmp_starty = y;
                }
                dlzka++;

                if(dlzka > 1 && dlzka > save_dlzka)   // zapisujem iba ak mam vecsiu usecku ako predtym a musi to byt usecka
                {
                    // prave mam najvecsu usecku
                    *x1 = tmp_startx;
                    *y1 = tmp_starty;
                    *x2 = x;
                    *y2 = y;
                    save_dlzka = dlzka;               // ulozim dlzku najvecsej usecky
                }
            }

            else                                     // ak nemam farbu vynulujem
            {
                dlzka = 0;
            }
        }
    }
    if(save_dlzka == 0)
    {
        printf("Nenasla sa ziadna horizontana usecka\n");
        return FALSE;
    }

    return TRUE;
}


int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)           // najde najdlhsiu vertikalnu usecku
{
    int x;
    int y;
    int tmp_startx = 0;
    int tmp_starty = 0;
    int dlzka;
    int save_dlzka = 0;

    for(y = 0; y < bitmap->cols; y++)         // stlpec  .. y
    {
        dlzka = 0;

        for(x = 0; x < bitmap->rows ; x++ )           //riadok .. to je suradnica x
        {
            if( ((bitmap->rows) - x)  == save_dlzka )   // nemusi hladat dalej
                break;

            if(getcolor(bitmap, x ,y ))           // ak mam 1 teda farbu
            {
                if(dlzka == 0)
                {
                    tmp_startx = x;               // ulozim zaciatocne suradnice
                    tmp_starty = y;
                }
                dlzka++;

                if(dlzka > 1 && dlzka > save_dlzka)   // zapisujem iba ak mam vecsiu usecku ako predtym a musi to byt usecka
                {
                    // prave mam najvecsu usecku
                    *x1 = tmp_startx;
                    *y1 = tmp_starty;
                    *x2 = x;
                    *y2 = y;
                    save_dlzka = dlzka;               // ulozim dlzku najvecsej usecky
                }

                if((dlzka == save_dlzka) && (*x1 > tmp_startx))      // musim hladat ciaru ktora zacina vo vissom radecku
                {
                    *x1 = tmp_startx;
                    *y1 = tmp_starty;
                    *x2 = x;
                    *y2 = y;
                    save_dlzka = dlzka;
                }
            }

            else                                     // ak nemam farbu vynulujem
            {
                dlzka = 0;
            }
        }
    }
    if(save_dlzka == 0)
    {
        printf("Nenasla sa ziadna vertikalna usecka\n");
        return FALSE;
    }

    return TRUE;
}


int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    int XX;
    int YY;
    int save_dlzka = 0;
    int posuv;
    int MAX;
    int MAX_X;
    int MAX_Y;
    int PODMIENKA;

    int x;
    int y;

    for(x = 0; x < bitmap->rows ; x++ )           //riadok .. to je suradnica x
    {

        MAX_Y = (bitmap->rows - 1) - x;


        for(y = 0; y < bitmap->cols; y++)                 // stlpec  .. y
        {
            MAX_X = (bitmap->cols - 1) - y;




            if(getcolor(bitmap, x ,y ))                // ak mam 1 teda farbu
            {
                if(MAX_X <= MAX_Y)
                {
                    MAX = MAX_X;
                }
                else
                    MAX = MAX_Y;

                if(MAX == 0)
                    break;                            // stopnem a idem na dalsi riadok


                XX = x;             // tu prevediem do akoby novej suradnicovej sustavy. potom si predstavim ako keby by zacinala 0,0
                YY = y;
                PODMIENKA = TRUE;
                MAX = MAX + 1;

                while(PODMIENKA == TRUE)   // tu sa mi to cikli   prejde do else
                {
                    MAX--;
                    if(MAX == 0 || MAX <= save_dlzka)
                        break;

                    if((getcolor(bitmap, XX + MAX, YY + MAX)) == 0)
                    {
                        continue;
                    }

                    else                 // ok mame zaciatocnu suradnicu ..
                    {
                        for(posuv = 0;   ; posuv++)     // to bude bez podmienky, riesim ukoncenie v tele sam
                        {
                            if(posuv == MAX)
                            {

                                if( (getcolor(bitmap, XX + posuv, YY)) && (getcolor(bitmap, XX, YY + posuv)) && (MAX > save_dlzka) )
                                {
                                    *x1 = XX;
                                    *y1 = YY;
                                    *x2 = XX + MAX;
                                    *y2 = YY + MAX;
                                    save_dlzka = MAX;
                                    PODMIENKA = FALSE;
                                }
                                break;

                            }

                            else
                            {
                                if((getcolor(bitmap, XX + posuv, YY)) && (getcolor(bitmap, XX, YY + posuv)))
                                    ;   // ok
                                else
                                    break;       // tu to treba vypat  brejknem for vypitam max-1

                                if((getcolor(bitmap, XX + MAX - posuv, YY + MAX )) && (getcolor(bitmap, XX + MAX, YY + MAX - posuv)))
                                    ;    // ok
                                else
                                    break;        // tu to treba vypat  brejknem for vypitam max-1
                            }
                        }
                    }
                }
            }
        }
    }

    if(save_dlzka == 0)
    {
        printf("Nenasiel sa stvorec\n");
        return FALSE;
    }
    return TRUE;
}
