#ifndef CAM_cat9555_h
#define CAM_cat9555_h

#include "Arduino.h"
#include "Wire.h"
/****************************************************************/
class CAT9555 {
private:
  uint8_t i2C_ADDR;  // I2C-Adresse des CAT9555-Expanders
  TwoWire myWire = Wire; // Die I2C-Schnittstelle, die wir verwenden

  // Schreibt einen Wert in ein Register des CAT9555
  void Write_reg(uint8_t regsiter, uint8_t value) {
    myWire.beginTransmission(i2C_ADDR);
    myWire.write(regsiter);
    myWire.write(value);
    myWire.endTransmission();
  }

  // Liest einen Wert aus einem Register des CAT9555
  uint8_t Read_reg(uint8_t regsiter) {
    myWire.beginTransmission(i2C_ADDR);
    myWire.write(regsiter);
    myWire.endTransmission();
    myWire.requestFrom(int(i2C_ADDR), 1);
    return myWire.read();
  }
public:
  
  /** CAT9555 regsiter *********/
  const uint8_t IN_PORT0 = 0;   // Input Port 0
  const uint8_t IN_PORT1 = 1;   // Input Port 1
  const uint8_t OUT_PORT0 = 2;  // Output Port 0
  const uint8_t OUT_PORT1 = 3;  // Output Port 1
  const uint8_t PI_PORT0 = 4;   // Polarity Inversion Port 0
  const uint8_t PI_PORT1 = 5;   // Polarity Inversion Port 1
  const uint8_t CFG_PORT0 = 6;  // Configuration Port 0
  const uint8_t CFG_PORT1 = 7;  // Configuration Port 1
  /** Functions *********/
  void begin(uint8_t i2c_addr, uint8_t p0, uint8_t p1); // Initialisiert die CAT9555-Kommunikation mit der angegebenen I2C-Adresse
  void begin(TwoWire newWire, uint8_t i2c_addr, uint8_t p0, uint8_t p1); // Initialisiert die CAT9555-Kommunikation mit einer benutzerdefinierten I2C-Schnittstelle

  void setPort_0(uint8_t value); // Setzt den Zustand des Ports 0
  void setPort_1(uint8_t value); // Setzt den Zustand des Ports 1

  uint8_t getPort_0(); // Liest den Zustand des Ports 0
  uint8_t getPort_1(); // Liest den Zustand des Ports 1

  void setBit(uint8_t pinPos, bool setOrClear); // Setzt einen bestimmten Bit-Position im Port
  bool getBit(uint8_t pinPos); // Liest den Zustand eines bestimmten Bits im Port
};
/****************************************************************/

/**
 * @brief Initialisiert die CAT9555-Kommunikation mit der angegebenen I2C-Adresse und konfiguriert die Byteports als Ein- oder Ausgänge.
 *
 * @param i2c_addr I2C-Adresse (0x20 - 0x27)
 * @param p0 Konfiguriert Byteport 0 (0x00 für alle Ausgänge / 0xff für alle Eingänge)
 * @param p1 Konfiguriert Byteport 1 (0x00 für alle Ausgänge / 0xff für alle Eingänge)
 */
void CAT9555::begin(uint8_t i2c_addr, uint8_t p0, uint8_t p1) {
  myWire = Wire;
  i2C_ADDR = i2c_addr;
  Write_reg(CFG_PORT0, p0);
  Write_reg(CFG_PORT1, p1);
}

/**
 * @brief Initialisiert die CAT9555-Kommunikationmit einer benutzerdefinierten I2C-Schnittstelle und mit der angegebenen I2C-Adresse und konfiguriert die Byteports als Ein- oder Ausgänge.
 *
 * @param newWire  benutzerdefinierten I2C-Schnittstelle
 * @param i2c_addr I2C-Adresse (0x20 - 0x27)
 * @param p0 Konfiguriert Byteport 0 (0x00 für alle Ausgänge / 0xff für alle Eingänge)
 * @param p1 Konfiguriert Byteport 1 (0x00 für alle Ausgänge / 0xff für alle Eingänge)
 */
void CAT9555::begin(TwoWire newWire, uint8_t i2c_addr, uint8_t p0, uint8_t p1) {
  newWire = newWire;
  i2C_ADDR = i2c_addr;
  Write_reg(CFG_PORT0, p0);
  Write_reg(CFG_PORT1, p1);
}

/**
 * @brief Setzt den Zustand des Ports 0.
 *
 * @param value Wert, der auf Byteport 0 gesetzt werden soll
 */
void CAT9555::setPort_0(uint8_t value) {
  Write_reg(OUT_PORT0, value);
}

/**
 * @brief Setzt den Zustand des Ports 1.
 *
 * @param value Wert, der auf Byteport 1 gesetzt werden soll
 */
void CAT9555::setPort_1(uint8_t value) {
  Write_reg(OUT_PORT1, value);
}

/**
 * @brief Liest den Zustand von Byteport 0.
 *
 * @return Zustand von Byteport 0 (high / low)
 */
uint8_t CAT9555::getPort_0() {
  return Read_reg(IN_PORT0);
}

/**
 * @brief Liest den Zustand von Byteport 1.
 *
 * @return Zustand von Byteport 1 (high / low)
 */
uint8_t CAT9555::getPort_1() {
  return Read_reg(IN_PORT1);
}

/**
 * @brief Setzt einen bestimmten Bit-Position im Port.
 *
 * @param pinPos Position des Bits im Port (0-15)
 * @param setOrClear true, um das Bit zu setzen, false, um es zu löschen
 */
void CAT9555::setBit(uint8_t pinPos, bool setOrClear) {
  if ((pinPos >= 0) & (pinPos < 8)) {
    uint8_t v = getPort_0();
    setPort_0((v & ~(1 << pinPos)) | (setOrClear << pinPos));
  } else if ((pinPos >= 8) & pinPos < 16) {
    uint8_t v = getPort_1();
    setPort_1((v & ~(1 << pinPos - 8)) | (setOrClear << pinPos - 8));
  }
}

/**
 * @brief Liest den Zustand eines bestimmten Bits im Port.
 *
 * @param pinPos Position des Bits im Port (0-15)
 * @return Zustand des Bits (high / low)
 */
bool CAT9555::getBit(uint8_t pinPos) {
  if ((pinPos >= 0) & (pinPos < 8)) {
    return (getPort_0() & (1 << pinPos)) != 0;
  } else if ((pinPos >= 8) & pinPos < 16) {
    return (getPort_1() & (1 << pinPos - 8)) != 0;
  }
}

#endif
/****************************************************************/
