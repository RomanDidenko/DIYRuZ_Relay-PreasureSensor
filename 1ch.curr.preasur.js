const zigbeeHerdsmanConverters = require('zigbee-herdsman-converters');

const exposes = zigbeeHerdsmanConverters.exposes;
const ea = exposes.access;
const e = exposes.presets;
const fz = zigbeeHerdsmanConverters.fromZigbeeConverters;
const tz = zigbeeHerdsmanConverters.toZigbeeConverters;

const ptvo_switch = zigbeeHerdsmanConverters.findByDevice({modelID: 'ptvo.switch'});
fz.legacy = ptvo_switch.meta.tuyaThermostatPreset;

const device = {
    zigbeeModel: ['1ch.curr.preasur'],
    model: '1ch.curr.preasur',
    vendor: 'Custom devices (DiY)',
    description: '[Configurable firmware](https://ptvo.info/zigbee-configurable-firmware-features/)',
    fromZigbee: [fz.ignore_basic_report, fz.ptvo_switch_analog_input, fz.on_off,],
    toZigbee: [tz.ptvo_switch_trigger, tz.ptvo_switch_analog_input, tz.on_off,],
    exposes: [e.current().withAccess(ea.STATE_SET).withEndpoint('l1'),
      e.switch().withEndpoint('l2'),
      e.voltage().withAccess(ea.STATE).withValueMin(0).withValueMax(1.2).withEndpoint('l4'),
      e.cpu_temperature().withProperty('temperature').withEndpoint('l5'),
],
    meta: {
        multiEndpoint: true,
        
    },
    endpoint: (device) => {
        return {
            l1: 1, l2: 2, l4: 4, l5: 5,
        };
    },
    
};

module.exports = device;
