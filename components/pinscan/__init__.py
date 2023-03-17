import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, text_sensor
from esphome.const import CONF_ID, CONF_PIN, CONF_MODE
from esphome import automation

AUTO_LOAD = ["text_sensor"]

pinscan_ns = cg.esphome_ns.namespace("pinscan")

PinscanComponent = pinscan_ns.class_("Pinscan", cg.Component, cg.PollingComponent)
SetPinAction = pinscan_ns.class_("SetPinAction", automation.Action)
SetModeAction = pinscan_ns.class_("SetModeAction", automation.Action)

CONF_PIN_STATE_SENSOR = "pin_state_sensor"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(PinscanComponent),
        cv.Optional(CONF_PIN_STATE_SENSOR): text_sensor.text_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    if CONF_PIN_STATE_SENSOR in config:
        sens = await text_sensor.new_text_sensor(config[CONF_PIN_STATE_SENSOR])
        cg.add(var.set_pin_state_sensor(sens))


@automation.register_action(
    "pinscan.set_pin",
    SetPinAction,
    automation.maybe_simple_id(
        cv.Schema(
            {
                cv.Required(CONF_ID): cv.use_id(PinscanComponent),
                cv.Required(CONF_PIN): cv.templatable(cv.int_),
            }
        )
    ),
)
async def pinscan_set_pin_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_PIN], args, cg.int_)
    cg.add(var.set_pin(template_))
    return var


@automation.register_action(
    "pinscan.set_mode",
    SetModeAction,
    automation.maybe_simple_id(
        cv.Schema(
            {
                cv.Required(CONF_ID): cv.use_id(PinscanComponent),
                cv.Required(CONF_MODE): cv.templatable(cv.int_),
            }
        )
    ),
)
async def pinscan_set_mode_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_MODE], args, cg.int_)
    cg.add(var.set_mode(template_))
    return var
