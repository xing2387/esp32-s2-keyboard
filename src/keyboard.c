#include "keyboard.h"
#include "esp_log.h"
// #include "freertos/task.h"

static int col_gpio_list[] = {
    GPIO_NUM_0,GPIO_NUM_1,GPIO_NUM_2,GPIO_NUM_3,
    GPIO_NUM_4,GPIO_NUM_5,GPIO_NUM_6,GPIO_NUM_7,
    GPIO_NUM_8,GPIO_NUM_9,GPIO_NUM_10,GPIO_NUM_11,
    GPIO_NUM_12,GPIO_NUM_13,GPIO_NUM_14,GPIO_NUM_15
};

static int row_gpio_list[] = {
    GPIO_NUM_18, GPIO_NUM_21, GPIO_NUM_26, GPIO_NUM_33,
    GPIO_NUM_34, GPIO_NUM_35, GPIO_NUM_36, GPIO_NUM_37
};

static uint8_t keyboard_matrix[][KEYBOARD_ROW_NUM] = {
    {HID_KEY_H,     HID_KEY_6,      HID_KEY_Y,              HID_KEY_U, HID_KEY_J, HID_KEY_7, HID_KEY_M, HID_KEY_N},
    {HID_KEY_F6,    HID_KEY_EQUAL,  HID_KEY_BRACKET_RIGHT,  HID_KEY_I, HID_KEY_K, HID_KEY_8, HID_KEY_COMMA, HID_KEY_NONE},
    {HID_KEY_F5,    HID_KEY_F9,     HID_KEY_BACKSPACE,      HID_KEY_NONE, HID_KEY_BACKSLASH, HID_KEY_F10, HID_KEY_ENTER, HID_KEY_SPACE},
    {HID_KEY_NONE,  HID_KEY_F8,     HID_KEY_F7, HID_KEY_O, HID_KEY_L, HID_KEY_9, HID_KEY_PERIOD, HID_KEY_NONE},
    {HID_KEY_G,     HID_KEY_5,      HID_KEY_T, HID_KEY_R, HID_KEY_F, HID_KEY_4, HID_KEY_V, HID_KEY_B},
    {HID_KEY_APOSTROPHE, HID_KEY_MINUS, HID_KEY_BRACKET_LEFT, HID_KEY_P, HID_KEY_SEMICOLON, HID_KEY_0, HID_KEY_NONE, HID_KEY_SLASH},
    {HID_KEY_F4, HID_KEY_F2, HID_KEY_F3, HID_KEY_E, HID_KEY_D, HID_KEY_3, HID_KEY_C, HID_KEY_NONE},
    {HID_KEY_POWER /*1*/, HID_KEY_DELETE, HID_KEY_VOLUME_UP, HID_KEY_VOLUME_UP, HID_KEY_MUTE, HID_KEY_F11, HID_KEY_NONE/*2*/, HID_KEY_ARROW_DOWN},
    {HID_KEY_NONE, HID_KEY_F1, HID_KEY_CAPS_LOCK, HID_KEY_W, HID_KEY_S, HID_KEY_2, HID_KEY_X, HID_KEY_NONE},
    {HID_KEY_NONE, HID_KEY_INSERT, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_F12, HID_KEY_NONE, HID_KEY_ARROW_RIGHT},
    {HID_KEY_ESCAPE, HID_KEY_GRAVE, HID_KEY_TAB, HID_KEY_Q, HID_KEY_A, HID_KEY_1, HID_KEY_Z, HID_KEY_NONE},
    {HID_KEY_NONE, HID_KEY_PAGE_UP, HID_KEY_NONE, HID_KEY_GUI_LEFT, HID_KEY_APPLICATION, HID_KEY_PAGE_DOWN, HID_KEY_PAGE_UP/*3*/, HID_KEY_PAGE_DOWN/*4*/},
    {HID_KEY_NONE, HID_KEY_NONE, HID_KEY_SHIFT_LEFT, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_SHIFT_RIGHT, HID_KEY_NONE},
    {HID_KEY_ARROW_UP, HID_KEY_HOME, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_END, HID_KEY_PAUSE, HID_KEY_ARROW_LEFT},
    {HID_KEY_NONE, HID_KEY_CONTROL_LEFT, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_CONTROL_RIGHT, HID_KEY_NONE},
    {HID_KEY_ALT_LEFT, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_SCROLL_LOCK, HID_KEY_NONE, HID_KEY_PRINT_SCREEN, HID_KEY_NONE, HID_KEY_ALT_RIGHT}
};


