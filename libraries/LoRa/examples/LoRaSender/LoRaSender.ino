#include <SPI.h>
#include <LoRa.h>
#include <Peripherals.h>

int counter = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  setUpOutput();
  beep();
  delay(500);
  if (!LoRa.begin(915E6)) {
    while (1);
  }
  beep();
  delay(500);
}

void loop() {

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();
  beep();
  counter++;

  delay(5000);
}
