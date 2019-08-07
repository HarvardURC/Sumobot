#include <Arduino.h>
#include <VL53L0X.h>
#include <i2c_t3.h>
#include <vector>

#pragma once

enum TofSensorIndex { Left = 0, LeftDiag, Front, RightDiag, Right };

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

    // Gets readings from all ToF sensors+updates instance variable
    virtual void readAll();

    // Reads a single ToF sensor
    virtual int16_t readTof(TofSensorIndex sensor_index);
};

class TofArray : public TofArrayInterface {
  public:
    TofArray();
    void readAll() override;
    int16_t readTof(TofSensorIndex sensor_index) override;
    void readAllToSerial();
    bool objectVisible(TofSensorIndex sensor_index);

    std::vector<int16_t> sensor_readings_;

  private:
    void initSensor_(uint8_t index);
    std::vector<uint8_t> sensor_pins_;
    std::vector<VL53L0X*> sensors_;
    std::vector<String> sensor_names_;
};
