
#include "pinscan.h"
#include "Arduino.h"

namespace esphome {
namespace pinscan {

static const char *const TAG = "pinscan";

#if defined(LT_BK7231N) || defined(LT_BK7231T)
static const int PIN_MAP[] = {
    PIN_P0,   // P0 / UART2_TX / I2C2_SCL
    PIN_P1,   // P1 / UART2_RX / I2C2_SDA
    PIN_P6,   // P6 / PWM0
    PIN_P7,   // P7 / PWM1
    PIN_P8,   // P8 / PWM2
    PIN_P9,   // P9 / PWM3
    PIN_P10,  // P10 / UART1_RX
    PIN_P11,  // P11 / UART1_TX
    PIN_P14,  // P14 / SCK
    PIN_P15,  // P15 / CS
    PIN_P16,  // P16 / MOSI
    PIN_P17,  // P17 / MISO
    PIN_P20,  // P20 / I2C1_SCL / TCK
    PIN_P21,  // P21 / I2C1_SDA / TMS
    PIN_P22,  // P22 / TDI
    PIN_P23,  // P23 / TDO / ADC3
    PIN_P24,  // P24 / PWM4
    PIN_P26,  // P26 / PWM5
    PIN_P28,  // P28
};
#elif defined(LT_RTL8710B)
static const int PIN_MAP[] = {
    PIN_PA00,  // PA00 / PWM2
    PIN_PA05,  // PA05 / PWM4
    PIN_PA06,  // PA06 / FCS
    PIN_PA07,  // PA07 / FD1
    PIN_PA08,  // PA08 / FD2
    PIN_PA09,  // PA09 / FD0
    PIN_PA10,  // PA10 / FSCK
    PIN_PA11,  // PA11 / FD3
    PIN_PA12,  // PA12 / PWM3
    PIN_PA14,  // PA14 / PWM0 / SWCLK
    PIN_PA15,  // PA15 / PWM1 / SWDIO
    PIN_PA18,  // PA18 / UART0_RX / I2C1_SCL / SPI0_SCK / SPI1_SCK
    PIN_PA19,  // PA19 / UART0_CTS / I2C0_SDA / SPI0_CS / SPI1_CS
    PIN_PA22,  // PA22 / UART0_RTS / I2C0_SCL / SPI0_MISO / SPI1_MISO / PWM5
    PIN_PA23,  // PA23 / UART0_TX / I2C1_SDA / SPI0_MISO / SPI1_MISO / PWM0
    PIN_PA29,  // PA29 / UART2_RX / I2C0_SCL / PWM4
    PIN_PA30,  // PA30 / UART2_TX / I2C0_SDA / PWM4

};
#else
#error "Unsupported chip type"
#endif

void Pinscan::setup() { ESP_LOGCONFIG(TAG, "Setting up Pinscan..."); }

void Pinscan::dump_config() { ESP_LOGCONFIG(TAG, "Pinscan:"); }

void Pinscan::set_pin(int pin) {
  if (pin != -1) {
    pin = PIN_MAP[pin];
  }
  if (this->current_pin_ != pin && this->current_pin_ != -1) {
    pinMode(this->current_pin_, INPUT);
  }
  this->current_pin_ = pin;
}

void Pinscan::set_mode(int mode) {
  if (this->current_pin_ != -1) {
    std::string modeStr = "";
    switch (mode) {
      case 0:
        pinMode(this->current_pin_, INPUT);
        this->current_state_ = -1;
        this->current_mode_ = 1;
        modeStr = "INPUT";
        break;
      case 1:
        pinMode(this->current_pin_, INPUT_PULLUP);
        this->current_state_ = -1;
        this->current_mode_ = 1;
        modeStr = "INPUT_PULLUP";
        break;
      case 2:
        pinMode(this->current_pin_, OUTPUT);
        digitalWrite(this->current_pin_, HIGH);
        this->current_state_ = -1;
        this->current_mode_ = 0;
        modeStr = "OUTPUT_HIGH";
        break;
      case 3:
        pinMode(this->current_pin_, OUTPUT);
        digitalWrite(this->current_pin_, LOW);
        this->current_state_ = -1;
        this->current_mode_ = 0;
        modeStr = "OUTPUT_LOW";
        break;
      default:
        pinMode(this->current_pin_, INPUT);
        modeStr = "OUTPUT_HIGH";
        break;
    }
    ESP_LOGD(TAG, "Changed pin to %d, mode: %s", this->current_pin_, modeStr.c_str());
  }
}

void Pinscan::update() {
  if (this->current_mode_ == 1 && this->current_pin_ != -1) {
    int newState = digitalRead(this->current_pin_);
    if (newState != this->current_state_) {
      this->current_state_ = newState;
      ESP_LOGD(TAG, "Pin changed to %s", (this->current_state_) ? "HIGH" : "LOW");
      if (this->pin_state_sensor_ != nullptr) {
        this->pin_state_sensor_->publish_state((this->current_state_) ? "HIGH" : "LOW");
      }
    }
  }
}

}  // namespace pinscan
}  // namespace esphome
