#include <TridentTD_Ubidots.h>

#define ssid    "------SSID------"
#define pass    "----PASSWORD----"

#define TOKEN   "-----TOKEN------"

TridentTD_Ubidots ubidots(TOKEN);


#define RED_LED    D3
#define GREEN_LED  D4
#define BLUE_LED   D5

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(ubidots.getVersion());

  //-------- WiFi Initialize ----------------------------------------------
  ubidots.wificonnect(ssid,pass);

  pinMode(RED_LED,  OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}

void loop() {

  //--------- Load values from Ubidots's VariableName ----------------------
  ubidots.loadAll();
  
  int redstatus   = (int)ubidots.getValue("RED");   // the "RED" is Ubidots's VariableName
  int greenstatus = (int)ubidots.getValue("GREEN");
  int bluestatus  = (int)ubidots.getValue("BLUE");
  
      digitalWrite(RED_LED,  redstatus);            // the RED_LED is ESP8266's I/O pin
      digitalWrite(GREEN_LED,greenstatus);
      digitalWrite(BLUE_LED, bluestatus);

  //---------- Send values to Ubidots's VariableName  ---------------------
  ubidots.setValue("LDR", analogRead(A0));          // the "LDR" is Ubidots's VariableName

  ubidots.sendAll();
  Serial.println("[UBIDOTS] Data send.");
}