/**
 * @file main.cpp
 * @brief Main application entry point for DEMIR motor controller with I2C support
 * @author Muhammet Şükrü Demir
 * @date 2025
 * @version 1.1.2
 * @details Initializes DEMIR motor control system, implements main control loop,
 *          handles serial and I2C commands, motor control execution, and data logging.
 */

#include <Arduino.h>
#include <Wire.h>

#include "Motion.h"
#include "MotorDriver.h"
#include "SerialSolver.h"
#include "board/DEMIRv1/pins_DEMIRv1.h"
#include "com_def.h"
#include "conf.h"
#include "controller.h"
#include "i2c_comm.h"  // I2C interface header

/// @brief Main control loop rate in milliseconds
uint8_t loopRate = 3;

/**
 * @brief Arduino setup function - system initialization
 */
void setup() {
    Serial.begin(BAUDRATE);
    while (!Serial) {}
    Serial.setTimeout(TIMEOUT);

    Serial.println(F("DEMIR v1.1.2"));

    Driver.init();
    Serial.println(F("ok!"));

    pinMode(STATUS_LED_BLUE, OUTPUT);
    pinMode(STATUS_LED_RED, OUTPUT);

    digitalWrite(STATUS_LED_BLUE, HIGH);
    delay(300);
    digitalWrite(STATUS_LED_BLUE, LOW);

    digitalWrite(STATUS_LED_RED, HIGH);
    delay(300);
    digitalWrite(STATUS_LED_RED, LOW);

    // Initialize I2C interface
    Wire.begin(DEMIR_I2C_ADDR);
    Wire.onReceive(receiveI2CCommand);
    Wire.onRequest(sendI2CResponse);
}

/**
 * @brief Arduino main loop function
 */
void loop() {
    Controller.currentTime = millis();
    Controller.deltaT = Controller.currentTime - Controller.oldTime;

    if (Controller.deltaT >= loopRate) {
        Controller.oldTime = Controller.currentTime;

        // Execute motor control algorithms
        Driver.run();

        // Handle data logging if enabled
        if (Solver.logEnable) {
            switch (Solver.logWhat) {
                case LOG_POSITION:
                    Solver.logForMatlab[Solver.logCounter] = (unsigned long)Motor.getPosition(MOTOR_1);
                    break;
                case LOG_CURRENT:
                    break;
            }

            if (++Solver.logCounter == Solver.logSize) {
                Solver.logEnable = false;
                Solver.logCounter = 0;
            }
        }
    }

    // Process incoming serial commands
    Solver.proccessCommands();

    // Send logged data to MATLAB if requested
    if (Solver.sendLogToMatlab) {
        Solver.sendLogToMatlab = false;
        for (uint16_t j = 0; j < Solver.logSize; j++) {
            Serial.write(Solver.logForMatlab[j]);
            Serial.write(Solver.logForMatlab[j] >> 8);
            Serial.write(Solver.logForMatlab[j] >> 16);
            Serial.write(Solver.logForMatlab[j] >> 24);
        }
    }
}
