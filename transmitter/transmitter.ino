//VERİCİ



#include <SPI.h>
#include <RH_ASK.h>

RH_ASK driver;

const int xAxisPin = A2; // Joystick X eksen pin bağlantısı
const int yAxisPin = A3; // Joystick Y eksen pin bağlantısı

void setup() {
  Serial.begin(9600);
  if (!driver.init()) {
    Serial.println("Verici başlatılamadı");
    while (1);
  }
}

void loop() {
  int xValue = analogRead(xAxisPin); // Joystick X eksen değeri okunuyor
  int yValue = analogRead(yAxisPin); // Joystick Y eksen değeri okunuyor

  // Joystick değerlerini 0-255 aralığına ölçeklendirme
  byte scaledXValue = map(xValue, 0, 1023, 0, 255);
  byte scaledYValue = map(yValue, 0, 1023, 0, 255);

  // Veri paketi oluşturma
  uint8_t data[2];
  data[0] = scaledXValue;
  data[1] = scaledYValue;

  // Veriyi alıcıya gönderme
  driver.send(data, sizeof(data));
  driver.waitPacketSent();
  
  delay(50); // Gönderim aralığını istediğiniz gibi ayarlayabilirsiniz
}
