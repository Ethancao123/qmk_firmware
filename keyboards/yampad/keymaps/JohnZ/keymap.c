#include QMK_KEYBOARD_H
extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layers {
  _BL = 0,
  _NV,
  _FN
};

enum custom_keycodes {
  KC_DBL0 = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap _BL: (Base Layer) Default Layer (This is wrong)
 * ,-------------------.
 * | NV | << | || | >> |
 * |----|----|----|----|
 * | 7  | 8  | 9  |    |
 * |----|----|----| V+ |
 * | 4  | 5  | 6  |    |
 * |----|----|----|----|
 * | 1  | 2  | 3  |    |
 * |----|----|----| V- |
 * | 0  |F24 |Mute|    |
 * `-------------------'
 */
  [_BL] = LAYOUT(
    KC_MSTP,  KC_MPRV,  KC_MPLY,   KC_MNXT, \
    KC_7,    KC_8,    KC_9,              \
    KC_4,    KC_5,    KC_6,     KC_VOLU, \
    KC_1,    KC_2,    KC_3,              \
    KC_0,    TO(_NV),  KC_MUTE,  KC_VOLD
  ),

/* Keymap _NV: Navigation layer (This is wrong)
 * ,-------------------.
 * | NV |    |    | FN |
 * |----|----|----|----|
 * |    | UP |    |    |
 * |----|----|----|    |
 * |LEFT|DOWN|RIGH|    |
 * |----|----|----|----|
 * |    |    |    |    |
 * |----|----|----|    |
 * |    |    |    |    |
 * `-------------------'
 */
  [_NV] = LAYOUT(
    KC_MSTP,  KC_MPRV,  KC_MPLY,   KC_MNX, \
    KC_F13,   KC_UP,   KC_F14,            \
    KC_LEFT,  KC_DOWN,  KC_RGHT,   KC_VOLU, \
    KC_F4,  KC_F5,    KC_F6,            \
    KC_F11,  TO(_FN),  KC_MUTE,   KC_VOLD
  ),

/* Keymap _FN: RGB Function Layer
 * ,-------------------.
 * |RMOD|RGBP|RTOG| FN |
 * |----|----|----|----|
 * |HUD |HUI |    |    |
 * |----|----|----|    |
 * |SAD |SAI |    |    |
 * |----|----|----|----|
 * |VAD |VAS |    |    |
 * |----|----|----|    |
 * |RST |    |    |    |
 * `-------------------'
 */
  [_FN] = LAYOUT(
    RGB_MOD,  RGB_M_P,  RGB_TOG,   RGB_M_G, \
    RGB_HUD,  RGB_HUI,  RGB_M_R,            \
    RGB_SAD,  RGB_SAI,  RGB_M_SW,   RGB_SPI, \
    RGB_VAD,  RGB_VAI,  RGB_M_SN,            \
    RGB_M_B,    TO(_BL),  RGB_M_K,   RGB_SPD
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_DBL0:
      if (record->event.pressed) {
        SEND_STRING("00");
      } else {
        // when keycode KC_DBL0 is released
      }
      break;

  }
  return true;
};

#ifdef OLED_DRIVER_ENABLE
extern rgblight_config_t rgblight_config;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;  // flips the display 270 degrees
}

void oled_task_user(void) {
  // Host Keyboard Layer Status
  oled_write_P(PSTR("Layer"), false);
  switch (biton32(layer_state)) {
    case _BL:
      oled_write_ln_P(PSTR(" BAS"), false);
      break;
    case _NV:
      oled_write_ln_P(PSTR(" NAV"), false);
      break;
    case _FN:
      oled_write_ln_P(PSTR(" RGB"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR(" UND"), false);
  }

  // Host Keyboard LED Status
  uint8_t led_usb_state = host_keyboard_leds();
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("Stats"), false);
  oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("num:*") : PSTR("num:."), false);
  oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("cap:*") : PSTR("cap:."), false);
  oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("scr:*") : PSTR("scr:."), false);

  // Host Keyboard RGB backlight status
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("Light"), false);

  static char led_buf[30];
  snprintf(led_buf, sizeof(led_buf) - 1, "RGB:%cM: %2d\nh: %2ds: %2dv: %2d\n",
      rgblight_config.enable ? '*' : '.', (uint8_t)rgblight_config.mode,
      (uint8_t)(rgblight_config.hue / RGBLIGHT_HUE_STEP),
      (uint8_t)(rgblight_config.sat / RGBLIGHT_SAT_STEP),
      (uint8_t)(rgblight_config.val / RGBLIGHT_VAL_STEP));
  oled_write(led_buf, false);
}
#endif
