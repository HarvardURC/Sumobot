// CONSTANT declarations
#include <QTRSensors.h>
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
QTRSensors qtr;

// `debug` can be set TRUE to output debugging information to the serial port.
const boolean debug = true;   // If TRUE debugging information sent via serial.

// Define time delay constants - they can be adjusted to get the best display.

const int FSM_FREQ = 15;   // Frequency (in Hz) of the FSM
const int ROLL_DELAY = 4;  // Roll time (in Sec) after button released
const int DISP_DELAY = 3;  // Time to display final value (in Sec) before clear.

const int ref_threshhold = 20; // These are dummy threshold values, change these later
const int tof_threshhold = 20; 

const uint8_t SensorCount = 4;
uint16_t sensorValues[SensorCount];
// I/O Pin Constants
#define FL_MOTOR_PIN 3  
#define BL_MOTOR_PIN 4 
#define FR_MOTOR_PIN 9    
#define BR_MOTOR_PIN 10   

#define LEFT_DIR_PIN 2
#define RIGHT_DIR_PIN 8

// L, FL, F, FR, R
#define XSHUT_L 5 
#define XSHUT_FL 6 
#define XSHUT_F 7
#define XSHUT_FR 11
#define XSHUT_R 1
// SCL = 16
// SDA = 17
#define SCL_REF_PIN 16
#define SDA_REF_PIN 17

#define FL_REF 14
#define BL_REF 15
#define FR_REF 20
#define BR_REF 21

#define BUTTON_PIN 12
// Program Variables
int cur_state;      // FSM state variable
                    // 1: Start (init sensors)
                    // 2: Stop (motors = 0)
                    // 3: straight (motors = high)
                    // 4: turn_left (right motors = high)
                    // 5: turn_right (left motors = high)
                    // 6: backup (motors = backwards)
               
/*********************************************************
 * void setup()
 *
 * Runs once. Used to initialize variables
 * and set up I/O pins.
 *********************************************************/
void setup()
{
    // Initialize our I/O pins.
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(FL_MOTOR_PIN, OUTPUT);
    pinMode(BL_MOTOR_PIN, OUTPUT);
    pinMode(FR_MOTOR_PIN, OUTPUT);
    pinMode(BR_MOTOR_PIN, OUTPUT);

    pinMode(LEFT_DIR_PIN, OUTPUT);
    pinMode(RIGHT_DIR_PIN, OUTPUT);

    pinMode(SCL_REF_PIN, OUTPUT);
    // pinMode(SDA_REF_PIN, INPUT);

    pinMode(FL_REF, INPUT);
    pinMode(BL_REF, INPUT);
    pinMode(FR_REF, INPUT);
    pinMode(BR_REF, INPUT);

    // configure the sensors
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){FL_REF, BL_REF, FR_REF, BR_REF}, SensorCount);
    qtr.setEmitterPin(2);

    // need to run qtr.calibrate somewhere

    // Initialize program variables.
    cur_state = 1; // Initialize for 1st state in program loop; wait for input.

   /* Set up serial port if debugging enabled */
    if (debug)
    {
        Serial.begin(9600); // Pipes serial print data to the serial monitor
        Serial.println("Initialization complete.");
    }
}

/*********************************************************
 * boolean checkStart()
 * 
 * Function to see if TEST pushbutton is active
 * 
 * It might seem silly to have this simple test in a function, 
 * but isolating it in a function will make it easy to switch
 * in the more complicated accelerometer later without changing
 * the FSM. In addition it makes replacing polled operation
 * with interrupt operation easier as well.
 * 
 * Returns TRUE if pushbutton pressed; FALSE otherwise
 *********************************************************/

boolean checkStart()
{
    // BUTTON_PIN has a pullup resistor attached, it is LOW when pressed.
    if (digitalRead(BUTTON_PIN) == LOW)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*********************************************************
 * void loop()
 *  
 * State Machine Loop
 * 
 * Uses the switch/case statement to only execute the code for current state;
 * figures out what the next state is and continues the main program loop. 
 *
 * Runs forever
 *********************************************************/

void loop()
{
    if (debug)               // if debug enabled, print state to serial monitor
    {
        Serial.print("State: ");
        Serial.println(cur_state);
    }

    switch (cur_state)
    {
        // Sensor init
        case 1:                            
            // TODO: init sensors heres (use micromouse init)
            
            // digitalWrite(XSHUT_L, HIGH);
            // digitalWrite(XSHUT_FL, HIGH);
            // digitalWrite(XSHUT_F, HIGH);
            // digitalWrite(XSHUT_FR, HIGH);
            // digitalWrite(XSHUT_R, HIGH);
            cur_state = 2; 
            break;
        // Move init
        case 2:
            // TODO: Set the motors to zero
            if (digitalRead(BUTTON_PIN) == LOW)        // Choose our next state.
            {
                cur_state = 3;
            }
            else
            {
                cur_state = 2;
            }
            break;
        // Straight
        case 3:  
            // TODO: Run the motors straight
            if (!)
            {
                cur_state = 2;
            }
            else if (roll_timeout >= ROLL_DELAY*FSM_FREQ) // Monostable is done.
            {
                roll_timeout = 0;
                cur_state = 4;
            }
            else // Monostable continues, but roll_timeout has been incremented.
            {
                cur_state = 3;
            }
            break;  

        case 4:                                             // Show final value.
            display_timeout++;   // Increment display_timeout b/c on this cycle.
            if (checkStart())    // User re-pressed button; return to rolling.
            {
                cur_state = 2;
            }
            else if (display_timeout >= DISP_DELAY*FSM_FREQ) // Final disp=done.
            {
                // Reset timeout, go to state 1 noting that we need to sleep.
                display_timeout = 0;
                need_sleep = 1;
                cur_state = 1;
            }
            else              // Still have cycles to go before display timeout.
            {
                cur_state = 4;
            } 
            break;
    }
    delay(1000/FSM_FREQ);             // wait for next state machine clock cycle
}


