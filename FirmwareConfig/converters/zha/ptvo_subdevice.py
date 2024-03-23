    @MULTI_MATCH(
        cluster_handler_names=CLUSTER_NAME_PTVO_ANALOG_INPUT_VALUE2 + "_endpoint_",
        models={PTVO_MODEL_ID},
    )
    class PtvoAnalogInputSensorEp_endpoint_Attr_attr_dec_(Sensor):
        _attr_name = "_attr_description_ _endpoint_"
        _attribute_name = "_attr_value_id_"
        _attr_translation_key = "_attr_value_id_"
        _unique_id_suffix = "_attr_value_id__ep_endpoint_"
        _attr_icon = "_attr_icon_"
        _attr_device_class = _attr_device_class_
        _attr_state_class = _attr_state_class_
        _attr_native_unit_of_measurement = "_attr_unit_"
