
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**	                                      Úpravy: Bohuslav Křena, říjen 2014
**                                         Úpravy: Filip Ježovica,xjezov01, 2014
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
** 
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L->Act = NULL;
	L->First = NULL;
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
	tElemPtr PomUk; // pomocny ukazatel
	
	while (L->First != NULL) // ak by bol prazdny nič sa nedeje
	{
		PomUk = L->First;

		L->First = L->First->ptr; // prvym prvkom bude ten čo bol za ním
		free(PomUk); // uvolníme pameť
	}
	L->Act = NULL; // zrusime aktivitu zoznamu kedze je prazdny, teraz je v stave ako po inicializacii
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
	tElemPtr PomUk;

	if ((PomUk = malloc(sizeof(struct tElem))) != NULL)
	{
		PomUk->data = val; //nastavenie dátovej zložky
		PomUk->ptr = L->First; // predám ukazatel, ktorý ukazoval na začiatok
		L->First = PomUk; // začiatok bude ukazovať na nový prvok
	}
	else
	{
		Error();
	}
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
	L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	if (L->First != NULL)
	{
		*val = L->First->data; // skopirujeme hodnotu prveho prvku
	}
	else
	{
		Error(); // zoznam je prazdny
	}	
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
	if (L->First != NULL) // ak by bol prazdny nič sa nedeje
	{
		tElemPtr PomUk; // pomocny ukazatel
		PomUk = L->First;

		if (L->First == L->Act) // ak bol aktívny zrušíme aktivitu
		{
			L->Act = NULL;
		}

		L->First = L->First->ptr; // prvym prvkom bude ten čo bol za ním
		free(PomUk); // uvolníme pameť
	}
}

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
	// ak by bol prazdny nič sa nedeje, ak za aktivny nic nieje nič sa nedeje
	if (L->First != NULL && L->Act->ptr != NULL)
	{
		tElemPtr PomUk; // pomocny ukazatel
		
		PomUk = L->Act->ptr; // máme prvok ktory chceme mazať
		L->Act->ptr = PomUk->ptr; // naviežeme na prvok ktory nasleduje za zmazaným
		free(PomUk); // uvolníme pameť
	}
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
	if (L->Act != NULL) // ak by bol prazdny nič sa nedeje
	{
		tElemPtr PomUk;

		if ((PomUk = malloc(sizeof(struct tElem))) != NULL)
			{
				PomUk->data = val; // nastavenie dátovej zložky
				PomUk->ptr = L->Act->ptr; // predám ukazatel, ktorý ukazoval na dalsi prvok
				L->Act->ptr = PomUk; // aktivny bude ukazovať na nový prvok
			}
		else
		{
			Error(); // chyba s pametou
		}
	}	
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
	if (L->Act != NULL)
	{
		*val = L->Act->data;
	}
	else
	{
		Error(); // zoznam neni aktívny
	}
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
	if (L->Act != NULL)
	{
		L->Act->data = val;
	}
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if (L->Act != NULL) // ak je neaktívny nič sa nerobí
	{
		if (L->Act->ptr != NULL) // ci existuje dalsi prvok
		{
			L->Act = L->Act->ptr; // aktivitu dostáva daľší prvok
		}
		else
		{
			L->Act = NULL; // dalsi prvok neexistuje, strácame aktivitu
		}
	}
}

int Active (tList *L) {		
/*
** Je-li seznam L aktivní, vrací TRUE. V opačném případě vrací FALSE.
** Tuto funkci implementujte jako jediný příkaz return. 
**/	
	return(L->Act == NULL ? FALSE : TRUE);
}

/* Konec c201.c */
