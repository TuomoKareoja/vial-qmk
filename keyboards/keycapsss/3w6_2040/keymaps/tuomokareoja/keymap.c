// Copyright 2021 weteor | 2022 Conor Burns (@Conor-Burns)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _NAV,
    _SYM,
    _NUM,
    _MOUSE,
    _FUNC,
    _MEDIA,
    _GAME,
    _GAMESWAP,
};

enum custom_keycodes {
    M_COPY = SAFE_RANGE,
    M_PASTE,
    M_CUT,
    M_UNDO,
    M_BSPC_LINE,
    M_BSPC_WORD,
    M_DEL_LINE,
    M_DEL_WORD,
    M_CLOSE_WINDOW,
};

// Define macros for keycodes and other overrides
uint8_t mod_state;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    mod_state = get_mods();
    switch (keycode) {
        case M_COPY:
            if (record->event.pressed) {
                SEND_STRING(SS_RCTL("c"));
            }
            break;
        case M_PASTE:
            if (record->event.pressed) {
                SEND_STRING(SS_RCTL("v"));
            }
            break;
        case M_CUT:
            if (record->event.pressed) {
                SEND_STRING(SS_RCTL("x"));
            }
            break;
        case M_UNDO:
            if (record->event.pressed) {
                SEND_STRING(SS_RCTL("z"));
            }
            break;
        case M_BSPC_LINE:
            if (record->event.pressed) {
                register_code(KC_RSFT);
                register_code(KC_HOME);
                register_code(KC_BSPC);
            } else {
                unregister_code(KC_BSPC);
                unregister_code(KC_HOME);
                unregister_code(KC_RSFT);
            }
            break;
        case M_BSPC_WORD:
            if (record->event.pressed) {
                SEND_STRING(SS_RCTL(SS_TAP(X_BSPC)));
            }
            break;
        case M_DEL_LINE:
            if (record->event.pressed) {
                register_code(KC_RSFT);
                register_code(KC_END);
                register_code(KC_DEL);
            } else {
                unregister_code(KC_DEL);
                unregister_code(KC_END);
                unregister_code(KC_RSFT);
            }
            break;
        case M_DEL_WORD:
            if (record->event.pressed) {
                SEND_STRING(SS_RCTL(SS_TAP(X_DEL)));
            }
            break;
        case M_CLOSE_WINDOW:
            if (record->event.pressed) {
                SEND_STRING(SS_RALT(SS_TAP(X_F4)));
            }
            break;
        // modify the behaviour of the l_gui key to put out the right tap code
        case RGUI_T(KC_COLN):
            if (record->event.pressed) {
                if (mod_state && MOD_MASK_SHIFT) {
                    // temporarily remove the shift modifier
                    del_mods(MOD_MASK_SHIFT);
                    tap_code16(KC_SCLN);
                    // turn the mods back on
                    set_mods(mod_state);
                    return false;
                }
                if (record->tap.count) {
                    tap_code16(KC_COLN);
                    return false;
                }
            }
            break;
    }
    return true;
};

// Define tapping terms per key
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // the thumb keys have infinite tapping term meaning that there
        // is no way to trigger the key without pressing another
        // as we have set then to
        // without triggering another key trough retro tap
        case LT(_SYM, KC_SPC):
            return -1;
        case RSFT_T(KC_BSPC):
            return -1;
        case LT(_NUM, KC_ENT):
            return -1;
        case LT(_NAV, KC_TAB):
            return -1;
        default:
            return TAPPING_TERM;
    }
};

// Define hold on other key press
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // only the thumb layer keys have hold on to other key press enabled
        case LT(_SYM, KC_SPC):
            return true;
        case RSFT_T(KC_BSPC):
            return true;
        case LT(_NUM, KC_ENT):
            return true;
        case LT(_NAV, KC_TAB):
            return true;
        default:
            return false;
    }
};

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // the only keys where we allow quick tap are the homerow mod tap keys
        // on the right because they might be kept pressed in VIM
        case RCTL_T(KC_J):
            return QUICK_TAP_TERM;
        case RSFT_T(KC_K):
            return QUICK_TAP_TERM;
        case RALT_T(KC_L):
            return QUICK_TAP_TERM;
        default:
            return 0;
    }
};

