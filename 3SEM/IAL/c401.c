
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Filip Ježovica, xjezov01, 2014
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů 
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**	
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.	
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/

	*RootPtr = NULL;
}	

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/

	if (RootPtr == NULL)
	{
		return FALSE; // nenasiel sa kluc
	}
	else
	{
		if (RootPtr->Key == K)
		{
			/* nasli sme , vratime obsah uzla v CONTENT */
			*Content = RootPtr->BSTNodeCont;
			return TRUE;
		}
		else // nenasli sme Kluc
		{
			if ( K < RootPtr->Key) // ci mame hladat vpravo alebo vlavo
			{
				/* budeme hladat VLAVO  KLUC JE MENSI ..LPTR  */
				return BSTSearch(RootPtr->LPtr, K, Content);
			}
			else
			{
				/* budeme hladat VPRAVO KLUC JE VECSI ..RPTR */
				return BSTSearch(RootPtr->RPtr, K, Content);
			}
		}
	}	
} 


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{	
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/

	if (*RootPtr == NULL ) // mozeme vlozit novy prvok
	{
		tBSTNodePtr novy;
		if(  (novy = malloc(sizeof(struct tBSTNode)) ) == NULL ) // alokujeme mu pamet
			return; /* chyba alokacie ! */

		novy->Key = K; // naplnime kluc
		novy->BSTNodeCont = Content;  // naplnime data

		/* Nema ziadne listy */
		novy->LPtr = novy->RPtr = NULL;
		*RootPtr = novy; // predame a hotovo
	}
	else // musime vyhladat vhodne miesto / ak existuje aktualizovat
	{
		if ( (*RootPtr)->Key == K )
		{
			/* AKTUALIZACIA >>> naplnime data a KONIEC */
			(*RootPtr)->BSTNodeCont = Content;
		}
		else if ( K < (*RootPtr)->Key ) // dolava hladame
		{
			/* budeme hladat VLAVO  KLUC JE MENSI ..LPTR  */
			BSTInsert( &((*RootPtr)->LPtr), K, Content ); // dame mu adresu laveho
		}
		else
		{
			/* budeme hladat VPRAVO  KLUC JE VECSI ..RPTR  */
			BSTInsert( &((*RootPtr)->RPtr), K, Content );
		}
	}
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně. 
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete(). 
**/
	
	if ( (*RootPtr == NULL) )
	{
		/* halt .. nemozeme pokracovat lebo tam nic neni*/
		return;
	}
	
	if ( (*RootPtr)->RPtr == NULL )
	{
		/* napravo uz nic nieje mozeme prehodit a uvolnit pamet */
		PtrReplaced->BSTNodeCont = (*RootPtr)->BSTNodeCont; // data
		PtrReplaced->Key = (*RootPtr)->Key;	// kluc

		tBSTNodePtr pomoc = (*RootPtr); // premiestneny/mazany
		(*RootPtr) = (*RootPtr)->LPtr; // musime ist na lavy podstrom
		free(pomoc);	// uvolnime pamet po presunutom prvku

	}
	else
	{
		/* Rekurzivne volanie ak mame nieco NAPRAVO ! */
		ReplaceByRightmost( PtrReplaced, &(*RootPtr)->RPtr ); // ideme napravo
	}
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic. 
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/

	if (*RootPtr != NULL) // nemoze byt null pred volanim funkcie REPLACE
	{
		if ( (*RootPtr)->Key == K)
		{
			/* TOTO JE UZOL KT RUSIME - nasli sme kluc*/
			
			if (  ((*RootPtr)->LPtr != NULL) && ((*RootPtr)->RPtr != NULL)    )
			{
				/* pravy aj lavy syn existuje musime pouzit fciu*/
				ReplaceByRightmost( (*RootPtr), (&(*RootPtr)->LPtr) );

			}
			else 
			{	
				tBSTNodePtr pomoc = (*RootPtr); // mazany

				if ( ((*RootPtr)->LPtr != NULL) )
					{
						*RootPtr = (*RootPtr)->LPtr;
					}
					else
					{
						*RootPtr = (*RootPtr)->RPtr;
					}

					free(pomoc); // uvolnenie pamete
			}
			
		}
		else if ( K < (*RootPtr)->Key ) // dolava hladame
		{
			/* budeme hladat VLAVO  KLUC JE MENSI ..LPTR  */
			BSTDelete( &((*RootPtr)->LPtr), K ); // dame mu adresu laveho
		}
		else if(K > (*RootPtr)->Key)
		{
			/* budeme hladat VPRAVO  KLUC JE VECSI ..RPTR  */
			BSTDelete( &((*RootPtr)->RPtr), K );
		}
	}
} 

void BSTDispose (tBSTNodePtr *RootPtr) {	
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/

	if (*RootPtr != NULL) // keby je NULL tak nemame co vnom rusit {neexistuje}
	{
		/* budeme rusit dolava i doprava */

		/* dolava */
		BSTDispose( &(*RootPtr)->LPtr );  // asi &

		/* doprava */
		BSTDispose( &(*RootPtr)->RPtr );


		free(*RootPtr);  // uvolnime pamet
		*RootPtr = NULL;
	}
}

/* konec c401.c */

