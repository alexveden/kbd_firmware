#include QMK_KEYBOARD_H
#include "print.h"

enum custom_keycodes
{
    STICKY_SHIFT = QK_KB_0,
};

static bool caps_word_active = false;
static uint16_t sticky_timer = 0;

//
// sticky shift/caps word implementation
// if STICKY_SHIFT tapped - next key is upper
// if STICKY_SHIFT is held - acts like an original shift
// if STICKY_SHIFT is double tapped - acts like a caps word
//
bool
process_record_user(uint16_t keycode, keyrecord_t* record)
{
    switch (keycode) {
        case STICKY_SHIFT:
            if (record->event.pressed) {
                uint16_t current_time = timer_read();

                // Register the shift mod immediately for normal hold behavior
                register_code(KC_LSFT);

                if (caps_word_active) {
                    // caps word enabled, turn off when STICKY_SHIFT is pressed again
                    caps_word_active = false;
                    sticky_timer = 0;
                } else {
                    // Check for double tap (for caps word)
                    if (timer_elapsed(sticky_timer) < STICKY_SHIFT_DTAP_TERM) {
                        caps_word_active = true;
                    }
                    sticky_timer = current_time;
                }
            } else {
                // Check if this was a quick tap for sticky behavior
                unregister_code(KC_LSFT);

                if (!caps_word_active) {
                    if (sticky_timer && timer_elapsed(sticky_timer) < STICKY_SHIFT_DTAP_TERM) {
                        // Quick tap - activate one-shot shift
                        add_oneshot_mods(MOD_LSFT);
                        sticky_timer = timer_read();
                    }
                }
            }
            return false;
        default: {
            if (!caps_word_active) return true;

            if (record->event.pressed && keycode < QK_BASIC_MAX) {
                if ((keycode >= KC_A && keycode <= KC_Z) || keycode == KC_MINUS ||
                    keycode == KC_BACKSPACE || keycode == KC_DEL || keycode == KC_LEFT ||
                    keycode == KC_RIGHT) {
                    sticky_timer = timer_read();
                    add_oneshot_mods(MOD_LSFT);
                } else if ((keycode >= KC_1 && keycode <= KC_0)) {
                    sticky_timer = timer_read();
                    // numbers are typed without shifting
                    del_oneshot_mods(MOD_LSFT);
                } else {
                    // Non-letter key pressed - deactivate caps word
                    caps_word_active = false;
                    sticky_timer = 0;
                    unregister_code(KC_LSFT);
                    del_oneshot_mods(MOD_LSFT);
                }
            }
        }
    }
    return true;
}

void
matrix_scan_user(void)
{
    // Handle caps word / sticky shift timeout
    if (caps_word_active && sticky_timer && timer_elapsed(sticky_timer) > CAPS_WORD_IDLE_TIMEOUT) {
        caps_word_active = false;
        sticky_timer = 0;
        del_oneshot_mods(MOD_LSFT);
        unregister_code(KC_LSFT);
    }
}


