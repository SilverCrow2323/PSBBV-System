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


static uint32_t crc32_bytes(const uint8_t *data, uint32_t length) {
 uint32_t crc=0xffffffffu, i;
 for(i=0;i<length;i++) { unsigned bit; crc ^= data[i]; for(bit=0;bit<8;bit++) crc=(crc>>1)^((crc&1u)?0xedb88320u:0u); }
 return crc^0xffffffffu;
}
static void write_u32le(uint8_t *out,uint32_t value) {
 out[0]=(uint8_t)value; out[1]=(uint8_t)(value>>8); out[2]=(uint8_t)(value>>16); out[3]=(uint8_t)(value>>24);
}
static uint32_t read_u32le(const uint8_t *in) {
 return (uint32_t)in[0]|((uint32_t)in[1]<<8)|((uint32_t)in[2]<<16)|((uint32_t)in[3]<<24);
}
psbbv_preferences_result psbbv_preferences_encode(const psbbv_preferences *p,uint8_t *out,uint32_t size) {
 uint32_t crc; psbbv_preferences_result result;
 if(!p||!out||size!=PSBBV_PREFERENCES_ENCODED_SIZE) return PSBBV_PREFS_INVALID_ARGUMENT;
 result=psbbv_preferences_validate(p); if(result!=PSBBV_PREFS_OK) return result;
 out[0]='P'; out[1]='S'; out[2]='Y'; out[3]='S';
 out[4]=(uint8_t)p->version; out[5]=(uint8_t)p->theme; out[6]=(uint8_t)p->language;
 out[7]=p->ui_scale; out[8]=p->reduce_motion; out[9]=p->confirm_destructive; out[10]=0; out[11]=0;
 crc=crc32_bytes(out,12); write_u32le(out+12,crc); return PSBBV_PREFS_OK;
}
psbbv_preferences_result psbbv_preferences_decode(const uint8_t *in,uint32_t size,psbbv_preferences *out) {
 psbbv_preferences candidate;
 if(!in||!out||size!=PSBBV_PREFERENCES_ENCODED_SIZE) return PSBBV_PREFS_INVALID_ARGUMENT;
 if(in[0]!='P'||in[1]!='S'||in[2]!='Y'||in[3]!='S'||in[10]!=0||in[11]!=0) return PSBBV_PREFS_INVALID_VALUE;
 if(read_u32le(in+12)!=crc32_bytes(in,12)) return PSBBV_PREFS_INVALID_VALUE;
 if(in[4]!=PSBBV_PREFERENCES_VERSION) return PSBBV_PREFS_UNSUPPORTED_VERSION;
 candidate.version=in[4]; candidate.theme=(psbbv_theme)in[5]; candidate.language=(psbbv_language)in[6];
 candidate.ui_scale=in[7]; candidate.reduce_motion=in[8]; candidate.confirm_destructive=in[9];
 if(psbbv_preferences_validate(&candidate)!=PSBBV_PREFS_OK) return PSBBV_PREFS_INVALID_VALUE;
 *out=candidate; return PSBBV_PREFS_OK;
}
