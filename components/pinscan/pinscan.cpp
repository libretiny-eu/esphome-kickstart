
#include "pinscan.h"
#include "Arduino.h"

namespace esphome {
namespace pinscan {

static const char *const TAG = "pinscan";

void Pinscan::setup() { ESP_LOGCONFIG(TAG, "Setting up Pinscan..."); }

void Pinscan::dump_config() { ESP_LOGCONFIG(TAG, "Pinscan:"); }

void Pinscan::set_pin(int pin) {
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
    ESP_LOGD(TAG, "Changed pin to %d, mode: %s", this->current_pin_,
             modeStr.c_str());
  }
}

void Pinscan::update() {
  if (this->current_mode_ == 1 && this->current_pin_ != -1) {
    int newState = digitalRead(this->current_pin_);
    if (newState != this->current_state_) {
      this->current_state_ = newState;
      ESP_LOGD(TAG, "Pin changed to %s",
               (this->current_state_) ? "HIGH" : "LOW");
      if (this->pin_state_sensor_ != nullptr) {
        this->pin_state_sensor_->publish_state((this->current_state_) ? "HIGH"
                                                                      : "LOW");
      }
    }
  }
}

} // namespace pinscan
} // namespace esphome