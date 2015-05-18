/*
 * Subor:   proj1.c
 * Datum:   2013/10/21
 * Autor:   Filip Ježovica, xjezov01@stud.fit.vutbr.cz
 * Projekt: Pocitani slov, projekt c. 1 pre predmet IZP
 * Popis:   Program pocita slová
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHYBA printf("ERROR!! Nezadal si platny argument potrebny pre chod programu! pre viac info spusti napovedu  --help / --h   \n")

int CIZNAK(char ss);
int CisloPoradia(char *str);
int FUN_ARGUMENT_X(char *str, int ss);

int main(int argc, char *argv[])
{
    int pocitam = 0, debug = 0;
    int uzivatel = 0;
    int medzera = 0;

    if(argc == 1)                                            //ak nemam vstupne argumenty od uzivatela programu
    {
        CHYBA;
        return 1;
    }

    if((strcmp(argv[1], "--help") == 0)   ||   (strcmp(argv[1], "--h") == 0) )
    {
        printf("Vitaj! Spustil si napovedu! (--help / --h)  \n\n"
               "Toto je program na pocitanie slov .....autor: FILIP JEZOVICA, xjezov01@stud.fit.vutbr.cz\n\n\n"
               "ako prvy argument moze byt :       pozor naraz iba 1\n\n"
               "      --> . <-- vsetky slova                --> : <-- slova v ktorych je cislo \n"
               "      --> ^ <-- slova s velkym pismenkom    --> X <-- akykolvek *dovoleny znak \n\n\n"
               "ako druhy alebo treti argument moze byt :       mozu byt obe naraz \n\n"
               "      --> -d <-- vypise vsetky slova s obmedzenim na 80 znakov  \n"
               "      --> N <-- akekovek cislo , tym urcim poradie hladaneho znaku alebo minimalnu dlzku, obmedzene na intmax\n\n\n"
               " *dovoleny znak = a-z, A-Z, 0-9, _ , -  ,,, slova su oddelene akymkolvek inym znakom\n");
        return 0;
    }

    if(argv[1][1] != '\0')                                  // ak mam v 1 argumente viac znakov ako jeden = chyba ,, napr cc .c ^c4
    {
        CHYBA;
        return 1;
    }

    if(( (argv[1][0] != '^') && (argv[1][0]!= '.') && (argv[1][0] != ':'))  && (!CIZNAK(argv[1][0]))  )    //  ak nemam prvy argument X . ^ :  a zaroven ani vstupny znak
    {
        CHYBA;
        return 1;
    }

    if(argc > 2)                                           // ak mam viac ako prvy argument tak skusam co je na 2hom a 3tom argumente
    {
        int j = 2;                                         // pomocna premenna na pocitanie na ktorom argumente som

        while(j<argc)
        {
            if(strcmp(argv[j], "-d") == 0)                // ak chce debugera tak pomocna premenna debug sa zapne - ako keby prepinac
            {
                debug = 1;
                j++;
            }

            if(j>=argc)
                break;

            if(uzivatel == 0)                            // ak je 0 znamena to ze este nenasiel argument na pocitanie, tak moze hladat  .. uzivatel je hodnota poradia na ktorom ma byt pozadovany znak
            {

                uzivatel = CisloPoradia(argv[j]);       // funkcia (pozri dolu) ulozi vysledne vypocitane cislo do premennej
                if(uzivatel > 0)                        //ak sa do uzivatela priradilo  cislo vecsie ako 0 tak pocitame ... pomocna premenna - prepinac
                    pocitam = 1;
                else                                    // ak ma 0 tak pocitat poradie na 0 mieste - nezmysel,
                {
                    CHYBA;
                    return 1;
                }

            }
            j++;
        }
        if(!debug && !pocitam)
        {
            CHYBA;
            return 1;
        }
    }                                                      // potialto je to praca s argumentami


    int i = 0;                                             // pocita pocet slov
    int MAM = 0;                                           // ak najde hladany znak nastavi sa na 1
    int poradie = 0;                                       // urcuje poradie znaku v slove
    char s;                                                // v nom zapisany prave citany znak zo vstupu

    while ((s = getchar()) != EOF )                        // citanie zo vstupu po znaku
    {

        if(   (FUN_ARGUMENT_X(argv[1], s))    ||  (((CIZNAK(argv[1][0]))  && (argv[1][0] == s) )))        // ak mam . : ^ alebo vstupny znak je prave znak ktory mam v premennej s
        {
            MAM = 1;
        }

        if(CIZNAK(s))                                         // ci mam znak / medzeru(oddelovac)
        {
            if(pocitam)
            {
                poradie++;                                     // mam znak takze som na dalsom znaku v poradi
                if((MAM == 1) && (uzivatel == poradie))        // ak mam pozadovany znak a pozadovanu podmienku pricitam, vynulujem
                {
                    i++;
                }

                MAM = 0;
            }
        }

        else                                              // mam medzeru
        {
            poradie = 0;                                   //pri pocitani na ktorom znaku som, je medzera tak nulujem

            if( (MAM == 1) && (!pocitam) )                 // ak mam pozadovany znak a nepocitam poradie, vynulujem
            {
                i++;
                MAM = 0;
            }
        }

        if(debug)                                            //tu je funkcia debuggera ... vypise slova s obmedzenim na 80 znakov
        {
            int debug_max = 0;

            if(CIZNAK(s))
            {
                if(debug_max < 80)
                {
                    printf("%c",s);
                }

                medzera = 0;
                debug_max++;
            }
            else if(!medzera)
            {
                printf("\n");
                medzera = 1;
                debug_max = 0;
            }
        }

    }

    if(MAM == 1 && uzivatel == poradie )                  // osetrenie ak posledne slovo obsahuje pozadovany znak a my sme nezadali oddelujuci znak slova
    {
        i++;
        MAM = 0;
    }

    printf("%d\n", i);                                   // vypis poctu slov so znakom
    return 0;                                            // tu program konci
}


//  funkcie


int FUN_ARGUMENT_X(char *str, int ss)                          // funkcia zisti co hladam .. vid dole
{
    if((str[0] == '^') && ((ss >= 'A') && (ss <= 'Z'))   )     //velke
        return 1;

    if((str[0] == ':') && ((ss >= '0') && (ss <= '9'))  )     // cisla
        return 1;

    if((str[0] == '.') && (CIZNAK(ss))  )                     // vsetky
        return 1;

    else
        return 0;
}


int CIZNAK(char ss)
{
    if(((ss >= 'a') && (ss <= 'z')) || ((ss >= 'A') && (ss <= 'Z')) || ((ss >= '0') && (ss <= '9')) || (ss == '-') || (ss == '_'))
        return 1;
    else
        return 0;
}



int CisloPoradia(char *retazec)                             // predpokladajme "001235..." nie len 1-9
{
    int cislo = 0;
    int i = 0;

    while((retazec[i]) != '\0')                             // robim dovtedy dokym neni koniec retazca
    {
        if(((retazec[i] >= '0') && (retazec[i] <= '9')) )
        {
            cislo *= 10;                                    // posuniem cislo o 1 radu pretoze citam od najvyssieho cisla s najvyssiu hodnotu
            cislo += retazec[i]- '0';
            i++;
        }
        else
            return 0;                                       // poradie 0 je nezmysel .. teda chyba
    }
    return cislo;
}
