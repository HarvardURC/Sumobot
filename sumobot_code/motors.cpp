#include "motors.hpp"
#include <Arduino.h>

void Motor::drive(int16_t speed) {
    digitalWrite(direction_pin_, speed > 0);
    analogWrite(pwm_pin_, abs(speed));
}

void Driver::drive(int16_t speed) {
    motor_bl_.drive(speed);
    motor_fl_.drive(speed);
    motor_br_.drive(speed);
    motor_fr_.drive(speed);
}

void Driver::drive(int16_t speed_l, int16_t speed_r) {
    motor_bl_.drive(speed_l);
    motor_fl_.drive(speed_l);
    motor_br_.drive(speed_r);
    motor_fr_.drive(speed_r);
}

void VirtualDriver::drive(int16_t speed) {
    left_speed = speed;
    right_speed = speed;
}

void VirtualDriver::drive(int16_t speed_l, int16_t speed_r) {
    left_speed = speed_l;
    right_speed = speed_r;
}
