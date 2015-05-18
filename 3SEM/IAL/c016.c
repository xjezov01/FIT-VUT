
/* c016.c: **********************************************************}
{* T�ma:  Tabulka s Rozpt�len�mi Polo�kami
**                      Prvn� implementace: Petr P�ikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      �pravy: Karel Masa��k, ��jen 2014
**                      �pravy: Radek Hranick�, ��jen 2014
**
** Vytvo�ete abstraktn� datov� typ
** TRP (Tabulka s Rozpt�len�mi Polo�kami = Hash table)
** s explicitn� �et�zen�mi synonymy. Tabulka je implementov�na polem
** line�rn�ch seznam� synonym.
**
** Implementujte n�sleduj�c� procedury a funkce.
**
**  HTInit ....... inicializuje tabulku p�ed prvn�m pou�it�m
**  HTInsert ..... vlo�en� prvku
**  HTSearch ..... zji�t�n� p��tomnosti prvku v tabulce
**  HTDelete ..... zru�en� prvku
**  HTRead ....... p�e�ten� hodnoty prvku
**  HTClearAll ... zru�en� obsahu cel� tabulky (inicializace tabulky
**                 pot�, co ji� byla pou�ita)
**
** Definici typ� naleznete v souboru c016.h.
**
** Tabulka je reprezentov�na datovou strukturou typu tHTable,
** kter� se skl�d� z ukazatel� na polo�ky, je� obsahuj� slo�ky
** kl��e 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dal�� synonymum 'ptrnext'. P�i implementaci funkc�
** uva�ujte maxim�ln� rozm�r pole HTSIZE.
**
** U v�ech procedur vyu��vejte rozptylovou funkci hashCode.  Pov�imn�te si
** zp�sobu p�ed�v�n� parametr� a zamyslete se nad t�m, zda je mo�n� parametry
** p�ed�vat jin�m zp�sobem (hodnotou/odkazem) a v p��pad�, �e jsou ob�
** mo�nosti funk�n� p��pustn�, jak� jsou v�hody �i nev�hody toho �i onoho
** zp�sobu.
**
** V p��kladech jsou pou�ity polo�ky, kde kl��em je �et�zec, ke kter�mu
** je p�id�n obsah - re�ln� ��slo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovac� funkce - jej�m �kolem je zpracovat zadan� kl�� a p�id�lit
** mu index v rozmez� 0..HTSize-1.  V ide�ln�m p��pad� by m�lo doj�t
** k rovnom�rn�mu rozpt�len� t�chto kl��� po cel� tabulce.  V r�mci
** pokus� se m��ete zamyslet nad kvalitou t�to funkce.  (Funkce nebyla
** volena s ohledem na maxim�ln� kvalitu v�sledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitn� z�et�zen�mi synonymy.  Tato procedura
** se vol� pouze p�ed prvn�m pou�it�m tabulky.
*/

void htInit ( tHTable* ptrht ) {
/*  */

	if (ptrht == NULL)
	{
		/* volaco je zle */
		return;
	}
	
	for (int i = 0; i < HTSIZE; ++i)
	{
		(*ptrht)[i]= NULL; // vsetky pozicie ukazuju na NULL
	}
}

