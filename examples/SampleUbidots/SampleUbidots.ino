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

  //---------- set the Ubidots's Device  ---------------------
  ubidots.setDevice( "--- YOUR UBIDOTS'S DEVICENAME ---"); 
  
  //---------- get value of the VARIABLENAME on specific Device
  int redstatus   = (int)ubidots.getLastValue("RED");   // the "RED" is Ubidots's VariableName
  int greenstatus = (int)ubidots.getLastValue("GREEN");
  int bluestatus  = (int)ubidots.getLastValue("BLUE");
  
      digitalWrite(RED_LED,  redstatus);            // the RED_LED is ESP8266's I/O pin
      digitalWrite(GREEN_LED,greenstatus);
      digitalWrite(BLUE_LED, bluestatus);

  //---------- Send values to the VariableName on specific Device  ---------------------
  ubidots.setValue("LDR", analogRead(A0));          // the "LDR" is Ubidots's VariableName

  ubidots.sendAll();
  Serial.println("[UBIDOTS] Data send.");
}