// Define combos
const uint16_t PROGMEM game_layer[] = {KC_Q, KC_Z, KC_P, KC_SLSH, COMBO_END};
const uint16_t PROGMEM base_layer[] = {KC_ESC, KC_TAB, KC_P, KC_SLSH, COMBO_END};
combo_t key_combos[] = {
    COMBO(game_layer, DF(_GAME)),
    COMBO(base_layer, DF(_BASE)),
};

// define overrides
const key_override_t space_underscore = ko_make_basic(MOD_MASK_SHIFT, LT(_SYM, KC_SPC), KC_UNDS);
const key_override_t **key_overrides = (const key_override_t *[]){
	&space_underscore,
	NULL // Null terminate the array of overrides!
};

// LOGIC FOR IGNORING MOD-TAP KEYS WHEN TYPING

// Decision macro for mod-tap keys to override
#define IS_LEFT_HOMEROW_MOD_TAP(kc) ( \
    IS_QK_MOD_TAP(kc) && \
    ( \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_A || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_S || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_D || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_F || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_1 || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_2 || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_3 || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_4 \
    ) \
)

#define IS_RIGHT_HOMEROW_MOD_TAP(kc) ( \
    IS_QK_MOD_TAP(kc) && \
    ( \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_J || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_K || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_L || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_COLN || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_7 || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_8 || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_9 || \
        QK_MOD_TAP_GET_TAP_KEYCODE(kc) == KC_0 \
    ) \
)

// Decision macro for preceding trigger key and typing interval
#define IS_TYPING_LEFT(k) ( \
    (last_input_activity_elapsed() < PRIOR_IDLE) && \
    ( \
        (uint8_t)(k) == KC_Q || \
        (uint8_t)(k) == KC_W || \
        (uint8_t)(k) == KC_E || \
        (uint8_t)(k) == KC_R || \
        (uint8_t)(k) == KC_T || \
        (uint8_t)(k) == KC_Y || \
        (uint8_t)(k) == KC_U || \
        (uint8_t)(k) == KC_I || \
        (uint8_t)(k) == KC_O || \
        (uint8_t)(k) == KC_P || \
        (uint8_t)(k) == KC_G || \
        (uint8_t)(k) == KC_H || \
        (uint8_t)(k) == KC_J || \
        (uint8_t)(k) == KC_K || \
        (uint8_t)(k) == KC_L || \
        (uint8_t)(k) == KC_COLN || \
        (uint8_t)(k) == KC_Z || \
        (uint8_t)(k) == KC_X || \
        (uint8_t)(k) == KC_C || \
        (uint8_t)(k) == KC_V || \
        (uint8_t)(k) == KC_B || \
        (uint8_t)(k) == KC_N || \
        (uint8_t)(k) == KC_M || \
        (uint8_t)(k) == KC_COMM || \
        (uint8_t)(k) == KC_DOT || \
        (uint8_t)(k) == KC_SLSH || \
        (uint8_t)(k) == KC_5 || \
        (uint8_t)(k) == KC_6 || \
        (uint8_t)(k) == KC_7 || \
        (uint8_t)(k) == KC_8 || \
        (uint8_t)(k) == KC_9 || \
        (uint8_t)(k) == KC_0 \
    ) \
)

