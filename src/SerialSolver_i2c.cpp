/**
 * @file SerialSolver_i2c.cpp
 * @brief I2C command handling for DEMIR controller
 * @author Muhammet Şam Rossiter
 * @date 2025
 */

#include "SerialSolver.h"
#include "motor.h"
#include "controller.h"

void SerialSolver::proccessI2CCommand(uint8_t* data, uint8_t length) {
    if (length < 2) return;

    uint8_t motorIndex = data[0];
    uint8_t command    = data[1];
    int16_t param      = 0;

    if (length >= 4) {
        param = (data[2] << 8) | data[3];
    }

    switch (command) {
        case 0x01:  // set motor speed
            Controller.set_uManuel(param, motorIndex);
            break;
        case 0x02:  // enable motor
            Motor.enable(motorIndex);
            break;
        case 0x03:  // disable motor
            Motor.disable(motorIndex);
            break;
        case 0x04:  // reset encoder
            Motor.resetPosition(motorIndex);
            break;
        case 0x05:  // set position reference
            Controller.setPositionReference(param, motorIndex);
            break;
        default:
            break;
    }
}