/* TRP s explicitn� z�et�zen�mi synonymy.
** Vyhled�n� prvku v TRP ptrht podle zadan�ho kl��e key.  Pokud je
** dan� prvek nalezen, vrac� se ukazatel na dan� prvek. Pokud prvek nalezen nen�, 
** vrac� se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	tHTItem* prvok = (*ptrht)[hashCode(key)]; // hashcode nad da index >> mame prvok podla kluca

	while(prvok != NULL)
	{
		if(strcmp(prvok->key, key) == 0)
		{
			return prvok;
		}
		else
			prvok = prvok->ptrnext;
	}
	return NULL; // nenaslo sa
}

/* 
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura vkl�d� do tabulky ptrht polo�ku s kl��em key a s daty
** data.  Proto�e jde o vyhled�vac� tabulku, nem��e b�t prvek se stejn�m
** kl��em ulo�en v tabulce v�ce ne� jedenkr�t.  Pokud se vkl�d� prvek,
** jeho� kl�� se ji� v tabulce nach�z�, aktualizujte jeho datovou ��st.
**
** Vyu�ijte d��ve vytvo�enou funkci htSearch.  P�i vkl�d�n� nov�ho
** prvku do seznamu synonym pou�ijte co nejefektivn�j�� zp�sob,
** tedy prove�te.vlo�en� prvku na za��tek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

	if ( ptrht == NULL )
	{
		/* dalej nemozeme pokracovat */
		return;
	}

	tHTItem *prvok = htSearch(ptrht, key); // vyhlada ci taky kluc uz nahodu neni ulozeny

	if (prvok != NULL)
	{
		prvok->data = data;  // nasli sme tak AKTUALIZUJEME
	}
	else // taky kluc tam neni vytvorime novy na zaciatok
	{
		prvok = (*ptrht)[hashCode(key)]; // hashcode nad da index >> mame prvok podla kluca
		tHTItem *novy;
		
		if( (novy = malloc(sizeof(tHTItem)) ) == NULL )
		{
			// chyba alokacie
		}
		else if ( (novy->key = malloc(strlen(key)+1) )  == NULL )
		{
			free(novy);
			// chyba alokacie + rusim to predtym ak sa to padarilo
		}

		strcpy(novy->key,  key); // nakopirujeme kluc "string"
		novy->data = data; // naplnime data float

		// naviazeme
		novy->ptrnext = prvok;
		(*ptrht)[hashCode(key)] = novy; // vlozime na zaciatok
	}
}

/*
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato funkce zji��uje hodnotu datov� ��sti polo�ky zadan� kl��em.
** Pokud je polo�ka nalezena, vrac� funkce ukazatel na polo�ku
** Pokud polo�ka nalezena nebyla, vrac� se funk�n� hodnota NULL
**
** Vyu�ijte d��ve vytvo�enou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	tHTItem* prvok = htSearch(ptrht, key); // vyhlada ci taky kluc uz nahodu neni ulozeny

	if (prvok != NULL) // nasli sme
	{
		return &(prvok->data);
	}
	return NULL;
}

/*
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura vyjme polo�ku s kl��em key z tabulky
** ptrht.  Uvoln�nou polo�ku korektn� zru�te.  Pokud polo�ka s uveden�m
** kl��em neexistuje, d�lejte, jako kdyby se nic nestalo (tj. ned�lejte
** nic).
**
** V tomto p��pad� NEVYU��VEJTE d��ve vytvo�enou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

	if (ptrht == NULL)
		return; // nieco je zle KONIEC

	int i = hashCode(key);

	tHTItem* prvok = (*ptrht)[i]; // hashcode nad da index >> mame prvok podla kluca
	tHTItem* prvokPRED = (*ptrht)[i]; // zo zaciatku tam musime mat to iste

	if (prvok == NULL)
	{
		/* nemame ziadny prvok */
		return;
	}

		while(prvok != NULL)
		{
			if( strcmp(prvok->key, key) == 0 )
				break;

			prvokPRED = prvok;
			prvok = prvok->ptrnext;	
			if (prvok == NULL)
				{
					/* nenasli sme ho KONEC */
					return;
				}	
		}

		// naviazeme odkazy
		prvokPRED->ptrnext = prvok->ptrnext;

		if (prvokPRED == prvok)
		{
			/* ak by sme rusili prvy prvok.. moze byt aj jediny aj viac */
			(*ptrht)[i] = prvok->ptrnext;
		}
		
		free(prvok->key); // uvolnit
		free(prvok);
}

/* TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura zru�� v�echny polo�ky tabulky, korektn� uvoln� prostor,
** kter� tyto polo�ky zab�raly, a uvede tabulku do po��te�n�ho stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	if (ptrht == NULL)
		return; // nieco je zle KONIEC

	tHTItem *temp;
	tHTItem *dalsi;

	for (int i = 0; i < HTSIZE; ++i) // prejdeme celu tabulku
	{
		dalsi = (*ptrht)[i]; // nahrame si prvok do premennej dalsi

		while(dalsi != NULL)
		{
			temp = dalsi; // ulozime si ho aby sme ho mohli vypraznit mozme do prvku lebo ten sa pri fore zmeni
			dalsi = dalsi->ptrnext;

			free(temp->key); // vypraznit kluc
			free(temp);
		}

		(*ptrht)[i]= NULL; // vsetky pozicie ukazuju na NULL
	}
}
