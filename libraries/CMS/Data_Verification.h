#ifndef _DATA_VERIFICATION_H_
#define _DATA_VERIFICATION_H_

#include <CMS_Definitions.h>

class Data_Packet {
  public:
    unsigned long last_send;
    uint8_t       id;
    uint8_t       ack_count;
    uint8_t*      raw_data;
    bool*         node_acks;

    Data_Packet(uint8_t* buffer,
                uint8_t  node_id,
                uint8_t  total_units) {

      //Save buffer
      raw_data  = new uint8_t[7];
      memcpy(raw_data, buffer, 7);

      //Track node acks
      id        = node_id;
      ack_count = total_units;
      node_acks = new bool[ack_count];
      for (uint8_t index=0; index<ack_count; index++)
        node_acks[index] = false;
      node_acks[id-1] = true;

      //Save send time
      last_send = millis();
    }
    bool complete() {
      for (uint8_t index=0; index<ack_count; index++) {
        if (!node_acks[index])
          return false;
      }
      return true;
    }
    bool resend_needed() {
      return timePassedSince(last_send, FIVE_SECONDS);
    }
    ~Data_Packet() {
      delete[] raw_data;
      delete[] node_acks;
    }
};

#endif