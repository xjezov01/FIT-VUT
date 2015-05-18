/**
 * Projekt c.2 - Rivercrossing Problem 
 * Autor: Filip Jezovica, xjezov01@stud.fit.vutbr.cz
 * Datum: 04.05.2014
 * Subor: rivercrossing.c
 *
 * Popis:
 * Implementacia modifikovaneho synchronizacneho problemu "Rivercrossing"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// fork() a wait()
#include <unistd.h>
#include <sys/wait.h>

// Semafory a zdielana pamat
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

 //konstanty 
enum konstanty {
    EC_OK = 0,
    EC_HODNOTA,
    EC_NIEJECISLO,
    EC_ARGUM,
    EC_ALOC,
    EC_FOPEN,
    EC_SEMAF,
    EC_FORK,
    KAPITAN,
    MEMBER,
    EC_ERR = 1,
    EC_SYSERR = 2,
};

 //errory a help .. vypis podla konstant
 const char const *ErrorCode[] = {
    [EC_OK] =      "ok\n",
    [EC_HODNOTA] = "Niektory argument ma chybnu hodnotu\n",
    [EC_NIEJECISLO] =   "Zadane argumenty nie su cisla\n",
    [EC_ARGUM] = "Chybne zadane argumenty\n",
    [EC_ALOC] =     "SYSERR! Chyba pri alokovani zdielanej pameti\n",
    [EC_FOPEN] =    "SYSERR! Chyba pri otvarani suboru pre vypis\n",
    [EC_SEMAF] =    "SYSERR! Chyba pri vytvarani semaforu\n",
    [EC_FORK] =     "SYSERR! Chyba pri pokuse o fork procesu\n",
};

// subor pre vystup
FILE *suborOUT = NULL;

//globalne semafory 9x
sem_t *Sem_Write;
sem_t *Sem_Citac1;
sem_t *Sem_Citac2;
sem_t *Sem_Molo;
sem_t *Sem_Hacker;
sem_t *Sem_Serf;
sem_t *Sem_VarSafe;
sem_t *Sem_VarAll_Safe;
sem_t *Sem_Finish;

// globalne prem pre shared pamet 5x
int shm_fd1;
int shm_fd2;
int shm_fd3;
int shm_fd4;
int shm_fd5;

//globalne premenne 5x
int *Shared_Citac=NULL;
int *Shared_Line=NULL;
int *Shared_Hacker=NULL;
int *Shared_Serf=NULL;
int *Shared_All=NULL;

int captain=0;
int ParamP;
int ParamH;
int ParamS;
int ParamR;

// main je dole
/***********************FUNKCIE************************************************/
// funkcia na overenie spravnych clenov skupiny+ oznacenie capt/member
int SkupinaControl()
{
    // kontrola skupiny
    if ((*Shared_Hacker == 4 && (*Shared_Serf == 0 || *Shared_Serf == 1)) || 
        ((*Shared_Hacker == 0 || *Shared_Hacker == 1) && *Shared_Serf == 4) ||
        ((*Shared_Hacker == 2 || *Shared_Hacker == 3 ) && *Shared_Serf == 2) ||
        (*Shared_Hacker == 2 && *Shared_Serf == 3)
        )
    {
        // kolko ich zostalo na MOLE
        if ((*Shared_Hacker == 2 && *Shared_Serf == 3) ||
            (*Shared_Hacker == 4 && *Shared_Serf == 1)
            )
        {
            *Shared_Hacker = 0;
            *Shared_Serf = 1;
        }
        else if ((*Shared_Hacker == 3 && *Shared_Serf == 2) ||
            (*Shared_Hacker == 1 && *Shared_Serf == 4)
            )
        {
            *Shared_Hacker = 1;
            *Shared_Serf = 0;
        }
        else
        {
            *Shared_Hacker = 0;
            *Shared_Serf = 0;
        }

        // toto je kapitan
        return KAPITAN;
    }
    else
    {
        // toto je member
        return MEMBER;
    }
}

