#include <Arduino.h>
#include "userUsbHidKeyboardMouse/USBHIDKeyboardMouse.h"
#include "led.h"
#include "keyboard.h"
#include "dataflash.h"
#include "util.h"
#include "../configuration.h"

static compact_profile_t active_profile_s;
static uint8_t active_profile_idx_s = 0;
static uint8_t menu_mode_s = 0;
static bool in_menu_s = false;

static void set_menu_led(void) {
  int led_index = menu_mode_s % 3;
  int color_background = NEO_CYAN + (menu_mode_s / 3) * 32;

  led_set_mode(LED_FIX);
  led_set_color_hue((led_index == 0) ? NEO_RED : color_background,
                    (led_index == 1) ? NEO_RED : color_background,
                    (led_index == 2) ? NEO_RED : color_background);
}

void keyboard_press_enc(keyboard_button_keyboard_mode_t mode) {
  if (mode == BTM_PRESS) {
    in_menu_s = true;
    menu_mode_s = active_profile_idx_s;
    set_menu_led();
  } else if (mode == BTM_RELEASE) {
    in_menu_s = false;
    keyboard_set_active_profile(menu_mode_s);
    led_set_mode(LED_LOOP);
  }
}

void button_menu_up(keyboard_button_keyboard_mode_t mode) {
  if (mode == BTM_CLICK) {
    if (menu_mode_s >= NUM_PROFILES - 1) {
      menu_mode_s = NUM_PROFILES - 1;
    } else {
      menu_mode_s++;
    }
    set_menu_led();
  }
}

void button_menu_down(keyboard_button_keyboard_mode_t mode) {
  if (mode == BTM_CLICK) {
    if (menu_mode_s <= 0) {
      menu_mode_s = 0;
    } else {
      menu_mode_s--;
    }
    set_menu_led();
  }
}

static uint16_t get_consumer_code(uint8_t code) {
  switch (code) {
    case MEDIA_VOL_UP:      return 0x00E9;
    case MEDIA_VOL_DOWN:    return 0x00EA;
    case MEDIA_MUTE:        return 0x00E2;
    case MEDIA_PLAY_PAUSE:  return 0x00CD;
    case MEDIA_NEXT:        return 0x00B5;
    case MEDIA_PREV:        return 0x00B6;
    case MEDIA_BRIGHT_UP:   return 0x006F;
    case MEDIA_BRIGHT_DOWN: return 0x0070;
    default:                return 0;
  }
}

static void execute_key_combo(const compact_action_t *action, keyboard_button_keyboard_mode_t mode) {
  if (mode == BTM_RELEASE) {
    Keyboard_releaseAll();
    return;
  }

  uint8_t mod = action->param1;
  if (mod & MOD_LEFT_CTRL)   Keyboard_press(KEY_LEFT_CTRL);
  if (mod & MOD_LEFT_SHIFT)  Keyboard_press(KEY_LEFT_SHIFT);
  if (mod & MOD_LEFT_ALT)    Keyboard_press(KEY_LEFT_ALT);
  if (mod & MOD_LEFT_GUI)    Keyboard_press(KEY_LEFT_GUI);
  if (mod & MOD_RIGHT_CTRL)  Keyboard_press(KEY_RIGHT_CTRL);
  if (mod & MOD_RIGHT_SHIFT) Keyboard_press(KEY_RIGHT_SHIFT);
  if (mod & MOD_RIGHT_ALT)   Keyboard_press(KEY_RIGHT_ALT);
  if (mod & MOD_RIGHT_GUI)   Keyboard_press(KEY_RIGHT_GUI);

  if (action->param2 != 0) {
    Keyboard_press(action->param2);
  }
  if (action->param3 != 0) {
    Keyboard_press(action->param3);
  }

  if (mode == BTM_CLICK) {
    delay(15);
    Keyboard_releaseAll();
  }
}

static void execute_mouse(const compact_action_t *action, keyboard_button_keyboard_mode_t mode) {
  if (action->param3 != 0) {
    Keyboard_press(action->param3);
    delay(10);
  }

  uint8_t mouse_act = action->param1;
  switch (mouse_act) {
    case MOUSE_ACT_LEFT_CLICK:
      if (mode == BTM_PRESS) Mouse_press(MOUSE_LEFT);
      else if (mode == BTM_RELEASE) Mouse_release(MOUSE_LEFT);
      else Mouse_click(MOUSE_LEFT);
      break;
    case MOUSE_ACT_RIGHT_CLICK:
      if (mode == BTM_PRESS) Mouse_press(MOUSE_RIGHT);
      else if (mode == BTM_RELEASE) Mouse_release(MOUSE_RIGHT);
      else Mouse_click(MOUSE_RIGHT);
      break;
    case MOUSE_ACT_MIDDLE_CLICK:
      if (mode == BTM_PRESS) Mouse_press(MOUSE_MIDDLE);
      else if (mode == BTM_RELEASE) Mouse_release(MOUSE_MIDDLE);
      else Mouse_click(MOUSE_MIDDLE);
      break;
    case MOUSE_ACT_SCROLL_UP: {
      int8_t steps = (action->param2 > 0) ? (int8_t)action->param2 : 1;
      Mouse_scroll(steps);
      break;
    }
    case MOUSE_ACT_SCROLL_DOWN: {
      int8_t steps = (action->param2 > 0) ? (int8_t)action->param2 : 1;
      Mouse_scroll(-steps);
      break;
    }
    default:
      break;
  }

  if (action->param3 != 0) {
    Keyboard_releaseAll();
  }
}

