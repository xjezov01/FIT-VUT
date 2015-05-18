
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS 
**                                     Implementace: Petr Pøikryl, prosinec 1994
**                                           Úpravy: Petr Pøikryl, listopad 1997
**                                                     Petr Pøikryl, kvìten 1998
**			  	                        Pøevod do jazyka C: Martin Tuèek, srpen 2005
**                                         Úpravy: Bohuslav Køena, listopad 2009
**                                         Úpravy: Karel Masaøík, øíjen 2013
**                                         Úpravy: Radek Hranický, øíjen 2014
**
** S vyu¾itím dynamického pøidìlování pamìti, implementujte NEREKURZIVNÌ
** následující operace nad binárním vyhledávacím stromem (pøedpona BT znamená
** Binary Tree a je u identifikátorù uvedena kvùli mo¾né kolizi s ostatními
** pøíklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vlo¾ení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní prùchod typu pre-order
**     BTInorder ....... nerekurzivní prùchod typu in-order
**     BTPostorder ..... nerekurzivní prùchod typu post-order
**     BTDisposeTree ... zru¹ v¹echny uzly stromu
**
** U v¹ech funkcí, které vyu¾ívají nìkterý z prùchodù stromem, implementujte
** pomocnou funkci pro nalezení nejlevìj¹ího uzlu v podstromu.
**
** Pøesné definice typù naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na nìj je typu tBTNodePtr. Jeden uzel obsahuje polo¾ku int Cont,
** která souèasnì slou¾í jako u¾iteèný obsah i jako vyhledávací klíè 
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Pøíklad slou¾í zejména k procvièení nerekurzivních zápisù algoritmù
** nad stromy. Ne¾ zaènete tento pøíklad øe¹it, prostudujte si dùkladnì
** principy pøevodu rekurzivních algoritmù na nerekurzivní. Programování
** je pøedev¹ím in¾enýrská disciplína, kde opìtné objevování Ameriky nemá
** místo. Pokud se Vám zdá, ¾e by nìco ¹lo zapsat optimálnìji, promyslete
** si v¹echny detaily Va¹eho øe¹ení. Pov¹imnìte si typického umístìní akcí
** pro rùzné typy prùchodù. Zamyslete se nad modifikací øe¹ených algoritmù
** napøíklad pro výpoèet poètu uzlù stromu, poètu listù stromu, vý¹ky stromu
** nebo pro vytvoøení zrcadlového obrazu stromu (pouze popøehazování ukazatelù
** bez vytváøení nových uzlù a ru¹ení starých).
**
** Pøi prùchodech stromem pou¾ijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodu¹ení práce máte pøedem pøipraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte 
** Pozor! Je tøeba správnì rozli¹ovat, kdy pou¾ít dereferenèní operátor *
** (typicky pøi modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (napø. pøi vyhledávání). V tomto pøíkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, pou¾ijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocná funkce, kterou budete volat pøi prùchodech stromem pro zpracování
** uzlu urèeného ukazatelem Ptr. Tuto funkci neupravujte.
**/
			
	if (Ptr==NULL) 
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else 
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
}
	
/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)  
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;  
}	

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vlo¾í hodnotu na vrchol zásobníku.
**/
{ 
                 /* Pøi implementaci v poli mù¾e dojít k pøeteèení zásobníku. */
  if (S->top==MAXSTACK) 
    printf("Chyba: Do¹lo k pøeteèení zásobníku s ukazateli!\n");
  else {  
		S->top++;  
		S->a[S->top]=ptr;
	}
}	

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem zpùsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Do¹lo k podteèení zásobníku s ukazateli!\n");
		return(NULL);	
	}	
	else {
		return (S->a[S->top--]);
	}	
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}	

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;  
}	

