// Desko-style Cherry Black ANSI
static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: qwerty */
    KEYMAP_ANSI(\
        ESC,      F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,       INS, HOME,PGUP,PSCR, \
                                                                                    DEL, END, PGDN,SLCK, \
        GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC,      NLCK,PSLS,PAST,BRK,  \
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,      P7,  P8,  P9,  PMNS, \
        FN0, A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,       P4,  P5,  P6,  PPLS, \
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,     RSFT,UP,        P1,  P2,  P3,  PENT, \
        LCTL,LGUI,LALT,               SPC,      RALT,RGUI,APP, RCTL,LEFT,DOWN,      RGHT,P0,  PDOT),
    /* 1: media keys */
    KEYMAP_ANSI(\
        TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,      TRNS,TRNS,TRNS,SLEP, \
                                                                                    TRNS,TRNS,TRNS,SLEP, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,MUTE,VOLD,VOLU,TRNS,      TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,MSTP,MPLY,MPRV,MNXT,MSEL,      TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     TRNS,      TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,CALC,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,     CAPS,TRNS,      TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,               TRNS,     TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,      TRNS,TRNS,TRNS)
};
static const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1)
};
