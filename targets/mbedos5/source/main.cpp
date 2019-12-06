#include "jerryscript-mbed-launcher/launcher.h"
#include "jerryscript-mbed-library-registry/registry.h"
#include "jerryscript-mbed-drivers/lib_drivers.h"
#include "jerryscript-mbed-event-loop/EventLoop.h"
#include "jerryscript-mbed-ble/ble_lib.h"
#include "Callback.h"
#include "Serial.h"

using mbed::js::EventLoop;
using mbed::Callback;

static void ble_ScheduleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context)
{
    BLE& ble = BLE::Instance();
    EventLoop::getInstance().getQueue().call(Callback<void()>(&ble, &BLE::processEvents));
}

int main() {
    mbed::Serial pc(USBTX, USBRX);
    pc.baud(9600);

    JERRY_USE_MBED_LIBRARY(base);
    JERRY_USE_MBED_LIBRARY(ble);

    BLE& ble = BLE::Instance();
    ble.onEventsToProcess(ble_ScheduleEventsProcessing);

    jsmbed_js_launch();

    return 0;
}
