/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H


/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6057
#define DEVICE_VER      0x0001
/*#define MANUFACTURER    t.m.k. */
#define MANUFACTURER "TMK"
#define USBSTR_MANUFACTURER    'T', '\x00', 'M', '\x00', 'K', '\x00', ' ', '\x00', '\xc6', '\x00'
/* #define PRODUCT         Phantom */
#define PRODUCT "TMK Chibi Phantom"
#define USBSTR_PRODUCT         'T', '\x00', 'M', '\x00', 'K', '\x00', ' ', '\x00', 'C', '\x00', 'h', '\x00', 'i', '\x00', 'b', '\x00', 'i', '\x00', ' ', '\x00', 'P', '\x00', 'h', '\x00', 'a', '\x00', 'n', '\x00', 't', '\x00', 'o', '\x00', 'm', '\x00'
#define DESCRIPTION "TMK firmware for Phantom"

/* message strings */
/*#define DESCRIPTION     t.m.k. keyboard firmware for Phantom */

/* matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 17

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if need no debouncing */
//#define DEBOUNCE    7
//#define DEBOUNCE    5
//#define DEBOUNCE    2
#define DEBOUNCE    1

/* Set LED brightness 0-255.
 * This have no effect if sleep LED is enabled. */
#define LED_BRIGHTNESS  250

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

#endif
