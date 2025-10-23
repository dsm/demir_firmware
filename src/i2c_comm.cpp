/**
 * @file i2c_comm.cpp
 * @brief I2C communication interface implementation for DEMIR controller
 * @author Şam Rossiter
 * @date 2025
 * @details Implements I2C master/slave routines for sending commands to
 *          the DEMIR motor controller. References controller I2C addresses
 *          defined in conf.h
 */

#include <Wire.h>
#include "i2c_comm.h"
#include "conf.h"       // for I2C addresses
#include "controller.h" // for Solver
#include "motor.h"      // for Motor

uint8_t i2cCmdBuffer[I2C_CMD_BUF_SIZE];
uint8_t i2cCmdIndex = 0;

/**
 * @brief I2C receive event callback
 * @param byteCount Number of bytes received
 * @details Called when I2C master writes data to this controller.
 *          Buffers incoming command and triggers Solver for processing.
 */
void receiveI2CCommand(int byteCount) {
    while (Wire.available() && i2cCmdIndex < I2C_CMD_BUF_SIZE) {
        i2cCmdBuffer[i2cCmdIndex++] = Wire.read();
    }
    if (i2cCmdIndex > 0) {
        Solver.proccessI2CCommand(i2cCmdBuffer, i2cCmdIndex);
        i2cCmdIndex = 0;
    }
}

/**
 * @brief I2C request event callback
 * @details Called when I2C master requests data from this controller.
 *          Sends the current motor position (MOTOR_1) as a 4-byte response.
 */
void sendI2CResponse() {
    uint32_t pos = Motor.getPosition(MOTOR_1);
    Wire.write((uint8_t*)&pos, sizeof(pos));
}

/**
 * @brief Initialize I2C interface
 * @param slaveAddress I2C address of this controller
 * @details Configures Wire library in slave mode and attaches receive/request callbacks.
 */
void initI2C(uint8_t slaveAddress) {
    Wire.begin(slaveAddress);          // join I2C bus with defined address
    Wire.onReceive(receiveI2CCommand); // attach receive handler
    Wire.onRequest(sendI2CResponse);   // attach request handler
}
