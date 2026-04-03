#include <Servo.h>
#include <SoftwareSerial.h>

Servo steering;

SoftwareSerial BT(2, 3); // RX, TX

// Motor pins
int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;
int ENA = 5;
int ENB = 6;

int speedVal = 180; // motor speed (0-255)

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  steering.attach(4);
  steering.write(90); // center position

  BT.begin(9600);
}

void loop() {
  if (BT.available()) {
    char cmd = BT.read();

    if (cmd == 'F') forward();
    else if (cmd == 'B') backward();
    else if (cmd == 'L') left();
    else if (cmd == 'R') right();
    else if (cmd == 'S') stopCar();
  }
}

// 🚗 Movement functions

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);

  steering.write(90); // straight
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);

  steering.write(90);
}

void left() {
  steering.write(60); // turn left
}

void right() {
  steering.write(120); // turn right
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
