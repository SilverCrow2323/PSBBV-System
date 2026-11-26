#include "psbbv/system/preferences.h"
void psbbv_preferences_defaults(psbbv_preferences *p) {
 if(!p) return;
 p->version=PSBBV_PREFERENCES_VERSION;
 p->theme=PSBBV_THEME_SYSTEM;
 p->language=PSBBV_LANGUAGE_ENGLISH;
 p->ui_scale=1;
 p->reduce_motion=0;
 p->confirm_destructive=1;
}
psbbv_preferences_result psbbv_preferences_validate(const psbbv_preferences *p) {
 if(!p) return PSBBV_PREFS_INVALID_ARGUMENT;
 if(p->version!=PSBBV_PREFERENCES_VERSION) return PSBBV_PREFS_UNSUPPORTED_VERSION;
 if(p->theme<PSBBV_THEME_SYSTEM||p->theme>PSBBV_THEME_LIGHT) return PSBBV_PREFS_INVALID_VALUE;
 if(p->language<PSBBV_LANGUAGE_ENGLISH||p->language>PSBBV_LANGUAGE_ITALIAN) return PSBBV_PREFS_INVALID_VALUE;
 if(p->ui_scale<1||p->ui_scale>3) return PSBBV_PREFS_INVALID_VALUE;
 if(p->reduce_motion>1||p->confirm_destructive>1) return PSBBV_PREFS_INVALID_VALUE;
 return PSBBV_PREFS_OK;
}
