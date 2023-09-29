#include "painlessMesh.h"

// TODO: convert to hiden network and don't save password in plain text
#define   MESH_PREFIX     "test"
#define   MESH_PASSWORD   "testNetwork"
#define   MESH_PORT       5555

Scheduler userScheduler;
painlessMesh  mesh;
String msg = "default1";
int i = 0;
void sendMessage() ;
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

// TODO: add stuff that would be executed as/in task 1
void sendMessage() {
  if (i == 0){ // only run once.
    Serial.print("Query: ");

    while(Serial.available() == 0){}
  
    msg = Serial.readString();                                                      // main broadcasted msg
    msg += " Query from head.";
    i += 1;
  }
  // Serial.printf("MESSaGE : %s I = %d\n ", msg, i);
  // digitalWrite(LED_BUILTIN, HIGH);                                          // extra
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
  // delay(1000);                                                              // extra
  // digitalWrite(LED_BUILTIN, LOW);                                           // extra
}

void receivedCallback( uint32_t from, String &mesg ) {
  Serial.printf("startHere: Received from %u msg= %s\n", from, mesg.c_str()); // extra msg
  // add functionality here like ON/OFF to control the connected IOs.
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