// Decision macro for preceding trigger key and typing interval
#define IS_TYPING_RIGHT(k) ( \
    (last_input_activity_elapsed() < PRIOR_IDLE) && \
    ( \
        (uint8_t)(k) == KC_Q || \
        (uint8_t)(k) == KC_W || \
        (uint8_t)(k) == KC_E || \
        (uint8_t)(k) == KC_R || \
        (uint8_t)(k) == KC_T || \
        (uint8_t)(k) == KC_Y || \
        (uint8_t)(k) == KC_U || \
        (uint8_t)(k) == KC_I || \
        (uint8_t)(k) == KC_O || \
        (uint8_t)(k) == KC_P || \
        (uint8_t)(k) == KC_A || \
        (uint8_t)(k) == KC_S || \
        (uint8_t)(k) == KC_D || \
        (uint8_t)(k) == KC_F || \
        (uint8_t)(k) == KC_G || \
        (uint8_t)(k) == KC_H || \
        (uint8_t)(k) == KC_Z || \
        (uint8_t)(k) == KC_X || \
        (uint8_t)(k) == KC_C || \
        (uint8_t)(k) == KC_V || \
        (uint8_t)(k) == KC_B || \
        (uint8_t)(k) == KC_N || \
        (uint8_t)(k) == KC_M || \
        (uint8_t)(k) == KC_COMM || \
        (uint8_t)(k) == KC_DOT || \
        (uint8_t)(k) == KC_SLSH || \
        (uint8_t)(k) == KC_1 || \
        (uint8_t)(k) == KC_2 || \
        (uint8_t)(k) == KC_3 || \
        (uint8_t)(k) == KC_4 || \
        (uint8_t)(k) == KC_5 || \
        (uint8_t)(k) == KC_6 \
    ) \
)

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool     is_pressed[UINT8_MAX];
    static uint16_t prev_keycode;
    const  uint8_t  tap_keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);

    if (record->event.pressed) {
        // Press the tap keycode if the tap-hold key follows the previous key swiftly
        if (IS_LEFT_HOMEROW_MOD_TAP(keycode) && IS_TYPING_LEFT(prev_keycode)) {
            is_pressed[tap_keycode] = true;
            record->keycode = tap_keycode;
        }
        if (IS_RIGHT_HOMEROW_MOD_TAP(keycode) && IS_TYPING_RIGHT(prev_keycode)) {
            is_pressed[tap_keycode] = true;
            record->keycode = tap_keycode;
        }
        // Cache the keycode for subsequent tap decision
        prev_keycode = keycode;
    }

    // Release the tap keycode if pressed
    else if (is_pressed[tap_keycode]) {
        is_pressed[tap_keycode] = false;
        record->keycode = tap_keycode;
    }

    return true;

};

uint8_t is_base_layer_active = 1;

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(152, 232, 200);
};

//update when default layer changes
layer_state_t default_layer_state_set_user(layer_state_t state) {
    switch(biton32(state)) {
        case _BASE:
            rgblight_sethsv_noeeprom(152, 232, 200);
            break;
        default:
            rgblight_sethsv_noeeprom(0, 255, 200);
            break;
    };
    return state;
};

layer_state_t layer_state_set_user(layer_state_t state) {
    // if nav and num layers are active, activate the mouse layer
    state = update_tri_layer_state(state, _NAV, _NUM, _MOUSE);
    return state;
};

