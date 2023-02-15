#include <KeyboardController.h>
#include <Peripherals.h>
#include <SPI.h>
#include <LoRa.h>

USBHost usbh;
KeyboardController Numpad(usbh);

void setup() {
  setUpOutput();
  if (!LoRa.begin(915E6)) while(true);
}

String received_code;
void loop() {
  usbh.Task();
}

void keyPressed() {
  beep();
  LoRa.beginPacket();
  LoRa.write(String((int)Numpad.getKey()));
  LoRa.endPacket();
}