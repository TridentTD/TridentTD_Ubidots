#include <TridentTD_Ubidots.h>
#include <DHT.h>

#define ssid    "------SSID------"
#define pass    "----PASSWORD----"

#define TOKEN   "-----TOKEN------"
#define DHTPIN  D5

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

  //---------- Send values to Ubidots's VariableName  ---------------------
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.println("[DHT11] Temperature "+String(t)+" C : Humidity "+String(h)+" %");
  
  ubidots.setValue("TEMPERATURE", t);
  ubidots.setValue("HUMIDITY", h);

  ubidots.sendAll();
  Serial.println("[UBIDOTS] Data send.");
  
}