//funkcia odpocita 4 ludi a potom im odomkne cestu
void CITAC(sem_t *Sem_Citac)
{
    //ochrana premennej lock
    sem_wait(Sem_VarSafe);
    if (++(*Shared_Citac) == 4)
    {
        *Shared_Citac = 0;
        for (int i = 0; i < 4; ++i)
        {
            sem_post(Sem_Citac);
        }
    }
    //ochrana premennej uvolnenie
    sem_post(Sem_VarSafe);
    sem_wait(Sem_Citac);
}

void Landing_wait()
{
    // pockame dokym sa vsetci vylodia a iba posldeny moze otvorit molo
    //ochrana premennej lock
    sem_wait(Sem_VarSafe);
    if (++(*Shared_Citac) == 4)
    {
        *Shared_Citac = 0;
        sem_post(Sem_Molo);
    }
    //ochrana premennej uvolnenie
    sem_post(Sem_VarSafe);
}

// funkcia ktora uzavre vsetky semafory
void Sem_closer()
{
    // Semafory musi uzavriet kazdy proces, co s nimi pracoval 9x
    sem_close(Sem_Write);
    sem_close(Sem_Citac1);
    sem_close(Sem_Citac2);
    sem_close(Sem_Molo);
    sem_close(Sem_Hacker);
    sem_close(Sem_Serf);
    sem_close(Sem_VarSafe);
    sem_close(Sem_VarAll_Safe);
    sem_close(Sem_Finish);

    // Kompletne vycistenie pamati po semaforoch 9x
    sem_unlink("/xjezov01_SEM01");
    sem_unlink("/xjezov01_SEM02");
    sem_unlink("/xjezov01_SEM03");
    sem_unlink("/xjezov01_SEM04");
    sem_unlink("/xjezov01_SEM05");
    sem_unlink("/xjezov01_SEM06");
    sem_unlink("/xjezov01_SEM07");
    sem_unlink("/xjezov01_SEM08");
    sem_unlink("/xjezov01_SEM09");
}

void Shm_closer()
{
    // Odmapujeme pamat 5x
    munmap(Shared_Line, sizeof(int));
    munmap(Shared_Citac, sizeof(int));
    munmap(Shared_Hacker, sizeof(int));
    munmap(Shared_Serf, sizeof(int));
    munmap(Shared_All, sizeof(int));

    // Zmazeme pamat a zavreme ju 5x
    shm_unlink("/xjezov01_SMEM01");
    shm_unlink("/xjezov01_SMEM02");
    shm_unlink("/xjezov01_SMEM03");
    shm_unlink("/xjezov01_SMEM04");
    shm_unlink("/xjezov01_SMEM05");
    close(shm_fd1);
    close(shm_fd2);
    close(shm_fd3);
    close(shm_fd4);
    close(shm_fd5);
}

// funkcia na zavretie vsetkeho
void CloseALL()
{
    Sem_closer(); // vsetky semafory
    fclose(suborOUT); // zavre subor
    Shm_closer(); // zavre vsetky shared mem
}

