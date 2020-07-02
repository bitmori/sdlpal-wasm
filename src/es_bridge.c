#include "main.h"

static duk_ret_t native_print(duk_context *ctx) {
	// UTIL_LogOutput(LOGLEVEL_ERROR, "%s\n", duk_to_string(ctx, 0));
	printf("%s\n", duk_to_string(ctx, 0));
  	return 0;  /* no return value (= undefined) */
}
static duk_ret_t give_cash(LPDUKCONTEXT ctx) {
   int money = 1000;
   int argc = duk_get_top(ctx);  /* #args */
   if (argc != 0) {
       money = duk_to_int(ctx, 0);
   }
   gpGlobals->dwCash += money;
   return 0;
}

static duk_ret_t lock_team(LPDUKCONTEXT ctx) {
   gpGlobals->fLockTeamMember = TRUE;
   return 0;
}

static duk_ret_t unlock_team(LPDUKCONTEXT ctx) {
   gpGlobals->fLockTeamMember = FALSE;
   return 0;
}

static duk_ret_t add_inventory(LPDUKCONTEXT ctx) {
   int argc = duk_get_top(ctx);
   if (argc < 2) {
       return -1;
   }
   int id = duk_to_int(ctx, 0);
   int cn = duk_to_int(ctx, 1);
   PAL_AddItemToInventory(id, cn);
   return 0;
}

static duk_ret_t make_fantastic4(LPDUKCONTEXT ctx)
{
   //
   // Set the player party
   //
   gpGlobals->wMaxPartyMemberIndex = 3;
   gpGlobals->rgParty[0].wPlayerRole = 0; // => XY
   gpGlobals->rgParty[1].wPlayerRole = 1; // => 02
   gpGlobals->rgParty[2].wPlayerRole = 4; // => ANU
   gpGlobals->rgParty[3].wPlayerRole = 2; // => Miss
   g_Battle.rgPlayer[0].action.ActionType = kBattleActionAttack;
   g_Battle.rgPlayer[1].action.ActionType = kBattleActionAttack;
   g_Battle.rgPlayer[2].action.ActionType = kBattleActionAttack;
   g_Battle.rgPlayer[3].action.ActionType = kBattleActionAttack;

   if (gpGlobals->wMaxPartyMemberIndex == 0)
   {
      // HACK for Dream 2.11
      gpGlobals->rgParty[0].wPlayerRole = 0;
      gpGlobals->wMaxPartyMemberIndex = 1;
   }

   //
   // Reload the player sprites
   //
   PAL_SetLoadFlags(kLoadPlayerSprite);
   PAL_LoadResources();

   memset(gpGlobals->rgPoisonStatus, 0, sizeof(gpGlobals->rgPoisonStatus));
   PAL_UpdateEquipments();
   return 0;
}

static char * va(const char *format, ...) {
   static char string[256];
   va_list     argptr;

   va_start(argptr, format);
   vsnprintf(string, 256, format, argptr);
   va_end(argptr);

   return string;
}

WORD PAL_ExecuteECMAScript(LPDUKCONTEXT ctx, WORD wScriptID) {
   if (gpGlobals->duk == NULL)
   {
      return -1;
   }
   FILE *file = fopen(va("%s%d%s", PAL_SAVE_PREFIX, wScriptID, ".js"), "r");
   if (file == NULL)
   {
      return -1;
   }
   char buf[0xFFFF];
   size_t len = fread((void *) buf, 1, sizeof(buf), file);
   fclose(file);
   duk_push_lstring(ctx, (const char *) buf, (duk_size_t) len);
   if (duk_peval(ctx) != 0) {
    printf("Script error: %s\n", duk_safe_to_string(ctx, -1));
   }
   duk_pop(ctx);
   return 0;
}

static void sdlpal_add_func(duk_context *ctx, duk_c_function func, const char *name, int argc) {
	duk_push_c_function(ctx, func, argc);
	duk_push_string(ctx, "name");
	duk_push_string(ctx, name);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_FORCE);
	duk_put_prop_string(ctx, -2, name);
}


VOID PAL_InitESHandlers(LPDUKCONTEXT ctx) {
   duk_push_c_function(ctx, native_print, 1);
   duk_put_global_string(ctx, "print");
   
   duk_push_object(ctx); // add an empty object as Sdlpal
   sdlpal_add_func(ctx, give_cash, "give_cash", 1);
   sdlpal_add_func(ctx, make_fantastic4, "make_team", 0);
   sdlpal_add_func(ctx, lock_team, "lock_team", 0);
   sdlpal_add_func(ctx, unlock_team, "unlock_team", 0);
   sdlpal_add_func(ctx, add_inventory, "add_item", 2);
   duk_put_global_string(ctx, "Sdlpal");  // register 'Sdlpal' as a global object
}
