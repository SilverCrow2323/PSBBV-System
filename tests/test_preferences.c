#include "psbbv/system/preferences.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
 psbbv_preferences p;
 psbbv_preferences_defaults(&p);
 assert(psbbv_preferences_validate(&p)==PSBBV_PREFS_OK);
 assert(p.confirm_destructive==1 && p.ui_scale==1 && p.language==PSBBV_LANGUAGE_ENGLISH);
 p.theme=PSBBV_THEME_DARK; p.language=PSBBV_LANGUAGE_ITALIAN; p.ui_scale=3;
 assert(psbbv_preferences_validate(&p)==PSBBV_PREFS_OK);
 p.ui_scale=4; assert(psbbv_preferences_validate(&p)==PSBBV_PREFS_INVALID_VALUE);
 p.ui_scale=2; p.confirm_destructive=2; assert(psbbv_preferences_validate(&p)==PSBBV_PREFS_INVALID_VALUE);
 p.confirm_destructive=1; p.version=99; assert(psbbv_preferences_validate(&p)==PSBBV_PREFS_UNSUPPORTED_VERSION);
 assert(psbbv_preferences_validate(NULL)==PSBBV_PREFS_INVALID_ARGUMENT);
 puts("PSBBV preferences tests passed"); return 0;
}
