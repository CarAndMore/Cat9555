
uint8_t cat9555_Addr = 0x27;
const uint8_t all_IOs_Output = 0x00;

int Relay_no[16] = {
  0, 1, 2, 3, 4, 5, 6, 7,
  15, 14, 13, 12, 11, 10, 9, 8
};

#define pause 750

/**************************/
#include "cam_cat9555.h"
#include <Wire.h>
/**************************/
#define Relay_on 0
#define Relay_off 1

CAT9555 i2c_exp;
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

/* gib die 16 bit als String aus ("b0011001100110011"). */
String bitMaske() {
  String text = "b";
  for (int p = 15; p >= 0; p--) {
    text += String(!i2c_exp.readBit(p));
  }
  return text;
}

void loop() {
  for (int i = 0; i < 16; i++) {
    i2c_exp.set_bit(Relay_no[i], Relay_on); // Schaltet Relay No. (i) ein
    Serial.println(bitMaske()); // gib die 16 bit als String aus ("b0011001100110011").
    delay(pause);
  }
  delay(pause * 10);
  for (int i = 0; i < 16; i++) {
    i2c_exp.set_bit(Relay_no[i], Relay_off); // Schaltet Relay No. (i) aus
    Serial.println(bitMaske()); // gib die 16 bit als String aus ("b0011001100110011").
    delay(pause);
  }
  delay(pause * 4);
}
