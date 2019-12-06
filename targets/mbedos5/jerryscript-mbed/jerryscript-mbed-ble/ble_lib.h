#ifndef JERRYSCRIPT_MBED_BLE_BLE_LIB_H
#define JERRYSCRIPT_MBED_BLE_BLE_LIB_H

#include "ble/BLE.h"

#include "jerryscript-mbed-util/wrappers.h"
#include "jerryscript-mbed-ble/GattCharacteristic-js.h"
#include "jerryscript-mbed-ble/GattService-js.h"
#include "jerryscript-mbed-ble/Gap-js.h"
#include "jerryscript-mbed-ble/BLE-js.h"

DECLARE_JS_WRAPPER_REGISTRATION(ble) {
    Init_GattCharacteristic();
    Init_GattService();
    Init_Gap();
    Init_BLE();
}

#endif // JERRYSCRIPT_MBED_BLE_BLE_LIB_H
