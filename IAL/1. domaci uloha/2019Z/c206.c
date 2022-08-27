
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
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
    printf("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList(tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

    // inicializuje prazdny seznam
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLDisposeList(tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

    // vymaze vsechny prvky, prvek po prvku z listu a nastavi ho jako pri inicializaci na NULL
    tDLElemPtr delete;
    while((delete = L->First) != NULL){
        L->First = delete->rptr;
        delete->rptr = delete->lptr = NULL;
        free(delete);
    }
    DLInitList(L);
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLInsertFirst(tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    // alokoju misto pro prvni prvek a vlozi do nej val
    struct tDLElem *new = malloc(sizeof(struct tDLElem));
    if(new == NULL){
        DLError();
        return;
    }
    new->data = val;

    // vlozi ho na prvni misto
    if(L->First == NULL){
      L->First = new;
      L->Last = new;
      new->rptr = NULL;
      new->lptr = NULL;
    } else {
        // kdyz uz tam nejake prvky jsou tak je nastavi jako dalsi
        new->lptr = NULL;
        new->rptr = L->First;
        L->First->lptr = new;
        L->First = new;
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    // alokoju misto pro posledni prvek a vlozi do nej val
    struct tDLElem *new = malloc(sizeof(struct tDLElem));
    if(new == NULL){
        DLError();
        return;
    }
    new->data = val;

    // vlozi ho na posledni misto
    if(L->First == NULL){
        L->First = new;
        L->Last = new;
        new->lptr = NULL;
        new->rptr = NULL;
    } else {
        // kdyz uz tam nejake prvky jsou tak je nastavi jako pred sebe
        new->lptr = L->Last;
        new->rptr = NULL;
        L->Last->rptr = new;
        L->Last = new;
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLFirst(tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    // prvni prvek nastavi jako aktivni
    L->Act = L->First;
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLLast(tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    // posledni prvek nastavi jako aktivni
    L->Act = L->Last;
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopyFirst(tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    // zkontroluje jestli je prvni prvek inicializovany a kdyz je tak si vezme jeho hodnotu
    if(L->First == NULL){
        DLError();
    } else {
        *val = L->First->data;
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopyLast(tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    // zkontroluje jestli je posledni prvek inicializovany a kdyz je tak si vezme jeho hodnotu
    if(L->Last == NULL){
        DLError();
    } else {
        *val = L->Last->data;
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLDeleteFirst(tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

    // zkontroluje jeslti je prvni prvek inicializovany
    if(L->First != NULL){
        // pokud je prvni prvek inicializovany a aktivni nastavi aktivitu na NULL
        if(L->Act == L->First)
            L->Act = NULL;
        // pokud je prvni prvek inicializovany a zaroven posledni, tak nastavi posledni na NULL
        if(L->First == L->Last)
            L->Last = NULL;

        // pote prvek smaze a nastavidruhy prvek jako prvni
        tDLElemPtr pom;
        pom = L->First->rptr;
        free(L->First);
        L->First = pom;
        if(L->First == NULL)
            return;
        L->First->lptr = NULL;
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLDeleteLast(tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

    // zkontroluje jeslti je posledni prvek inicializovany
    if(L->Last != NULL){
        // pokud je posledni prvek inicializovany a aktivni nastavi aktivitu na NULL
        if(L->Act == L->Last)
            L->Act = NULL;
        // pokud je posledni prvek inicializovany a zaroven prvni, tak nastavi prvni na NULL
        if(L->Last == L->First)
            L->First = NULL;

        // pote prvek smaze a nastavi predposledni prvek jako posledni
        tDLElemPtr pom;
        pom = L->Last->lptr;
        free(L->Last);
        L->Last = pom;
        if(L->Last == NULL)
            return;
        L->Last->rptr = NULL;
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPostDelete(tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

    // zkontroluje jestli existuje aktivni prvek a jestli aktivni prvek neni zaroven posledni
    if(L->Act != L->Last && L->Act){
        tDLElemPtr delete = L->Act->rptr;
        tDLElemPtr join = delete->rptr;

        // posune prvky k sobe aby neodkazovali na prvek, ktery cheme smazat a pote smaze prvek, ktery chceme smazat
        if(join != NULL){
            join->lptr = L->Act;
            L->Act->rptr = join;
        } else {
            L->Act->rptr = NULL;
            L->Last = L->Act;
        }
        free(delete);
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPreDelete(tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

    // zkontroluje jestli existuje aktivni prvek a jestli aktivni prvek neni zaroven prvni
    if(L->Act != L->First && L->Act){
        tDLElemPtr delete = L->Act->lptr;
        tDLElemPtr join = delete->lptr;

        // posune prvky k sobe aby neodkazovali na prvek, ktery cheme smazat a pote smaze prvek, ktery chceme smazat
        if(join != NULL){
            join->rptr = L->Act;
            L->Act->lptr = join;
        } else {
            L->Act->lptr = NULL;
            L->First = L->Act;
        }
        free(delete);
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPostInsert(tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    // kontrola jestli existuje aktivni prvek a pokud ano alokuje na pro nej misto v pameti
    if(L->Act != NULL) {
        struct tDLElem *new = malloc(sizeof(struct tDLElem));
        if (new == NULL) {
            DLError();
            return;
        }
        new->data = val;

        // pote prvek zaradi pred aktivni prvek a ten na ktery doposud ukazoval, pokud existuje
        tDLElemPtr pom = L->Act->rptr;
        if(pom == NULL){
            new->lptr = L->Act;
            L->Act->rptr = new;
            L->Last = new;
        } else {
            new->lptr = L->Act;
            new->rptr = pom;
            pom->lptr = new;
            L->Act->rptr = new;
        }
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPreInsert(tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    // kontrola jestli existuje aktivni prvek a pokud ano alokuje na pro nej misto v pameti
    if(L->Act != NULL){
        struct tDLElem *new = malloc(sizeof(struct tDLElem));
        if(new == NULL){
            DLError();
            return;
        }
        new->data = val;

        // pote prvek zaradi pred aktivni prvek a ten na ktery doposud ukazoval, pokud existuje
        tDLElemPtr pom = L->Act->lptr;
        if(pom == NULL){
            new->rptr = L->Act;
            L->Act->lptr = new;
            L->First = new;
        } else {
            new->rptr = L->Act;
            new->lptr = pom;
            pom->rptr = new;
            L->Act->lptr = new;
        }
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopy(tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

    if(L->Act != NULL){
        *val = L->Act->data;
    } else {
        DLError();
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLActualize(tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/

    if(L->Act != NULL){
        L->Act->data = val;
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLSucc(tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

    if(L->Act != NULL){
        L->Act = L->Act->rptr;
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}


void DLPred(tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

    if(L->Act != NULL){
        L->Act = L->Act->lptr;
    }
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

int DLActive(tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/

    return (L->Act != NULL);
    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

/* Konec c206.c*/
