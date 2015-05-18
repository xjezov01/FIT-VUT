/*
 * Subor:   proj2.c
 * Datum:   2013/10/21
 * Autor:   Filip Ježovica, xjezov01@stud.fit.vutbr.cz
 * Projekt: Iteracni vypocty, projekt c. 2 pre predmet IZP
 * Popis:   Program 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHYBA printf("ERROR!! Neplatn0 spustenie programu! pre viac info spusti napovedu  --help    \n")
#define CHYBNECISLO printf("ERROR! Zle zadane cislo.... Pre viac info spusti napovedu  --help")
#define PIpol 1.5707963267948966192313216916397514420985846996875529104874722961539082031431044993140174126710585339910740432566411533
#define NAN (0.0/0.0)
#define EPSILON 0.0000000000000001
#define FALSE 1

int FUNK_CI_CISLO(char *cislo);
double ABSOLUT(double x);
double my_sqrt(double x);
double my_asin(double x);
double my_pytagoras(double X1, double Y1, double X2, double Y2);
double triangle(double *arg);

int main(int argc, char *argv[])
{
    if(argc == 1)                                            //ak nemam vstupne argumenty od uzivatela programu
    {
        CHYBA;
        return 1;
    }

    if((strcmp(argv[1], "--help") == 0))
    {
        printf("Vitaj! Spustil si napovedu! (--help )  \n\n"
               "Toto je program na vypocet uhlov trojuholnika .....autor: FILIP JEZOVICA, xjezov01@stud.fit.vutbr.cz\n"
               "Program taktiez vypocita arcus sinus alebo druhu odmocninu\n\n\n"
               "Argumenty pri spustani programu: \n\n"
               "      --> --sqrt X <-- druha odmocnina z cisla -- X --   *X je nezaporne cislo  \n"
               "      --> --asin X <-- arcus sinus     z cisla -- X --   *X je z intervalu <-1, 1 >\n\n"
               "      --> --triangle AX AY BX BY CX CY <--  vypocita uhly trojuholnika \n"
               "      *trojuholnik je zadany suradnicami jeho 3 vrcholov v poradi A,B,C\n\n\n");
        return 0;
    }



    if(  (argc == 3 )  &&  (argv[3] == '\0'))

    {
        if((strcmp(argv[1], "--sqrt") == 0) )
        {

            //prevedieme cislo...
            char *ERR;
            double argX = strtod(argv[2], &ERR);
            if((ERR == argv[2]) || (*ERR != '\n' && *ERR !=0) )
            {
                CHYBNECISLO;
                return 1;
            }
            // previedli sme

            printf("%.10e\n", my_sqrt(argX) );
            return 0;
        }


        if((strcmp(argv[1], "--asin") == 0))
        {
            //prevedieme cislo...
            char *ERR;
            double argX = strtod(argv[2], &ERR);
            if((ERR == argv[2]) || (*ERR != '\n' && *ERR !=0) )
            {
                CHYBNECISLO;;
                return 1;
            }
            // previedli sme


            printf("%.10e\n", my_asin(argX) );
            return 0;
        }

        CHYBA;
        return 1;   // ak nastane neaka chyba
    }



    if( (argc == 8)  &&  (strcmp(argv[1], "--triangle") == 0) )     // TRIANGLE
    {



        int h = 0;
        int i;
        double arg[6];      // pole pre 6
        for(i=2; i<8; i++)                                          // prevedieme cislo
        {

            char *ERR;
            arg[h] = strtod(argv[i], &ERR);                        //AX=arg[0] | AY=arg[1] | BX=arg[2] | BY=arg[3] | CX=arg[4] CY=arg[5]
            if((ERR == argv[i]) || (*ERR != '\n' && *ERR !=0) )
            {
                CHYBNECISLO;
                return FALSE;
            }
            // previedli sme

            h++;
        }


        if(triangle(arg) == FALSE)
            return FALSE;

        return 0;
    }

    CHYBA;
    return 1;   // cize nastalo cokolvek ine a to je zle...
}



// tu budu napisane funkcie...


// absolutna hodnota

double ABSOLUT(double x)
{
    return  (x<0.0? -x: x);
}


// druha odmocnina

double my_sqrt(double x)
{
    if(x == 0.0)
        return 0.0;
    if(x < 0.0 )
        return -NAN;

    double nove = 1.0;
    double stare = 0.0;

    while((ABSOLUT(nove-stare)) >= EPSILON )
    {
        stare = nove;
        nove = (1.0/2.0) * ( (x/stare) + (stare)  );
    }

    return nove;
}



// arcus sinus

double my_asin(double x)
{
    if((x == 1) || (x == -1) )
        return (x*PIpol);
    if(x==0.0)
        return 0.0;

    if(ABSOLUT(x) > 1.0)
        return -NAN;

    double cislo = x;
    double starecislo = cislo+EPSILON;
    double a = 1.0;
    double k = x;
    double xx = x*x;                                         // aby nemusel zakazdym pocitat tak raz pocitam tu
    double z = 0.0;

    while( ABSOLUT(cislo-starecislo) >= (EPSILON)* ABSOLUT(cislo)    )
    {

        starecislo = cislo;

        a = (a * (z+1.0)) / (z+2.0);

        k = k * xx;

        if(ABSOLUT(x) < 1 && ABSOLUT(x) > 0.9)                            // osetrenie pre kriticke hodnoty v intervale
            cislo = PIpol - my_asin(my_sqrt(1 - x*x));
        else
        {
            cislo = starecislo + (a * (k/(z+3.0) ) );
        }

        z = z + 2.0;

    }
    if(ABSOLUT(x) < 1 && ABSOLUT(x) > 0.9)
        return -cislo;
    return cislo;
}


double my_pytagoras(double X1, double Y1, double X2, double Y2)          //pytagorova veta .... pre suradnicovu sustavu v x-y
{
    return my_sqrt(((X1 - X2) * (X1 - X2)) + ((Y1 - Y2) * (Y1 - Y2)));
}


double triangle(double *arg)
{

    //AX=arg[0] | AY=arg[1] | BX=arg[2] | BY=arg[3] | CX=arg[4] CY=arg[5]

    double AB = my_pytagoras(arg[0], arg[1], arg[2], arg[3] );  // AX AY  BX BY
    double AC = my_pytagoras(arg[0], arg[1], arg[4], arg[5] );  // AX AY  CX CY
    double BC = my_pytagoras(arg[2], arg[3], arg[4], arg[5] );  // BX BY  CX CY

    if((AB+AC<=BC) || (AB+BC<=AC) || (AC+BC<=AB) || AB==0 || AC==0 || BC==0)
    {
        printf("nan\nnan\nnan\n");
        return FALSE;
    }


    // uhol = (pi/2) - (my_asin(prilahla1 na 2 + prilahla2 na 2 - protilahla) / 2*prilahla1*prilahla2))
    double alfa = PIpol - (my_asin( ((AC * AC) + (AB * AB) - (BC * BC)) / (2 * AC * AB) ) );
    double beta = PIpol - (my_asin( ((AB * AB) + (BC * BC) - (AC * AC)) / (2 * AB * BC) ) );
    double gama = PIpol - (my_asin( ((AC * AC) + (BC * BC) - (AB * AB)) / (2 * AC * BC) ) );

    printf("%.10e\n", alfa);
    printf("%.10e\n", beta);
    printf("%.10e\n", gama);

    return 0;
}