void SPushB (tStackB *S,bool val) {
/*   ------
** Vlo¾í hodnotu na vrchol zásobníku.
**/
                 /* Pøi implementaci v poli mù¾e dojít k pøeteèení zásobníku. */
	if (S->top==MAXSTACK) 
		printf("Chyba: Do¹lo k pøeteèení zásobníku pro boolean!\n");
	else {
		S->top++;  
		S->a[S->top]=val;
	}	
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem zpùsobí chybu. */
	if (S->top==0) {
		printf("Chyba: Do¹lo k podteèení zásobníku pro boolean!\n");
		return(NULL);	
	}	
	else {  
		return(S->a[S->top--]); 
	}	
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat. 
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze pøed prvním pou¾itím binárního
** stromu, proto¾e neuvolòuje uzly neprázdného stromu (a ani to dìlat nemù¾e,
** proto¾e pøed inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zru¹ení binárního stromu slou¾í procedura BTDisposeTree.
**	
** V¹imnìte si, ¾e zde se poprvé v hlavièce objevuje typ ukazatel na ukazatel,	
** proto je tøeba pøi práci s RootPtr pou¾ít dereferenèní operátor *.
**/

	*RootPtr = NULL;	
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vlo¾í do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytváøený strom jako binární vyhledávací strom,
** kde uzly s hodnotou men¹í ne¾ má otec le¾í v levém podstromu a uzly vìt¹í
** le¾í vpravo. Pokud vkládaný uzel ji¾ existuje, neprovádí se nic (daná hodnota
** se ve stromu mù¾e vyskytnout nejvý¹e jednou). Pokud se vytváøí nový uzel,
** vzniká v¾dy jako list stromu. Funkci implementujte nerekurzivnì.
**/
	
	tBTNodePtr prvok;
	tBTNodePtr otec;
	
	if ( (*RootPtr) == NULL)
	{
		/* Nic tam neni tak to budeme moct vlozit */
		if( (prvok = malloc(sizeof(struct tBTNode))) == NULL )
			return; // chyba alokacie

		prvok->Cont = Content;
		prvok->LPtr = prvok->RPtr = NULL; // synovia su prazdny

		(*RootPtr) = prvok; // ulozime do korena
	}
	else // nieco je v binarnom strome
	{
		prvok = otec = (*RootPtr);

		while(prvok != NULL)
		{
			otec = prvok;

			if (prvok->Cont > Content)
			{
				/* to co vkladame je mensie cize DOLAVA */
				prvok = prvok->LPtr;
			}
			else if (prvok->Cont < Content)
			{
				/* to co vkladame je vecsie cize DOPRAVA */
				prvok = prvok->RPtr;
			}
			else /* Cont == Content */
				return; // nasli sme rovnaky kluc KONIEC !!
		}

		/* OK mozeme vlozit */
		if( (prvok = malloc(sizeof(struct tBTNode))) == NULL )
			return; // chyba alokacie

		prvok->Cont = Content;
		prvok->LPtr = prvok->RPtr = NULL; // synovia su prazdny

		if (otec->Cont > Content)
		{
			/* to co vkladame je mensie cize DOLAVA */
			otec->LPtr = prvok;
		}
		else /* to co vkladame je vecsie cize DOPRAVA */
		{
			otec->RPtr = prvok;
		}
	}
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìj¹í uzel.
**
** Pøi prùchodu Preorder nav¹tívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na nì is ulo¾íme do zásobníku.
**/
	
	while(ptr != NULL)
	{
		SPushP(Stack, ptr); // ulozime na zasobnik
		BTWorkOut(ptr); 	// ked prechadzam vypisujem

		ptr = ptr->LPtr;  // ideme uplne dolava
	}
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** Prùchod stromem typu preorder implementovaný nerekurzivnì s vyu¾itím funkce
** Leftmost_Preorder a zásobníku ukazatelù. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/

	/* v tejo funkcii vypisujeme priamo ked prechadzame */

	if (RootPtr == NULL)
	{
		/* nic tam neni KONIEC */
		return;
	}

	tStackP zasobnik;
	SInitP(&zasobnik);	// inicializujeme zasobnik

	tBTNodePtr pomoc;

	Leftmost_Preorder(RootPtr, &zasobnik); // lave uzle

	while( !(SEmptyP(&zasobnik)) ) // dokym neni zasobnik prazdny
	{
		pomoc = STopPopP(&zasobnik);
		Leftmost_Preorder(pomoc->RPtr, &zasobnik); // pravy uzol, spracuje jeho lave uzle
	}
}


/*                                  INORDER                                   */ 

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìj¹í uzel.
**
** Pøi prùchodu Inorder ukládáme ukazatele na v¹echny nav¹tívené uzly do
** zásobníku. 
**/
	
	while(ptr != NULL)
	{
		SPushP(Stack, ptr);	// ulozime na zasobnik
		ptr= ptr->LPtr;		// ideme dolava
	}
}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Prùchod stromem typu inorder implementovaný nerekurzivnì s vyu¾itím funkce
** Leftmost_Inorder a zásobníku ukazatelù. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/

	/* v tejo funkcii vypisujeme ked sa vraciame zlava na uzol */

	if (RootPtr == NULL)
	{
		/* nic tam neni KONIEC */
		return;
	}
	
	tStackP zasobnik;
	SInitP(&zasobnik);	// inicializujeme zasobnik

	tBTNodePtr pomoc;

	Leftmost_Inorder(RootPtr, &zasobnik); // lave uzle

	while( !(SEmptyP(&zasobnik)) ) // dokym neni zasobnik prazdny
	{
		pomoc = STopPopP(&zasobnik); // ked prechazdam vyhadzuje
		BTWorkOut(pomoc);	// ked sa vracam ZLAVA tak vypisujem
		Leftmost_Inorder(pomoc->RPtr, &zasobnik); // pravy uzol, spracuje jeho lave uzle
	}
}

