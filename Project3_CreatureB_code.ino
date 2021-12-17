#include "config.h"
#include <ESP32Servo.h>
Servo myservo;

int pos = 0;
int reading = 0;
int servoPin = 13;

//photocell analog pin
#define CELL_PIN A2

int current = 0;
int prev = -1;

// set up the 'analog' feed
AdafruitIO_Feed *analog = io.feed("mic-ct3", "AAmaya");
AdafruitIO_Feed *photocell = io.feed("ct3_photocell");

void setup() {

  pinMode(CELL_PIN, INPUT);

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 500, 2400);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'analog' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  analog->onMessage(handleMessage);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  photocell->get();
}

void loop() {

  io.run();
  current = analogRead (CELL_PIN);

  // when there are multiple sensors
  if (current != prev) {
    Serial.print("sending: ");
    Serial.println(current);
    photocell->save(current);
  }

  if (reading < 90) {

    for (pos = 0; pos <= 90; pos += 1) {
      myservo.write(pos);
      delay(15);
    }
    for (pos = 90; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(15);
    }
  }

  else {

    for (pos = 91; pos <= 180; pos += 1) {
      myservo.write(pos);
      delay(15);
    }
    for (pos = 180; pos >= 91; pos -= 1) {
      myservo.write(pos);
      delay(15);
    }
  }
}

void handleMessage(AdafruitIO_Data *data) {

  reading = map (data->toInt(), 0, 4095, 0, 180);

  Serial.print("received <- ");
  Serial.println(reading);
}
