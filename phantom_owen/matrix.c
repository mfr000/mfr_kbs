/* Copyright 2012 Jun Wako <wakojun@gmail.com>
 *
 * This is heavily based on phantom/board.{c|h}.
 * https://github.com/BathroomEpiphanies/AVR-Keyboard
 *
 * Copyright (c) 2012 Fredrik Atmer, Bathroom Epiphanies Inc
 * http://bathroomepiphanies.com
 *
 * As for liscensing consult with the original files or its author.
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "wait.h"
#include "timer.h"

// MFR: DEBOUNCE is defined in config.h
#ifndef DEBOUNCE
#   define DEBOUNCE	0
#endif
static uint8_t debouncing = DEBOUNCE;

// bit array of key state(1:on, 0:off)
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static uint8_t read_rows(void);
static void init_rows(void);
static void init_cols(void);
static void unselect_col(uint8_t col);
static void select_col(uint8_t col);

#ifndef SLEEP_LED_ENABLE
/* LEDs are on output compare pins OC1B OC1C
   This activates fast PWM mode on them.
   Prescaler 256 and 8-bit counter results in
   16000000/256/256 = 244 Hz blink frequency.
   LED_A: Caps Lock
   LED_B: Scroll Lock  */
/* Output on PWM pins are turned off when the timer 
   reaches the value in the output compare register,
   and are turned on when it reaches TOP (=256). */
static
void setup_leds(void)
{
/* MFR:
    TCCR1A |=      // Timer control register 1A
        (1<<WGM10) | // Fast PWM 8-bit
        (1<<COM1B1)| // Clear OC1B on match, set at TOP
        (1<<COM1C1); // Clear OC1C on match, set at TOP
    TCCR1B |=      // Timer control register 1B
        (1<<WGM12) | // Fast PWM 8-bit
        (1<<CS12);   // Prescaler 256
    OCR1B = LED_BRIGHTNESS;    // Output compare register 1B
    OCR1C = LED_BRIGHTNESS;    // Output compare register 1C
    // LEDs: LED_A -> PORTB6, LED_B -> PORTB7
    DDRB  |= (1<<6) | (1<<7);
    PORTB  &= ~((1<<6) | (1<<7));
*/
}
#endif

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // To use GPIO PA13-PA15 and PB3-PB4, disable JTAG by writing 0b100 to 
    // MAPR register bits 23-26
    AFIO->MAPR |= 0x04000000; // SWJ_CFG[2:0] =0b100 - JTAG & SWJ disabled
    AFIO->MAPR &= 0xFCFFFFFF;

    // MFR: debug on
    debug_config.enable = false;
    debug_config.keyboard = false;
    debug_config.matrix = false;

    // Turn off the onboard LED (already configured as output by board.c)
    palClearPad(GPIOB, 1);
	
    // initialize row and col
    init_cols();
    init_rows();
#ifndef SLEEP_LED_ENABLE
    setup_leds();
#endif

    // initialize matrix state: all keys off
    memset(matrix, 0, MATRIX_ROWS);
    memset(matrix_debouncing, 0, MATRIX_ROWS);

}

