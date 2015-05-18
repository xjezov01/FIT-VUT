	
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2014
**                                        Úpravy: Filip Ježovica, xjezov01, 2014
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
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

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL; 
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	tDLElemPtr PomUk;

	while (L->First != NULL) // ak by bol prazdny nič sa nedeje
	{
		PomUk = L->First;

		L->First = L->First->rptr; // prvym prvkom bude ten čo bol za ním
		free(PomUk); // uvolníme pameť
	}
	L->Act = NULL; // zrusime aktivitu zoznamu kedze je prazdny, teraz je v stave ako po inicializacii
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr PomUk;

	if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
	{
		PomUk->data = val;

		PomUk->rptr = L->First; // novy musi ukazat napravo teda prvok kt. bol doteraz prvy, ak bol NULL tak sa NULL skopiruje
		PomUk->lptr = NULL; // prvy prvok ma vzdy NALAVO NULL

		if (L->Last == NULL) // ak je zoznam prazdny
			L->Last = PomUk;  // znamena to že je aj posledný
		else
			L->First->lptr = PomUk; // ten co bol doteraz prvy musi ukazovat naLAVO na novo vytvoreny
		
		L->First = PomUk;
	}
	else
		DLError(); // chyba alokacie
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr PomUk;

	if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
	{
		PomUk->data = val;

		PomUk->rptr = NULL; // posledny prvok ma vzdy NAPRAVO NULL
		PomUk->lptr = L->Last; // novy musi ukazat napravo teda prvok kt. bol doteraz prvy, ak bol NULL tak sa NULL skopiruje

		if (L->Last == NULL) // ak je zoznam prazdny
			L->First = PomUk;  // znamena to že je aj prvý
		else
			L->Last->rptr = PomUk; // ten co bol doteraz prvy musi ukazovat naLAVO na novo vytvoreny
		
		L->Last = PomUk;
	}
	else
		DLError(); // chyba alokacie
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->Last != NULL)
	{
		*val = L->First->data;
	}
	else
		DLError(); // zoznam je prázdny
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->Last != NULL)
	{
		*val = L->Last->data;
	}
	else
		DLError(); // zoznam je prázdny
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if (L->Last != NULL) // nieco robime iba ak nieje prazdny
	{
		tDLElemPtr PomUk;
		PomUk = L->First;
		if (L->First == L->Last) // jediny prvok
		{
			L->First = NULL;
			L->Last = NULL;
			L->Act = NULL;
		}
		else // viac prvkov
		{
			if (PomUk == L->Act)
				L->Act = NULL; // ak bol aktívny - strácame aktivitu
		
		L->First = PomUk->rptr; // prvym sa stal dalsi
		L->First->lptr = NULL; // prvy prvok tak nalavo musi byt NULL
		
		}

		free(PomUk); // uvolníme
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	if (L->Last != NULL) // nieco robime iba ak nieje prazdny
	{
		tDLElemPtr PomUk;
		PomUk = L->Last;
		if (L->First == L->Last) // jediny prvok
		{
			L->First = NULL;
			L->Last = NULL;
			L->Act = NULL;
		}
		else // viac prvkov
		{
			if (PomUk == L->Act)
				L->Act = NULL; // ak bol aktívny - strácame aktivitu
		
		L->Last = PomUk->lptr; // poslednym sa stal predchadzajuci teda nalavo
		L->Last->rptr = NULL; // posledny prvok tak napravo musi byt NULL
		
		}

		free(PomUk); // uvolníme
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	if (L->Act != NULL && L->Act != L->Last)
	{
		tDLElemPtr PomUk;
		PomUk = L->Act->rptr; // prvok ktory rusime

		L->Act->rptr =  PomUk->rptr; // skopirujeme ukazatel prvku ktory je za aktívnym a ukazuje napravo na DALSI/NULL

		if (PomUk->rptr != NULL) // za prvkom ktory rusime je este dalsi prvok
			PomUk->rptr->lptr = L->Act; // ten dalsi prvok musi mat NALAVO náš aktuálny prvok
		else 
			L->Last = L->Act; // zrusili sme posledný prvok a AKTIVNY sa stane aj LAST

		free(PomUk); // uvolníme
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if (L->Act != NULL && L->Act != L->First)
	{
		tDLElemPtr PomUk;
		PomUk = L->Act->lptr; // prvok ktory rusime TEDA ten NALAVO

		L->Act->lptr =  PomUk->lptr; // skopirujeme ukazatel prvku ktory je PRED aktívnym a ukazuje NALAVO na PREDCHADZAJUCI/NULL

		if (PomUk->lptr != NULL) // PRED prvkom ktory rusime je este dalsi prvok
			PomUk->lptr->rptr = L->Act; // ten dalsi prvok musi mat NALAVO náš aktuálny prvok
		else 
			L->First = L->Act; // zrusili sme PRVÝ prvok a AKTIVNY sa stane aj prvým - FIRST

		free(PomUk); // uvolníme
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L->Act != NULL) // je aktívny
	{
		tDLElemPtr PomUk;
		
		if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
		{
			PomUk->data = val;

			if (L->Act->rptr != NULL) // ak je za aktívnym neaký prvok
				L->Act->rptr->lptr = PomUk; // prvok za aktívnym musí NALAVO ukazovať na nový prvok, ten medzi nimi
			else // Za aktívnym nieje další prvok = bude nový LAST
				L->Last = PomUk;

			PomUk->rptr = L->Act->rptr; // nový prvok musí ukazovať NAPRAVO na další prvok/NULL


			L->Act->rptr = PomUk; // aktívny musí ukazovať NAPRAVO na nový prvok
			PomUk->lptr = L->Act; // nový prvok musí ukazovať NALAVO na ten čo je pred ním (na aktívny)
		}
		else
			DLError(); // chyba alokacie
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L->Act != NULL) // je aktívny
	{
		tDLElemPtr PomUk;
		
		if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
		{
			PomUk->data = val;

			if (L->Act->lptr != NULL) // ak je PRED aktívnym neaký prvok
				L->Act->lptr->rptr = PomUk; // prvok PRED aktívnym musí NAPRAVO ukazovať na nový prvok, ten medzi nimi
			else // PRED aktívnym nieje další prvok = bude nový FIRST
				L->First = PomUk;

			PomUk->lptr = L->Act->lptr; // nový prvok musí ukazovať NALAVO na predchádzajúci prvok/NULL

			L->Act->lptr = PomUk; // aktívny musí ukazovať NALAVO na nový prvok
			PomUk->rptr = L->Act; // nový prvok musí ukazovať NAPRAVO na ten čo je za ním (na aktívny)
		}
		else
			DLError(); // chyba alokacie
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if (L->Act != NULL)
	{
		*val = L->Act->data;
	}
	else
		DLError(); // zoznam nieje AKTÍVNY
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if (L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
		L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
		L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivní, vrací true. V opačném případě vrací false.
** Funkci implementujte jako jediný příkaz.
**/
	return (L->Act != NULL);
}

/* Konec c206.c*/
