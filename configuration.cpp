#include "configuration.h"
#include "src/userUsbHidKeyboardMouse/USBHIDKeyboardMouse.h"

const compact_profile_t default_profiles[NUM_PROFILES] = {
    // Profile 0: Productivity / Office (Copy, Paste, Undo, Scroll, Menu)
    {
        .button = {
            [BTN_1] = { .type = ACT_KEY_COMBO, .param1 = MOD_LEFT_CTRL, .param2 = 'c', .param3 = 0 },
            [BTN_2] = { .type = ACT_KEY_COMBO, .param1 = MOD_LEFT_CTRL, .param2 = 'v', .param3 = 0 },
            [BTN_3] = { .type = ACT_KEY_COMBO, .param1 = MOD_LEFT_CTRL, .param2 = 'z', .param3 = 0 },
            [ENC_CW] = { .type = ACT_MOUSE, .param1 = MOUSE_ACT_SCROLL_DOWN, .param2 = 2, .param3 = 0 },
            [ENC_CCW] = { .type = ACT_MOUSE, .param1 = MOUSE_ACT_SCROLL_UP, .param2 = 2, .param3 = 0 },
            [BTN_ENC] = { .type = ACT_SPECIAL, .param1 = SPECIAL_MENU, .param2 = 0, .param3 = 0 },
        }
    },
    // Profile 1: Media Control (Prev, Play/Pause, Next, Volume Down/Up, Menu)
    {
        .button = {
            [BTN_1] = { .type = ACT_CONSUMER, .param1 = MEDIA_PREV, .param2 = 0, .param3 = 0 },
            [BTN_2] = { .type = ACT_CONSUMER, .param1 = MEDIA_PLAY_PAUSE, .param2 = 0, .param3 = 0 },
            [BTN_3] = { .type = ACT_CONSUMER, .param1 = MEDIA_NEXT, .param2 = 0, .param3 = 0 },
            [ENC_CW] = { .type = ACT_CONSUMER, .param1 = MEDIA_VOL_UP, .param2 = 0, .param3 = 0 },
            [ENC_CCW] = { .type = ACT_CONSUMER, .param1 = MEDIA_VOL_DOWN, .param2 = 0, .param3 = 0 },
            [BTN_ENC] = { .type = ACT_SPECIAL, .param1 = SPECIAL_MENU, .param2 = 0, .param3 = 0 },
        }
    },
    // Profile 2: Creative / Photoshop (Eraser, Stamp, Alt+Click, Scroll/Zoom, Menu)
    {
        .button = {
            [BTN_1] = { .type = ACT_KEY_COMBO, .param1 = MOD_NONE, .param2 = 'e', .param3 = 0 },
            [BTN_2] = { .type = ACT_KEY_COMBO, .param1 = MOD_NONE, .param2 = 's', .param3 = 0 },
            [BTN_3] = { .type = ACT_MOUSE, .param1 = MOUSE_ACT_LEFT_CLICK, .param2 = 1, .param3 = MOD_LEFT_ALT },
            [ENC_CW] = { .type = ACT_MOUSE, .param1 = MOUSE_ACT_SCROLL_DOWN, .param2 = 2, .param3 = 0 },
            [ENC_CCW] = { .type = ACT_MOUSE, .param1 = MOUSE_ACT_SCROLL_UP, .param2 = 2, .param3 = 0 },
            [BTN_ENC] = { .type = ACT_SPECIAL, .param1 = SPECIAL_MENU, .param2 = 0, .param3 = 0 },
        }
    },
    // Profile 3: Browser Navigation (Prev Tab, Next Tab, Close Tab, Scroll, Menu)
    {
        .button = {
            [BTN_1] = { .type = ACT_KEY_COMBO, .param1 = (uint8_t)(MOD_LEFT_CTRL | MOD_LEFT_SHIFT), .param2 = KEY_TAB, .param3 = 0 },
            [BTN_2] = { .type = ACT_KEY_COMBO, .param1 = MOD_LEFT_CTRL, .param2 = KEY_TAB, .param3 = 0 },
            [BTN_3] = { .type = ACT_KEY_COMBO, .param1 = MOD_LEFT_CTRL, .param2 = 'w', .param3 = 0 },
            [ENC_CW] = { .type = ACT_MOUSE, .param1 = MOUSE_ACT_SCROLL_DOWN, .param2 = 2, .param3 = 0 },
            [ENC_CCW] = { .type = ACT_MOUSE, .param1 = MOUSE_ACT_SCROLL_UP, .param2 = 2, .param3 = 0 },
            [BTN_ENC] = { .type = ACT_SPECIAL, .param1 = SPECIAL_MENU, .param2 = 0, .param3 = 0 },
        }
    }
};