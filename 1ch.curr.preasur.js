const zigbeeHerdsmanConverters = require('zigbee-herdsman-converters');
const zigbeeHerdsmanUtils = require('zigbee-herdsman-converters/lib/utils');


const exposes = zigbeeHerdsmanConverters['exposes'] || require("zigbee-herdsman-converters/lib/exposes");
const ea = exposes.access;
const e = exposes.presets;
const fz = zigbeeHerdsmanConverters.fromZigbeeConverters || zigbeeHerdsmanConverters.fromZigbee;
const tz = zigbeeHerdsmanConverters.toZigbeeConverters || zigbeeHerdsmanConverters.toZigbee;

const ptvo_switch = (zigbeeHerdsmanConverters.findByModel)?zigbeeHerdsmanConverters.findByModel('ptvo.switch'):zigbeeHerdsmanConverters.findByDevice({modelID: 'ptvo.switch'});
fz.legacy = ptvo_switch.meta.tuyaThermostatPreset;
fz.ptvo_on_off = {
  cluster: 'genOnOff',
  type: ['attributeReport', 'readResponse'],
  convert: (model, msg, publish, options, meta) => {
      if (msg.data.hasOwnProperty('onOff')) {
          const channel = msg.endpoint.ID;
          const endpointName = `l${channel}`;
          const binaryEndpoint = model.meta && model.meta.binaryEndpoints && model.meta.binaryEndpoints[endpointName];
          const prefix = (binaryEndpoint) ? model.meta.binaryEndpoints[endpointName] : 'state';
          const property = `${prefix}_${endpointName}`;
	  if (binaryEndpoint) {
            return {[property]: msg.data['onOff'] === 1};
          }
          return {[property]: msg.data['onOff'] === 1 ? 'ON' : 'OFF'};
      }
  },
};


const switchTypesList = {
    'switch': 0x00,
    'single click': 0x01,
    'multi-click': 0x02,
    'reset to defaults': 0xff,
};

const switchActionsList = {
    on: 0x00,
    off: 0x01,
    toggle: 0x02,
};

const inputLinkList = {
    no: 0x00,
    yes: 0x01,
};

const bindCommandList = {
    'on/off': 0x00,
    'toggle': 0x01,
    'change level up': 0x02,
    'change level down': 0x03,
    'change level up with off': 0x04,
    'change level down with off': 0x05,
    'recall scene 0': 0x06,
    'recall scene 1': 0x07,
    'recall scene 2': 0x08,
    'recall scene 3': 0x09,
    'recall scene 4': 0x0A,
    'recall scene 5': 0x0B,
    'dimmer': 0x0C,
    'dimmer (hue)': 0x0D,
    'dimmer (saturation)': 0x0E,
    'dimmer (color temperature)': 0x0F,
    'intruder alarm systems (ias)': 0x20,
};

function getSortedList(source) {
    const keysSorted = [];
    for (const key in source) {
        keysSorted.push([key, source[key]]);
    }

    keysSorted.sort(function(a, b) {
        return a[1] - b[1];
    });

    const result = [];
    keysSorted.forEach((item) => {
        result.push(item[0]);
    });
    return result;
}

function getListValueByKey(source, value) {
    const intVal = parseInt(value, 10);
    return source.hasOwnProperty(value) ? source[value] : intVal;
}

const getKey = (object, value) => {
    for (const key in object) {
        if (object[key] == value) return key;
    }
};

tz.ptvo_on_off_config = {
    key: ['switch_type', 'switch_actions', 'link_to_output', 'bind_command'],
    convertGet: async (entity, key, meta) => {
        await entity.read('genOnOffSwitchCfg', ['switchType', 'switchActions', 0x4001, 0x4002]);
    },
    convertSet: async (entity, key, value, meta) => {
        let payload;
        let data;
        switch (key) {
        case 'switch_type':
            data = getListValueByKey(switchTypesList, value);
            payload = {switchType: data};
            break;
        case 'switch_actions':
            data = getListValueByKey(switchActionsList, value);
            payload = {switchActions: data};
            break;
        case 'link_to_output':
            data = getListValueByKey(inputLinkList, value);
            payload = {0x4001: {value: data, type: 32 /* uint8 */}};
            break;
        case 'bind_command':
            data = getListValueByKey(bindCommandList, value);
            payload = {0x4002: {value: data, type: 32 /* uint8 */}};
            break;
        }
        await entity.write('genOnOffSwitchCfg', payload);
    },
};

