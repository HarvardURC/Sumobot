#include "tof_sensors.hpp"
#include "hardware_config.hpp"
#include <Arduino.h>

TofArray::TofArray() {
    sensor_pins_ = {pins::tof_left,
                    pins::tof_leftdiag,
                    pins::tof_front,
                    pins::tof_right,
                    pins::tof_rightdiag};

    sensors_ = {new VL53L0X, new VL53L0X, new VL53L0X, new VL53L0X, new VL53L0X};
    sensor_names_ = {"left", "left diag", "front", "right diag, right"};

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

std::vector<int16_t> TofArray::readAll() {
    std::vector<int16_t> sensor_readings{};
    for (uint8_t i = 0; i < sensors_.size(); i++) {
        int16_t reading = sensors_[i]->readRangeSingleMillimeters();
        sensor_readings.push_back(reading);
    }
    return sensor_readings;
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
