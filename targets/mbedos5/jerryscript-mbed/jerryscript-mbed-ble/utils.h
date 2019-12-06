#ifndef JERRYSCRIPT_MBED_BLE_UTILS_H
#define JERRYSCRIPT_MBED_BLE_UTILS_H

#define REGIST_FUNC(OBJ, NAME, FUNC)                                       \
do {                                                                       \
  jerry_value_t name = jerry_create_string ((const jerry_char_t*) #NAME);  \
  jerry_value_t func = jerry_create_external_function (FUNC);              \
  jerry_value_t ret = jerry_set_property (OBJ, name, func);                \
  jerry_release_value (name);                                              \
  jerry_release_value (func);                                              \
  jerry_release_value (ret);                                               \
} while (0)

#define REGIST_ENUM(OBJ, NAME, VALUE)                                       \
do {                                                                        \
  jerry_property_descriptor_t prop_desc;                                    \
  jerry_init_property_descriptor_fields (&prop_desc);                       \
  prop_desc.is_value_defined = true;                                        \
  prop_desc.value = jerry_create_number (VALUE);                            \
  jerry_value_t name = jerry_create_string ((const jerry_char_t *) #NAME);  \
  jerry_value_t ret = jerry_define_own_property (OBJ, name, &prop_desc);    \
  jerry_release_value (name);                                               \
  jerry_release_value (ret);                                                \
  jerry_free_property_descriptor_fields (&prop_desc);                       \
} while (0)

#endif // JERRYSCRIPT_MBED_BLE_UTILS_H