#pragma once

#ifdef USE_ARDUINO

#include <map>
#include <utility>

#include "esphome/components/web_server_base/web_server_base.h"
#include "esphome/core/controller.h"
#include "esphome/core/component.h"

namespace esphome {
namespace hub_api {

class HubAPI : public AsyncWebHandler, public Component {
 public:
  HubAPI(web_server_base::WebServerBase *base) : base_(base) {}

  bool canHandle(AsyncWebServerRequest *request) override {
    if (request->method() == HTTP_GET) {
      if (request->url().startsWith("/hub"))
        return true;
    }

    return false;
  }

  void handleRequest(AsyncWebServerRequest *req) override;

  void setup() override {
    this->base_->init();
    this->base_->add_handler(this);
  }
  float get_setup_priority() const override {
    // After WiFi
    return setup_priority::WIFI - 1.0f;
  }

 protected:
  web_server_base::WebServerBase *base_;
};

}  // namespace hub_api
}  // namespace esphome

#endif  // USE_ARDUINO
