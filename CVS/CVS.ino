  #include <Codes.h>
  #include <Transceiver.h>
  #include <Numberpad.h>

  #define NODE_ID     2
  #define TOTAL_NODES 2
  Transceiver Node(NODE_ID, TOTAL_NODES);
  #undef  NODE_ID
  #undef  TOTAL_NODES

  String entered_code;

  void setup() {
    beginNumberpad();
    if (!Node.start()) {
      while (true);
    }
  }

  void loop() {
    monitorTimers();
    Node.check();
    entered_code = checkNumberpad();
    if (entered_code != "") {
      Node.send(entered_code, EXPIRE);
    }
  }
