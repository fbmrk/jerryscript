var SERVICE_UUID = 0xAA00;
var CHARACTERISTIC_UUID = 0xAA0F;

var value = 2;
var valueChar = GattCharacteristic(CHARACTERISTIC_UUID, value,
    GattCharacteristic.BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic.BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic.BLE_GATT_CHAR_PROPERTIES_NOTIFY);
var service = GattService(SERVICE_UUID, valueChar);
var ble = BLE.Instance();

function set_dev_name() {
    str= "ble_js";
    var buf = new ArrayBuffer(str.length);
    var name = new Uint8Array(buf);
    for (var i=0; i < str.length; i++) {
        name[i] = str.charCodeAt(i);
    }

    ble.gap.accumulateAdvertisingPayload(
        GapAdvertisingData.COMPLETE_LOCAL_NAME,
        name, str.length);
}

function valueIncrement() {
    value++;
    ble.gattServer.write(valueChar, value);
}

function data_write_cb(data) {
    print("Recived Data!");
    print(" Data: ", data);

    value = data;
    ble.gattServer.write(valueChar, value);
}

function add_counter_service() {
    ble.gattServer.addService(service);
    ble.gattServer.onDataWritten(data_write_cb);

    setInterval(valueIncrement, 600);

    var buf = new ArrayBuffer(2);
    var service_uuid_list = new Uint16Array(buf);
    service_uuid_list[0] = SERVICE_UUID;

    ble.gap.accumulateAdvertisingPayload(
        GapAdvertisingData.COMPLETE_LIST_16BIT_SERVICE_IDS,
        service_uuid_list, 2);
}

function init_cb() {
    set_dev_name();
    add_counter_service();

    ble.gap.accumulateAdvertisingPayload(
        GapAdvertisingData.BREDR_NOT_SUPPORTED | GapAdvertisingData.LE_GENERAL_DISCOVERABLE);

    ble.gap.setAdvertisingType(GapAdvertisingParams.ADV_CONNECTABLE_UNDIRECTED);
    ble.gap.setAdvertisingInterval(1000);
    ble.gap.startAdvertising();
}

ble.init(init_cb);

print("ble.js has finished executing.");