// Define keymaps
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // clang-format off

    [_BASE] = LAYOUT_split_3x5_3(

        KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,          KC_Y,    KC_U,           KC_I,           KC_O,           KC_P,
        LGUI_T(KC_A),   RALT_T(KC_S),   LSFT_T(KC_D),   RCTL_T(KC_F),   KC_G,          KC_H,    RCTL_T(KC_J),   RSFT_T(KC_K),   RALT_T(KC_L),   RGUI_T(KC_COLN),
        KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,          KC_N,    KC_M,           KC_COMM,        KC_DOT,         KC_SLSH,

        LT(_MEDIA, KC_ESC), LT(_SYM, KC_SPC), LT(_NAV, KC_TAB),                     LT(_NUM, KC_ENT), RSFT_T(KC_BSPC), LT(_FUNC, KC_DEL)

    ),

    [_NAV] = LAYOUT_split_3x5_3(

        XXXXXXX, M_BSPC_LINE, M_BSPC_WORD, M_DEL_WORD, M_DEL_LINE,          KC_PSCR, KC_HOME,  KC_END,  XXXXXXX,  M_CLOSE_WINDOW,
        KC_LGUI, KC_RALT,     KC_LSFT,     KC_RCTL,    XXXXXXX,             KC_LEFT, KC_DOWN,  KC_UP,   KC_RGHT,  KC_APP,
        M_UNDO,  M_CUT,       M_COPY,      M_PASTE,    XXXXXXX,             XXXXXXX, KC_PGDN,  KC_PGUP, XXXXXXX,  XXXXXXX,

        _______, _______,     _______,                                      _______,  KC_BSPC,  KC_DEL

    ),

    [_SYM] = LAYOUT_split_3x5_3(

        XXXXXXX,  KC_AT,   KC_HASH, KC_ASTR,  KC_PIPE,        KC_AMPR,  KC_CIRC,     KC_DLR,  KC_PERC, XXXXXXX,
        KC_GRAVE, KC_LBRC, KC_LPRN, KC_RPRN,  KC_RBRC,        KC_TILD,  KC_RCBR,     KC_LCBR, KC_DQUO, KC_QUOT,
        XXXXXXX,  KC_BSLS, KC_PLUS, KC_MINUS, XXXXXXX,        XXXXXXX,  KC_EQUAL,    KC_LABK, KC_RABK, KC_EXLM,

        _______,  _______, _______,                           KC_ENT,   M_BSPC_WORD, KC_DEL

    ),

    [_NUM] = LAYOUT_split_3x5_3(

        XXXXXXX,      KC_LPRN,      KC_RPRN,      KC_ASTR,      KC_RPRN,        XXXXXXX, KC_CIRC,      KC_COLN,      KC_PERC,      XXXXXXX,
        LGUI_T(KC_1), RALT_T(KC_2), LSFT_T(KC_3), RCTL_T(KC_4), KC_5,           KC_6,    RCTL_T(KC_7), RSFT_T(KC_8), RALT_T(KC_9), RGUI_T(KC_0),
        XXXXXXX,      KC_BSLS,      KC_PLUS,      KC_MINUS,     XXXXXXX,        XXXXXXX, KC_EQUAL,     KC_COMM,      KC_DOT,       KC_SLSH,

        KC_ESC,       KC_SPC,       _______,                                    _______, _______,      _______

    ),

    [_MOUSE] = LAYOUT_split_3x5_3(

        XXXXXXX, KC_ACL2, KC_ACL1, KC_ACL0, XXXXXXX,        XXXXXXX, KC_WH_L, KC_MS_U, KC_WH_R, XXXXXXX,
        KC_RCTL, KC_BTN3, KC_BTN2, KC_BTN1, XXXXXXX,        XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,
        M_UNDO,  M_CUT,   M_COPY,  M_PASTE, XXXXXXX,        XXXXXXX, KC_WH_D, KC_WH_U, XXXXXXX, XXXXXXX,

        _______, _______, _______,                          _______, _______, _______

    ),

    [_FUNC] = LAYOUT_split_3x5_3(

        KC_F10,  KC_F7,   KC_F8,   KC_F9, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_F11,  KC_F4,   KC_F5,   KC_F6, XXXXXXX,      XXXXXXX, KC_RCTL, KC_RSFT, KC_RALT, KC_RGUI,
        KC_F12,  KC_F1,   KC_2,    KC_3,  XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

        KC_ESC,  KC_SPC,  KC_TAB,                       _______, _______, _______

    ),

    [_MEDIA] = LAYOUT_split_3x5_3(

        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LGUI, KC_RALT, KC_LSFT, KC_RCTL, XXXXXXX,        KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MUTE,
        M_UNDO,  M_CUT,   M_COPY,  M_PASTE, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

        _______, _______, _______,                          _______, KC_MPLY, _______

    ),

    [_GAME] = LAYOUT_split_3x5_3(

        KC_ESC,  KC_Q,   KC_W,    KC_E, KC_R,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_LSFT, KC_A,   KC_S,    KC_D, KC_F,        KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MUTE,
        KC_TAB,  KC_Z,   KC_X,    KC_C, KC_V,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,

        OSL(_GAMESWAP), KC_SPC, KC_RCTL,             XXXXXXX, KC_MPLY, XXXXXXX

    ),

    [_GAMESWAP] = LAYOUT_split_3x5_3(

        KC_F9,   KC_1, KC_2, KC_3, KC_F5,            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_4, KC_5, KC_6, KC_0,             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_7, KC_8, KC_9, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

        _______, _______, _______,                   XXXXXXX, XXXXXXX, XXXXXXX

    ),

    // clang-format on
};
