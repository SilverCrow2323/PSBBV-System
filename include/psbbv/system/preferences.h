#ifndef PSBBV_SYSTEM_PREFERENCES_H
#define PSBBV_SYSTEM_PREFERENCES_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define PSBBV_PREFERENCES_VERSION 1u
#define PSBBV_PREFERENCES_ENCODED_SIZE 16u
typedef enum { PSBBV_THEME_SYSTEM=0, PSBBV_THEME_DARK=1, PSBBV_THEME_LIGHT=2 } psbbv_theme;
typedef enum { PSBBV_LANGUAGE_ENGLISH=0, PSBBV_LANGUAGE_ITALIAN=1 } psbbv_language;
typedef struct {
 uint32_t version;
 psbbv_theme theme;
 psbbv_language language;
 uint8_t ui_scale;
 uint8_t reduce_motion;
 uint8_t confirm_destructive;
} psbbv_preferences;
typedef enum { PSBBV_PREFS_OK=0, PSBBV_PREFS_INVALID_ARGUMENT, PSBBV_PREFS_UNSUPPORTED_VERSION, PSBBV_PREFS_INVALID_VALUE } psbbv_preferences_result;
psbbv_preferences_result psbbv_preferences_encode(const psbbv_preferences *preferences, uint8_t *output, uint32_t output_size);
psbbv_preferences_result psbbv_preferences_decode(const uint8_t *input, uint32_t input_size, psbbv_preferences *preferences);
void psbbv_preferences_defaults(psbbv_preferences *preferences);
psbbv_preferences_result psbbv_preferences_validate(const psbbv_preferences *preferences);
#ifdef __cplusplus
}
#endif
#endif
