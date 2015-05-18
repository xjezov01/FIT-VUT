	
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2014
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu, 
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek, 
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL; 
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
**/
	tDLElemPtr PomUk;

	while (L->First != NULL) // ak by bol prazdny ni� sa nedeje
	{
		PomUk = L->First;

		L->First = L->First->rptr; // prvym prvkom bude ten �o bol za n�m
		free(PomUk); // uvoln�me pame�
	}
	L->Act = NULL; // zrusime aktivitu zoznamu kedze je prazdny, teraz je v stave ako po inicializacii
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	tDLElemPtr PomUk;

	if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
	{
		PomUk->data = val;

		PomUk->rptr = L->First; // novy musi ukazat napravo teda prvok kt. bol doteraz prvy, ak bol NULL tak sa NULL skopiruje
		PomUk->lptr = NULL; // prvy prvok ma vzdy NALAVO NULL

		if (L->Last == NULL) // ak je zoznam prazdny
			L->Last = PomUk;  // znamena to �e je aj posledn�
		else
			L->First->lptr = PomUk; // ten co bol doteraz prvy musi ukazovat naLAVO na novo vytvoreny
		
		L->First = PomUk;
	}
	else
		DLError(); // chyba alokacie
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	tDLElemPtr PomUk;

	if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
	{
		PomUk->data = val;

		PomUk->rptr = NULL; // posledny prvok ma vzdy NAPRAVO NULL
		PomUk->lptr = L->Last; // novy musi ukazat napravo teda prvok kt. bol doteraz prvy, ak bol NULL tak sa NULL skopiruje

		if (L->Last == NULL) // ak je zoznam prazdny
			L->First = PomUk;  // znamena to �e je aj prv�
		else
			L->Last->rptr = PomUk; // ten co bol doteraz prvy musi ukazovat naLAVO na novo vytvoreny
		
		L->Last = PomUk;
	}
	else
		DLError(); // chyba alokacie
}

void DLFirst (tDLList *L) {
/*
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if (L->Last != NULL)
	{
		*val = L->First->data;
	}
	else
		DLError(); // zoznam je pr�zdny
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if (L->Last != NULL)
	{
		*val = L->Last->data;
	}
	else
		DLError(); // zoznam je pr�zdny
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
				L->Act = NULL; // ak bol akt�vny - str�came aktivitu
		
		L->First = PomUk->rptr; // prvym sa stal dalsi
		L->First->lptr = NULL; // prvy prvok tak nalavo musi byt NULL
		
		}

		free(PomUk); // uvoln�me
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
				L->Act = NULL; // ak bol akt�vny - str�came aktivitu
		
		L->Last = PomUk->lptr; // poslednym sa stal predchadzajuci teda nalavo
		L->Last->rptr = NULL; // posledny prvok tak napravo musi byt NULL
		
		}

		free(PomUk); // uvoln�me
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
**/
	if (L->Act != NULL && L->Act != L->Last)
	{
		tDLElemPtr PomUk;
		PomUk = L->Act->rptr; // prvok ktory rusime

		L->Act->rptr =  PomUk->rptr; // skopirujeme ukazatel prvku ktory je za akt�vnym a ukazuje napravo na DALSI/NULL

		if (PomUk->rptr != NULL) // za prvkom ktory rusime je este dalsi prvok
			PomUk->rptr->lptr = L->Act; // ten dalsi prvok musi mat NALAVO n� aktu�lny prvok
		else 
			L->Last = L->Act; // zrusili sme posledn� prvok a AKTIVNY sa stane aj LAST

		free(PomUk); // uvoln�me
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
**/
	if (L->Act != NULL && L->Act != L->First)
	{
		tDLElemPtr PomUk;
		PomUk = L->Act->lptr; // prvok ktory rusime TEDA ten NALAVO

		L->Act->lptr =  PomUk->lptr; // skopirujeme ukazatel prvku ktory je PRED akt�vnym a ukazuje NALAVO na PREDCHADZAJUCI/NULL

		if (PomUk->lptr != NULL) // PRED prvkom ktory rusime je este dalsi prvok
			PomUk->lptr->rptr = L->Act; // ten dalsi prvok musi mat NALAVO n� aktu�lny prvok
		else 
			L->First = L->Act; // zrusili sme PRV� prvok a AKTIVNY sa stane aj prv�m - FIRST

		free(PomUk); // uvoln�me
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	if (L->Act != NULL) // je akt�vny
	{
		tDLElemPtr PomUk;
		
		if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
		{
			PomUk->data = val;

			if (L->Act->rptr != NULL) // ak je za akt�vnym neak� prvok
				L->Act->rptr->lptr = PomUk; // prvok za akt�vnym mus� NALAVO ukazova� na nov� prvok, ten medzi nimi
			else // Za akt�vnym nieje dal�� prvok = bude nov� LAST
				L->Last = PomUk;

			PomUk->rptr = L->Act->rptr; // nov� prvok mus� ukazova� NAPRAVO na dal�� prvok/NULL


			L->Act->rptr = PomUk; // akt�vny mus� ukazova� NAPRAVO na nov� prvok
			PomUk->lptr = L->Act; // nov� prvok mus� ukazova� NALAVO na ten �o je pred n�m (na akt�vny)
		}
		else
			DLError(); // chyba alokacie
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	if (L->Act != NULL) // je akt�vny
	{
		tDLElemPtr PomUk;
		
		if ((PomUk = malloc(sizeof(struct tDLElem))) != NULL)
		{
			PomUk->data = val;

			if (L->Act->lptr != NULL) // ak je PRED akt�vnym neak� prvok
				L->Act->lptr->rptr = PomUk; // prvok PRED akt�vnym mus� NAPRAVO ukazova� na nov� prvok, ten medzi nimi
			else // PRED akt�vnym nieje dal�� prvok = bude nov� FIRST
				L->First = PomUk;

			PomUk->lptr = L->Act->lptr; // nov� prvok mus� ukazova� NALAVO na predch�dzaj�ci prvok/NULL

			L->Act->lptr = PomUk; // akt�vny mus� ukazova� NALAVO na nov� prvok
			PomUk->rptr = L->Act; // nov� prvok mus� ukazova� NAPRAVO na ten �o je za n�m (na akt�vny)
		}
		else
			DLError(); // chyba alokacie
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/
	if (L->Act != NULL)
	{
		*val = L->Act->data;
	}
	else
		DLError(); // zoznam nieje AKT�VNY
}

void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
	if (L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/
	if (L->Act != NULL)
		L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
	if (L->Act != NULL)
		L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivn�, vrac� true. V opa�n�m p��pad� vrac� false.
** Funkci implementujte jako jedin� p��kaz.
**/
	return (L->Act != NULL);
}

/* Konec c206.c*/
