#include "hardware_config.hpp"
#include "motors.hpp"
#include "tof_sensors.hpp"
#include <QTRSensors.h>
#include <stdint.h>

#define DEBUG_ENABLED 0

#define NUM_SENSORS 3
#define NUM_SAMPLES_PER_SENSOR 4

// TODO: Tune the below
// TODO: We also may want different speeds for different actions
// (forward, turn, reverse, attack, etc.). See what's reasonable when it runs
#define DRIVE_SPEED 255

#define BACKUP_TIME_IN_MS 300
#define TURN_TIME_IN_MS 250

Driver* driver;
TofArray* tofs;
QTRSensors qtr;

uint16_t line_sensor_readings[NUM_SENSORS];

enum FsmState { Start = 0, Straight, TurnLeft, TurnRight, BackupAndTurn };
FsmState cur_state;

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

    cur_state = Start;

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
        case Start:
            // TODO: check if switch will be low or high
            if (digitalRead(pins::switch_pin) == HIGH) {
                cur_state = Straight;
            } else {
                cur_state = Start;
            }
            break;
        // Move forward state
        case Straight:
            driver->drive(DRIVE_SPEED);
            if (line_sensor_readings[0] || line_sensor_readings[1]) {
                cur_state = BackupAndTurn;
            } else if (tofs->objectVisible(Left) || tofs->objectVisible(LeftDiag)) {
                cur_state = TurnLeft;
            } else if (tofs->objectVisible(Right) || tofs->objectVisible(RightDiag)) {
                cur_state = TurnRight;
            } else {
                cur_state = Straight;
            }
            break;
        // Turn left
        case TurnLeft:
            driver->drive(-DRIVE_SPEED, DRIVE_SPEED);
            if (tofs->objectVisible(Front)) {
                cur_state = Straight;
            } else if (tofs->objectVisible(Right) || tofs->objectVisible(RightDiag)) {
                cur_state = TurnRight;
            } else {
                cur_state = TurnLeft;
            }
            break;
        // Turn right
        case TurnRight:
            driver->drive(DRIVE_SPEED, -DRIVE_SPEED);
            if (tofs->objectVisible(Front)) {
                cur_state = Straight;
            } else if (tofs->objectVisible(Left) || tofs->objectVisible(LeftDiag)) {
                cur_state = TurnLeft;
            } else {
                cur_state = TurnRight;
            }
            break;
        // Backup and turn
        case BackupAndTurn:
            driver->drive(-DRIVE_SPEED, -DRIVE_SPEED);
            uint64_t cur_time = millis();
            while (millis() < cur_time + BACKUP_TIME_IN_MS) {
            }
            driver->drive(-DRIVE_SPEED, DRIVE_SPEED);
            cur_time = millis();
            while (millis() < cur_time + TURN_TIME_IN_MS) {
            }
            cur_state = Straight;
            break;
    }
}
