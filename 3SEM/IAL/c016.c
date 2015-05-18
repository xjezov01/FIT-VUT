
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Polo¾kami
**                      První implementace: Petr Pøikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masaøík, øíjen 2014
**                      Úpravy: Radek Hranický, øíjen 2014
**
** Vytvoøete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Polo¾kami = Hash table)
** s explicitnì øetìzenými synonymy. Tabulka je implementována polem
** lineárních seznamù synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku pøed prvním pou¾itím
**  HTInsert ..... vlo¾ení prvku
**  HTSearch ..... zji¹tìní pøítomnosti prvku v tabulce
**  HTDelete ..... zru¹ení prvku
**  HTRead ....... pøeètení hodnoty prvku
**  HTClearAll ... zru¹ení obsahu celé tabulky (inicializace tabulky
**                 poté, co ji¾ byla pou¾ita)
**
** Definici typù naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelù na polo¾ky, je¾ obsahují slo¾ky
** klíèe 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dal¹í synonymum 'ptrnext'. Pøi implementaci funkcí
** uva¾ujte maximální rozmìr pole HTSIZE.
**
** U v¹ech procedur vyu¾ívejte rozptylovou funkci hashCode.  Pov¹imnìte si
** zpùsobu pøedávání parametrù a zamyslete se nad tím, zda je mo¾né parametry
** pøedávat jiným zpùsobem (hodnotou/odkazem) a v pøípadì, ¾e jsou obì
** mo¾nosti funkènì pøípustné, jaké jsou výhody èi nevýhody toho èi onoho
** zpùsobu.
**
** V pøíkladech jsou pou¾ity polo¾ky, kde klíèem je øetìzec, ke kterému
** je pøidán obsah - reálné èíslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíè a pøidìlit
** mu index v rozmezí 0..HTSize-1.  V ideálním pøípadì by mìlo dojít
** k rovnomìrnému rozptýlení tìchto klíèù po celé tabulce.  V rámci
** pokusù se mù¾ete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitnì zøetìzenými synonymy.  Tato procedura
** se volá pouze pøed prvním pou¾itím tabulky.
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

/* TRP s explicitnì zøetìzenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíèe key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
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
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vkládá do tabulky ptrht polo¾ku s klíèem key a s daty
** data.  Proto¾e jde o vyhledávací tabulku, nemù¾e být prvek se stejným
** klíèem ulo¾en v tabulce více ne¾ jedenkrát.  Pokud se vkládá prvek,
** jeho¾ klíè se ji¾ v tabulce nachází, aktualizujte jeho datovou èást.
**
** Vyu¾ijte døíve vytvoøenou funkci htSearch.  Pøi vkládání nového
** prvku do seznamu synonym pou¾ijte co nejefektivnìj¹í zpùsob,
** tedy proveïte.vlo¾ení prvku na zaèátek seznamu.
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
** TRP s explicitnì zøetìzenými synonymy.
** Tato funkce zji¹»uje hodnotu datové èásti polo¾ky zadané klíèem.
** Pokud je polo¾ka nalezena, vrací funkce ukazatel na polo¾ku
** Pokud polo¾ka nalezena nebyla, vrací se funkèní hodnota NULL
**
** Vyu¾ijte døíve vytvoøenou funkci HTSearch.
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
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vyjme polo¾ku s klíèem key z tabulky
** ptrht.  Uvolnìnou polo¾ku korektnì zru¹te.  Pokud polo¾ka s uvedeným
** klíèem neexistuje, dìlejte, jako kdyby se nic nestalo (tj. nedìlejte
** nic).
**
** V tomto pøípadì NEVYU®ÍVEJTE døíve vytvoøenou funkci HTSearch.
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

/* TRP s explicitnì zøetìzenými synonymy.
** Tato procedura zru¹í v¹echny polo¾ky tabulky, korektnì uvolní prostor,
** který tyto polo¾ky zabíraly, a uvede tabulku do poèáteèního stavu.
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
