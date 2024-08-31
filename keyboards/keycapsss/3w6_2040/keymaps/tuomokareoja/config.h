// Copyright 2022 Conor Burns (@Conor-Burns)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define VIAL_KEYBOARD_UID {0x5C, 0xD1, 0x02, 0x5A, 0x42, 0x02, 0x9E, 0x28}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 4 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 0 }

#define EE_HANDS

#define SPLIT_LAYER_STATE_ENABLE

#define MAX_DEFERRED_EXECUTORS 10

#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

// Delay between pressing a movement key and cursor movement
#define MOUSEKEY_DELAY 0
// Time between cursor movements
#define MOUSEKEY_INTERVAL 20
// Step size for cursor movement
#define MOUSEKEY_MOVE_DELTA 5
// Maximum speed for cursor movement at which acceleration stops
#define MOUSEKEY_MAX_SPEED 30
// Time until maximum cursor speed is reached
#define MOUSEKEY_TIME_TO_MAX 100
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
