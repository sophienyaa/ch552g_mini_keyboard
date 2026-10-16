#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdint.h>
#include <stdbool.h>

#define NUM_PROFILES 4
#define ACTIONS_PER_PROFILE 6

// Physical input identifiers
typedef enum {
  BTN_1 = 0,
  BTN_2 = 1,
  BTN_3 = 2,
  ENC_CW = 3,
  ENC_CCW = 4,
  BTN_ENC = 5,
  BTN_NUM = 6,
} keyboard_button_t;

typedef enum {
  BTM_PRESS,
  BTM_RELEASE,
  BTM_CLICK,
} keyboard_button_keyboard_mode_t;

// Action types
typedef enum {
  ACT_NONE = 0,
  ACT_KEY_COMBO = 1,
  ACT_CONSUMER = 2,
  ACT_MOUSE = 3,
  ACT_SPECIAL = 4,
} action_type_t;

// Modifiers bitmask for ACT_KEY_COMBO
#define MOD_NONE        0x00
#define MOD_LEFT_CTRL   0x01
#define MOD_LEFT_SHIFT  0x02
#define MOD_LEFT_ALT    0x04
#define MOD_LEFT_GUI    0x08
#define MOD_RIGHT_CTRL  0x10
#define MOD_RIGHT_SHIFT 0x20
#define MOD_RIGHT_ALT   0x40
#define MOD_RIGHT_GUI   0x80

// Consumer / Media control codes for ACT_CONSUMER
typedef enum {
  MEDIA_NONE = 0,
  MEDIA_VOL_UP = 1,
  MEDIA_VOL_DOWN = 2,
  MEDIA_MUTE = 3,
  MEDIA_PLAY_PAUSE = 4,
  MEDIA_NEXT = 5,
  MEDIA_PREV = 6,
  MEDIA_BRIGHT_UP = 7,
  MEDIA_BRIGHT_DOWN = 8,
} consumer_code_t;

// Mouse action types for ACT_MOUSE
typedef enum {
  MOUSE_ACT_NONE = 0,
  MOUSE_ACT_LEFT_CLICK = 1,
  MOUSE_ACT_RIGHT_CLICK = 2,
  MOUSE_ACT_MIDDLE_CLICK = 3,
  MOUSE_ACT_SCROLL_UP = 4,
  MOUSE_ACT_SCROLL_DOWN = 5,
} mouse_action_t;

// Special actions for ACT_SPECIAL
typedef enum {
  SPECIAL_NONE = 0,
  SPECIAL_MENU = 1,
  SPECIAL_NEXT_PROFILE = 2,
  SPECIAL_PREV_PROFILE = 3,
  SPECIAL_BOOTLOADER = 4,
  SPECIAL_SET_PROFILE = 5,
} special_action_t;

// Compact 4-byte action struct
typedef struct {
  uint8_t type;   // action_type_t
  uint8_t param1; // Modifier bitmask OR consumer_code_t OR mouse_action_t OR special_action_t
  uint8_t param2; // HID keycode OR scroll amount OR target profile
  uint8_t param3; // Secondary keycode / flags / modifier for mouse
} compact_action_t;

// 24-byte profile
typedef struct {
  compact_action_t button[ACTIONS_PER_PROFILE];
} compact_profile_t;

#ifdef __cplusplus
extern "C" {
#endif

void keyboard_setup(void);
void keyboard_press_button(keyboard_button_t button, keyboard_button_keyboard_mode_t mode);
void keyboard_press_enc(keyboard_button_keyboard_mode_t mode);
void button_menu_up(keyboard_button_keyboard_mode_t mode);
void button_menu_down(keyboard_button_keyboard_mode_t mode);

uint8_t keyboard_get_active_profile(void);
void keyboard_set_active_profile(uint8_t profile_idx);
compact_profile_t *keyboard_get_profile(uint8_t profile_idx);
void keyboard_update_profile(uint8_t profile_idx, const compact_profile_t *new_prof);
void keyboard_load_factory_defaults(void);
void keyboard_save_to_dataflash(void);

#ifdef __cplusplus
}
#endif

#endif // __KEYBOARD_H__
