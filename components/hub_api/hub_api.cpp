#ifdef USE_ARDUINO

#include "hub_api.h"
#include "esphome/core/application.h"

#include <Flash.h>

#undef min

namespace esphome {
namespace hub_api {

static const char *const TAG = "hub_api";

void HubAPI::handleRequest(AsyncWebServerRequest *req) {
  if (req->url().substring(4) == "/flash_read") {
    uint32_t offset, length;

    if (req->hasParam("offset")) {
      offset = req->getParam("offset")->value().toInt();
    } else {
      offset = 0;
    }

    if (req->hasParam("length")) {
      length = req->getParam("length")->value().toInt();
    } else {
      length = FLASH_LENGTH;
    }

    auto callback = [offset, length](uint8_t *buffer, size_t maxLen, size_t position) -> size_t {
      size_t blockStart = offset + position;
      size_t blockSize = std::min(static_cast<size_t>(maxLen), static_cast<size_t>(length - position));
      blockSize = std::min(blockSize, static_cast<size_t>(4096));

      if (blockSize) {
        ESP_LOGD(TAG, "Reading flash: offset=%06x, length=%u", blockStart, blockSize);
        Flash.readBlock(blockStart, buffer, blockSize);
      }

      return blockSize;
    };

    req->send("application/octet-stream", length, callback);
    return;
  }

EMPTY:
#ifdef LT_BANNER_STR
  req->send(200, "text/plain", LT_BANNER_STR);
#else
  req->send(200, "text/plain", "LibreTuya " LT_VERSION_STR);
#endif
}

}  // namespace hub_api
}  // namespace esphome

#endif  // USE_ARDUINO