static void execute_action(const compact_action_t *action, keyboard_button_keyboard_mode_t mode) {
  switch (action->type) {
    case ACT_KEY_COMBO:
      execute_key_combo(action, mode);
      break;
    case ACT_CONSUMER:
      if (mode == BTM_PRESS || mode == BTM_CLICK) {
        uint16_t c = get_consumer_code(action->param1);
        if (c != 0) {
          Consumer_click(c);
        }
      }
      break;
    case ACT_MOUSE:
      execute_mouse(action, mode);
      break;
    case ACT_SPECIAL:
      switch (action->param1) {
        case SPECIAL_MENU:
          keyboard_press_enc(mode);
          break;
        case SPECIAL_NEXT_PROFILE:
          if (mode == BTM_PRESS || mode == BTM_CLICK) {
            uint8_t next = (active_profile_idx_s + 1) % NUM_PROFILES;
            keyboard_set_active_profile(next);
          }
          break;
        case SPECIAL_PREV_PROFILE:
          if (mode == BTM_PRESS || mode == BTM_CLICK) {
            uint8_t prev = (active_profile_idx_s == 0) ? (NUM_PROFILES - 1) : (active_profile_idx_s - 1);
            keyboard_set_active_profile(prev);
          }
          break;
        case SPECIAL_BOOTLOADER:
          BOOT_now();
          break;
        case SPECIAL_SET_PROFILE:
          if (mode == BTM_PRESS || mode == BTM_CLICK) {
            keyboard_set_active_profile(action->param2 % NUM_PROFILES);
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

void keyboard_press_button(keyboard_button_t button, keyboard_button_keyboard_mode_t mode) {
  // LED feedback on 3 mechanical buttons
  if (button >= BTN_1 && button <= BTN_3) {
    if (mode == BTM_PRESS) {
      led_presskey(button);
    } else {
      led_presskey(-1);
    }
  }

  // Handle menu navigation if encoder is held
  if (in_menu_s) {
    if (button == ENC_CW) {
      button_menu_up(mode);
      return;
    } else if (button == ENC_CCW) {
      button_menu_down(mode);
      return;
    } else if (button == BTN_ENC) {
      keyboard_press_enc(mode);
      return;
    }
  }

  if (button < ACTIONS_PER_PROFILE) {
    execute_action(&active_profile_s.button[button], mode);
  }
}

uint8_t keyboard_get_active_profile(void) {
  return active_profile_idx_s;
}

void keyboard_set_active_profile(uint8_t profile_idx) {
  if (profile_idx >= NUM_PROFILES) {
    profile_idx = 0;
  }
  active_profile_idx_s = profile_idx;
  dataflash_read(DF_OFFSET_PROFILES + profile_idx * sizeof(compact_profile_t),
                 (uint8_t *)&active_profile_s, sizeof(compact_profile_t));
  dataflash_write_byte(DF_OFFSET_ACTIVE_PROF, active_profile_idx_s);
}

compact_profile_t *keyboard_get_profile(uint8_t profile_idx) {
  static compact_profile_t temp_prof;
  if (profile_idx >= NUM_PROFILES) {
    profile_idx = 0;
  }
  dataflash_read(DF_OFFSET_PROFILES + profile_idx * sizeof(compact_profile_t),
                 (uint8_t *)&temp_prof, sizeof(compact_profile_t));
  return &temp_prof;
}

void keyboard_update_profile(uint8_t profile_idx, const compact_profile_t *new_prof) {
  if (profile_idx >= NUM_PROFILES || new_prof == 0) {
    return;
  }
  dataflash_write(DF_OFFSET_PROFILES + profile_idx * sizeof(compact_profile_t),
                  (const uint8_t *)new_prof, sizeof(compact_profile_t));
  if (profile_idx == active_profile_idx_s) {
    active_profile_s = *new_prof;
  }
}

void keyboard_load_factory_defaults(void) {
  dataflash_write_byte(DF_OFFSET_MAGIC0, DATAFLASH_MAGIC_0);
  dataflash_write_byte(DF_OFFSET_MAGIC1, DATAFLASH_MAGIC_1);
  dataflash_write_byte(DF_OFFSET_VERSION, DATAFLASH_VERSION);
  dataflash_write_byte(DF_OFFSET_ACTIVE_PROF, 0);
  dataflash_write_byte(DF_OFFSET_LED_MODE, 0);
  dataflash_write_byte(DF_OFFSET_CHECKSUM, 0);

  for (uint8_t i = 0; i < NUM_PROFILES; i++) {
    dataflash_write(DF_OFFSET_PROFILES + i * sizeof(compact_profile_t),
                    (const uint8_t *)&default_profiles[i], sizeof(compact_profile_t));
  }

  active_profile_idx_s = 0;
  active_profile_s = default_profiles[0];
}

void keyboard_save_to_dataflash(void) {
  dataflash_write(DF_OFFSET_PROFILES + active_profile_idx_s * sizeof(compact_profile_t),
                  (const uint8_t *)&active_profile_s, sizeof(compact_profile_t));
  dataflash_write_byte(DF_OFFSET_ACTIVE_PROF, active_profile_idx_s);
}

void keyboard_setup(void) {
  if (!dataflash_is_valid()) {
    keyboard_load_factory_defaults();
  } else {
    active_profile_idx_s = dataflash_read_byte(DF_OFFSET_ACTIVE_PROF);
    if (active_profile_idx_s >= NUM_PROFILES) {
      active_profile_idx_s = 0;
    }
    dataflash_read(DF_OFFSET_PROFILES + active_profile_idx_s * sizeof(compact_profile_t),
                   (uint8_t *)&active_profile_s, sizeof(compact_profile_t));
  }
}