#ifndef USB_KEYBOARD_H_
#define USB_KEYBOARD_H_

#include "device/usbd.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "class/hid/hid.h"

#define KEYBOARD_COL_NUM 16
#define KEYBOARD_ROW_NUM 8
#define KEY_MAX_COMBO_COUNT 6


void init_keyboard_gpios();
int keyboard_scan(uint8_t result[], char max_count);


#endif /* USB_KEYBOARD_H_ */