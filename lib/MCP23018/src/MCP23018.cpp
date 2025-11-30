#include <Arduino.h>
#include <stdexcept>
#include <Wire.h>
#include "MCP23018.h"

#define EMPTY_REG 0
#define FULL_REG 0xFFFF

bool ignoreInterrupt = false;

void manageGpioExpanderInterrupt() {
    if (!ignoreInterrupt) {
        ignoreInterrupt = true;
    }
}

bool MCP23018::init() {
    if (!_initDone) {
        if (!resetVerify()) {
            Serial.println("Failed to reset-verify the expander");

            throw std::runtime_error("mcp23018 init failed");
        }

        _initDone = true;

        for (int i = 0; i < 16; i++) {
            setPinState(i, true);
            setPinMode(i, MCP_OUTPUT);
        }
    }
    return true;
}

bool MCP23018::resetVerify() {
    readRegister(INTCAP);

    writeRegister(0, FULL_REG);
    for (uint8_t i = 2; i < 22; i = i + 2) {
        writeRegister(i, EMPTY_REG);
    }

    delay(10);
    readRegister(INTCAP);

    bool error = false;
    if (readRegister(0) != FULL_REG) {
        Serial.println("First register is wrong");
        error = true;
    }

    for (uint8_t i = 2; i < 22; i += 2) {
        if (i == 18 || i == 16) {
            continue;
        }

        if (readRegister(i) != EMPTY_REG) {
            Serial.println("Register " + String(i) + " is wrong");
            error = true;
        }
    }

    if (error) {
        Serial.println("Something is really wrong with the expander!");
        return false;
    }

    uint8_t iocon = 0b01000001 | (1 << 2);

    writeSingleRegister(IOCON, iocon);
    writeSingleRegister(IOCON + 1, iocon);

    _ioDirectionRegister = FULL_REG;
    _gpioInterruptEnableRegister = EMPTY_REG;
    _gpioPullupRegister = EMPTY_REG;
    _outputLatchRegister = FULL_REG;

    _interruptControlRegister = EMPTY_REG;
    _defvalReg = EMPTY_REG;

    return true;
}

void MCP23018::deInit() {
    setInterrupt(MCP_STAT_IN, false);
    setPinMode(MCP_STAT_IN, MCP_OUTPUT);
}

bool MCP23018::digitalRead(uint8_t pin) const {
    if (!_initDone) {
        return false;
    }

    return checkBit(readRegister(GPIO), pin);
}

void MCP23018::setInterrupt(uint8_t pin, bool interrupt) {
    if (!_initDone) {
        return;
    }
    setBit(_gpioInterruptEnableRegister, pin, interrupt);

    writeRegister(GPINTEN, _gpioInterruptEnableRegister);
}

void MCP23018::setInterruptCause(uint8_t pin, bool enableCause, bool causeState) {
    if (!_initDone) {
        return;
    }
    setBit(_interruptControlRegister, pin, enableCause);
    setBit(_defvalReg, pin, !causeState);

    writeRegister(INTCON, _interruptControlRegister);
    writeRegister(DEFVAL, _defvalReg);
}

void MCP23018::setPinMode(uint8_t pin, bool mode) {
    if (!_initDone) {
        return;
    }

    setBit(_ioDirectionRegister, pin, !mode);

    writeRegister(IODIR, _ioDirectionRegister);
}

void MCP23018::setPinState(uint8_t pin, bool state) {
    if (!_initDone) {
        return;
    }
    setBit(_outputLatchRegister, pin, state);

    writeRegister(OLAT, _outputLatchRegister);
}

void MCP23018::setPinPullUp(uint8_t pin, bool pull) {
    if (!_initDone) {
        return;
    }
    setBit(_gpioPullupRegister, pin, pull);

    writeRegister(GPPU, _gpioPullupRegister);
}

void MCP23018::setBit(uint16_t &val, uint8_t bit, bool state) {
    if (state) {
        val |= (1 << bit);
    } else {
        val &= ~(1 << bit);
    }
}

bool MCP23018::checkBit(uint16_t val, uint8_t bit) {
    return (val & (1 << bit)) == 0;
}

void MCP23018::writeSingleRegister(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(MCP23018_ADDRESS);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

void MCP23018::writeRegister(uint8_t reg, uint16_t val) {
    uint8_t byte0 = val & 0xFF;
    uint8_t byte1 = (val >> 8) & 0xFF;

    Wire.beginTransmission(MCP23018_ADDRESS);
    Wire.write(reg);
    Wire.write(byte0);
    Wire.write(byte1);
    Wire.endTransmission();
}

uint16_t MCP23018::readRegister(uint8_t reg) {
    Wire.beginTransmission(MCP23018_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(MCP23018_ADDRESS, 2);
    Wire.available();
    byte data1 = Wire.read();
    byte data2 = Wire.read();

    return (static_cast<uint16_t>(data1)) | data2 << 8;
}

uint8_t MCP23018::readSingleRegister(uint8_t reg) {
    Wire.beginTransmission(MCP23018_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(MCP23018_ADDRESS, 1);
    Wire.available();
    return Wire.read();
}

void MCP23018::dumpAllRegisters() {
    if (!_initDone) {
        return;
    }

    for (byte i = 0; i < 22; i = i + 2)
    {
        Serial.println("Register: " + decimalToHexString(i) + " is: " + uint16ToBinaryString(readRegister(i)));
    }
}

String MCP23018::uint16ToBinaryString(uint16_t value) {
    String binaryString = "";
    for (int i = 15; i >= 0; i--)
    {
        binaryString += (value & (1 << i)) ? '1' : '0';
    }
    return binaryString;
}

String MCP23018::uint8ToBinaryString(uint8_t value) {
    String binaryString = "";
    for (int i = 7; i >= 0; i--)
    {
        binaryString += (value & (1 << i)) ? '1' : '0';
    }
    return binaryString;
}

String MCP23018::decimalToHexString(int decimal) {
    char hexString[10];
    sprintf(hexString, "%X", decimal);
    String str = String(hexString);
    if (str.length() < 2)
    {
        str = "0" + str;
    }
    return "0x" + str;
}

void MCP23018::clearInterrupt() {
    while (MCP23018::readRegister(INTF) != 0) {
        MCP23018::readRegister(INTCAP);
        delay(10);
    }
}
