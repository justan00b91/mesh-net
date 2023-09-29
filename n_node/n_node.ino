#include "painlessMesh.h"

// TODO: convert to hiden network and don't save password in plain text
#define   MESH_PREFIX     "test"
#define   MESH_PASSWORD   "testNetwork"
#define   MESH_PORT       5555

Scheduler userScheduler;
painlessMesh  mesh;
String msg = "default";

void sendMessage() ;
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

// TODO: add stuff that would be executed as/in task 1
void sendMessage() {
  // digitalWrite(LED_BUILTIN, HIGH);                                          // extra
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
  // delay(1000);                                                              // extra
  // digitalWrite(LED_BUILTIN, LOW);                                           // extra
}

void receivedCallback( uint32_t from, String &mesg ) {
  Serial.printf("startHere: Received from %u msg= %s\n", from, mesg.c_str()); // extra msg
  msg = mesg.c_str();
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  mesh.setDebugMsgTypes( ERROR | STARTUP );
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

// TODO: add more task
void loop() {
  mesh.update();
}
