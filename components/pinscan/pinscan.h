#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pinscan {

class Pinscan : public PollingComponent {
public:
  Pinscan() : PollingComponent(50) {}
  void setup() override;
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void set_pin(int pin);
  void set_mode(int mode);
  void set_pin_state_sensor(binary_sensor::BinarySensor *pin_state_sensor) {
    pin_state_sensor_ = pin_state_sensor;
  }

protected:
  int current_pin_{-1};
  int current_mode_{-1};
  int current_state_{-1};
  binary_sensor::BinarySensor *pin_state_sensor_{nullptr};
};

} // namespace pinscan
} // namespace esphome