fz.ptvo_on_off_config = {
    cluster: 'genOnOffSwitchCfg',
    type: ['readResponse', 'attributeReport'],
    convert: (model, msg, publish, options, meta) => {
        const channel = getKey(model.endpoint(msg.device), msg.endpoint.ID);
        const {switchActions, switchType} = msg.data;
        const inputLink = msg.data[0x4001];
        const bindCommand = msg.data[0x4002];
        return {
            [`switch_type_${channel}`]: getKey(switchTypesList, switchType),
            [`switch_actions_${channel}`]: getKey(switchActionsList, switchActions),
            [`link_to_output_${channel}`]: getKey(inputLinkList, inputLink),
            [`bind_command_${channel}`]: getKey(bindCommandList, bindCommand),
        };
    },
};

function ptvo_on_off_config_exposes(epName) {
    const features = [];
    features.push(exposes.enum('switch_type', exposes.access.ALL,
        getSortedList(switchTypesList)).withEndpoint(epName));
    features.push(exposes.enum('switch_actions', exposes.access.ALL,
        getSortedList(switchActionsList)).withEndpoint(epName));
    features.push(exposes.enum('link_to_output', exposes.access.ALL,
        getSortedList(inputLinkList)).withEndpoint(epName));
    features.push(exposes.enum('bind_command', exposes.access.ALL,
        getSortedList(bindCommandList)).withEndpoint(epName));
    return features;
}


const minReportIntervalDataType = 'report_interval';
const minReportIntervalInMilliseconds = 100;

const probIntervalDataType = 'prob_interval';
const minProbIntervalInMilliseconds = 1;

const averageOnItemsDataType = 'average_on_items';
const minAverageOnItems = 1;

const valueDeltaToForceReportDataType = 'delta';
const minValueDeltaToForceReport = 1;

const uptimeDataType = 'uptime';

const allDataTypes = [minReportIntervalDataType, probIntervalDataType, averageOnItemsDataType, valueDeltaToForceReportDataType, uptimeDataType];

const requestValuesUpdate = async (entity, device, valueType, logger) => {
    const value = allDataTypes.some(d => d === valueType) ? 'state;' : 'help-json;';
    const payload = {14: {value, type: 0x42}};

    for (const endpoint of device.endpoints) {
        const cluster = 'genMultistateValue';
        if (endpoint.supportsInputCluster(cluster) || endpoint.supportsOutputCluster(cluster)) {
            await endpoint.write(cluster, payload);
            return;
        }
    }

    await entity.write('genMultistateValue', payload);
}


const custom_converters = {
    from_uart: {
        cluster: 'genMultistateValue',
        type: ['attributeReport', 'readResponse'],
        //options: [exposes.numeric(minReportIntervalDataType, ea.STATE_SET).withDescription(`Poll interval (minimum is ${minReportIntervalInMilliseconds} seconds)`).withUnit('seconds')], //check this options, not tested
        convert: (model, msg, publish, options, meta) => {
            var response = fz.ptvo_switch_uart.convert(model, msg, publish, options, meta);

            if (typeof response.action === 'string' && response.action) {
                if(/^\d+$/.test(response.action)) {
                    return {'preassure_adc': response.action};
                }

                if(response.action[0] == '{' && response.action[response.action.length - 1] == '}') {
                    const state = JSON.parse(response.action);

                    return state;
                }
            }

            return response;
        },
    },
    to_uart: {
        key: allDataTypes,
        //options: [exposes.numeric(minReportIntervalDataType, ea.STATE_SET).withDescription(`Poll interval (minimum is ${minReportIntervalInMilliseconds} seconds)`).withUnit('seconds')],
        convertGet: async (entity, key, meta) => {
            await requestValuesUpdate(entity, meta.device, key, meta.logger);
        },
        convertSet: async (entity, key, value, meta) => {
            if(key !== minReportIntervalDataType && key !== probIntervalDataType && key !== averageOnItemsDataType && key !== valueDeltaToForceReportDataType){
                return;
            }

            var valueToSet = null;

            if(key === minReportIntervalDataType) {
                if (typeof value === 'string' && isNaN(value)) {
                    throw new Error(`Invalid min report interval '${value}'`);
                }

                if (value < minReportIntervalInMilliseconds) {
                    throw new Error(`Min report interval should be not less than ${minReportIntervalInMilliseconds} ms but '${value}' ms is provided`);
                }

                valueToSet =`ri ${value};`;
            }
            else if(key === probIntervalDataType) {
                if (typeof value === 'string' && isNaN(value)) {
                    throw new Error(`Invalid prob interval '${value}'`);
                }

                if (value < minProbIntervalInMilliseconds) {
                    throw new Error(`Prob interval should be not less than ${minProbIntervalInMilliseconds} ms but '${value}' ms is provided`);
                }

                valueToSet =`pi ${value};`;
            }
            else if(key === averageOnItemsDataType) {
                if (typeof value === 'string' && isNaN(value)) {
                    throw new Error(`Invalid number of probes to take average on '${value}'`);
                }

                if (value < minAverageOnItems) {
                    throw new Error(`Number of probes to take average on should be not less than ${minAverageOnItems} but '${value}' is provided`);
                }

                valueToSet =`rn ${value};`;
            }
            else if(key === valueDeltaToForceReportDataType) {
                if (typeof value === 'string' && isNaN(value)) {
                    throw new Error(`Invalid value delta to force report '${value}'`);
                }

                if (value < minValueDeltaToForceReport) {
                    throw new Error(`Value delta to force report should be not less than ${minValueDeltaToForceReport} but '${value}' is provided`);
                }

                valueToSet =`d ${value};`;
            }
            else {
                valueToSet = 'state;';
            }

            meta.logger.info(valueToSet);

            const payload = {14: {value: valueToSet, type: 0x42}};

            for (const endpoint of meta.device.endpoints) {
                const cluster = 'genMultistateValue';
                if (endpoint.supportsInputCluster(cluster) || endpoint.supportsOutputCluster(cluster)) {
                    await endpoint.write(cluster, payload);
                    return;
                }
            }
            await entity.write('genMultistateValue', payload);
        },
    },
}

