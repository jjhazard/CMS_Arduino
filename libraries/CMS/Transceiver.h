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

    //Send functions
    void   sendBuffer();
    void   sendPacket(String digits, PACKET_TYPE op, uint8_t destination);
    void      sendAck();
    void resendPacket(Data_Packet* packet);

    //Receive functions
    bool validPacket();
    void     receive();

    //Verification functions
    void    addPacket();
    void   checkSends();
    void updatePacket();
    void deletePacket(uint8_t index);
};

#endif