/*                                 POSTORDER                                  */ 

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìj¹í uzel.
**
** Pøi prùchodu Postorder ukládáme ukazatele na nav¹tívené uzly do zásobníku
** a souèasnì do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** nav¹tíven poprvé a ¾e se tedy je¹tì nemá zpracovávat. 
**/
	
	while(ptr != NULL)
	{
		SPushP(StackP, ptr);	// ukladame ukazatele na navstivene uzle
		SPushB(StackB, TRUE);	// tento uzol bol navstiveny PRVY KRAT
		ptr = ptr->LPtr;
	}
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Prùchod stromem typu postorder implementovaný nerekurzivnì s vyu¾itím funkce
** Leftmost_Postorder, zásobníku ukazatelù a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut(). 
**/
	
	/* v tejo funkcii vypisujeme ked sa vraciame zprava na uzol */

	if (RootPtr == NULL)
	{
		/* nic tam neni KONIEC */
		return;
	}

	tStackP zasobnik;
	SInitP(&zasobnik);	// inicializujeme zasobnik
	tStackB BOOLzas;	// potrebujeme i boolean zasobnik
	SInitB(&BOOLzas);

	tBTNodePtr pomoc;
	pomoc = RootPtr;	//ulozime popredu

	Leftmost_Postorder(pomoc, &zasobnik, &BOOLzas); // lave uzle

	while( !(SEmptyP(&zasobnik)) ) // dokym neni zasobnik prazdny
	{
		pomoc = STopPopP(&zasobnik); // ked prechazdam vyhadzuje, ale my to potrebujeme mat na zasobniku
		SPushP(&zasobnik, pomoc);	// vratime to na zasobnik

		if ( STopPopB(&BOOLzas) )  // true/false
		{
			/* ano uzol bol navstiveny prvy krat ale teraz uz ideme zas cise mu nastavime */
			SPushB(&BOOLzas, FALSE);
			Leftmost_Postorder(pomoc->RPtr, &zasobnik, &BOOLzas); // pravy uzol, spracuje jeho lave uzle
		}
		else // vracame sa z pravej strany k uzlu
		{
			/* ked sa vracam na uzol tak vypisem */
			BTWorkOut(pomoc);
			/* a vyprazdnim zo zasobnika, nepotrebujem ukladat */
			STopPopP(&zasobnik);
		}
	}
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zru¹í v¹echny uzly stromu a korektnì uvolní jimi zabranou pamì».
**
** Funkci implementujte nerekurzivnì s vyu¾itím zásobníku ukazatelù.
**/
	
	if ( *RootPtr == NULL )
	{
		/* nic tam neni KONIEC */
		return;
	}
	
	tStackP zasobnik;
	SInitP(&zasobnik);	// inicializujeme zasobnik

	tBTNodePtr pomoc;

	SPushP(&zasobnik, *RootPtr);	// na zasobnik si dame koren stromu

	while( !(SEmptyP(&zasobnik)) ) // dokym neni zasobnik prazdny
	{
		pomoc = STopPopP(&zasobnik);
		if ( pomoc != NULL ) // ak je prazndy tam nic...
		{
			/* musime dat na zasobnik oboch synov */
			SPushP(&zasobnik, pomoc->RPtr);
			SPushP(&zasobnik, pomoc->LPtr);
			free(pomoc);
		}
	}

	*RootPtr = NULL;
}

/* konec c402.c */
