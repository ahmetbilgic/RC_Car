//alıcı


#include <Servo.h>
#include <AFMotor.h>
#include <RH_ASK.h>

RH_ASK driver;

Servo servoMotor;
AF_DCMotor motorY(3); // Motor Shield'daki Motor 2 (Y Ekseni)

const int tolerance = 10; // Tolerans değeri, joystick değerindeki küçük değişiklikleri yok saymak için kullanılır

void setup() {
  Serial.begin(9600);
  if (!driver.init()) {
    Serial.println("Alıcı başlatılamadı");
    while (1);
  }
  servoMotor.attach(9); // Servo motorun bağlı olduğu pin (örneğin, D9 pinine bağlıysa)
  motorY.setSpeed(255); // Motor hızını ayarlayın
  motorY.run(RELEASE);
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) {
    int yValue = buf[0]; // RF modülünden gelen y ekseni değeri
    Serial.println(yValue);
    // Tolerans değeri içindeki değerleri yok sayarak motoru durdurun
    if (yValue > 127 - tolerance && yValue < 127 + tolerance) {
      motorY.run(RELEASE);
      Serial.println("DURDU");
      
    } else {
      // Y ekseni motorunun hızını ve yönünü ayarlayın
      if (yValue < 127) {
        motorY.setSpeed(map(yValue, 0, 127 - tolerance, 255, 0));
        motorY.run(FORWARD);
        Serial.println("Ileri");
       
      } else {
        motorY.setSpeed(map(yValue, 127 + tolerance, 255, 0, 255));
        motorY.run(BACKWARD);
        Serial.println("Geri");
        
      }
    }

    int xValue = buf[1]; // RF modülünden gelen x ekseni değeri
    int servoAngle = map(xValue, 0, 255, 0, 180); // Servo açısını x ekseni değerine göre ayarlayın
    Serial.println("servo açısı");
    Serial.println(servoAngle);
    servoMotor.write(servoAngle); // Servo motor açısını ayarlayın
  }
}


