# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.


# FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)
# idf_component_register(SRCS ${app_sources})
idf_component_register(SRCS "main.c" "usb_descriptors.c" "keyboard.c" "mouse.c"
                    INCLUDE_DIRS "."
                    REQUIRES freertos soc)

# include("/home/xing/.platformio/packages/framework-espidf@3.40401.0/components/tinyusb/tinyusb/hw/bsp/esp32s2/boards/espressif_saola_1/board.cmake")

idf_component_get_property( FREERTOS_ORIG_INCLUDE_PATH freertos ORIG_INCLUDE_PATH)
target_include_directories(${COMPONENT_TARGET} PUBLIC
  "${FREERTOS_ORIG_INCLUDE_PATH}"
  "${TOP}/hw"
  "${TOP}/src"
)


# target_sources(${COMPONENT_TARGET} PUBLIC
#   "${TOP}/src/tusb.c"
#   "${TOP}/src/common/tusb_fifo.c"
#   "${TOP}/src/device/usbd.c"
#   "${TOP}/src/device/usbd_control.c"
#   "${TOP}/src/class/cdc/cdc_device.c"
#   "${TOP}/src/class/dfu/dfu_rt_device.c"
#   "${TOP}/src/class/hid/hid_device.c"
#   "${TOP}/src/class/midi/midi_device.c"
#   "${TOP}/src/class/msc/msc_device.c"
#   "${TOP}/src/class/net/net_device.c"
#   "${TOP}/src/class/usbtmc/usbtmc_device.c"
#   "${TOP}/src/class/vendor/vendor_device.c"
#   "${TOP}/src/portable/espressif/esp32sx/dcd_esp32sx.c"
# )