#ifdef LAYOUT_split_3x6_3_ex2

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_split_3x6_3_ex2(
    'L',
    'L',
    'L',
    'L',
    'L',
    'L',
    'L',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'L',
    'L',
    'L',
    'L',
    'L',
    'L',
    'L',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'L',
    'L',
    'L',
    'L',
    'L',
    'L',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'L',
    'L',
    'L',
    'R',
    'R',
    'R'
);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3_ex2(
        //,--------------------------------------------------------------.
        //,--------------------------------------------------------------.
        KC_TAB,
        KC_Q,
        KC_W,
        KC_E,
        KC_R,
        KC_T,
        QK_BOOTLOADER,
        QK_BOOTLOADER,
        KC_Y,
        KC_U,
        KC_I,
        KC_O,
        KC_P,
        KC_BSPC,
        //|--------+--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------+--------|
        STICKY_SHIFT,
        KC_A,
        KC_S,
        KC_D,
        KC_F,
        KC_G,
        KC_LALT,
        KC_RALT,
        KC_H,
        KC_J,
        KC_K,
        KC_L,
        KC_SCLN,
        KC_QUOT,
        //|--------+--------+--------+--------+--------+--------+--------'
        //`--------+--------+--------+--------+--------+--------+--------|
        KC_LSFT,
        KC_Z,
        KC_X,
        KC_C,
        KC_V,
        KC_B,
        KC_N,
        KC_M,
        KC_COMM,
        KC_DOT,
        KC_SLSH,
        KC_RSFT,
        //|--------+--------+--------+--------+--------+--------+--------.
        //,--------+--------+--------+--------+--------+--------+--------|
        KC_LGUI,
        TL_LOWR,
        KC_SPC,
        KC_ENT,
        TL_UPPR,
        KC_RGUI
        //`--------------------------'  `--------------------------'

    ),

    [1] = LAYOUT_split_3x6_3_ex2(
        //,--------------------------------------------------------------.
        //,--------------------------------------------------------------.
        KC_TAB,
        KC_1,
        KC_2,
        KC_3,
        KC_4,
        KC_5,
        KC_LCTL,
        KC_RCTL,
        KC_6,
        KC_7,
        KC_8,
        KC_9,
        KC_0,
        KC_BSPC,
        //|--------+--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------+--------|
        KC_LCTL,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        KC_LALT,
        KC_RALT,
        KC_LEFT,
        KC_DOWN,
        KC_UP,
        KC_RIGHT,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------'
        //`--------+--------+--------+--------+--------+--------+--------|
        KC_LSFT,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------.
        //,--------+--------+--------+--------+--------+--------+--------|
        KC_LGUI,
        _______,
        KC_SPC,
        KC_ENT,
        _______,
        KC_RGUI
        //`--------------------------'  `--------------------------'
    ),

    [2] = LAYOUT_split_3x6_3_ex2(
        //,--------------------------------------------------------------.
        //,--------------------------------------------------------------.
        KC_TAB,
        KC_EXLM,
        KC_AT,
        KC_HASH,
        KC_DLR,
        KC_PERC,
        KC_LCTL,
        KC_RCTL,
        KC_CIRC,
        KC_AMPR,
        KC_ASTR,
        KC_LPRN,
        KC_RPRN,
        KC_BSPC,
        //|--------+--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------+--------|
        KC_LCTL,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        KC_LALT,
        KC_RALT,
        KC_MINS,
        KC_EQL,
        KC_LBRC,
        KC_RBRC,
        KC_BSLS,
        KC_GRV,
        //|--------+--------+--------+--------+--------+--------+--------'
        //`--------+--------+--------+--------+--------+--------+--------|
        KC_LSFT,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        KC_UNDS,
        KC_PLUS,
        KC_LCBR,
        KC_RCBR,
        KC_PIPE,
        KC_TILD,
        //|--------+--------+--------+--------+--------+--------+--------.
        //,--------+--------+--------+--------+--------+--------+--------|
        KC_LGUI,
        _______,
        KC_SPC,
        KC_ENT,
        _______,
        KC_RGUI
        //`--------------------------'  `--------------------------'
    ),

    [3] = LAYOUT_split_3x6_3_ex2(
        //,--------------------------------------------------------------.
        //,--------------------------------------------------------------.
        QK_BOOT,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------+--------|
        RGB_TOG,
        RGB_HUI,
        RGB_SAI,
        RGB_VAI,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------'
        //`--------+--------+--------+--------+--------+--------+--------|
        RGB_MOD,
        RGB_HUD,
        RGB_SAD,
        RGB_VAD,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------.
        //,--------+--------+--------+--------+--------+--------+--------|
        KC_LGUI,
        _______,
        KC_SPC,
        KC_ENT,
        _______,
        KC_RGUI
        //`--------------------------'  `--------------------------'
    )
};
#else

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
        //,-----------------------------------------------------.
        //,-----------------------------------------------------.
        KC_TAB,
        KC_Q,
        KC_W,
        KC_E,
        KC_R,
        KC_T,
        KC_Y,
        KC_U,
        KC_I,
        KC_O,
        KC_P,
        KC_BSPC,
        //|--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------|
        KC_LCTL,
        KC_A,
        KC_S,
        KC_D,
        KC_F,
        KC_G,
        KC_H,
        KC_J,
        KC_K,
        KC_L,
        KC_SCLN,
        KC_QUOT,
        //|--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------|
        KC_LSFT,
        KC_Z,
        KC_X,
        KC_C,
        KC_V,
        KC_B,
        KC_N,
        KC_M,
        KC_COMM,
        KC_DOT,
        KC_SLSH,
        KC_ESC,
        //|--------+--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------+--------|
        KC_LGUI,
        TL_LOWR,
        KC_SPC,
        KC_ENT,
        TL_UPPR,
        KC_RALT
        //`--------------------------'  `--------------------------'

    ),

    [1] = LAYOUT_split_3x6_3(
        //,-----------------------------------------------------.
        //,-----------------------------------------------------.
        KC_TAB,
        KC_1,
        KC_2,
        KC_3,
        KC_4,
        KC_5,
        KC_6,
        KC_7,
        KC_8,
        KC_9,
        KC_0,
        KC_BSPC,
        //|--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------|
        KC_LCTL,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        KC_LEFT,
        KC_DOWN,
        KC_UP,
        KC_RIGHT,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------|
        KC_LSFT,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------+--------|
        KC_LGUI,
        _______,
        KC_SPC,
        KC_ENT,
        _______,
        KC_RALT
        //`--------------------------'  `--------------------------'
    ),

    [2] = LAYOUT_split_3x6_3(
        //,-----------------------------------------------------.
        //,-----------------------------------------------------.
        KC_TAB,
        KC_EXLM,
        KC_AT,
        KC_HASH,
        KC_DLR,
        KC_PERC,
        KC_CIRC,
        KC_AMPR,
        KC_ASTR,
        KC_LPRN,
        KC_RPRN,
        KC_BSPC,
        //|--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------|
        KC_LCTL,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        KC_MINS,
        KC_EQL,
        KC_LBRC,
        KC_RBRC,
        KC_BSLS,
        KC_GRV,
        //|--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------|
        KC_LSFT,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        KC_UNDS,
        KC_PLUS,
        KC_LCBR,
        KC_RCBR,
        KC_PIPE,
        KC_TILD,
        //|--------+--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------+--------|
        KC_LGUI,
        _______,
        KC_SPC,
        KC_ENT,
        _______,
        KC_RALT
        //`--------------------------'  `--------------------------'
    ),

    [3] = LAYOUT_split_3x6_3(
        //,-----------------------------------------------------.
        //,-----------------------------------------------------.
        QK_BOOT,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------|
        RGB_TOG,
        RGB_HUI,
        RGB_SAI,
        RGB_VAI,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------|
        RGB_MOD,
        RGB_HUD,
        RGB_SAD,
        RGB_VAD,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|
        //|--------+--------+--------+--------+--------+--------+--------|
        KC_LGUI,
        _______,
        KC_SPC,
        KC_ENT,
        _______,
        KC_RALT
        //`--------------------------'  `--------------------------'
    )
};
#endif

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [0] = { ENCODER_CCW_CW(RGB_MOD, RGB_RMOD), ENCODER_CCW_CW(RGB_HUI, RGB_HUD), ENCODER_CCW_CW(RGB_VAI, RGB_VAD), ENCODER_CCW_CW(RGB_SAI, RGB_SAD), },
  [1] = { ENCODER_CCW_CW(RGB_MOD, RGB_RMOD), ENCODER_CCW_CW(RGB_HUI, RGB_HUD), ENCODER_CCW_CW(RGB_VAI, RGB_VAD), ENCODER_CCW_CW(RGB_SAI, RGB_SAD), },
  [2] = { ENCODER_CCW_CW(RGB_MOD, RGB_RMOD), ENCODER_CCW_CW(RGB_HUI, RGB_HUD), ENCODER_CCW_CW(RGB_VAI, RGB_VAD), ENCODER_CCW_CW(RGB_SAI, RGB_SAD), },
  [3] = { ENCODER_CCW_CW(RGB_MOD, RGB_RMOD), ENCODER_CCW_CW(RGB_HUI, RGB_HUD), ENCODER_CCW_CW(RGB_VAI, RGB_VAD), ENCODER_CCW_CW(RGB_SAI, RGB_SAD), },
};
#endif
