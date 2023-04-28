/**
 * This file is part of esp32s3-keyboard.
 *
 * Copyright (C) 2020-2021 Yuquan He <heyuquan20b at ict dot ac dot cn> 
 * (Institute of Computing Technology, Chinese Academy of Sciences)
 *
 * esp32s3-keyboard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * esp32s3-keyboard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esp32s3-keyboard. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Keyboard pin configuration
 */
#ifndef MY_PIN_CFG_H
#define MY_PIN_CFG_H

#include "sdkconfig.h"
#include "driver/gpio.h"


/****************************************************************
 * 
 *  GPIO allocation
 * 
 ****************************************************************/


// Trackpad GPIO
#define PS2_CLK_PIN     16
#define PS2_DATA_PIN    15
#define PS2_RESET_PIN   17

#define PS2_CLK_STATE   gpio_get_level(PS2_CLK_PIN)
#define PS2_CLK_OUTPUT  gpio_set_direction(PS2_CLK_PIN,  GPIO_MODE_OUTPUT)
#define PS2_CLK_INPUT   gpio_set_direction(PS2_CLK_PIN,  GPIO_MODE_INPUT)
#define PS2_CLK_LOW     gpio_set_level(PS2_CLK_PIN, 0)
#define PS2_CLK_HIGH    gpio_set_level(PS2_CLK_PIN, 1)

#define PS2_DATA_STATE  gpio_get_level(PS2_DATA_PIN)
#define PS2_DATA_OUTPUT gpio_set_direction(PS2_DATA_PIN, GPIO_MODE_OUTPUT)
#define PS2_DATA_INPUT  gpio_set_direction(PS2_DATA_PIN, GPIO_MODE_INPUT)
#define PS2_DATA_LOW    gpio_set_level(PS2_DATA_PIN, 0)
#define PS2_DATA_HIGH   gpio_set_level(PS2_DATA_PIN, 1)


/**
 * FN functions
 */
typedef enum {
  FN_NOP = 0,
  FN_FNLOCK, 
  FN_BACKLIGHT,
} fn_function_t;


/****************************************************************
 * 
 *  Handy GPIO helpers
 * 
 ****************************************************************/

#define GPIO_INIT_IN_FLOATING(pinnum) \
  do { \
    gpio_reset_pin(pinnum); \
    gpio_set_direction(pinnum, GPIO_MODE_INPUT); \
    gpio_pullup_dis(pinnum); \
    gpio_pulldown_dis(pinnum); \
  } while(0)

#define GPIO_INIT_IN_PULLUP(pinnum) \
  do { \
    gpio_reset_pin(pinnum); \
    gpio_set_direction(pinnum, GPIO_MODE_INPUT); \
    gpio_pullup_en(pinnum); \
    gpio_pulldown_dis(pinnum); \
  } while(0)

#define GPIO_INIT_IN_PULLDOWN(pinnum) \
  do { \
    gpio_reset_pin(pinnum); \
    gpio_set_direction(pinnum, GPIO_MODE_INPUT); \
    gpio_pullup_dis(pinnum); \
    gpio_pulldown_en(pinnum); \
  } while(0)

#define GPIO_SET_PULLUP(pinnum) \
  do { \
    gpio_pullup_en(pinnum); \
    gpio_pulldown_dis(pinnum); \
  } while(0)

#define GPIO_SET_PULLDOWN(pinnum) \
  do { \
    gpio_pullup_dis(pinnum); \
    gpio_pulldown_en(pinnum); \
  } while(0)

#define GPIO_INIT_OUT_PULLUP(pinnum) \
  do { \
    gpio_reset_pin(pinnum); \
    gpio_set_direction(pinnum, GPIO_MODE_OUTPUT); \
    gpio_pullup_en(pinnum); \
  } while(0)

#define GPIO_INIT_OUT_PULLDOWN(pinnum) \
  do { \
    gpio_reset_pin(pinnum); \
    gpio_set_direction(pinnum, GPIO_MODE_OUTPUT); \
    gpio_pulldown_en(pinnum); \
  } while(0)

#endif
