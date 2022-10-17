#include "FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

static int output = 1;
static int pin_input = GPIO_NUM_18;
static int pin_output = GPIO_NUM_0;
void app_main(void) {
    gpio_config_t col_config = {
        .mode = GPIO_MODE_OUTPUT,               //输出模式
        .pin_bit_mask = 1ULL << pin_output,     //列引脚
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  //下拉失能
        .pull_up_en = GPIO_PULLUP_DISABLE,      //上拉失能
        .intr_type = GPIO_INTR_DISABLE,         //无中断
    };
    gpio_config(&col_config);

    gpio_config_t row_config = {
        .mode = GPIO_MODE_INPUT,                //输入模式
        .pin_bit_mask = 1ULL << pin_input,      //行引脚
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  //下拉失能
        .pull_up_en = GPIO_PULLUP_ENABLE,       //上拉使能
        .intr_type = GPIO_INTR_DISABLE,         //无中断
    };
    gpio_config(&row_config);

    while (1) {
        vTaskDelay(10);
        output = 1;
        gpio_set_level(pin_output, output);
        int status = gpio_get_level(pin_input);
        ESP_LOGI("app_main", "set(%d) = %d, get(%d) = %d\n", pin_output, output, pin_input, status);
        // vTaskDelay(100);
        // status = gpio_get_level(pin_input);
        // ESP_LOGI("app_main", "set(%d) = %d, get(%d) = %d\n", pin_output, output, pin_input,
        // status);

        output = 0;
        gpio_set_level(pin_output, output);
        status = gpio_get_level(pin_input);
        ESP_LOGI("app_main", "set(%d) = %d, get(%d) = %d\n", pin_output, output, pin_input, status);
        // vTaskDelay(100);
        // status = gpio_get_level(pin_input);
        // ESP_LOGI("app_main", "set(%d) = %d, get(%d) = %d\n", pin_output, output, pin_input,
        // status);

        ESP_LOGI("app_main", "==============================");
    }
}