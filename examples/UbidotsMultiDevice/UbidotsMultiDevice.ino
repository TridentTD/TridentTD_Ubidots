#include <TridentTD_Ubidots.h>
#include <DHT.h>

#define ssid    "------SSID------"
#define pass    "----PASSWORD----"

#define TOKEN   "-----TOKEN------"

TridentTD_Ubidots ubidots(TOKEN);


DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(ubidots.getVersion());
  
  //-------- WiFi Initialize ----------------------------------------------
  ubidots.wificonnect(ssid,pass);

  dht.begin();
}

void loop() {
  //=================================================================================
  //---------- set the Ubidots's DeviceName    ---------------------

  ubidots.setDevice("--- DEVICE NAME1----");   // if you have the DeviceName on Ubidots.
    
    float humid = ubidots.getLastValue("HUMIDITY");
    float temp  = ubidots.getLastValue("TEMPERATURE");

            Serial.println("[UBIDOTS] Temperature "+String(temp)+" C : Humidity "+String(humid)+" %"+"\n");
            delay(1000);
          
            //---------- Send values to Ubidots's VariableName of the DeviceName ---------------------
            float h = dht.readHumidity();
            float t = dht.readTemperature();
          
            Serial.println("[DHT11]   Temperature "+String(t)+" C : Humidity "+String(h)+" %");
            
    
    ubidots.setValue("TEMPERATURE", t);             // "TEMPERATURE" on DeviceName1
    ubidots.setValue("HUMIDITY", h);
  
    ubidots.sendAll();                              // send data to the specific DeviceName1

  //=================================================================================
  //---------- set to another Ubidots's DeviceName    ---------------------
  ubidots.setDevice("--- DEVICE NAME2----");     // change to another DeviceName  on Ubidots.
    
    ubidots.setValue("TEMPERATURE", random(100));   // "TEMPERATURE" on DeviceName2
    ubidots.sendAll();                              // send data to the specific DeviceName2
    
  //=================================================================================
  
  
}