#ifndef _TRANSCEIVER_H_
#define _TRANSCEIVER_H_

#include <CMS_Definitions.h>
#include <Data_Verification.h>

class Transceiver {
  public:
    Transceiver(uint8_t this_node, uint8_t total_nodes);
    int   start();
    void   send(String code,
                PACKET_TYPE op,
                uint8_t destination=0);
    void  check();
    ~Transceiver();

  private:
    //Core variables
    uint8_t id;
    uint8_t id_max;
    uint8_t* buffer;

    //Multinode variables
    uint8_t       packet_max   = 20;
    uint8_t       packet_count =  0;
    Data_Packet** sent_packets;

    //Core functions
    void   bufferize(String digits, PACKET_TYPE op, uint8_t destination);
    void  sendBuffer();
    bool validPacket();
    void     receive();

    //Multinode functions
    void       add_packet(String digits, PACKET_TYPE op);
    void        bufferize(Data_Packet* packet, uint8_t destination);
    void    resend_packet(Data_Packet* packet);
    void    delete_packet(uint8_t packet);
    void check_last_sends();
    void    update_packet(String digits, uint8_t from);
};

#endif