/****************************CINNOSTI*************************************/
//funkcia cinnosti Serfa
void SerfPracuj(int i, int AllPeople)
{
    sem_wait(Sem_Write);
    fprintf(suborOUT, "%d: serf: %d: started\n",(*Shared_Line)++, i);
    sem_post(Sem_Write);

    // musime cakat ci mozeme ist na MOLO
    sem_wait(Sem_Molo);

    sem_wait(Sem_Write);
    (*Shared_Serf)++;
    fprintf(suborOUT, "%d: serf: %d: waiting for boarding: %d: %d\n",(*Shared_Line)++, i, *Shared_Hacker, *Shared_Serf);
    sem_post(Sem_Write);

    //kontrola skupiny
    if (SkupinaControl() == KAPITAN)
    {
        captain=KAPITAN; // toto je kapitan
        // 2x pusti hackera a 1x serfa ... lebo 1 SERF je CAPTAIN
        sem_post(Sem_Hacker);
        sem_post(Sem_Hacker);
        sem_post(Sem_Serf);
    }
    else
    {
        //toto je member
        sem_post(Sem_Molo); // pusti na molo dalseho
        sem_wait(Sem_Serf); // Serf cakaj
    }

    //teraz mozu nastupit na lod
    sem_wait(Sem_Write);
    fprintf(suborOUT, "%d: serf: %d: boarding: %d: %d\n",(*Shared_Line)++, i, *Shared_Hacker, *Shared_Serf);
    sem_post(Sem_Write);

    // pockame dokym sa vsetci nalodia
    CITAC(Sem_Citac1);

    // vypiseme posadku members + captain
    if (captain == KAPITAN)
    {
        sem_wait(Sem_Write);
        fprintf(suborOUT, "%d: serf: %d: captain\n",(*Shared_Line)++, i);
        sem_post(Sem_Write);

        // plavba trva nejaku dobu ale len kapitan vetva
        usleep(rand()%(ParamR+1)*1000);
    }
    else
    {
        sem_wait(Sem_Write);
        fprintf(suborOUT, "%d: serf: %d: member\n",(*Shared_Line)++, i);
        sem_post(Sem_Write);
    }

    // pockame dokym sa vypise cela posadka
    CITAC(Sem_Citac2);

    // vylodovanie
    sem_wait(Sem_Write);
    fprintf(suborOUT, "%d: serf: %d: landing: %d: %d\n",(*Shared_Line)++, i, *Shared_Hacker, *Shared_Serf);
    sem_post(Sem_Write);

    // pockame dokym sa vsetci vylodia a iba posldeny moze otvorit molo
    Landing_wait();

    // vsetci cakaju na uplne posledneho, potom je mozne finished
    //ochrana premennej lock
    sem_wait(Sem_VarAll_Safe);
    if (++(*Shared_All) == AllPeople)
    {
        for (int x = 0; x < AllPeople; ++x)
        {
            sem_post(Sem_Finish);
        }
    }
    //ochrana premennej uvolnenie
    sem_post(Sem_VarAll_Safe);
    sem_wait(Sem_Finish);

    sem_wait(Sem_Write);
    fprintf(suborOUT, "%d: serf: %d: finished\n",(*Shared_Line)++, i);
    sem_post(Sem_Write);
}

//funkcia cinnosti Hackera
void HackerPracuj(int i, int AllPeople)
{
    sem_wait(Sem_Write);
    fprintf(suborOUT, "%d: hacker: %d: started\n",(*Shared_Line)++, i);
    sem_post(Sem_Write);

    // musime cakat ci mozeme ist na MOLO
    sem_wait(Sem_Molo);

    sem_wait(Sem_Write);
    (*Shared_Hacker)++;
    fprintf(suborOUT, "%d: hacker: %d: waiting for boarding: %d: %d\n",(*Shared_Line)++, i, *Shared_Hacker, *Shared_Serf);
    sem_post(Sem_Write);

    //kontrola skupiny
    if (SkupinaControl() == KAPITAN)
    {
        captain=KAPITAN; // toto je kapitan

        // 2x serfa pusti a 1x pusti hackera... 1 hacker je CAPTAIN
        sem_post(Sem_Serf);
        sem_post(Sem_Serf);
        sem_post(Sem_Hacker);
    }
    else
    {
        //toto je member
        sem_post(Sem_Molo); // pusti na molo dalseho
        sem_wait(Sem_Hacker); // Hacker cakaj
    }

    //teraz mozu nastupit na lod
    sem_wait(Sem_Write);
    fprintf(suborOUT, "%d: hacker: %d: boarding: %d: %d\n",(*Shared_Line)++, i, *Shared_Hacker, *Shared_Serf);
    sem_post(Sem_Write);

    // pockame dokym sa vsetci nalodia
    CITAC(Sem_Citac1);

    // vypiseme posadku members + captain
    if (captain == KAPITAN)
    {
        sem_wait(Sem_Write);
        fprintf(suborOUT, "%d: hacker: %d: captain\n",(*Shared_Line)++, i);
        sem_post(Sem_Write);

        // plavba trva nejaku dobu ale len kapitan vetva
        usleep(rand()%(ParamR+1)*1000);
    }
    else
    {
        sem_wait(Sem_Write);
        fprintf(suborOUT, "%d: hacker: %d: member\n",(*Shared_Line)++, i);
        sem_post(Sem_Write);
    }

    // pockame dokym sa vypise cela posadka
    CITAC(Sem_Citac2);

    // vylodovanie
    sem_wait(Sem_Write);
    fprintf(suborOUT, "%d: hacker: %d: landing: %d: %d\n",(*Shared_Line)++, i, *Shared_Hacker, *Shared_Serf);
    sem_post(Sem_Write);

    // pockame dokym sa vsetci vylodia a iba posldeny moze otvorit molo
    Landing_wait();

    // vsetci cakaju na uplne posledneho, potom je mozne finished
    //ochrana premennej lock
    sem_wait(Sem_VarAll_Safe);
    if (++(*Shared_All) == AllPeople)
    {
        for (int x = 0; x < AllPeople; ++x)
        {
            sem_post(Sem_Finish);
        }
    }
    //ochrana premennej uvolnenie
    sem_post(Sem_VarAll_Safe);
    sem_wait(Sem_Finish);

    sem_wait(Sem_Write);
    fprintf(suborOUT, "%d: hacker: %d: finished\n",(*Shared_Line)++, i);
    sem_post(Sem_Write);
}

