// Copyright 2022 Conor Burns (@Conor-Burns)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define VIAL_KEYBOARD_UID {0x5C, 0xD1, 0x02, 0x5A, 0x42, 0x02, 0x9E, 0x28}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 4 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 0 }

#define QUICK_TAP_TERM 150
#define TAPPING_TERM 180
#define TAPPING_TERM_PER_KEY
#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

// Delay between pressing a movement key and cursor movement
#define MOUSEKEY_DELAY 4
// Time between cursor movements
#define MOUSEKEY_INTERVAL 20
// Step size for cursor movement
#define MOUSEKEY_MOVE_DELTA 5
// Maximum speed for cursor movement at which acceleration stops
#define MOUSEKEY_MAX_SPEED 20
// Time until maximum cursor speed is reached
#define MOUSEKEY_TIME_TO_MAX 70
// Delay between pressing a wheel key and wheel movement
#define MOUSEKEY_WHEEL_DELAY 5
// Time between wheel movements
#define MOUSEKEY_WHEEL_INTERVAL 80
// Wheel movement step size
#define MOUSEKEY_WHEEL_DELTA 1
// Maximum number of scroll steps per scroll action
#define MOUSEKEY_WHEEL_MAX_SPEED 8
// Time until maximum scroll speed is reached
#define MOUSEKEY_WHEEL_MAX_TIME_TO_MAX 40

#define CAPS_WORD_IDLE_TIMEOUT 2000
#define CAPS_WORD_INVERT_ON_SHIFT

// RGB color for normal layers
#define BASE_RGB
// RGB color for game layers
#define GAME_RGB
