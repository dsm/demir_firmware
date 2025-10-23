#ifndef CONF_H
#define CONF_H

// Serial configuration
#define BAUDRATE 115200
#define TIMEOUT 100  // Serial timeout in ms

// I2C configuration - 
#define MOTOR_CONTROLLER_ADDR_1 0x08

// Control loop timing
#define LOOP_RATE_MS 3  // Main control loop rate

#endif // CONF_H
