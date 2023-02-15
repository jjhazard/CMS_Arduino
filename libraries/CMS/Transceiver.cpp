#include <Transceiver.h>
#include <SPI.h>
#include <LoRa.h> 
#include <Codes.h>
#include <Peripherals.h>

#define ORIGIN_NODE                 (buffer[0])
#define TYPE_OF_OP                  (buffer[1])
#define TARGET_NODE                 (buffer[2])
#define NODE_INDEX                  (buffer[0]-1)
#define CODE_DIGITS(array)          ((char*)&array[3])
#define SAME_DIGITS(array1, array2) (0 == memcmp(CODE_DIGITS(buffer), CODE_DIGITS(packet->raw_data), 4))

/*

PUBLIC FUNCTIONS

*/

Transceiver::Transceiver(uint8_t this_node, uint8_t total_nodes) {
  id     = this_node;
  id_max = total_nodes;
  buffer = new uint8_t[8];
  buffer[7] = '\0';
  sent_packets = new Data_Packet*[20];
}

int Transceiver::start() {
  if (id == 0  || id_max < 2 || id > id_max) {
    return 0;
  }
  if (!LoRa.begin(915E6)) {
    return 0;
  }
  LoRa.setSpreadingFactor(8);
  return 1;
}

void Transceiver::send(String digits, PACKET_TYPE op, uint8_t destination) {
  sendPacket(digits, op, destination);
  switch (op) {
    case DISPATCH:
    case EXPIRE:
      addPacket();
  }
}

void Transceiver::check() {
  if (LoRa.parsePacket() > 0) {
    receive();
  }
  checkSends();
}

Transceiver::~Transceiver() {
  delete[] buffer;
  delete[] sent_packets;
}

/*

PRIVATE FUNCTIONS
Sending

*/

void Transceiver::sendBuffer() {
  LoRa.beginPacket();
  LoRa.write(buffer, 7);
  LoRa.endPacket();
}

void Transceiver::sendPacket(String digits, PACKET_TYPE op, uint8_t destination) {
  ORIGIN_NODE = id;
  TYPE_OF_OP  = op;
  TARGET_NODE = destination;
  memcpy(CODE_DIGITS(buffer), &digits[0], 4);
  sendBuffer();
}

void Transceiver::sendAck(){
  TARGET_NODE = ORIGIN_NODE;
  ORIGIN_NODE = id;
  TYPE_OF_OP  = DISPATCH_ACK;
  sendBuffer();
}

void Transceiver::resendPacket(Data_Packet* packet) {
  memcpy(buffer, packet->raw_data, 7);
  for (uint8_t index = 0; index < id_max; index++) {
    if (!packet->node_acks[index]) {
      TARGET_NODE = index+1;
      sendBuffer();
    }
  }
  packet->last_send = millis();
}

/*

PRIVATE FUNCTIONS
Receiving

*/

bool Transceiver::validPacket() {
  //validate packet length
  if (LoRa.available() != 7) {
    while (LoRa.available()) {
      LoRa.read();
    }
    return false;
  }

  //gather packet
  for (uint8_t index = 0; index < 7; index++) {
    buffer[index] = (uint8_t)LoRa.read();
  }

  //validate origin 
  if (!ORIGIN_NODE || ORIGIN_NODE == id || ORIGIN_NODE > id_max) {
    return false;
  }

  //validate packet type and destination
  switch ((PACKET_TYPE)TYPE_OF_OP) {
    case DISPATCH:
    case EXPIRE:
      if (TARGET_NODE != 0 && TARGET_NODE != id){
        return false;
      }
      break;
    case DISPATCH_ACK:
    case EXPIRE_ACK:
      if (TARGET_NODE != id) {
        return false;
      }
      break;
    default:
      return false;
  }
  
  //Validate 4 digits received
  for (uint8_t index=3; index<7; index++) {
    if (!isDigit((char)buffer[index])) {
      return false;
    }
  }

  //code validated
  return true;
}

void Transceiver::receive() {
  //validate packet
  if (!validPacket()) {
    return;
  }

  //extract code
  uint16_t code = atoi(CODE_DIGITS(buffer));

  //operate
  switch ((PACKET_TYPE)TYPE_OF_OP) {
    case DISPATCH:
      if (dispatch(code) || (dispatched(code) && TARGET_NODE == id)) {
        sendAck();
      }
      return;
    case EXPIRE:
      if (expire(code) || (expired(code) && TARGET_NODE == id)) {
        sendAck();
      }
      return;
    case DISPATCH_ACK:
    case EXPIRE_ACK:
      updatePacket();
  }
  return;
}

/*

PRIVATE FUNCTIONS
Multinode

*/

void Transceiver::addPacket() {
  if (packet_count == packet_max) {
    return;
  }
  sent_packets[packet_count++] = new Data_Packet(buffer, id, id_max);
}

void Transceiver::checkSends() {
  Data_Packet* packet;
  for (uint8_t index = 0; index < packet_count; index++) {
    packet = sent_packets[index];
    if (packet->resend_needed()) {
      resendPacket(packet);
    }
  }
}

void Transceiver::updatePacket() {
  Data_Packet* packet;
  for (uint8_t index = 0; index < packet_count; index++) {
    packet = sent_packets[index];
    if (SAME_DIGITS(buffer, packet->raw_data)) {
      packet->node_acks[NODE_INDEX] = true;
      if (packet->complete()) {
         deletePacket(index);
      }
    }
  }
}

void Transceiver::deletePacket(uint8_t index) {
  delete sent_packets[index];
  packet_count -= 1;
  for (uint8_t index = 0; index != packet_count; index++) {
    sent_packets[index] = sent_packets[index + 1];
  }
}
