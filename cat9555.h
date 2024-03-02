#ifndef CAM_cat9555_h
#define CAM_cat9555_h
/*****************************************/
#include "Arduino.h"
#include "Wire.h"
/*****************************************/
/*****************************************/
class CAT9555 {
private:
  uint8_t i2C_ADDR;
  TwoWire myWire = Wire;
  void Write_reg(uint8_t regsiter, uint8_t value) {
    myWire.beginTransmission(i2C_ADDR);
    myWire.write(regsiter);
    myWire.write(value);
    myWire.endTransmission();
  }
  uint8_t Read_reg(uint8_t regsiter) {
    myWire.beginTransmission(i2C_ADDR);
    myWire.write(regsiter);
    myWire.endTransmission();
    myWire.requestFrom(int(i2C_ADDR), 1);
    return myWire.read();
  }

public:
  //CAT9555 regsiter
  const uint8_t IN_PORT0 = 0;   // Input Port 0
  const uint8_t IN_PORT1 = 1;   // Input Port 1
  const uint8_t OUT_PORT0 = 2;  // Output Port 0
  const uint8_t OUT_PORT1 = 3;  // Output Port 1
  const uint8_t PI_PORT0 = 4;   // Polarity Inversion Port 0
  const uint8_t PI_PORT1 = 5;   // Polarity Inversion Port 1
  const uint8_t CFG_PORT0 = 6;  // Configuration Port 0
  const uint8_t CFG_PORT1 = 7;  // Configuration Port 1

  void begin(uint8_t i2c_addr, uint8_t p0, uint8_t p1);
  void begin(TwoWire newWire, uint8_t i2c_addr, uint8_t p0, uint8_t p1);

  void setPort_0(uint8_t value);
  void setPort_1(uint8_t value);

  uint8_t getPort_0();
  uint8_t getPort_1();

  void set_bit(uint8_t pinPos, bool setOrClear);

  bool readBit(uint8_t pinPos);
};
/*****************************************/
/*****************************************/
void CAT9555::begin(uint8_t i2c_addr, uint8_t p0, uint8_t p1) {
  myWire = Wire;
  i2C_ADDR = i2c_addr;
  Write_reg(CFG_PORT0, p0);
  Write_reg(CFG_PORT1, p1);
}
void CAT9555::begin(TwoWire newWire, uint8_t i2c_addr, uint8_t p0, uint8_t p1) {
  newWire = newWire;
  i2C_ADDR = i2c_addr;
  Write_reg(CFG_PORT0, p0);
  Write_reg(CFG_PORT1, p1);
}

void CAT9555::setPort_0(uint8_t value) {
  Write_reg(OUT_PORT0, value);
}
void CAT9555::setPort_1(uint8_t value) {
  Write_reg(OUT_PORT1, value);
}

uint8_t CAT9555::getPort_0() {
  return Read_reg(IN_PORT0);
}
uint8_t CAT9555::getPort_1() {
  return Read_reg(IN_PORT1);
}

void CAT9555::set_bit(uint8_t pinPos, bool setOrClear) {
  if ((pinPos >= 0) & (pinPos < 8)) {
    uint8_t v = getPort_0();
    setPort_0((v & ~(1 << pinPos)) | (setOrClear << pinPos));
  } else if ((pinPos >= 8) & pinPos < 16) {
    uint8_t v = getPort_1();
    setPort_1((v & ~(1 << pinPos - 8)) | (setOrClear << pinPos - 8));
  }
}
bool CAT9555::readBit(uint8_t pinPos) {
    if ((pinPos >= 0) & (pinPos < 8)) {
      return (getPort_0() & (1 << pinPos)) != 0;
    }else if ((pinPos >= 8) & pinPos < 16) {
      return (getPort_1() & (1 << pinPos - 8)) != 0;
    }
  }
#endif
