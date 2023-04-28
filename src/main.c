#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "bsp/board.h"
#include "keyboard.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"
#include "tusb.h"
#include "usb_descriptors.h"

#define USBD_STACK_SIZE (3 * configMINIMAL_STACK_SIZE / 2)

StackType_t usb_device_stack[USBD_STACK_SIZE];
StaticTask_t usb_device_taskdef;

// static task for hid
#define HID_STACK_SZIE configMINIMAL_STACK_SIZE
StackType_t hid_stack[HID_STACK_SZIE];
StaticTask_t hid_taskdef;

void usb_device_task(void* param);
void hid_task(void* params);

int main(void) {
    board_init();
    init_keyboard_gpios();

    // Create a task for tinyusb device stack
    (void)xTaskCreateStatic(usb_device_task, "usbd", USBD_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, usb_device_stack, &usb_device_taskdef);

    //Create HID task
    (void)xTaskCreateStatic(hid_task, "hid", HID_STACK_SZIE, NULL, configMAX_PRIORITIES - 2, hid_stack, &hid_taskdef);

    void mouse_task(void *arg);
    xTaskCreate(&mouse_task,  "mouse_task", 4096, NULL, configMAX_PRIORITIES, NULL);
    // hid_task(1);
    return 0;
}

// USB Device Driver task
// This top level thread process all usb events and invoke callbacks
void usb_device_task(void* param) {
    (void)param;

    // This should be called after scheduler/kernel is started.
    // Otherwise it could cause kernel issue since USB IRQ handler does use RTOS queue API.
    tusb_init();

    // RTOS forever loop
    while (1) {
        // tinyusb device task
        tud_task();
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// // Invoked when device is mounted
// void tud_mount_cb(void) {}

// // Invoked when device is unmounted
// void tud_umount_cb(void) {}

// // Invoked when usb bus is suspended
// // remote_wakeup_en : if host allow us  to perform remote wakeup
// // Within 7ms, device must draw an average of current less than 2.5 mA from bus
// void tud_suspend_cb(bool remote_wakeup_en) { (void)remote_wakeup_en; }

// // Invoked when usb bus is resumed
// void tud_resume_cb(void) {}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id, uint8_t keycode[], int key_count) {
    // skip if hid is not ready yet
    if (!tud_hid_ready()) return;

    switch (report_id) {
        case REPORT_ID_KEYBOARD: {
            // use to avoid send multiple consecutive zero report for keyboard
            static bool has_keyboard_key = false;

            if (key_count > 0) {
                // uint8_t keycode[6] = {0};
                // keycode[0] = HID_KEY_A;
                uint8_t modifier = keycode[key_count - 1];
                keycode[key_count - 1] = 0;
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, keycode);
                has_keyboard_key = true;
            } else {
                // send empty key report if previously has key pressed
                if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
                has_keyboard_key = false;
            }
        } break;

        case REPORT_ID_MOUSE: {
            int8_t const delta = 5;

            // no button, right + down, no scroll, no pan
            tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
        } break;

        default:
            break;
    }
}

uint8_t scan_result[KEY_MAX_COMBO_COUNT];

void hid_task(void* param) {
    (void)param;

    while (1) {
        // Poll every 10ms
        vTaskDelay(pdMS_TO_TICKS(10));

        // uint32_t const btn = board_button_read();
        int key_count = keyboard_scan(scan_result, KEY_MAX_COMBO_COUNT);


        // Remote wakeup
        if (tud_suspended() && key_count > 0) {
            // Wake up host if we are in suspend mode
            // and REMOTE_WAKEUP feature is enabled by host
            tud_remote_wakeup();
        } else {
            // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
            send_hid_report(REPORT_ID_KEYBOARD, scan_result, key_count);
        }
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t itf, uint8_t const* report, uint8_t len) {
    (void)itf;
    (void)len;

    uint8_t next_report_id = report[0] + 1;

    if (next_report_id < REPORT_ID_COUNT) {
        // send_hid_report(next_report_id, board_button_read());
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    // TODO not Implemented
    (void)itf;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    // TODO set LED based on CAPLOCK, NUMLOCK etc...
    (void)itf;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)bufsize;
}
void app_main(void) { main(); }