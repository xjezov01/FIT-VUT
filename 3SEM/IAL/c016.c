
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                      Úpravy: Radek Hranický, říjen 2014
**                      Úpravy: Filip Ježovica, xjezov01, 2014
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
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
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
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

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
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
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
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
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
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
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
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

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
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
