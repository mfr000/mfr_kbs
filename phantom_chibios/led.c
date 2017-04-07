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

#include "hal.h"

#include "led.h"


void led_set(uint8_t usb_led)
{
    /* MFR: what to do about this? This is what flabbergast does for STM32F0
     * discovery */
    (void)usb_led;
    if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
        // Output high.
//        palSetPadMode(GPIOC, GPIOC_LED_GREEN, PAL_MODE_OUTPUT_PUSHPULL);
//        palSetPad(GPIOC, GPIOC_LED_GREEN);
    } else {
        // HI-Z
//        palSetPadMode(GPIOC, GPIOC_LED_GREEN, PAL_MODE_INPUT);
    }

    if (usb_led & (1<<USB_LED_SCROLL_LOCK)) {
        // Output high.
    } else {
        // HI-Z
    }
}
