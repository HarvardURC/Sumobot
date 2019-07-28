#include "hardware_config.hpp"
#include <stdint.h>

#pragma once

// Represents a single physical motor.
class Motor {
  public:
    inline Motor(uint8_t pwm_pin, uint8_t direction_pin) :
        pwm_pin_(pwm_pin), direction_pin_(direction_pin) {};

    // Runs the motor at `speed`
    void drive(int16_t speed);

  private:
    uint8_t pwm_pin_;
    uint8_t direction_pin_;
};

/*
 * This is the interface Driver classes must implement. We have 2 classes that
 * implement this interface, `Driver` and `VirtualDriver`. The Driver class will
 * interface with actual hardware, VirtualDriver will move a virtual robot in
 * the serial monitor.
 */
class DriverInterface {
  public:
    inline DriverInterface() {};
    virtual void drive(int16_t speed);
    virtual void drive(int16_t speed_l, int16_t speed_r);
};

class Driver : public DriverInterface {
  public:
    inline Driver() :
        motor_bl_(Motor(pins::motor_pwm_bl, pins::motor_direction_bl)),
        motor_fl_(Motor(pins::motor_pwm_fl, pins::motor_direction_fl)),
        motor_br_(Motor(pins::motor_pwm_br, pins::motor_direction_br)),
        motor_fr_(Motor(pins::motor_pwm_fr, pins::motor_direction_fr)) {};

    // Runs all motors at `speed`
    void drive(int16_t speed) override;

    // Runs left motors at `speed_l` and right motors at `speed_r`
    void drive(int16_t speed_l, int16_t speed_r) override;

  private:
    Motor motor_bl_;
    Motor motor_fl_;
    Motor motor_br_;
    Motor motor_fr_;
};

class VirtualDriver : public DriverInterface {
  public:
    inline VirtualDriver(float x_coordinate,
                         float y_coordinate,
                         uint16_t degree_heading) :
        x_coordinate(x_coordinate),
        y_coordinate(y_coordinate), degree_heading(degree_heading), left_speed(0),
        right_speed(0) {};

    // Sets left_speed_ and right_speed_ to `speed`
    void drive(int16_t speed) override;
    // Sets left_speed_ to `speed_l` and right_speed_ to `speed_r`
    void drive(int16_t speed_l, int16_t speed_r) override;

    float x_coordinate;
    float y_coordinate;
    // Heading in degrees, 0 is towards the top of the serial monitor
    // (north) 90 is towards the right of the serial monitor (east), so on
    uint16_t degree_heading;
    int16_t left_speed;
    int16_t right_speed;
};
