#include <Codes.h>
#include <Transceiver.h>
#include <Requests.h>

#define THIS_NODE   2
#define TOTAL_NODES 2
Transceiver Node(THIS_NODE, TOTAL_NODES);
#undef  THIS_NODE
#undef  TOTAL_NODES

String requested_code;

void setup() {
  beginRequests();
  if (!Node.start()) {
    while(true);
  }
}

void loop() {
  monitorTimers();
  Node.check();
  requested_code = checkRequests();
  if (requested_code != "") {
    Node.send(requested_code, DISPATCH);
  }
}
