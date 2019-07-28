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
}   // namespace pins
