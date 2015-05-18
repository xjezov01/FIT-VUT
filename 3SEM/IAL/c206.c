	
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2014
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu, 
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem, 
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu, 
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funkcí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam 
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL; 
}

void DLDisposeList (tDLList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free. 
**/
	tDLElemPtr PomUk;

	while (L->First != NULL) // ak by bol prazdny niè sa nedeje
	{
		PomUk = L->First;

		L->First = L->First->rptr; // prvym prvkom bude ten èo bol za ním
		free(PomUk); // uvolníme pame»
	}
	L->Act = NULL; // zrusime aktivitu zoznamu kedze je prazdny, teraz je v stave ako po inicializacii
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr PomUk;

	if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
	{
		PomUk->data = val;

		PomUk->rptr = L->First; // novy musi ukazat napravo teda prvok kt. bol doteraz prvy, ak bol NULL tak sa NULL skopiruje
		PomUk->lptr = NULL; // prvy prvok ma vzdy NALAVO NULL

		if (L->Last == NULL) // ak je zoznam prazdny
			L->Last = PomUk;  // znamena to ¾e je aj posledný
		else
			L->First->lptr = PomUk; // ten co bol doteraz prvy musi ukazovat naLAVO na novo vytvoreny
		
		L->First = PomUk;
	}
	else
		DLError(); // chyba alokacie
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr PomUk;

	if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
	{
		PomUk->data = val;

		PomUk->rptr = NULL; // posledny prvok ma vzdy NAPRAVO NULL
		PomUk->lptr = L->Last; // novy musi ukazat napravo teda prvok kt. bol doteraz prvy, ak bol NULL tak sa NULL skopiruje

		if (L->Last == NULL) // ak je zoznam prazdny
			L->First = PomUk;  // znamena to ¾e je aj prvý
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
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
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
** Prostøednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
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
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
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
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
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
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
**/
	if (L->Act != NULL && L->Act != L->Last)
	{
		tDLElemPtr PomUk;
		PomUk = L->Act->rptr; // prvok ktory rusime

		L->Act->rptr =  PomUk->rptr; // skopirujeme ukazatel prvku ktory je za aktívnym a ukazuje napravo na DALSI/NULL

		if (PomUk->rptr != NULL) // za prvkom ktory rusime je este dalsi prvok
			PomUk->rptr->lptr = L->Act; // ten dalsi prvok musi mat NALAVO ná¹ aktuálny prvok
		else 
			L->Last = L->Act; // zrusili sme posledný prvok a AKTIVNY sa stane aj LAST

		free(PomUk); // uvolníme
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
**/
	if (L->Act != NULL && L->Act != L->First)
	{
		tDLElemPtr PomUk;
		PomUk = L->Act->lptr; // prvok ktory rusime TEDA ten NALAVO

		L->Act->lptr =  PomUk->lptr; // skopirujeme ukazatel prvku ktory je PRED aktívnym a ukazuje NALAVO na PREDCHADZAJUCI/NULL

		if (PomUk->lptr != NULL) // PRED prvkom ktory rusime je este dalsi prvok
			PomUk->lptr->rptr = L->Act; // ten dalsi prvok musi mat NALAVO ná¹ aktuálny prvok
		else 
			L->First = L->Act; // zrusili sme PRVÝ prvok a AKTIVNY sa stane aj prvým - FIRST

		free(PomUk); // uvolníme
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if (L->Act != NULL) // je aktívny
	{
		tDLElemPtr PomUk;
		
		if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
		{
			PomUk->data = val;

			if (L->Act->rptr != NULL) // ak je za aktívnym neaký prvok
				L->Act->rptr->lptr = PomUk; // prvok za aktívnym musí NALAVO ukazova» na nový prvok, ten medzi nimi
			else // Za aktívnym nieje dal¹í prvok = bude nový LAST
				L->Last = PomUk;

			PomUk->rptr = L->Act->rptr; // nový prvok musí ukazova» NAPRAVO na dal¹í prvok/NULL


			L->Act->rptr = PomUk; // aktívny musí ukazova» NAPRAVO na nový prvok
			PomUk->lptr = L->Act; // nový prvok musí ukazova» NALAVO na ten èo je pred ním (na aktívny)
		}
		else
			DLError(); // chyba alokacie
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if (L->Act != NULL) // je aktívny
	{
		tDLElemPtr PomUk;
		
		if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
		{
			PomUk->data = val;

			if (L->Act->lptr != NULL) // ak je PRED aktívnym neaký prvok
				L->Act->lptr->rptr = PomUk; // prvok PRED aktívnym musí NAPRAVO ukazova» na nový prvok, ten medzi nimi
			else // PRED aktívnym nieje dal¹í prvok = bude nový FIRST
				L->First = PomUk;

			PomUk->lptr = L->Act->lptr; // nový prvok musí ukazova» NALAVO na predchádzajúci prvok/NULL

			L->Act->lptr = PomUk; // aktívny musí ukazova» NALAVO na nový prvok
			PomUk->rptr = L->Act; // nový prvok musí ukazova» NAPRAVO na ten èo je za ním (na aktívny)
		}
		else
			DLError(); // chyba alokacie
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
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
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/
	if (L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
		L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
		L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivní, vrací true. V opaèném pøípadì vrací false.
** Funkci implementujte jako jediný pøíkaz.
**/
	return (L->Act != NULL);
}

/* Konec c206.c*/
