#include "ble/BLE.h"

#include "jerryscript-mbed-ble/GattService-js.h"
#include "jerryscript-mbed-ble/GattCharacteristic-js.h"
#include "jerryscript-mbed-ble/utils.h"

void GattService_js_destructor(void* ptr) {
    delete (GattService*)ptr;
}

void delete_char_table(void* ptr) {
    delete (GattCharacteristic**)ptr;
}

const jerry_object_native_info_t GattService_char_table_type_info = {
    .free_cb = delete_char_table
};

const jerry_object_native_info_t GattService_type_info = {
    .free_cb = GattService_js_destructor
};

jerry_value_t GattService_js_constructor (const jerry_value_t function_obj,
                                          const jerry_value_t this_val,
                                          const jerry_value_t args_p[],
                                          const jerry_length_t args_cnt)
{
  GattService* gattService;
 
  uint16_t uuid = (uint16_t)jerry_get_number_value (args_p[0]);

  void *native_p;
  bool has_ptr = jerry_get_object_native_pointer (args_p[1], &native_p, &GattCharacteristic_type_info);

  if (!has_ptr)
  {
    char const *msg = "Failed to get native GattCharacteristic pointer";
    return jerry_create_error(JERRY_ERROR_TYPE, (const jerry_char_t *)msg);
  }

  GattCharacteristic** gattChar = new GattCharacteristic*;
  *gattChar = (GattCharacteristic*)native_p;

  gattService = new GattService(uuid, gattChar, 1);

  jerry_value_t gattService_object = jerry_create_object();
  jerry_set_object_native_pointer(gattService_object, gattChar, &GattService_char_table_type_info);
  jerry_set_object_native_pointer(gattService_object, gattService, &GattService_type_info);
  return gattService_object;
}

void Init_GattService()
{
  jerry_value_t global = jerry_get_global_object();
  REGIST_FUNC(global, GattService, GattService_js_constructor);
  jerry_release_value(global);
}
