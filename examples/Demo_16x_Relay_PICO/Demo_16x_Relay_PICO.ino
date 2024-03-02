// Demo_16x_Output_Relay_PICO.ino
uint8_t cat9555_Addr = 0x27;
const uint8_t all_IOs_Output = 0x00;
const uint8_t all_IOs_Input = 0xFF;
int Relay_no[16] = {
  0, 1, 2, 3, 4, 5, 6, 7,
  15, 14, 13, 12, 11, 10, 9, 8
};

#define WarteZeit 500 /* ms delay */
/**************************/
#include <cat9555.h>
#include <Wire.h>
/**************************/
#define Relay_on 0
#define Relay_off 1

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
/**************************/
void setup() {
  Serial.begin(115200);
  Serial.println("0123456789abc");
  Serial.println("0123456789abc");

  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();

  i2c_exp.begin(cat9555_Addr, all_IOs_Output, all_IOs_Output);
  i2c_exp.setPort_0(0xff); // alle 8 Relays auf Port 0 aus
  i2c_exp.setPort_1(0xff); // alle 8 Relays auf Port 1 aus
}
void loop() {
  for (int i = 0; i < 16; i++) {
    i2c_exp.setBit(Relay_no[i], Relay_on); // Schaltet Relay No. (i) ein
    Serial.println(bitMaske()); // gib die 16 bit als String aus ("b0011001100110011").
    delay(WarteZeit);
  }
  delay(WarteZeit * 4);
  for (int i = 0; i < 16; i++) {
    i2c_exp.setBit(Relay_no[i], Relay_off); // Schaltet Relay No. (i) aus
    Serial.println(bitMaske()); // gib die 16 bit als String aus ("b0011001100110011").
    delay(WarteZeit);
  }
  delay(WarteZeit * 4);
}
/**************************/
/**************************/
