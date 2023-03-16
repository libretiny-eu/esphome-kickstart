
#pragma once

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

protected:
  int current_pin_{-1};
  int current_mode_{-1};
  int current_state_{-1};
};

} // namespace pinscan
} // namespace esphome
