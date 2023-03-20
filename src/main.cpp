#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Test message");

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
}

void loop()
{
  digitalWrite(13, digitalRead(13) == HIGH ? LOW : HIGH);
  digitalWrite(12, digitalRead(12) == HIGH ? LOW : HIGH);

  delay(1000);
}
