#include "ble/BLE.h"

#include "jerryscript-mbed-ble/GattCharacteristic-js.h"
#include "jerryscript-mbed-ble/utils.h"

void GattCharacteristic_js_destructor(void* ptr)
{
  delete (GattCharacteristic*)ptr;
}

void delete_value(void* ptr)
{
  delete (uint8_t*)ptr;
}

const jerry_object_native_info_t GattCharacteristic_type_info = {
  .free_cb = GattCharacteristic_js_destructor
};

const jerry_object_native_info_t GattCharacteristic_value_type_info = {
  .free_cb = delete_value
};

jerry_value_t GattCharacteristic_js_constructor (const jerry_value_t function_obj,
                                                 const jerry_value_t this_val,
                                                 const jerry_value_t args_p[],
                                                 const jerry_length_t args_cnt)
{
  GattCharacteristic* gattChar;

  uint16_t uuid = (uint16_t)jerry_get_number_value (args_p[0]);

  uint8_t* valuePtr = new uint8_t;
  *valuePtr = (uint8_t)jerry_get_number_value (args_p[1]);

  uint8_t props = (uint8_t)jerry_get_number_value (args_p[2]);
  
  gattChar = new GattCharacteristic(uuid, valuePtr, sizeof(uint8_t), sizeof(uint8_t), props);

  jerry_value_t gattChar_object = jerry_create_object();
  jerry_set_object_native_pointer(gattChar_object, gattChar, &GattCharacteristic_type_info);
  jerry_set_object_native_pointer(gattChar_object, valuePtr, &GattCharacteristic_value_type_info);
  return gattChar_object;
}

void Init_GattCharacteristic()
{
  jerry_value_t global = jerry_get_global_object();
  jerry_value_t name = jerry_create_string ((const jerry_char_t*) "GattCharacteristic");
  jerry_value_t func = jerry_create_external_function (GattCharacteristic_js_constructor);
  REGIST_ENUM(func, BLE_GATT_CHAR_PROPERTIES_READ,
              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
  REGIST_ENUM(func, BLE_GATT_CHAR_PROPERTIES_WRITE,
              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);
  REGIST_ENUM(func, BLE_GATT_CHAR_PROPERTIES_NOTIFY,
              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);
  jerry_release_value (jerry_set_property (global, name, func));
  jerry_release_value(func);
  jerry_release_value(name);
  jerry_release_value(global);
}