void init_keyboard_gpios() {
    uint64_t pin_bit_mask = 0;
    for (size_t i = 0; i < KEYBOARD_COL_NUM; i++) {
        pin_bit_mask |= 1ULL << col_gpio_list[i];
    }
    gpio_config_t col_config = {
        .mode = GPIO_MODE_OUTPUT,               //输出模式
        .pin_bit_mask = pin_bit_mask,           //列引脚
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  //下拉失能
        .pull_up_en = GPIO_PULLUP_DISABLE,      //上拉失能
        .intr_type = GPIO_INTR_DISABLE,         //无中断
    };
    gpio_config(&col_config);

    pin_bit_mask = 0;
    for (size_t i = 0; i < KEYBOARD_ROW_NUM; i++) {
        pin_bit_mask |= 1ULL << row_gpio_list[i];
    }
    gpio_config_t row_config = {
        .mode = GPIO_MODE_INPUT,                //输入模式
        .pin_bit_mask = pin_bit_mask,           //行引脚
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  //下拉失能
        .pull_up_en = GPIO_PULLUP_ENABLE,       //上拉使能
        .intr_type = GPIO_INTR_DISABLE,         //无中断
    };
    gpio_config(&row_config);
}

uint8_t get_modify_bit(char keycode) {
    switch (keycode) {
        case HID_KEY_CONTROL_LEFT:
            return KEYBOARD_MODIFIER_LEFTCTRL;
        case HID_KEY_SHIFT_LEFT:
            return KEYBOARD_MODIFIER_LEFTSHIFT;
        case HID_KEY_ALT_LEFT:
            return KEYBOARD_MODIFIER_LEFTALT;
        case HID_KEY_GUI_LEFT:
            return KEYBOARD_MODIFIER_LEFTGUI;
        case HID_KEY_CONTROL_RIGHT:
            return KEYBOARD_MODIFIER_RIGHTCTRL;
        case HID_KEY_SHIFT_RIGHT:
            return KEYBOARD_MODIFIER_RIGHTSHIFT;
        case HID_KEY_ALT_RIGHT:
            return KEYBOARD_MODIFIER_RIGHTALT;
        case HID_KEY_GUI_RIGHT:
            return KEYBOARD_MODIFIER_RIGHTGUI;
        default:
            return 0x00;
    }
}

int keyboard_scan(uint8_t result[], char max_count) {
    // ESP_LOGI("keyboard_scan", "keyboard_scan");
    // vTaskDelay(10);
    int count = 0;
    uint8_t modify_bits = 0;    
    for (size_t col = 0; col < KEYBOARD_COL_NUM; col++) {
        gpio_set_level(col_gpio_list[col], 0);
        ESP_LOGI("keyboard_scan", "gpio_set_level gpio_%d : %d", col_gpio_list[col], 0);
        for (size_t row = 0; row < KEYBOARD_ROW_NUM; row++) {
            int status = gpio_get_level(row_gpio_list[row]);
            if (status == 0) {
                uint8_t keycode = keyboard_matrix[col][row];
                ESP_LOGI("keyboard_scan", "col: %d, row: %d, keycode = %d\n", col, row, keycode);
                uint8_t modify_bit = get_modify_bit(keycode);
                if (modify_bit != 0) {
                    modify_bits |= modify_bit;
                } else {
                    result[count++] = keycode;
                }
                break;
                if (count >= max_count - 1) goto reach_limit;
            }
        }
        gpio_set_level(col_gpio_list[col], 1);
    }

reach_limit:
    result[count++] = modify_bits;

    return count;
}
