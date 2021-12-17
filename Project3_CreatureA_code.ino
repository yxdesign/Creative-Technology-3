#include <Adafruit_MotorShield.h>
#include "config.h"

#define MIC_PIN A2

int current = 0;
int prev = -1;
int reading = 0;

AdafruitIO_Feed *mic = io.feed("mic-ct3");
AdafruitIO_Feed *ct3_photocell = io.feed("ct3_photocell", "yxdesign");

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

void setup() {

  pinMode(MIC_PIN, OUTPUT);
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  ct3_photocell->onMessage(handleMessage);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  mic->get();

  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start
  myOtherMotor->setSpeed(150);
  myOtherMotor->run(FORWARD);

  // turn on motor
  myOtherMotor->run(RELEASE);
}

void loop() {
  io.run();
  uint8_t i;

  current = analogRead (MIC_PIN);

  if (current != prev) {
    Serial.print("sending: ");
    Serial.println(current);
    mic->save(current);
  }

  if (reading >= 150) {
    Serial.print("tock");
    myMotor->run(BACKWARD);
    myOtherMotor->run(BACKWARD);
    
    for (i = 0; i < 255; i++) {
      myMotor->setSpeed(i);
      myOtherMotor->setSpeed(i);
      delay(7);
    }
    
    for (i = 255; i != 0; i--) {
      myMotor->setSpeed(i);
      myOtherMotor->setSpeed(i);
      delay(7);
    }
  }

  else {
    Serial.print("tech");
    Serial.print("LW for");
    myMotor->run(FORWARD);
    myOtherMotor->run(FORWARD);

    for (i = 0; i < 255; i++) {
      myMotor->setSpeed(i);
      myOtherMotor->setSpeed(i);
      delay(7);
    }
    
    for (i = 255; i != 0; i--) {
      myMotor->setSpeed(i);
      myOtherMotor->setSpeed(i);
      delay(7);
    }
  }

  myMotor->run(RELEASE);
  myOtherMotor->run(RELEASE);
}

void handleMessage(AdafruitIO_Data *data) {
  
  reading = map(data->toInt(), 3000, 4095, 0, 255);
  Serial.print("received <- ");
  Serial.println(reading);
}
