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
    SMTD_KEYCODES_BEGIN = SAFE_RANGE,
    CKC_A,
    CKC_S,
    CKC_D,
    CKC_F,
    CKC_J,
    CKC_K,
    CKC_L,
    CKC_COLN,
    CKC_1,
    CKC_2,
    CKC_3,
    CKC_4,
    CKC_7,
    CKC_8,
    CKC_9,
    CKC_0,
    SMTD_KEYCODES_END,
    M_COPY,
    M_PASTE,
    M_CUT,
    M_UNDO,
    M_BSPC_LINE,
    M_BSPC_WORD,
    M_DEL_LINE,
    M_DEL_WORD,
};

#include "sm_td.h"

void on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) {
    switch (keycode) {
        SMTD_MT(CKC_A, KC_A, KC_LEFT_GUI)
        SMTD_MT(CKC_S, KC_S, KC_RIGHT_ALT)
        SMTD_MT(CKC_D, KC_D, KC_RIGHT_CTRL)
        SMTD_MT(CKC_F, KC_F, KC_LSFT)
        SMTD_MT(CKC_J, KC_J, KC_RSFT)
        SMTD_MT(CKC_K, KC_K, KC_RIGHT_CTRL)
        SMTD_MT(CKC_L, KC_L, KC_RIGHT_ALT)
        SMTD_MT(CKC_COLN, KC_COLN, KC_RIGHT_GUI)
        SMTD_MT(CKC_1, KC_1, KC_LEFT_GUI)
        SMTD_MT(CKC_2, KC_2, KC_RIGHT_ALT)
        SMTD_MT(CKC_3, KC_3, KC_RIGHT_CTRL)
        SMTD_MT(CKC_4, KC_4, KC_LSFT)
        SMTD_MT(CKC_7, KC_7, KC_RSFT)
        SMTD_MT(CKC_8, KC_8, KC_RIGHT_CTRL)
        SMTD_MT(CKC_9, KC_9, KC_RIGHT_ALT)
        SMTD_MT(CKC_0, KC_0, KC_RIGHT_GUI)
    }
}

