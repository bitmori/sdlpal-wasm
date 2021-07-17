#include "main.h"

static BOOL g_lock_alchemy_formula;

VOID
PAL_ControlAlchemyFormula(
   LPALCHEMYFORMULA    lpFormula,
   BYTE op
) {
    if (lpFormula == NULL) {
        return;
    }
    if (op == 1) {
        g_lock_alchemy_formula = TRUE;
    } else if (op == 2) {
        g_lock_alchemy_formula = FALSE;
    } else if (op == 3) {
        g_lock_alchemy_formula = FALSE;
        PAL_FreeAlchemyFormula(lpFormula);
    }
}

static LPALCHEMYFORMULA
PAL_QueryAlchemyFormula(
   LPALCHEMYFORMULA    lpFormula,
   WORD                wProductObjectID
)
{
    LPALCHEMYFORMULA pCurr = lpFormula;
    while (pCurr != NULL) {
        //fprintf(stderr, "PAL_AddAlchemyFormula %d \n", __LINE__);
        if (pCurr->wObjectID == wProductObjectID) {
            return pCurr;
        } else {
            pCurr = pCurr -> next;
        }
    }
    return NULL;
}

static LPALCHEMYINGREDIENT
PAL_QueryAlchemyIngredient(
   LPALCHEMYINGREDIENT lpIngredient,
   WORD                wObjectID
)
{
   LPALCHEMYINGREDIENT pCurr = lpIngredient;
   while (pCurr != NULL) {
       //fprintf(stderr, "PAL_AddAlchemyFormula %d \n", __LINE__);
       if (pCurr->wObjectID == wObjectID) {
           // the ingredient is already added
           return pCurr;
       } else {
           pCurr = pCurr -> next;
       }
   }
   return NULL;
}

static VOID
PAL_AddAlchemyIngredient(
   LPALCHEMYFORMULA    lpFormula,
   WORD                wObjectID,
   WORD                wCount
)
{
    LPALCHEMYINGREDIENT pEntry = NULL;
    pEntry = PAL_QueryAlchemyIngredient(lpFormula->ingredients, wObjectID);
    //fprintf(stderr, "PAL_AddAlchemyFormula %d \n", __LINE__);
    if (pEntry == NULL) {
        // create the ingredient entry
        pEntry = UTIL_calloc(1, sizeof(ALCHEMYINGREDIENT));
        pEntry->wObjectID = wObjectID;
        pEntry->next = lpFormula->ingredients;
        lpFormula->ingredients = pEntry;
        //fprintf(stderr, "PAL_AddAlchemyFormula %d \n", __LINE__);
    }
    pEntry->wCount = wCount;
}

LPALCHEMYFORMULA
PAL_AddAlchemyFormula(
   LPALCHEMYFORMULA    lpFormula,
   WORD                wObjectID,
   WORD                wCount,
   WORD                wProductObjectID
)
{
    if (g_lock_alchemy_formula) {
        return lpFormula;
    }
    LPALCHEMYFORMULA pEntry = NULL;
    pEntry = PAL_QueryAlchemyFormula(lpFormula, wProductObjectID);
    //fprintf(stderr, "PAL_AddAlchemyFormula %d \n", __LINE__);
    if (pEntry == NULL) {
        //fprintf(stderr, "PAL_AddAlchemyFormula %d \n", __LINE__);
        // create the formula first
        pEntry = UTIL_calloc(1, sizeof(ALCHEMYFORMULA));
        //fprintf(stderr, "PAL_AddAlchemyFormula %d \n", __LINE__);
        pEntry->wObjectID = wProductObjectID;
        pEntry->wKeyIngredientID = 0; // by default the product is visible
        pEntry->ingredients = NULL;
        pEntry->next = lpFormula;
        lpFormula = pEntry;
    }
    if (wCount == 0) {
        // set key ingredient
        pEntry->wKeyIngredientID = wObjectID;
        //fprintf(stderr, "PAL_AddAlchemyFormula %d \n", __LINE__);
    } else {
        // add an ingredient
        PAL_AddAlchemyIngredient(pEntry, wObjectID, wCount);
        //fprintf(stderr, "PAL_AddAlchemyFormula %d \n", __LINE__);
    }
    return lpFormula;
}

static VOID
PAL_FreeAlchemyIngredient(
    LPALCHEMYFORMULA    lpFormula
)
{
   LPALCHEMYINGREDIENT p;
   while (lpFormula->ingredients != NULL)
   {
       p = lpFormula->ingredients->next;
       free(lpFormula->ingredients);
       lpFormula->ingredients = p;
   }
}

VOID
PAL_FreeAlchemyFormula(
   LPALCHEMYFORMULA    lpFormula
) {
    LPALCHEMYFORMULA    p;
    while (lpFormula != NULL) {
        p = lpFormula->next;
        PAL_FreeAlchemyIngredient(lpFormula);
        free(lpFormula);
        lpFormula = p;
    }
}
