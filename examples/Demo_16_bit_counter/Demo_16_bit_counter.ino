
uint8_t cat9555_Addr = 0x27;

const uint8_t all_IOs_Output = 0x00;
const uint8_t all_IOs_Input = 0xFF;
/**************************/
#include <cat9555.h>
#include <Wire.h>
/**************************/
CAT9555 i2c_exp;
/**************************/

/**
 * @brief gib die 16 bit als String aus ("b0011001100110011").
 * 
 * @param invertiert gibt an ob der wert invertiert werden soll. default ( true )
 *
 * @return String: "b0011001100110000"
 
 */
String bitMaske(bool invertiert = true) {
  String text = "b";
  for (int p = 15; p >= 0; p--) {
    if (invertiert) {
      text += String(!i2c_exp.getBit(p));
    } else {
      text += String(i2c_exp.getBit(p));
    }
  }
  return text;
}

/**
 * @brief ein 16 bit Counter, ausgabe auf der Console
 * schreibt 1. 8 bit auf Port 0.
 * schreibt 2. 8 bit auf Port 1.
 * 
 * @param wartezeit die delay in ms. default ( 100 )
 */
void counter_all(int wartezeit = 100) {
  for (int counter = 0; counter < 0xFFFF; counter++) {
    i2c_exp.setPort_0(~(counter & 0x00FF)); // schreibt erten 8 bit auf Port 0.
    i2c_exp.setPort_1(~(counter >> 8)); // schreibt zweiten 8 bit auf Port 1.
    Serial.print(bitMaske(0)); // gibt Bitmaske aus.
    Serial.print( " - "); Serial.println(counter); // gibt counter aus.
    delay(wartezeit);
  }
}
/**************************/
void setup() {
  Serial.begin(115200);
  Serial.println("0123456789abc");
  Serial.println("0123456789abc");

  Wire.begin();
  i2c_exp.begin(Wire, cat9555_Addr, all_IOs_Output, all_IOs_Output);
  i2c_exp.setPort_0(0xff); // alle 8 Relays auf Port 0 aus
  i2c_exp.setPort_1(0xff); // alle 8 Relays auf Port 1 aus
}

void loop() {
  counter_all(400);
}
/**************************/
/**************************/