const device = {
    zigbeeModel: ['1ch.curr.preasur'],
    model: '1ch.curr.preasur',
    vendor: 'Custom devices (DiY)',
    description: '[Configurable firmware](https://ptvo.info/zigbee-configurable-firmware-features/)',
    fromZigbee: [fz.ignore_basic_report, fz.ptvo_switch_analog_input, fz.ptvo_on_off, fz.temperature, fz.ptvo_switch_uart, fz.ptvo_multistate_action, fz.legacy.ptvo_switch_buttons, fz.ptvo_on_off_config,],
    toZigbee: [tz.ptvo_switch_trigger, tz.ptvo_switch_analog_input, tz.on_off, tz.ptvo_switch_uart, tz.ptvo_on_off_config,],
    exposes: [
      exposes.numeric('current', ea.STATE_SET).withEndpoint('l1').withUnit('A').withDescription('Instantaneous measured electrical current'),
      e.switch().withEndpoint('l2'),
        e.switch().withEndpoint('l7').withDescription('RESTART: switch off and then on to reset Prob module'),
      e.voltage().withAccess(ea.STATE).withValueMin(0).withValueMax(1.2).withEndpoint('l4'),
      e.cpu_temperature().withProperty('temperature').withEndpoint('l5'),
        exposes.numeric('preassure_adc', ea.STATE).withDescription(`Preassure adc value: (0 - 1024) ~ (0V - 3.3V)`),
      //exposes.text('action', ea.STATE_SET).withDescription('button clicks or data from/to UART'),
      exposes.numeric(minReportIntervalDataType, ea.ALL).withDescription(`Minimum report interval`).withUnit('ms').withValueMin(minReportIntervalInMilliseconds).withValueMax(1000000000),
      exposes.numeric(probIntervalDataType, ea.ALL).withDescription(`Prob interval`).withUnit('ms').withValueMin(minProbIntervalInMilliseconds).withValueMax(1000000),
      exposes.numeric(averageOnItemsDataType, ea.ALL).withDescription(`Number of probes to take average on`).withValueMin(minAverageOnItems).withValueMax(1000000),
      exposes.numeric(valueDeltaToForceReportDataType, ea.ALL).withDescription(`Value delta to force report`).withValueMin(minValueDeltaToForceReport).withValueMax(10000),
      exposes.text(uptimeDataType, ea.Get).withDescription(`Uptime of the probe module`),
      ...ptvo_on_off_config_exposes('l2'),
      ...ptvo_on_off_config_exposes('l7'),
],
    meta: {
        multiEndpoint: true,
        
    },
    endpoint: (device) => {
        return {
            l1: 1, l2: 2, l4: 4, l5: 5, l6: 6, action: 1, l7: 7,
        };
    },
    configure: async (device, coordinatorEndpoint, logger) => {
            const endpoint = device.getEndpoint(1);
      await endpoint.read('genBasic', ['modelId', 'swBuildId', 'powerSource']);
    },

};

module.exports = device;