uint8_t matrix_scan(void)
{
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {  // 0-16
        select_col(col);
        wait_us(10);     // need to wait to settle pin state (3us was in old file)
        uint8_t data = read_rows();

        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {  // 0-5
            bool prev_bit = matrix_debouncing[row] & ((matrix_row_t)1<<col);
//            bool curr_bit = data & (1<<row);
            bool curr_bit = data & (1<<(MATRIX_ROWS-1-row));
            if (prev_bit != curr_bit) {
                matrix_debouncing[row] ^= ((matrix_row_t)1<<col);
                if (debouncing) {
                    dprint("bounce!: "); dprintf("%02X", debouncing); dprintln();
                }
                debouncing = DEBOUNCE;
            }
        }

        unselect_col(col);
    }

    if (debouncing) {
        if (--debouncing) {
            wait_ms(1);
//            wait_us(500); // Wait 1/2ms
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    xprintf("\nr/c 0123456789ABCDEF0\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%02X: ", row);
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                xprintf("1");
            else
                xprintf("0");
        }
        xprintf("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop32(matrix[i]);
    }
    return count;
}

/* Row pin configuration
 * row: 5   4   3   2   1   0
 * pin: PA0 PA1 PA2 PA3 PA4 PA5
 */
static void init_rows(void)
{
    // Input with pull-up(DDR:0, PORT:1)

    /* Row(sense) */
    palSetPadMode(GPIOA, 0,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, 1,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, 2,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, 3,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, 4,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, 5,  PAL_MODE_INPUT_PULLDOWN);
}

static uint8_t read_rows(void)
{
    /* MFR: we only use the lower 6 bits. should we <<2 and >>2 to clear the top
     * two bits?
     */
    return ((palReadPort(GPIOA)<<2)>>2);
}

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16
 * pin: PB7     PB5     PB3     PA14    PB0     PB10    PA8     PB14    PB12
 * pin:     PB6     PB4     PA15    PA13    PB2     PB11    PB15    PB13
 */
static void init_cols(void)
{
    /* Column(strobe) */
    palSetPadMode(GPIOB,  7,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB,  6,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB,  5,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB,  4,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB,  3,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 15,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 14,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 13,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB,  0,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB,  2,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 10,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 11,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA,  8,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 15,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 14,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 13,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 12,  PAL_MODE_OUTPUT_PUSHPULL);

    // Hi-Z(DDR:0, PORT:0) to unselect

    palClearPad(GPIOB,  7);
    palClearPad(GPIOB,  6);
    palClearPad(GPIOB,  5);
    palClearPad(GPIOB,  4);
    palClearPad(GPIOB,  3);
    palClearPad(GPIOA, 15);
    palClearPad(GPIOA, 14);
    palClearPad(GPIOA, 13);
    palClearPad(GPIOB,  0);
    palClearPad(GPIOB,  2);
    palClearPad(GPIOB, 10);
    palClearPad(GPIOB, 11);
    palClearPad(GPIOA,  8);
    palClearPad(GPIOB, 15);
    palClearPad(GPIOB, 14);
    palClearPad(GPIOB, 13);
    palClearPad(GPIOB, 12);
}

static void unselect_col(uint8_t col)
{
    switch (col) {
        case 0:  palClearPad(GPIOB,  7); break;
        case 1:  palClearPad(GPIOB,  6); break;
        case 2:  palClearPad(GPIOB,  5); break;
        case 3:  palClearPad(GPIOB,  4); break;
        case 4:  palClearPad(GPIOB,  3); break;
        case 5:  palClearPad(GPIOA, 15); break;
        case 6:  palClearPad(GPIOA, 14); break;
        case 7:  palClearPad(GPIOA, 13); break;
        case 8:  palClearPad(GPIOB,  0); break;
        case 9:  palClearPad(GPIOB,  2); break;
        case 10: palClearPad(GPIOB, 10); break;
        case 11: palClearPad(GPIOB, 11); break;
        case 12: palClearPad(GPIOA,  8); break;
        case 13: palClearPad(GPIOB, 15); break;
        case 14: palClearPad(GPIOB, 14); break;
        case 15: palClearPad(GPIOB, 13); break;
        case 16: palClearPad(GPIOB, 12); break;
    }
}

static void select_col(uint8_t col)
{
    switch (col) {
        case 0:  palSetPad(GPIOB,  7); break;
        case 1:  palSetPad(GPIOB,  6); break;
        case 2:  palSetPad(GPIOB,  5); break;
        case 3:  palSetPad(GPIOB,  4); break;
        case 4:  palSetPad(GPIOB,  3); break;
        case 5:  palSetPad(GPIOA, 15); break;
        case 6:  palSetPad(GPIOA, 14); break;
        case 7:  palSetPad(GPIOA, 13); break;
        case 8:  palSetPad(GPIOB,  0); break;
        case 9:  palSetPad(GPIOB,  2); break;
        case 10: palSetPad(GPIOB, 10); break;
        case 11: palSetPad(GPIOB, 11); break;
        case 12: palSetPad(GPIOA,  8); break;
        case 13: palSetPad(GPIOB, 15); break;
        case 14: palSetPad(GPIOB, 14); break;
        case 15: palSetPad(GPIOB, 13); break;
        case 16: palSetPad(GPIOB, 12); break;
    }
    // Output low(DDR:1, PORT:0) to select
}
