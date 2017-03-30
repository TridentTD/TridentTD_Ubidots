/*

 [TridentTD] : MANABU's Esp8266 IoT Library
 www.facebook.com/miniNodeMCU
 
 TridentTD_Ubidots.cpp - A simple client for UBIDOTS

 20/06/2559 Buddism Era  (2016)
 

Copyright (c) 2016 TridentTD

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/




#include "TridentTD_Ubidots.h"

/**
 * Constructor.
 */
TridentTD_Ubidots::TridentTD_Ubidots(char* token){
    _token = token;
    maxValues       = 10;  //
    currentValue    = 0;
    _variable_count = 30;

    val        = (Value *)malloc(maxValues*sizeof(Value));
    ubidotsVar = (Variable *)malloc(_variable_count*sizeof(Variable));
}


bool TridentTD_Ubidots::loadAll(){
  if(WiFi.status() != WL_CONNECTED){ return -1; }
  
  float num;
  String response;
  int start_idx;
  int end_idx;


  //------------ Ubidots API ----------------------------
  String url = "http://things.ubidots.com/api/v1.6/variables/";
         url += "?token=";
         url += _token;
  _http.begin(url);

  int httpCode = _http.GET();
  if(httpCode > 0) {
      //DEBUG_PRINT("[UBIDOTS] http code: "); DEBUG_PRINTLN(httpCode);
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = _http.getString();
          DEBUG_PRINTLN(payload);
		  
          _http.end();

          _variable_count = payload.substring( 9+ payload.indexOf("\"count\":"), payload.indexOf(", \"next\"")).toInt();
          
          DEBUG_PRINTLN(_variable_count);
            
          int start_idx = 12+ payload.indexOf("\"results\":");
          for(byte i =0; i < _variable_count ; i++){
            int end_idx = payload.indexOf("{\"id\":", payload.indexOf("\"derived_expr\":" , start_idx) );
            String variable_json = payload.substring(start_idx,end_idx);
			
			DEBUG_PRINTLN(variable_json);
            start_idx = end_idx;

            
            (ubidotsVar+i)->variable_id   = variable_json.substring( 8+ variable_json.indexOf("{\"id\": \""),  variable_json.indexOf("\", \"name\""));
            (ubidotsVar+i)->variable_name = variable_json.substring( 9+ variable_json.indexOf("\"name\": \""), variable_json.indexOf("\", \"icon\""));
            (ubidotsVar+i)->timestamp     = variable_json.substring( 28+ variable_json.indexOf("\"last_value\": {\"timestamp\":"), variable_json.indexOf("\", \"value\"")).toFloat();
            (ubidotsVar+i)->last_value    = variable_json.substring( 9+ variable_json.indexOf( "\"value\":" , variable_json.indexOf("\"last_value\": {\"timestamp\":")), variable_json.indexOf("\", \"context\"")).toFloat();

            
            DEBUG_PRINTLN((ubidotsVar+i)->variable_id);
            DEBUG_PRINTLN((ubidotsVar+i)->variable_name);
            DEBUG_PRINTLN((ubidotsVar+i)->timestamp);
            DEBUG_PRINTLN((ubidotsVar+i)->last_value);
          }
          
          return true;
      }
  } else {
      DEBUG_PRINT("[HTTP] GET... failed, error: "); DEBUG_PRINTLN(_http.errorToString(httpCode).c_str());
      _http.end();
      return false;
  }
}


float TridentTD_Ubidots::getValue(String variable_name){
  if(WiFi.status() != WL_CONNECTED){ return -10000; }
  
  float num;
  String response;
  int start_idx;
  int end_idx;

  String variable_id = "";
  
  for(byte i =0; i < _variable_count ; i++){
    if((ubidotsVar+i)->variable_name == variable_name ) {
      variable_id = ((ubidotsVar+i)->variable_id);
    }
  }

  if(variable_id == "") {
	Serial.println("[Ubidots] "+variable_name+" Not Found");
	return -10002;
  }

  //------------ Ubidots API ----------------------------
  String url = "http://things.ubidots.com/api/v1.6/variables/";
         url += variable_id;
         url += "/values/?page_size=1&token=";
         url += _token;
  _http.begin(url);

  int httpCode = _http.GET();
  if(httpCode > 0) {
      //DEBUG_PRINT("[UBIDOTS] http code: "); DEBUG_PRINTLN(httpCode);
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = _http.getString();
          _http.end();

          //DEBUG_PRINTLN(payload);
          start_idx = 9+ payload.indexOf("\"value\":");
          end_idx   = payload.indexOf(", \"timestamp\"");
          num       = payload.substring(start_idx,end_idx).toFloat();
          
          //DEBUG_PRINTLN("[UBIDOTS] variable_id : "+ String(variable_id) + ";value : "+payload.substring(start_idx,end_idx));
          return num;
      }
  } else {
      //DEBUG_PRINT("[HTTP] GET... failed, error: "); DEBUG_PRINTLN(_http.errorToString(httpCode).c_str());
      _http.end();
      return -10001;
  }
}

void TridentTD_Ubidots::setValue(String variable_name, double value){
  uint8_t i;
  int8_t variable_idx = -1;


  String variable_id = "";
  for(byte i =0; i < _variable_count ; i++){
    if((ubidotsVar+i)->variable_name == variable_name ) {
      variable_id = ((ubidotsVar+i)->variable_id);
    }
  }
  if(variable_id == "") { return; }

  for(i=0; i<= currentValue; i++){
    if( (val+i)->variable_id == variable_id) { variable_idx = i; }
  }
  if(variable_idx == -1) {
    (val+currentValue)->variable_id = variable_id;
    (val+currentValue)->value_id = value;
    currentValue++;
  }else{
    (val+variable_idx)->variable_id = variable_id;
    (val+variable_idx)->value_id = value;
  }
    
  if(currentValue>maxValues){
    Serial.println(F("You are sending more than 10 consecutives variables, you just could send 10 variables. Then other variables will be deleted!"));
    currentValue = maxValues;
  }
}

bool TridentTD_Ubidots::sendAll(){
    int i;
    String allvalues;
    String str;
    allvalues = "[";
    for(i=0; i<currentValue;){
        str = String(((val+i)->value_id),5);
        allvalues += "{\"variable\": \"";
        allvalues += String((val + i)->variable_id);
        allvalues += "\", \"value\":";
        allvalues += str;
        allvalues += "}";
        i++;
        if(i<currentValue){
            allvalues += ", "; 
        }
    }
    allvalues += "]";
    i = allvalues.length();

    currentValue=1;  // reset ตัวแปร value
    
    DEBUG_PRINTLN("[UBIDOTS] allvalues : " +allvalues);

  //------------ Ubidots API ----------------------------
  String url = "http://things.ubidots.com/api/v1.6/collections/values/?token=";
         url += _token;
  _http.begin(url);
  _http.addHeader("Content-Type","application/json");
  

  int httpCode = _http.POST(allvalues);
  if(httpCode > 0) {
      //DEBUG_PRINT("[UBIDOTS] http code: "); DEBUG_PRINTLN(httpCode);
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = _http.getString();
          _http.end();

          DEBUG_PRINTLN("[UBIDOTS] return    : " + payload);
          return true;
      }
  } else {
      DEBUG_PRINT("[HTTP] GET... failed, error: "); DEBUG_PRINTLN(_http.errorToString(httpCode).c_str());
      _http.end();
      return -1;
  }
  
}


bool TridentTD_Ubidots::wificonnect(char* ssid, char* pass){
  WiFi.begin(ssid, pass);

  Serial.println();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

String TridentTD_Ubidots::getVersion(){
  return _version;
}
