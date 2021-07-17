#ifndef ALCHEMY_H
#define ALCHEMY_H

#include "common.h"

typedef struct tagALCHEMYINGREDIENT
{
    WORD                          wObjectID;
    WORD                          wCount;
    struct tagALCHEMYINGREDIENT*  next;
} ALCHEMYINGREDIENT, *LPALCHEMYINGREDIENT;

typedef struct tagALCHEMYFORMULA
{
   WORD                           wObjectID;
   WORD                           wKeyIngredientID;
   struct tagALCHEMYINGREDIENT    *ingredients;
   struct tagALCHEMYFORMULA       *next;
} ALCHEMYFORMULA, *LPALCHEMYFORMULA;

VOID
PAL_ControlAlchemyFormula(
   LPALCHEMYFORMULA    lpFormula,
   BYTE op
);


LPALCHEMYFORMULA
PAL_AddAlchemyFormula(
   LPALCHEMYFORMULA    lpFormula,
   WORD                wObjectID,
   WORD                wCount,
   WORD                wProductObjectID
);

VOID
PAL_FreeAlchemyFormula(
   LPALCHEMYFORMULA    lpFormula
);

#endif