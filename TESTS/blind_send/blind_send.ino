#include <Peripherals.h>
#include <Codes.h>
#include <SPI.h>
#include <LoRa.h>

uint8_t* buffer = new uint8_t[7];

void setup() {
  buffer[0] = 1;
  buffer[1] = 3;
  buffer[2] = 2;
  buffer[3] = 49;
  buffer[4] = 50;
  buffer[5] = 51;
  buffer[6] = 52;
  if (!LoRa.begin(915E6)) while (true);
}

void loop() {
  LoRa.beginPacket();
  LoRa.println("Hello");
  LoRa.endPacket();
  delay(1000);
}
