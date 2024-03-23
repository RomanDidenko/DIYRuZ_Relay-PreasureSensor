const zigbeeHerdsmanConverters = require('zigbee-herdsman-converters');

const exposes = zigbeeHerdsmanConverters.exposes;
const ea = exposes.access;
const e = exposes.presets;
const fz = zigbeeHerdsmanConverters.fromZigbeeConverters;
const tz = zigbeeHerdsmanConverters.toZigbeeConverters;

const ptvo_switch = zigbeeHerdsmanConverters.findByDevice({modelID: 'ptvo.switch'});
fz.legacy = ptvo_switch.meta.tuyaThermostatPreset;

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

            meta.logger.error(valueToSet);

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
    fromZigbee: [fz.ignore_basic_report, fz.ptvo_switch_analog_input, fz.on_off, custom_converters.from_uart,],
    toZigbee: [tz.ptvo_switch_trigger, tz.ptvo_switch_analog_input, tz.on_off, custom_converters.to_uart, tz.ptvo_switch_uart,],
    exposes: [
      e.current().withAccess(ea.STATE_SET).withEndpoint('l1'),
      e.switch().withEndpoint('l2'),
      e.switch().withEndpoint('l7').withDescription('RESTART'),
      e.voltage().withAccess(ea.STATE).withValueMin(0).withValueMax(1.2).withEndpoint('l4'),
      e.cpu_temperature().withProperty('temperature').withEndpoint('l5'),
      exposes.numeric('preassure_adc', ea.STATE).withDescription(`Preassure adc value: (0 - 1024) ~ (0V - 3.3V)`),
      //exposes.text('action', ea.STATE_SET).withDescription('data from/to UART'),
      exposes.numeric(minReportIntervalDataType, ea.ALL).withDescription(`Minimum report interval`).withUnit('ms').withValueMin(minReportIntervalInMilliseconds).withValueMax(1000000000),
      exposes.numeric(probIntervalDataType, ea.ALL).withDescription(`Prob interval`).withUnit('ms').withValueMin(minProbIntervalInMilliseconds).withValueMax(1000000),
      exposes.numeric(averageOnItemsDataType, ea.ALL).withDescription(`Number of probes to take average on`).withValueMin(minAverageOnItems).withValueMax(1000000),
      exposes.numeric(valueDeltaToForceReportDataType, ea.ALL).withDescription(`Value delta to force report`).withValueMin(minValueDeltaToForceReport).withValueMax(10000),
      exposes.text(uptimeDataType, ea.Get).withDescription(`Uptime of the probe module`),
      e.voltage().withAccess(ea.STATE).withEndpoint('l8').withDescription(`Source Volatge`).withUnit('V'),
],
    meta: {
        multiEndpoint: true,
        
    },
    endpoint: (device) => {
        return {
            l1: 1, l2: 2, l4: 4, l5: 5, l6: 6, l7: 7, l8: 8 //action: 1, 
        };
    },
    
};

module.exports = device;
