#include <CMS_Definitions.h>
#include <Requests.h>

void setup() {
  beginRequests();
  Serial.begin(9600);
  while(!Serial);
  Serial.print("Setup done");
}

String generated_code = "";
void loop() {
  generated_code = checkRequests();
  if (generated_code != "") {
    Serial.println(generated_code);
  }
}
