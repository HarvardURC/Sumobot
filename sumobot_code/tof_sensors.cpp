#include "tof_sensors.hpp"
#include "hardware_config.hpp"
#include <Arduino.h>

// TODO: need to tune
#define TOF_THRESHOLD 250

// This hack is to allow the vector library to be used with Teensy
namespace std {
    void __throw_bad_alloc() {
        Serial.println("Unable to allocate memory");
    }
    void __throw_length_error(char const* e) {
        Serial.print("Length Error :");
        Serial.println(e);
    }
}   // namespace std

TofArray::TofArray() {
    sensor_pins_ = {pins::tof_left,
                    pins::tof_leftdiag,
                    pins::tof_front,
                    pins::tof_right,
                    pins::tof_rightdiag};

    sensors_ = {new VL53L0X, new VL53L0X, new VL53L0X, new VL53L0X, new VL53L0X};
    sensor_names_ = {"left", "left diag", "front", "right diag, right"};
    sensor_readings_ = {32767, 32767, 32767, 32767, 32767};

    for (uint8_t i = 0; i < sensor_pins_.size(); i++) {
        pinMode(sensor_pins_[i], OUTPUT);
        digitalWrite(sensor_pins_[i], LOW);
    }

    delay(50);
    Wire.begin();

    // Set sensor addresses
    for (uint8_t i = 0; i < sensor_pins_.size(); i++) {
        digitalWrite(sensor_pins_[i], HIGH);
        delay(50);
        sensors_[i]->setAddress(2 * i);
        // Uncomment to debug addresses of sensors
        // Serial.println(sensors[i]->readReg(0x212));
    }
    delay(50);

    // Initializes sensors
    for (uint8_t i = 0; i < sensor_pins_.size(); i++) {
        initSensor_(i);
    }

    Serial.println("All ToF sensors initialized.\n");
}

void TofArray::readAllToSerial() {
    for (uint8_t i = 0; i < sensors_.size(); i++) {
        Serial.print(sensor_names_[i]);
        Serial.print(": ");
        Serial.print(sensors_[i]->readRangeSingleMillimeters());
        Serial.print(" ");
        if (sensors_[i]->timeoutOccurred()) {
            Serial.print(" TIMEOUT");
        }
    }
}

void TofArray::readAll() {
    for (uint8_t i = 0; i < sensors_.size(); i++) {
        sensor_readings_[i] = sensors_[i]->readRangeSingleMillimeters();
    }
}
bool TofArray::objectVisible(TofSensorIndex sensor_index) {
    return sensor_readings_[sensor_index] <= TOF_THRESHOLD ? true : false;
}

int16_t TofArray::readTof(TofSensorIndex sensor_index) {
    VL53L0X* sensor = sensors_[sensor_index];
    int16_t reading = sensor->readRangeSingleMillimeters();
    return reading;
}

void TofArray::initSensor_(uint8_t index) {
    VL53L0X* sensor = sensors_[index];
    sensor->init();
    sensor->setTimeout(500);
    // Better accuracy, lower speed
    sensor->setMeasurementTimingBudget(200000);
    Serial.print(sensor_names_[index]);
    Serial.println(" online.");
    delay(200);
}
