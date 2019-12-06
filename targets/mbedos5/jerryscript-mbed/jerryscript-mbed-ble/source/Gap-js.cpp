#include "ble/BLE.h"

#include "jerryscript-mbed-ble/Gap-js.h"
#include "jerryscript-mbed-ble/utils.h"

jerry_value_t gap_object;

jerry_value_t Gap_accumulateAdvertisingPayload_handler (const jerry_value_t function_obj,
                                                        const jerry_value_t this_val,
                                                        const jerry_value_t args_p[],
                                                        const jerry_length_t args_cnt)
{
  BLE &ble = BLE::Instance();

  GapAdvertisingData::DataType type = (GapAdvertisingData::DataType)jerry_get_number_value (args_p[0]);

  if (args_cnt == 1)
  {
    ble.gap().accumulateAdvertisingPayload(type);
    return jerry_create_undefined();
  }

  jerry_length_t byteLength = 0;
  jerry_length_t byteOffset = 0;
  jerry_value_t buffer = jerry_get_typedarray_buffer (args_p[1], &byteOffset, &byteLength);
  uint8_t *data = new uint8_t[byteLength];
  jerry_arraybuffer_read (buffer, byteOffset, (uint8_t*)data, byteLength);

  uint8_t len = (uint8_t)jerry_get_number_value (args_p[2]);

  ble.gap().accumulateAdvertisingPayload(type, data, len);
  delete[] data;
 
  return jerry_create_undefined();
}

jerry_value_t Gap_setAdvertisingType_handler (const jerry_value_t function_obj,
                                              const jerry_value_t this_val,
                                              const jerry_value_t args_p[],
                                              const jerry_length_t args_cnt)
{
  BLE &ble = BLE::Instance();

  GapAdvertisingParams::AdvertisingType_t type = (GapAdvertisingParams::AdvertisingType_t)jerry_get_number_value (args_p[0]);

  ble.gap().setAdvertisingType(type);

  return jerry_create_undefined ();
}

jerry_value_t Gap_setAdvertisingInterval_handler (const jerry_value_t function_obj,
                                                  const jerry_value_t this_val,
                                                  const jerry_value_t args_p[],
                                                  const jerry_length_t args_cnt)
{
  BLE &ble = BLE::Instance();

  uint16_t interval = (uint16_t)jerry_get_number_value (args_p[0]);

  ble.gap().setAdvertisingInterval(interval);
  
  return jerry_create_undefined ();
}

jerry_value_t Gap_startAdvertising_handler (const jerry_value_t function_obj,
                                            const jerry_value_t this_val,
                                            const jerry_value_t args_p[],
                                            const jerry_length_t args_cnt)
{
  BLE &ble = BLE::Instance();

  ble.gap().startAdvertising();
  
  return jerry_create_undefined ();
}

void Init_Gap()
{
  gap_object = jerry_create_object();

  REGIST_FUNC(gap_object, accumulateAdvertisingPayload, Gap_accumulateAdvertisingPayload_handler);
  REGIST_FUNC(gap_object, setAdvertisingType, Gap_setAdvertisingType_handler);
  REGIST_FUNC(gap_object, setAdvertisingInterval, Gap_setAdvertisingInterval_handler);
  REGIST_FUNC(gap_object, startAdvertising, Gap_startAdvertising_handler);

  jerry_value_t gap_advertising_data = jerry_create_object();

  REGIST_ENUM(gap_advertising_data, COMPLETE_LIST_16BIT_SERVICE_IDS,
              GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS);
  REGIST_ENUM(gap_advertising_data, COMPLETE_LOCAL_NAME,
              GapAdvertisingData::COMPLETE_LOCAL_NAME);
  REGIST_ENUM(gap_advertising_data, BREDR_NOT_SUPPORTED,
              GapAdvertisingData::BREDR_NOT_SUPPORTED);
  REGIST_ENUM(gap_advertising_data, LE_GENERAL_DISCOVERABLE,
              GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
 
  jerry_value_t gap_advertising_params = jerry_create_object();

  REGIST_ENUM(gap_advertising_params, ADV_CONNECTABLE_UNDIRECTED,
              GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);

  jerry_value_t global = jerry_get_global_object();
  jerry_value_t name = jerry_create_string ((const jerry_char_t *)"GapAdvertisingData");
  jerry_value_t ret = jerry_set_property (global, name, gap_advertising_data);
  jerry_release_value(name);
  jerry_release_value(ret);
  name = jerry_create_string ((const jerry_char_t *)"GapAdvertisingParams");
  ret = jerry_set_property (global, name, gap_advertising_params);
  jerry_release_value(global);
  jerry_release_value(name);
  jerry_release_value(ret);
}
