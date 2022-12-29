#include <Transceiver.h>
#include <SPI.h>
#include <LoRa.h> 
#include <Codes.h>
#include <Peripherals.h>

Transceiver::Transceiver(uint8_t this_node, uint8_t total_nodes) {
  id     = this_node;
  id_max = total_nodes;
  buffer = new uint8_t[7];
  sent_packets = new Data_Packet*[20];
}

int Transceiver::start() {
  if (id == 0  || id_max < 2 || id > id_max)
    return 0; 
  return LoRa.begin(915E6);
}

void Transceiver::send(String digits,
                       PACKET_TYPE op,
                       uint8_t destination) {
  switch (op) {
    case DISPATCH:
    case EXPIRE:
//
      Serial.print("Packet count before send: ");
      Serial.println(packet_count);
//
      add_packet(digits, op);
//
      Serial.print("Packet count after send: ");
      Serial.println(packet_count);
//
  }
  bufferize(digits, op, destination);
  sendBuffer();
}

void Transceiver::check() {
  if (LoRa.parsePacket() > 0) receive();
  check_last_sends();
}

Transceiver::~Transceiver() {
  delete buffer;
  delete[] sent_packets;
}

/*
PRIVATE FUNCTIONS
Comms
*/

void Transceiver::bufferize(String digits,
                            PACKET_TYPE op,
                            uint8_t destination) {
  buffer[0] = (uint8_t)id;
  buffer[1] = (uint8_t)op;
  buffer[2] = (uint8_t)destination;
  buffer[3] = (uint8_t)digits[0];
  buffer[4] = (uint8_t)digits[1];
  buffer[5] = (uint8_t)digits[2];
  buffer[6] = (uint8_t)digits[3];
}

void Transceiver::sendBuffer() {
  LoRa.beginPacket();
  LoRa.write(buffer, 7);
  LoRa.endPacket();
}

bool Transceiver::validPacket() {
  //validate packet length
//
  Serial.print("Packet size: ");
  Serial.println(LoRa.available());
//
  if (LoRa.available() != 7) {
    while (LoRa.available()) LoRa.read();
    return false;
  }

  //gather packet
  for (uint8_t index = 0; index < 7; index++)
    buffer[index] = (uint8_t)LoRa.read();

  //validate origin 
  if (!buffer[0] || buffer[0] == id || buffer[0] > id_max)
    return false;

  //validate packet type
  switch ((PACKET_TYPE)buffer[1]) {
    case DISPATCH:
    case EXPIRE:
      if (buffer[2] != 0 && buffer[2] != id) return false;
      break;
    case DISPATCH_ACK:
    case EXPIRE_ACK:
      if (buffer[2] != id) return false;
      break;
    default:
      Serial.println("Invalid packet type.");
      return false;
  }
  
  //Validate 4 digits received
  for (uint8_t index=3; index<7; index++)
    if (!isDigit((char)buffer[index])) return false;

  //code validated
  return true;
}

void Transceiver::receive() {
  if (validPacket()) return;
  String digits = String(buffer[3, 7]);
  uint32_t code = digits.toInt();
  switch (buffer[1]) {
    case DISPATCH:
      Serial.println("Dispatch packet...");
      if (dispatch(code) || (dispatched(code) && buffer[2] == id))
        send(digits, DISPATCH_ACK, buffer[0]);
      return;
    case EXPIRE:
      Serial.println("Expire packet...");
      if (expire(code) || (expired(code) && buffer[2] == id))
        send(digits, EXPIRE_ACK, buffer[0]);
      return;
    case DISPATCH_ACK:
    case EXPIRE_ACK:
      Serial.println("ACK packet...");
      update_packet(digits, buffer[0]);
  }
  return;
}

/*
PRIVATE FUNCTIONS
Multinode
*/

void Transceiver::add_packet(String digits, PACKET_TYPE op) {
  if (packet_count == packet_max) return;
  sent_packets[packet_count++] = new Data_Packet(digits, op, id, id_max);
}

void Transceiver::check_last_sends() {
  Data_Packet* packet;
  for (uint8_t index=0; index<packet_count; index++) {
    packet = sent_packets[index];
    if (packet->resend_needed())
      resend_packet(packet);
  }
}

void Transceiver::bufferize(Data_Packet* packet, uint8_t destination) {
  buffer[0] = id;
  buffer[1] = packet->op;
  buffer[2] = destination;
  buffer[3] = (uint8_t)(packet->digits[0]);
  buffer[4] = (uint8_t)(packet->digits[1]);
  buffer[5] = (uint8_t)(packet->digits[2]);
  buffer[6] = (uint8_t)(packet->digits[3]);
}

void Transceiver::resend_packet(Data_Packet* packet) {
  for (uint8_t index=0; index<id_max; index++) {
    if (!packet->node_acks[index]) {
      bufferize(packet, index+1);
      sendBuffer();
    }
  }
  packet->lastSend = millis();
}

void Transceiver::delete_packet(uint8_t index) {
  delete sent_packets[index];
  packet_count -= 1;
  while (index != packet_count) {
    sent_packets[index] = sent_packets[index + 1];
    index += 1;
  }
}

void Transceiver::update_packet(String digits, uint8_t from) {
  Data_Packet* packet;
  for (uint8_t index=0; index<packet_count; index++) {
    packet = sent_packets[index];
    if (packet->digits == digits) {
      packet->node_acks[from-1] = true;
      if (packet->complete()) delete_packet(index);
    }
  }
}