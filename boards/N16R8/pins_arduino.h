#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

#ifndef DEVICE_NAME
#define DEVICE_NAME "ESP32S3_N16R8"
#endif

#define USB_VID 0x303A
#define USB_PID 0x1001

#define LED_BUILTIN 21

static const uint8_t TX = 43;
static const uint8_t RX = 44;
static const uint8_t SDA = 8;
static const uint8_t SCL = 9;
static const uint8_t SS    = 10;
static const uint8_t MOSI  = 11;
static const uint8_t MISO  = 13;
static const uint8_t SCK   = 12;

#define SPI_SCK_PIN 14
#define SPI_MOSI_PIN 3
#define SPI_MISO_PIN 2
#define SPI_SS_PIN 21

#define SDCARD_CS -1
#define SDCARD_SCK -1
#define SDCARD_MISO -1
#define SDCARD_MOSI -1

#define SERIAL_TX 43
#define SERIAL_RX 44
#define GPS_SERIAL_TX SERIAL_TX
#define GPS_SERIAL_RX SERIAL_RX

#define ANALOG_BAT_PIN 9
#define ANALOG_BAT_MULTIPLIER 2.0f

#define TXLED 2
#define RXLED 3
#define LED_ON HIGH
#define LED_OFF LOW

// IR/RF pins - using defaults

#define BAD_TX 16
#define BAD_RX 15

#define DEEPSLEEP_WAKEUP_PIN 0
#define DEEPSLEEP_PIN_ACT LOW

#endif
