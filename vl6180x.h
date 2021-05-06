#include "esphome.h"
#include "VL6180X.h"
#include "Wire.h"

VL6180X sensor1;
using namespace esphome;

class MyCustomSensor : public PollingComponent, public Sensor {

public:

Sensor *dist_sensor = new Sensor();
Sensor *ambient_sensor = new Sensor();
MyCustomSensor() : PollingComponent(1000) {}
float get_setup_priority() const override { return esphome::setup_priority::BUS; }

void setup() override {
    Wire.begin();
    sensor1.init();
    sensor1.configureDefault();
    sensor1.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
    sensor1.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
    sensor1.setTimeout(500);
    sensor1.stopContinuous();
    delay(300);
    sensor1.startInterleavedContinuous(100);
}
void update() override {
    int dist = sensor1.readRangeSingleMillimeters();
    dist_sensor->publish_state(dist);

    int ambient = sensor1.readAmbientContinuous();
    ambient_sensor->publish_state(ambient);
}
};
