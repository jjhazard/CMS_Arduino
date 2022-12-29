#ifndef _DATA_VERIFICATION_H_
#define _DATA_VERIFICATION_H_

#include <CMS_Definitions.h>

class Data_Packet {
  public:
    String        digits;
    PACKET_TYPE   op;
    unsigned long lastSend;
    uint8_t       id;
    uint8_t       ack_count;
    bool*         node_acks;

    Data_Packet(String      outgoing_code,
                PACKET_TYPE outgoing_type,
                uint8_t     node_id,
                uint8_t     total_units) {
      digits    = outgoing_code;
      op        = outgoing_type;
      id        = node_id;
      ack_count = total_units;
      node_acks = new bool[ack_count];
      lastSend  = millis();
      for (uint8_t index=0; index<ack_count; index++)
        node_acks[index] = false;
      node_acks[id-1] = true;
    }
    bool complete() {
      for (uint8_t ack=0; ack<ack_count; ack++)
        if (!node_acks[ack]) return false;
      return true;
    }
    bool resend_needed() {
      if (timePassedSince(lastSend, FIVE_SECONDS))
        return true;
      return false;
    }
    ~Data_Packet() {delete[] node_acks;}
};

#endif