// Define macros for keycodes and other overrides
uint8_t mod_state;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_smtd(keycode, record)) {
        return false;
    }
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
        // modify the behaviour of the r_gui key to put out the right tap code
        case CKC_COLN:
            if (record->event.pressed) {
                if (mod_state && MOD_MASK_SHIFT) {
                    // temporarily remove the shift modifier
                    del_mods(MOD_MASK_SHIFT);
                    tap_code16(KC_SCLN);
                    // turn the mods back on
                    set_mods(mod_state);
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
        case LT(_NUM, KC_SPC):
            return -1;
        case LT(_SYM, KC_BSPC):
            return -1;
        case LT(_MOUSE, KC_ENT):
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
        case LT(_NUM, KC_SPC):
            return true;
        case LT(_SYM, KC_BSPC):
            return true;
        case LT(_MOUSE, KC_ENT):
            return true;
        case LT(_NAV, KC_TAB):
            return true;
        default:
            return false;
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
const key_override_t coln_unds = ko_make_basic(MOD_MASK_SHIFT, CKC_COLN, KC_UNDS);
const key_override_t **key_overrides = (const key_override_t *[]){
	&space_underscore,
    &coln_unds,
	NULL // Null terminate the array of overrides!
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
        CKC_A,          CKC_S,          CKC_D,          CKC_F,          KC_G,          KC_H,    CKC_J,          CKC_K,          CKC_L,          CKC_COLN,
        KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,          KC_N,    KC_M,           KC_COMM,        KC_DOT,         KC_SLSH,

        LT(_MEDIA, KC_ESC), LT(_NUM, KC_SPC), LT(_NAV, KC_TAB),                        LT(_MOUSE, KC_ENT), LT(_SYM, KC_BSPC), LT(_FUNC, KC_DEL)

    ),

    [_NAV] = LAYOUT_split_3x5_3(

        XXXXXXX, M_BSPC_LINE, M_BSPC_WORD, M_DEL_WORD, M_DEL_LINE,          KC_PSCR, KC_HOME,  KC_END,  XXXXXXX,  XXXXXXX,
        KC_LGUI, KC_RALT,     KC_LSFT,     KC_RCTL,    XXXXXXX,             KC_LEFT, KC_DOWN,  KC_UP,   KC_RGHT,  KC_APP,
        M_UNDO,  M_CUT,       M_COPY,      M_PASTE,    XXXXXXX,             XXXXXXX, KC_PGDN,  KC_PGUP, XXXXXXX,  XXXXXXX,

        _______, _______,     _______,                                      KC_ENT,  KC_BSPC,  KC_DEL

    ),

    [_SYM] = LAYOUT_split_3x5_3(

        XXXXXXX,  KC_AT,   KC_HASH, KC_ASTR,  KC_PIPE,        KC_AMPR,  KC_CIRC,  KC_DLR,  KC_PERC, XXXXXXX,
        KC_GRAVE, KC_LBRC, KC_LPRN, KC_RPRN,  KC_RBRC,        KC_TILD,  KC_LCBR,  KC_RCBR, KC_DQUO, KC_QUOT,
        XXXXXXX,  KC_BSLS, KC_PLUS, KC_MINUS, XXXXXXX,        XXXXXXX,  KC_EQUAL, KC_LABK, KC_RABK, KC_EXLM,

        _______,  KC_UNDS, _______,                           _______,  _______, _______

    ),

    [_NUM] = LAYOUT_split_3x5_3(

        XXXXXXX,      KC_LPRN,      KC_RPRN,      KC_ASTR,      KC_RPRN,        XXXXXXX, KC_CIRC,      KC_COLN,     KC_PERC,      XXXXXXX,
        CKC_1,        CKC_2,        CKC_3,        CKC_4,        KC_5,           KC_6,    CKC_7,        CKC_8,       CKC_9,        CKC_0,
        XXXXXXX,      KC_GRAVE,     KC_PLUS,      KC_MINUS,     XXXXXXX,        XXXXXXX, KC_EQUAL,     KC_COMM,     KC_DOT,       KC_SLSH,

        _______,      _______,      _______,                                    _______, M_BSPC_WORD,  _______

    ),

    [_MOUSE] = LAYOUT_split_3x5_3(

        XXXXXXX, KC_ACL2, KC_ACL1, KC_ACL0, XXXXXXX,        XXXXXXX, KC_WH_L, KC_MS_U, KC_WH_R, XXXXXXX,
        KC_BTN4, KC_BTN3, KC_BTN2, KC_BTN1, XXXXXXX,        XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,
        M_UNDO,  M_CUT,   M_COPY,  M_PASTE, XXXXXXX,        XXXXXXX, KC_WH_D, KC_WH_U, XXXXXXX, XXXXXXX,

        _______, _______, _______,                          _______, _______, _______

    ),

    [_FUNC] = LAYOUT_split_3x5_3(

        KC_F10,  KC_F7,   KC_F8,    KC_F9, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_F11,   KC_F4,   KC_F5,   KC_F6, XXXXXXX,      XXXXXXX, KC_RCTL, KC_RSFT, KC_RALT, KC_RGUI,
        KC_F12,   KC_F1,   KC_2,    KC_3,  XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

        _______, _______, _______,                       _______, _______, _______

    ),

    [_MEDIA] = LAYOUT_split_3x5_3(

        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LGUI, KC_RALT, KC_LSFT, KC_RCTL, XXXXXXX,        KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MUTE,
        M_UNDO,  M_CUT,   M_COPY,  M_PASTE, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

        _______, _______, _______,                          _______, KC_MPLY, _______

    ),

    [_GAME] = LAYOUT_split_3x5_3(

        KC_ESC,  KC_Q,   KC_W,    KC_E, KC_R,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_LGUI, KC_A,   KC_S,    KC_D, KC_F,        KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MUTE,
        KC_TAB,  KC_Z,   KC_X,    KC_C, KC_V,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,

        OSL(_GAMESWAP), KC_SPC, KC_RCTL,             XXXXXXX, KC_MPLY, XXXXXXX

    ),

    [_GAMESWAP] = LAYOUT_split_3x5_3(

        XXXXXXX, KC_1, KC_2, KC_3, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_4, KC_5, KC_6, KC_0,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_7, KC_8, KC_9, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

        _______, _______, _______,                 XXXXXXX, XXXXXXX, XXXXXXX

    ),

    // clang-format on
};
