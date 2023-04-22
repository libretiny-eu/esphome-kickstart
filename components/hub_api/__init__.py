import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID
from esphome.components.web_server_base import CONF_WEB_SERVER_BASE_ID
from esphome.components import web_server_base

DEPENDENCIES = ["libretuya"]
AUTO_LOAD = ["web_server_base"]

hub_api_ns = cg.esphome_ns.namespace("hub_api")
HubAPIHandler = hub_api_ns.class_("HubAPI", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HubAPIHandler),
        cv.GenerateID(CONF_WEB_SERVER_BASE_ID): cv.use_id(
            web_server_base.WebServerBase
        ),
    },
    cv.only_with_arduino,
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_WEB_SERVER_BASE_ID])

    var = cg.new_Pvariable(config[CONF_ID], paren)
    await cg.register_component(var, config)
