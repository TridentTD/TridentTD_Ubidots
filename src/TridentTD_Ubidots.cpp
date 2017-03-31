/*

 [TridentTD] : MANABU's Esp8266 IoT Library
 www.facebook.com/miniNodeMCU
 
 TridentTD_Ubidots.cpp - A simple client for UBIDOTS

 Version 1.0.0  20/06/2559 Buddism Era  (2016)
 Version 1.0.1  31/03/2560 Buddism Era  (2017)
 

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
	_device_count   = 10;
	_current_device_id = "";

    val              = (Value *)malloc(maxValues*sizeof(Value));
    ubidotsVariables = (Variable *)malloc(_variable_count*sizeof(Variable));
	ubidotsDevices   = (Device *)malloc(_device_count*sizeof(Device));
	
}

bool TridentTD_Ubidots::loadAllDevices(){
  if(WiFi.status() != WL_CONNECTED){ return -1; }

  //http://things.ubidots.com/api/v1.6/datasources?token={TOKEN}

  
  float num;
  String response;
  int start_idx;
  int end_idx;


  //------------ Ubidots API ----------------------------
  String url = "http://things.ubidots.com/api/v1.6/datasources";
         url += "?token=";
         url += _token;
  _http.begin(url);

  int httpCode = _http.GET();
  if(httpCode > 0) {
      //DEBUG_PRINT("[UBIDOTS] http code: "); DEBUG_PRINTLN(httpCode);
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = _http.getString();
          DEBUG_PRINTLN(payload);DEBUG_PRINTLN();
		  
          _http.end();

          _device_count = payload.substring( 9+ payload.indexOf("\"count\":"), payload.indexOf(", \"next\"")).toInt();
          
          DEBUG_PRINT("Device Count:");DEBUG_PRINTLN(_device_count);DEBUG_PRINTLN();
            
          int start_idx = 12+ payload.indexOf("\"results\":");
          for(byte i =0; i < _device_count ; i++){
			start_idx   = payload.indexOf("{\"id\"",start_idx);
            int end_idx = 1+ payload.indexOf("}", payload.indexOf("\"description\":" , start_idx) );
            String device_json = payload.substring(start_idx,end_idx);
			
			DEBUG_PRINTLN(device_json);DEBUG_PRINTLN();
			
            start_idx = end_idx;

            
            (ubidotsDevices+i)->device_id   = device_json.substring( 8+ device_json.indexOf("{\"id\": \""),  device_json.indexOf("\", \"owner\""));
            (ubidotsDevices+i)->device_name = device_json.substring( 9+ device_json.indexOf("\"name\": \""), device_json.indexOf("\", \"url\""));
            (ubidotsDevices+i)->number_of_variables = device_json.substring( 23+ device_json.indexOf("\"number_of_variables\": "), device_json.indexOf(", \"last_activity\"")).toInt();

            
            DEBUG_PRINTLN((ubidotsDevices+i)->device_id);
            DEBUG_PRINTLN((ubidotsDevices+i)->device_name);
            DEBUG_PRINTLN((ubidotsDevices+i)->number_of_variables);
			DEBUG_PRINTLN();
          }
          
          return true;
      }
  } else {
      DEBUG_PRINT("[HTTP] GET... failed, error: "); DEBUG_PRINTLN(_http.errorToString(httpCode).c_str());
      _http.end();
      return false;
  }
}

String TridentTD_Ubidots::getDeviceID(String device_name){
  if(WiFi.status() != WL_CONNECTED){ return ""; }
  
  String device_id = "";
  
  for(byte i =0; i < _device_count ; i++){
    if((ubidotsDevices+i)->device_name == device_name ) {
      device_id = ((ubidotsDevices+i)->device_id);
	  DEBUG_PRINTLN(device_id);
	  return device_id;
    }
  }
  
  return "";
}

bool  TridentTD_Ubidots::setDevice(String device_name){
  if(WiFi.status() != WL_CONNECTED){ return -1; }
 
  // load all VARIABLES from DEVICE_NAME
  //http://things.ubidots.com/api/v1.6/datasources/[DEVICE_ID]/variables?token=[TOKEN]

 
  float num;
  String response;
  int start_idx;
  int end_idx;

  _current_device_id = getDeviceID(device_name);
  
  if(_current_device_id == "") {
	DEBUG_PRINT("[TRIDENTTD_UBIDOTS] Don't found ");
	DEBUG_PRINT(device_name);
    return -1;
  }
  
  //------------ Ubidots API ----------------------------
  String url = "http://things.ubidots.com/api/v1.6/datasources/";
         url += _current_device_id;
		 url += "/variables?token=";
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

            (ubidotsVariables+i)->device_id     = _current_device_id;
			(ubidotsVariables+i)->device_name	= device_name;
            (ubidotsVariables+i)->variable_id   = variable_json.substring( 8+ variable_json.indexOf("{\"id\": \""),  variable_json.indexOf("\", \"name\""));
            (ubidotsVariables+i)->variable_name = variable_json.substring( 9+ variable_json.indexOf("\"name\": \""), variable_json.indexOf("\", \"icon\""));
            (ubidotsVariables+i)->timestamp     = variable_json.substring( 28+ variable_json.indexOf("\"last_value\": {\"timestamp\":"), variable_json.indexOf("\", \"value\"")).toFloat();
            (ubidotsVariables+i)->last_value    = variable_json.substring( 9+ variable_json.indexOf( "\"value\":" , variable_json.indexOf("\"last_value\": {\"timestamp\":")), variable_json.indexOf("\", \"context\"")).toFloat();

            DEBUG_PRINTLN((ubidotsVariables+i)->device_id);
            DEBUG_PRINTLN((ubidotsVariables+i)->device_name);            
            DEBUG_PRINTLN((ubidotsVariables+i)->variable_id);
            DEBUG_PRINTLN((ubidotsVariables+i)->variable_name);
            DEBUG_PRINTLN((ubidotsVariables+i)->timestamp);
            DEBUG_PRINTLN((ubidotsVariables+i)->last_value);
          }
          
          return true;
      }
  } else {
      DEBUG_PRINT("[HTTP] GET... failed, error: "); DEBUG_PRINTLN(_http.errorToString(httpCode).c_str());
      _http.end();
      return false;
  }
}


float TridentTD_Ubidots::getLastValue(String variable_name){
  if(WiFi.status() != WL_CONNECTED){ return -10000; }
  
  String variable_id="";
  
  for(byte i =0; i < _variable_count ; i++){
    if((ubidotsVariables+i)->variable_name == variable_name ) {
	  variable_id = ((ubidotsVariables+i)->variable_id);
      float last_value = ((ubidotsVariables+i)->last_value);
	  
	  return last_value;
    }
  }

  if(variable_id == "") {
	Serial.println("[Ubidots] "+variable_name+" Not Found");
	return -10002;
  }

}

time_t TridentTD_Ubidots::getLastTimeStamp(String variable_name){
  if(WiFi.status() != WL_CONNECTED){ return -10000; }
  
  String variable_id="";
  
  for(byte i =0; i < _variable_count ; i++){
    if((ubidotsVariables+i)->variable_name == variable_name ) {
	  variable_id = ((ubidotsVariables+i)->variable_id);
      float timestamp = ((ubidotsVariables+i)->timestamp);
	  
	  return timestamp;
    }
  }

  if(variable_id == "") {
	Serial.println("[Ubidots] "+variable_name+" Not Found");
	return -10002;
  }

}

void TridentTD_Ubidots::setValue(String variable_name, double value){
  uint8_t i;
  int8_t variable_idx = -1;


  String variable_id = "";
  for(byte i =0; i < _variable_count ; i++){
    if((ubidotsVariables+i)->variable_name == variable_name ) {
      variable_id = ((ubidotsVariables+i)->variable_id);
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
  
  loadAllDevices();
}

String TridentTD_Ubidots::getVersion(){
  return _version;
}
