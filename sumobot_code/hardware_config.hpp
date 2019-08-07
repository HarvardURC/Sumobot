/**
 * hardware_config.hpp
 * Contains hardware constants for the HURC Sumobot
 */
#include <stdint.h>
#pragma once

namespace pins {
    const uint8_t motor_pwm_bl = 3;
    const uint8_t motor_direction_bl = 2;
    const uint8_t motor_pwm_fl = 3;
    const uint8_t motor_direction_fl = 2;
    const uint8_t motor_pwm_br = 9;
    const uint8_t motor_direction_br = 8;
    const uint8_t motor_pwm_fr = 9;
    const uint8_t motor_direction_fr = 8;

    const uint8_t tof_left = 0;
    const uint8_t tof_leftdiag = 1;
    const uint8_t tof_front = 5;
    const uint8_t tof_rightdiag = 6;
    const uint8_t tof_right = 7;

    const uint8_t reflectance_fl = 14;
    const uint8_t reflectance_bl = 15;
    const uint8_t reflectance_fr = 20;
    const uint8_t reflectance_br = 21;

    // TODO: change
    const uint8_t switch_pin = 4;
}   // namespace pins