void killproc(int sighandler)
{
    (void)sighandler; // koli prekladacu
    kill(getpid(), SIGTERM); // ukoncenie procesov
    CloseALL(); // uzavreme vsetko = semafory, subor, shared mem
    exit(EC_SYSERR);
}


/**********************MAIN***dole******************************************/

int main(int argc, char *argv[])
{
    // odchytavanie signalu
    signal(SIGTERM, killproc);
    signal(SIGINT, killproc);

    // pre nahodne generovanie cisel
    srand(time(0));

/***************************argumenty**dole**********************************************/
    char *test;

    if (argc == 5)
    {
        ParamP = strtol(argv[1], &test, 10);
        if (test[0] != 0)
        { 
            fprintf(stderr, "%s", ErrorCode[EC_NIEJECISLO]);
            return EC_ERR;
        }

        ParamH = strtol(argv[2], &test, 10);
        if (test[0] != 0)
        { 
            fprintf(stderr, "%s", ErrorCode[EC_NIEJECISLO]);
            return EC_ERR;
        }

        ParamS = strtol(argv[3], &test, 10);
        if (test[0] != 0)
        { 
            fprintf(stderr, "%s", ErrorCode[EC_NIEJECISLO]);
            return EC_ERR;
        }

        ParamR = strtol(argv[4], &test, 10);
        if (test[0] != 0)
        { 
            fprintf(stderr, "%s", ErrorCode[EC_NIEJECISLO]);
            return EC_ERR;
        }

        //overenie povoleneho rozsahu
        // overenie P ... P<0 && P %2 = 0
        if (ParamP <= 0)
        {
            fprintf(stderr, "%s", ErrorCode[EC_HODNOTA]);
            return EC_ERR;
        }
        else if (ParamP % 2 != 0)
        {
            fprintf(stderr, "%s", ErrorCode[EC_HODNOTA]);
            return EC_ERR;
        }

        // overenie H S R
        if ((ParamH < 0 || ParamH > 5001) ||
            (ParamS < 0 || ParamS > 5001) ||
            (ParamR < 0 || ParamR > 5001)
            )
        {
            fprintf(stderr, "%s", ErrorCode[EC_HODNOTA]);
            return EC_ERR;
        }
    }
    else
    {
        fprintf(stderr, "%s", ErrorCode[EC_ARGUM]);
        return EC_ERR;
    }

    /***************************argumenty**hore************************************************/

    int AllPeople = ParamP * 2; // kolko je hackerove+serfov

    //otvorenie suboru pre zapis
    suborOUT = fopen("rivercrossing.out", "w");
    if(suborOUT == NULL)
    {
        fprintf(stderr, "%s", ErrorCode[EC_FOPEN]);
        return EC_SYSERR; // ak sa nepodarilo otvorit subor pre zapis SYSERR
    }

    // nastavenie bufferu, koli zapisu do souboru
    setbuf(suborOUT, NULL);

    // Vytvorime semaforoy 9x
    Sem_Write = sem_open("/xjezov01_SEM01", O_CREAT | O_EXCL, 0644, 1);
    Sem_Citac1 = sem_open("/xjezov01_SEM02", O_CREAT | O_EXCL, 0644, 0);
    Sem_Citac2 = sem_open("/xjezov01_SEM03", O_CREAT | O_EXCL, 0644, 0);
    Sem_Molo = sem_open("/xjezov01_SEM04", O_CREAT | O_EXCL, 0644, 1);
    Sem_Hacker = sem_open("/xjezov01_SEM05", O_CREAT | O_EXCL, 0644, 0);
    Sem_Serf = sem_open("/xjezov01_SEM06", O_CREAT | O_EXCL, 0644, 0);
    Sem_VarSafe = sem_open("/xjezov01_SEM07", O_CREAT | O_EXCL, 0644, 1);
    Sem_VarAll_Safe = sem_open("/xjezov01_SEM08", O_CREAT | O_EXCL, 0644, 1);
    Sem_Finish = sem_open("/xjezov01_SEM09", O_CREAT | O_EXCL, 0644, 0);

    // prekontrolujeme vytvorenie semaforov 9x
    if(Sem_Write == SEM_FAILED || 
        Sem_Citac1 == SEM_FAILED || 
        Sem_Citac2 == SEM_FAILED ||
        Sem_Molo == SEM_FAILED ||
        Sem_Hacker == SEM_FAILED ||
        Sem_Serf == SEM_FAILED ||
        Sem_VarSafe == SEM_FAILED ||
        Sem_VarAll_Safe == SEM_FAILED ||
        Sem_Finish == SEM_FAILED
        )
    {
        fprintf(stderr, "%s", ErrorCode[EC_SEMAF]);
        fclose(suborOUT); // zavreme otvoreny subor
        Sem_closer(); // niektore sa mohli vytvorit tak ich uzavri
        return EC_SYSERR; // toto je SYStemova chyba SYSERR
    }

    // Vytvorime zdielanu pamat 5x
    shm_fd1 = shm_open("/xjezov01_SMEM01", O_CREAT | O_EXCL | O_RDWR, 0644);
    shm_fd2 = shm_open("/xjezov01_SMEM02", O_CREAT | O_EXCL | O_RDWR, 0644);
    shm_fd3 = shm_open("/xjezov01_SMEM03", O_CREAT | O_EXCL | O_RDWR, 0644);
    shm_fd4 = shm_open("/xjezov01_SMEM04", O_CREAT | O_EXCL | O_RDWR, 0644);
    shm_fd5 = shm_open("/xjezov01_SMEM05", O_CREAT | O_EXCL | O_RDWR, 0644);

    // Vytvorime si v nasej zdielanej pamati miesto pre 1 integer 5x
    ftruncate(shm_fd1, sizeof(int));
    ftruncate(shm_fd2, sizeof(int));
    ftruncate(shm_fd3, sizeof(int));
    ftruncate(shm_fd4, sizeof(int));
    ftruncate(shm_fd5, sizeof(int));

    // Namapujeme zdielanu pamat do adresneho priestoru procesu 5x
    Shared_Line = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0);
    Shared_Citac = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0);
    Shared_Hacker = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd3, 0);
    Shared_Serf = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd4, 0);
    Shared_All = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd5, 0);

    //ak sa nieco pokazilo
    if ((Shared_Line == MAP_FAILED) ||
        (Shared_Citac == MAP_FAILED) ||
        (Shared_Hacker == MAP_FAILED) ||
        (Shared_Serf == MAP_FAILED) ||
        (Shared_All == MAP_FAILED)
        )
    {
        fprintf(stderr, "%s", ErrorCode[EC_ALOC]);
        // niektore sa mohli namapovat tak ich zmazeme
        CloseALL(); // zahrnuje aj zavretie suboru, zrusenie semaforov a zrusenie shared mem 
        return EC_SYSERR;
    }

    // pociatocne nastavenie premennych
    *Shared_Line=1;

    // deklaracia procesov
    pid_t HeadSerf, HeadHacker;
    pid_t Serf, Hacker;


    /*****************tu zacina cely program*********************/

    HeadHacker = fork();
    if (HeadHacker == -1) // fork sa nepodaril == SYSERR
    {
        fprintf(stderr, "%s", ErrorCode[EC_FORK]);
        kill(HeadHacker, SIGTERM);
        killproc(0); // ukoncenie process
        exit(EC_SYSERR);
    }
    else if (HeadHacker == 0) // Childprocess tu budeme vytvarat Hacker-ov
    {
        for (int i = 0; i < ParamP; ++i)
        {
            usleep(rand()%(ParamH+1)*1000); // uspanie
            Hacker = fork();
            if (Hacker == 0) //child process
            {
                HackerPracuj(i+1, AllPeople); //tu sa vykonava cinnost Hackera
                Sem_closer(); // kazdy zavre semafory
                fclose(suborOUT); // kazdy zavre subor
                exit(EC_OK); // vsetko prebehlo ok
            }
            if (Hacker == -1) //ak sa nepodari fork == SYSERR
            {
                fprintf(stderr, "%s", ErrorCode[EC_FORK]);
                kill(HeadHacker, SIGTERM); // ukoncenie procesu Hlavneho Hackera
                killproc(0); // ukoncenie procesov
                exit(EC_SYSERR); // pretoze SYSERR
            }
        }

        //pockame na child procesy HeadHackera
        if (Hacker > 0)
        {
            while(wait(NULL) > 0);
        }
        CloseALL();
        exit(EC_OK);
    }

    HeadSerf = fork();
    if (HeadSerf == -1) // fork sa nepodaril == SYSERR
    {
        fprintf(stderr, "%s", ErrorCode[EC_FORK]);
        kill(HeadHacker, SIGTERM);
        kill(HeadSerf, SIGTERM);
        killproc(0); // ukoncenie process
        exit(EC_SYSERR);
    }
    else if (HeadSerf == 0) // Childprocess tu budeme vytvarat serf-ov
    {
        for (int i = 0; i < ParamP; ++i)
        {
            usleep(rand()%(ParamS+1)*1000); //uspanie
            Serf = fork();
            if (Serf == 0) // child process HeadSerfa
            {
                SerfPracuj(i+1, AllPeople); //tu sa vykonava cinnost Serfa
                Sem_closer(); // kazdy uzavre semafory
                fclose(suborOUT); // kazdy zavre subor
                exit(EC_OK); // vsetko ok
            }
            if (Serf == -1) //ak sa nepodari fork
            {
                fprintf(stderr, "%s", ErrorCode[EC_FORK]);
                kill(HeadSerf, SIGTERM); // ukoncenie procesu Hlavneho Serfa
                killproc(0); // ukoncenie procesov
                exit(EC_SYSERR); // pretoze SYSERR
            }
        }

        //pockame na child procesy
        if (Serf > 0)
        {
            while(wait(NULL) > 0);
        }
        CloseALL();
        exit(EC_OK);
    }

    // caka na parentov
    if (HeadHacker > 0 && HeadSerf > 0)
    {
        while(wait(NULL) > 0);
    }

    CloseALL(); // uzavri vsetko .. semafory, subor, shared mem

    return EC_OK; // vsetko ok
}