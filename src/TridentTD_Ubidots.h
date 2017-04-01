/*

 [TridentTD] : MANABU's Esp8266 IoT Library
 www.facebook.com/miniNodeMCU
 
 TridentTD_Ubidots.h - A simple client for UBIDOTS

 Version 1.0.0  20/06/2559 Buddism Era  (2016)  for old ubidots interface
 Version 1.0.1  31/03/2560 Buddism Era  (2017)  for new ubidots interface
 Version 1.0.2  01/04/2560 Buddism Era  (2017)  bug fix
 

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



#ifndef _TRIDENTTD_UBIDOTS_H_
#define _TRIDENTTD_UBIDOTS_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Time.h>

//#define  UBIDOTS_DEBUG_MODE

#ifdef UBIDOTS_DEBUG_MODE
    #define DEBUG_PRINTER Serial
    #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
    #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
    #define DEBUG_PRINT(...) { }
    #define DEBUG_PRINTLN(...) { }
#endif


typedef struct Value {
  String variable_id;
  float  value;
} Value;


typedef struct Variable {
  String device_id;
  String device_name;
  String variable_id;
  String variable_name;
  time_t timestamp;
  float  last_value;
} Variable;

typedef struct Device {
  String device_id;
  String device_name;
  uint8_t number_of_variables;
} Device;

class TridentTD_Ubidots {
   public:
      TridentTD_Ubidots(char* token);
      bool  wificonnect(char *ssid, char *pass);


      bool   setDevice(String device_name);
      float  getLastValue(String variable_name);
	  time_t getLastTimeStamp(String variable_name);
	  
	  void   setValue(String variable_name, double value);
	  bool   sendAll();  
	  
	  String getVersion();
   private:
	  String _version="[Trident_Ubidots] Version 1.0.2";
      char*  _token;
	  String _current_device_id;
	  
      uint8_t _maxValues;        // maxValues for send
      uint8_t _val_count;
      Value * val;
      
      uint8_t _variable_count;
      Variable * ubidotsVariables;
	  
	  uint8_t _device_count;
	  Device * ubidotsDevices;

      HTTPClient       _http;
	  
	  bool   loadAllDevices();
	  String getDeviceID(String device_name);
	  String getVariableID(String variable_name);


};
#endif /* _TRIDENTTD_UBIDOTS_H_ */
