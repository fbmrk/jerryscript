#include "ble/BLE.h"
#include "jerryscript.h"

#include "jerryscript-mbed-ble/BLE-js.h"
#include "jerryscript-mbed-ble/Gap-js.h"
#include "jerryscript-mbed-ble/GattService-js.h"
#include "jerryscript-mbed-ble/GattCharacteristic-js.h"
#include "jerryscript-mbed-ble/utils.h"

jerry_value_t ble_object;
jerry_value_t gattServer_object;

jerry_value_t BLE_addService_handler (const jerry_value_t function_obj,
                                      const jerry_value_t this_val,
                                      const jerry_value_t args_p[],
                                      const jerry_length_t args_cnt)
{
  void* native_p;
  bool has_ptr = jerry_get_object_native_pointer(args_p[0], &native_p, &GattService_type_info);

  if (!has_ptr) {
    char const *msg = "Failed to get native GattService pointer";
    return jerry_create_error(JERRY_ERROR_TYPE, (const jerry_char_t *)msg);
  }

  GattService* gattService = (GattService*)native_p;

  BLE &ble = BLE::Instance();

  ble.gattServer().addService(*gattService);

  return jerry_create_undefined ();
}

jerry_value_t onDataWritten_cb_js;
void onDataWritten_cb (const GattWriteCallbackParams* params)
{
  jerry_value_t this_val = jerry_create_undefined();
  jerry_value_t data = jerry_create_number(params->data[0]);
  jerry_call_function(onDataWritten_cb_js, this_val, &data, 1);
  jerry_release_value(this_val);
}

jerry_value_t BLE_onDataWritten_handler (const jerry_value_t function_obj,
                                         const jerry_value_t this_val,
                                         const jerry_value_t args_p[],
                                         const jerry_length_t args_cnt)
{
  BLE &ble = BLE::Instance();

  onDataWritten_cb_js = args_p[0];
  ble.gattServer().onDataWritten(onDataWritten_cb);

  return jerry_create_undefined ();
}

jerry_value_t BLE_write_handler (const jerry_value_t function_obj,
                                 const jerry_value_t this_val,
                                 const jerry_value_t args_p[],
                                 const jerry_length_t args_cnt)
{
  void* native_p;
  bool has_ptr = jerry_get_object_native_pointer(args_p[0], &native_p, &GattCharacteristic_type_info);

  if (!has_ptr) {
    char const *msg = "Failed to get native GattCharacteristic pointer";
    return jerry_create_error(JERRY_ERROR_TYPE, (const jerry_char_t *)msg);
  }

  GattCharacteristic* gattChar = (GattCharacteristic*)native_p;

  has_ptr = jerry_get_object_native_pointer(args_p[0], &native_p, &GattCharacteristic_value_type_info);

  if (!has_ptr) {
    char const *msg = "Failed to get native GattCharacteristic pointer";
    return jerry_create_error(JERRY_ERROR_TYPE, (const jerry_char_t *)msg);
  }

  uint8_t* valuePtr = (uint8_t*)native_p;
  *valuePtr = (uint8_t)jerry_get_number_value (args_p[1]);

  BLE &ble = BLE::Instance();

  ble.gattServer().write(gattChar->getValueAttribute().getHandle(), valuePtr, sizeof(uint8_t));

  return jerry_create_undefined ();
}

jerry_value_t BLE_Instance_handler (const jerry_value_t function_obj,
                                    const jerry_value_t this_val,
                                    const jerry_value_t args_p[],
                                    const jerry_length_t args_cnt)
{
  return ble_object;
}

jerry_value_t init_cb_js;
void init_cb (BLE::InitializationCompleteCallbackContext *params)
{
  jerry_value_t this_val = jerry_create_undefined();
  jerry_call_function(init_cb_js, this_val, NULL, 0);
  jerry_release_value(this_val);
}

jerry_value_t BLE_init_handler (const jerry_value_t function_obj,
                                const jerry_value_t this_val,
                                const jerry_value_t args_p[],
                                const jerry_length_t args_cnt)
{

  BLE &ble = BLE::Instance();

  init_cb_js = args_p[0];
  ble_error_t result = ble.init(init_cb);
  
  jerry_value_t ret_val = jerry_create_number (result);

  return ret_val;
}

void Init_BLE()
{
  ble_object = jerry_create_object();
  REGIST_FUNC(ble_object, init, BLE_init_handler);

  gattServer_object = jerry_create_object();
  REGIST_FUNC(gattServer_object, addService, BLE_addService_handler);
  REGIST_FUNC(gattServer_object, onDataWritten, BLE_onDataWritten_handler);
  REGIST_FUNC(gattServer_object, write, BLE_write_handler);

  jerry_value_t gatt_name = jerry_create_string ((const jerry_char_t*)"gattServer");
  jerry_release_value (jerry_set_property (ble_object, gatt_name, gattServer_object));
  jerry_release_value (gatt_name);

  jerry_value_t gap_name = jerry_create_string ((const jerry_char_t*)"gap");
  jerry_release_value (jerry_set_property (ble_object, gap_name, gap_object));
  jerry_release_value (gap_name);

  jerry_value_t ble = jerry_create_object();
  REGIST_FUNC(ble, Instance, BLE_Instance_handler);

  jerry_value_t global = jerry_get_global_object();
  jerry_value_t name = jerry_create_string ((const jerry_char_t *)"BLE");
  jerry_release_value (jerry_set_property (global, name, ble));
  jerry_release_value(global);
  jerry_release_value(name);
}
