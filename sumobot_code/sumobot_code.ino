#include "hardware_config.hpp"
#include "motors.hpp"
#include "tof_sensors.hpp"
#include <QTRSensors.h>
#include <stdint.h>

#define DEBUG_ENABLED 0

#define NUM_SENSORS 3
#define NUM_SAMPLES_PER_SENSOR 4

#define BACKUP_TIME_IN_MS 300
#define TURN_TIME_IN_MS 250

Driver* driver;
TofArray* tofs;
QTRSensors qtr;

uint16_t line_sensor_readings[NUM_SENSORS];

uint8_t cur_state;   // FSM state variable
                     // 1: start (init state)
                     // 2: straight
                     // 3: turn_left
                     // 4: turn_right
                     // 5: backup and turn
void qtr_setup() {
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]) {pins::reflectance_fl,
                                         pins::reflectance_fr,
                                         pins::reflectance_bl,
                                         pins::reflectance_br},
                      NUM_SENSORS);
}
void setup() {
    qtr_setup();
    driver = new Driver();
    tofs = new TofArray();
    pinMode(pins::switch_pin, INPUT);

    cur_state = 1;

    if (DEBUG_ENABLED) {
        Serial.begin(9600);
        Serial.println("Initialization complete.");
    }
}

void loop() {
    if (DEBUG_ENABLED) {
        Serial.print("State: ");
        Serial.println(cur_state);
    }

    // Fresh sensor readings
    qtr.read(line_sensor_readings);
    tofs->readAll();

    switch (cur_state) {
        // Start state
        case 1:
            // TODO: check if switch will be low or high
            if (digitalRead(pins::switch_pin) == HIGH) {
                cur_state = 2;
            } else {
                cur_state = 1;
            }
            break;
        // Move forward state
        case 2:
            driver->drive(255);
            if (line_sensor_readings[0] || line_sensor_readings[1]) {
                cur_state = 5;
            } else if (tofs->objectVisible(Left) || tofs->objectVisible(LeftDiag)) {
                cur_state = 3;
            } else if (tofs->objectVisible(Right) || tofs->objectVisible(RightDiag)) {
                cur_state = 4;
            } else {
                cur_state = 2;
            }
            break;
        // Turn left
        case 3:
            driver->drive(-255, 255);
            if (tofs->objectVisible(Front)) {
                cur_state = 2;
            } else if (tofs->objectVisible(Right) || tofs->objectVisible(RightDiag)) {
                cur_state = 4;
            } else {
                cur_state = 3;
            }
            break;
        // Turn right
        case 4:
            driver->drive(255, -255);
            if (tofs->objectVisible(Front)) {
                cur_state = 2;
            } else if (tofs->objectVisible(Left) || tofs->objectVisible(LeftDiag)) {
                cur_state = 3;
            } else {
                cur_state = 4;
            }
            break;
        // 5: backup and turn
        case 5:
            driver->drive(-255, -255);
            uint64_t cur_time = millis();
            while (millis() < cur_time + BACKUP_TIME_IN_MS) {
            }
            driver->drive(-255, 255);
            cur_time = millis();
            while (millis() < cur_time + TURN_TIME_IN_MS) {
            }
            cur_state = 2;
            break;
    }
}
