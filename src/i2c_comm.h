/**
 * @file i2c_comm.h
 * @brief I2C communication interface for DEMIR motor controller
 * @author Sam Rossiter
 * @date 2025
 * @details Handles receiving commands and sending responses over I2C.
 */

#pragma once
#include <Wire.h>
#include "controller.h"
#include "motor.h"
#include "SerialSolver.h"

#define DEMIR_I2C_ADDR 0x10
#define I2C_CMD_BUF_SIZE 8

extern uint8_t i2cCmdBuffer[I2C_CMD_BUF_SIZE];
extern uint8_t i2cCmdIndex;

void receiveI2CCommand(int byteCount);
void sendI2CResponse();
