#include <TridentTD_Ubidots.h>

#define ssid    "------SSID------"
#define pass    "----PASSWORD----"

#define TOKEN   "-----TOKEN------"

TridentTD_Ubidots ubidots(TOKEN);


#define RED_LED    RED
#define GREEN_LED  GREEN
#define BLUE_LED   BLUE

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
  
  
  //  ubidots.setValue("--- DEVICE NAME ---", "--- VARIABLE NAME ---" )
  
  int redstatus   = (int)ubidots.getLastValue("myDevice1", "RED");   // the "RED" is Ubidots's VariableName
  int greenstatus = (int)ubidots.getLastValue("myDevice1", "GREEN");
  int bluestatus  = (int)ubidots.getLastValue("myDevice1", "BLUE");
  
  int ldr_on_device1 = (int)ubidots.getLastValue("myDevice1", "LDR");
  int ldr_on_device2 = (int)ubidots.getLastValue("myDevice2", "LDR");
  
      digitalWrite(RED_LED,  redstatus);            // the RED_LED is ESP8266's I/O pin
      digitalWrite(GREEN_LED,greenstatus);
      digitalWrite(BLUE_LED, bluestatus);
      
      Serial.println("LDR1 :"+String(ldr_on_device1)+", LDR2 :"+ String(ldr_on_device2));


  //---------- Send values to Ubidots's VariableName  ---------------------
  //  ubidots.setValue("--- DEVICE NAME ---", "--- VARIABLE NAME ---", Value )

  ubidots.setValue("myDevice1", "LDR", analogRead(A0)); // the "LDR" is Ubidots's VariableName
  ubidots.setValue("myDevice2", "LDR", random(1000));   // the "LDR" is Ubidots's VariableName
  
  ubidots.sendAll();
  Serial.println("[UBIDOTS] Data send.");
}
