#include <Arduino.h>
#include <VL53L0X.h>
#include <i2c_t3.h>
#include <vector>

#pragma once

enum TofSensorIndex {
    left = 0, leftDiag, front, rightDiag, right
};

/*
 * This is the interface TofArray classes must implement. We have 2 classes that
 * implement this interface, `TofArray` and `VirtualTofArray`. The TofArray
 * class will interface with the actual ToF sensors, VirtualTofArray will
 * "sense" the area in the virtual arena (the serial monitor)
 */
class TofArrayInterface {
  public:
    // Constructor
    inline TofArrayInterface() {};

    // Gets readings from all ToF sensors; outputs to the Serial Monitor
    virtual void readAllToSerial();

    // Gets readings from all ToF sensors; returns readings in a vector
    virtual std::vector<int16_t> readAll();

    // Reads a single ToF sensor
    virtual int16_t readTof(TofSensorIndex sensor_index);
};

class TofArray : public TofArrayInterface {
  public:
    TofArray();
    void readAllToSerial() override;
    std::vector<int16_t> readAll() override;
    int16_t readTof(TofSensorIndex sensor_index) override;

  private:
    void initSensor_(uint8_t index);
    std::vector<uint8_t> sensor_pins_;
    std::vector<VL53L0X*> sensors_;
    std::vector<String> sensor_names_;
};
